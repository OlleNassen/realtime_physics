#ifndef CAM3P_HPP
#define CAM3P_HPP

#include <glm/glm.hpp>
#include "shader.hpp"

#define pi32 3.14159265359f

struct spherical_point
{
	float Radius;
	float Phi;
	float Theta;
};

struct cam3p
{
	glm::vec3 PlayerPosition;
	spherical_point CameraPosition;
	glm::mat4 View;
	glm::mat4 Projection;
	int DX;
	int DY;
	bool First;
};

static glm::vec3 DirectionVector(spherical_point P)
{
	glm::vec3 Result;
	Result.x = cos(P.Phi) * cos(P.Theta);
	Result.y = sin(P.Phi);
	Result.z = cos(P.Phi) * sin(P.Theta);
	return Result;
}

static void BindCamera(cam3p* P, shader* shad)
{
	glm::vec3 Forward = DirectionVector(P->CameraPosition);
	
	glm::vec3 Position = P->PlayerPosition;
	Position.x -= Forward.x * P->CameraPosition.Radius;
	Position.y -= Forward.y * P->CameraPosition.Radius;
	Position.z -= Forward.z * P->CameraPosition.Radius;

	shad->uniform("view_position", Position);
	shad->uniform("view", P->View);
	shad->uniform("projection", P->Projection);
	shad->uniform("view_projection", P->Projection * P->View);
}

static glm::mat4 CreateProjection(float FovY, float Aspect, float NearZ, float FarZ)
{
	float TanHalfFov = tanf(FovY / 2.0f);

	glm::mat4 Result = {};
	Result[0][0] = 1.0f / (Aspect * TanHalfFov);
	Result[1][1] = 1.0f / TanHalfFov;
	Result[2][2] = -(FarZ + NearZ) / (FarZ - NearZ);

	Result[2][3] = -1.0f;
	Result[3][2] = -(2.0f * FarZ * NearZ) / (FarZ - NearZ);

	return Result;
}

static void RotateCamera(spherical_point* P, float X, float Y)
{
	P->Phi += Y;
	P->Theta += X;

	float Max = pi32 * 0.4f;
	float Min = -Max;

	if (P->Phi > Max) P->Phi = Max;
	if (P->Phi < Min) P->Phi = Min;
}

static void UpdateCamera(cam3p* State, int MX, int MY, bool UpB, bool LeftB, bool DownB, bool RightB)
{
	if (State->First)
	{
		State->DX = MX;
		State->DY = MY;
		State->First = false;
	}

	int xoffset = MX - State->DX;
	int yoffset = MY - State->DY;
	State->DX = MX;
	State->DY = MY;
	
	float Timestep = 1.0f / 60.0f;
	float Sensitivity = 0.05f;
	
	RotateCamera(&State->CameraPosition,
		(float)xoffset * Sensitivity,
		-(float)yoffset * Sensitivity);

	glm::vec3 forward = DirectionVector(State->CameraPosition);

	glm::vec3 up = {};
	up.y = 1.0f;

	glm::vec3 right = glm::normalize(glm::cross(forward, up));
	up = glm::cross(right, forward);

	float speed = Timestep;
	float forward_direction = 0.0f;
	float right_direction = 0.0f;

	if (UpB) forward_direction -= speed;
	if (DownB) forward_direction += speed;
	if (LeftB) right_direction += speed;
	if (RightB) right_direction -= speed;

	glm::vec3* Player = &State->PlayerPosition;
	Player->x -= forward_direction * forward.x + right_direction * right.x;
	Player->y -= forward_direction * forward.y + right_direction * right.y;
	Player->z -= forward_direction * forward.z + right_direction * right.z;

	// stuff
	
	glm::vec3 Forward = DirectionVector(State->CameraPosition);

	glm::vec3 Position = State->PlayerPosition;
	Position.x -= Forward.x * State->CameraPosition.Radius;
	Position.y -= Forward.y * State->CameraPosition.Radius;
	Position.z -= Forward.z * State->CameraPosition.Radius;

	glm::vec3 Up = {};
	Up.y = 1.0f;
	
	glm::vec3 Right = glm::normalize(glm::cross(Forward, Up));
	Up = glm::cross(Right, Forward);

	glm::mat4 temp = {};
	State->View = temp;

	State->View[0][0] = 1.0f;
	State->View[1][1] = 1.0f;
	State->View[2][2] = 1.0f;
	State->View[3][3] = 1.0f;

	State->View[0][0] = Right.x;
	State->View[1][0] = Right.y;
	State->View[2][0] = Right.z;

	State->View[0][1] = Up.x;
	State->View[1][1] = Up.y;
	State->View[2][1] = Up.z;

	State->View[0][2] = -Forward.x;
	State->View[1][2] = -Forward.y;
	State->View[2][2] = -Forward.z;

	State->View[3][0] = -dot(Right, Position);
	State->View[3][1] = -dot(Up, Position);
	State->View[3][2] = dot(Forward, Position);

	State->Projection = CreateProjection(pi32 * 0.5f, 16.0f / 9.0f, 0.1f, 10000.0f);
}

#endif
