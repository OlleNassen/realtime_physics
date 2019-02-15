#include "game.hpp"

#ifdef IS_UNITY_BUILD
#include "unity_build.cpp"
#endif

int main()
{
    game g;
    g.run();

    return 0;
}
