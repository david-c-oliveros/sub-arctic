#include "app.h"


int main()
{
    App app(1600, 1200);

    if (app.create())
    {
        app.run();
    }

    return 0;
}
