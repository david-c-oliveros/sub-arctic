#include "app.h"


int main()
{
    App app(1440, 1080);

    if (app.create())
    {
        app.run();
    }

    return 0;
}
