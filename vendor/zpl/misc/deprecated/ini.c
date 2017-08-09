
#define ZPL_IMPLEMENTATION
#include "zpl.h"

#define ZPL_INI_IMPLEMENTATION
#include "zpl_ini.h"

#include "stdlib.h"

typedef struct Map {
    i32 width;
    i32 height;
    u8  climate;
    
    f64 glob_dim_x;
} Map;

static ZPL_INI_HANDLER(map_load_handler) {
    Map *map = cast(Map *)data;
    
#define CHECK(s,n) (!zpl_strcmp(section, s) && !zpl_strcmp(name, n))
    
    /**/ if (CHECK("game", "glob_dim_x")) {
        map->glob_dim_x = atof(value);
    }
    else if (CHECK("map", "width")) {
        map->width = atoi(value);
    }
    else if (CHECK("map", "height")) {
        map->height = atoi(value);
    }
    else if (CHECK("map", "climate")) {
        map->climate = atoi(value);
    }
    
#undef CHECK
    
    return 1;
}

int
main(void) {
    Map demo_map = {0};
    
    zplIniError err = zpl_ini_parse("moscow.txt", map_load_handler, &demo_map);
    
    zpl_printf("Error? %s[%d]\n", zpl_ini_error_string(err), err.line_num);
    
    zpl_printf("W: %d, H: %d, C: %d, %f\n", 
               demo_map.width,
               demo_map.height,
               demo_map.climate,
               demo_map.glob_dim_x);
    
    return 0;
}