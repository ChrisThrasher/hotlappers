#include "App.hpp"

#if defined(WIN32)
extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
extern "C" __declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 1;
#endif

int main()
{
    App app;
    app.run();
}
