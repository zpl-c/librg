
#define ZPL_IMPLEMENTATION
#include "zpl.h"

int
main(void) {
    
    char buf[4096] = {0};
    
    zpl_system_command("dir", buf);
    zpl_printf("Output: %s\n", buf);
    return 0;
}
