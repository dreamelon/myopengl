#include "voxelApp.h"

int main()
{
    VoxelApp app;
    if (!app.Init())
        return -1;
    app.Run();

    return 0;
}