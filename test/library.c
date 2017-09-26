#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG

// Windows stub, it is needed since CMake won't do this for us..
//
#ifdef WIN32
#	pragma comment(lib, "ws2_32.lib")
#	pragma comment(lib, "winmm.lib")
#endif

#include <librg.h>
