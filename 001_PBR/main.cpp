#include "PBRApp.h"

int main()
{
    PBRApp app;
    if (!app.Init())
        return -1;
    app.Run();

    return 0;
}
