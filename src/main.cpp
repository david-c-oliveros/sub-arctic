#include "app.h"


int main()
{
    App app(2000, 2000);

    if (app.create())
    {
        app.run();
    }

    return 0;
}
