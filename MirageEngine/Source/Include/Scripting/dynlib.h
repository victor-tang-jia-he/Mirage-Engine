#pragma once

#ifdef _WIN32
#    include <windows.h>
using LibHandle = HMODULE;
#else
#    include <dlfcn.h>
using LibHandle = void*;
#endif

inline LibHandle LoadSharedLibrary(const char* name)
{
#ifdef _WIN32
    return LoadLibraryA(name);
#else
    return dlopen(name, RTLD_NOW);
#endif
}

inline void* GetSymbol(LibHandle lib, const char* symbol)
{
#ifdef _WIN32
    return (void*)GetProcAddress(lib, symbol);
#else
    return dlsym(lib, symbol);
#endif
}

inline void UnloadSharedLibrary(LibHandle lib)
{
    if (lib)
    {
#ifdef _WIN32
        FreeLibrary(lib);
#else
        dlclose(lib);
#endif
    }
}
#if defined(_WIN32)
    #define MRG_POPEN  _popen
    #define MRG_PCLOSE _pclose
#else
    #define MRG_POPEN  popen
    #define MRG_PCLOSE pclose
#endif