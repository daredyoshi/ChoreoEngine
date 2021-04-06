#pragma once

// this just shits the bit by x places. 
// so if you have bit 1 you can offset it. 
// This way we can create a bitfield with multiple 
// events set and then just mask them out.
#define BIT(x) (1 << x)

#ifdef CE_ENABLE_ASSSERTS
    #ifdef _MSC_VER
        #define CE_ASSERT(x, ...) { if(!(x)) { CE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
        #define CE_CORE_ASSERT(x, ...) { if(!(x)) { CE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #else
        #define CE_ASSERT(x, ...) { if(!(x)) { CE_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
        #define CE_CORE_ASSERT(x, ...) { if(!(x)) { CE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
    #endif
#else
    #define CE_ASSERT(x, ...)
    #define CE_CORE_ASSERT(x, ...)
#endif


