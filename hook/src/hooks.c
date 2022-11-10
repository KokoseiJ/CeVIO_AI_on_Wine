#include <MinHook.h>

int init_hooks() {
    if (MH_Initialize() != MH_OK) return 1;
    
    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) return 1;
    
    return 0;
}

