#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION

// Windows stub, it is needed since CMake won't do this for us..
//
#ifdef WIN32
#	pragma comment(lib, "ws2_32.lib")
#	pragma comment(lib, "winmm.lib")
#endif

#include <librg.h>
