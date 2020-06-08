# Custom allocators

By default the library uses basic system heap allocator (`malloc`/`free`).
However, it provides a way for you to change the default allocator and deallocator by redefining macro:

## LIBRG_MEM_ALLOC

Allows to define a custom allocator

```c
void *my_custom_alloc(size_t size);

#define LIBRG_MEM_ALLOC(size) \
    my_custom_alloc(size)

#define LIBRG_IMPL
#include "librg.h"

void *my_custom_alloc(size_t size) {
    return /* some allocation stuff */;
}
```
## LIBRG_MEM_FREE

Allows to define a custom deallocator

```c
void my_custom_free(void *ptr);

#define LIBRG_MEM_FREE(ptr) \
    my_custom_free(ptr)

#define LIBRG_IMPL
#include "librg.h"

void my_custom_free(void *ptr) {
    /* some deallocation stuff */;
}
```
