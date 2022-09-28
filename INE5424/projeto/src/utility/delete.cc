// EPOS Dynamic Memory Allocators Implementation

#include <system.h>

// C++ dynamic memory deallocators
void operator delete(void * object) {
    return free(object);
}

void operator delete[](void * object) {
    return free(object);
}

void operator delete(void * object, size_t bytes) {
    return free(object);
}

void operator delete[](void * object, size_t bytes) {
    return free(object);
}
