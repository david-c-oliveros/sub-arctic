#include "app.h"


int main()
{
    App app(2000, 1500);

    if (app.create())
    {
        app.run();
    }

    return 0;
}
