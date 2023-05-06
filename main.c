#include "lib.h"
#include "init.h"

int 
main()
{
    board_clocking_init();
    board_gpio_init();

    while ( true )
    {
    }

    return 0;
}
