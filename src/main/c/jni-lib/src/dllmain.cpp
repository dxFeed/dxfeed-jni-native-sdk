// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

namespace dxfeed::jni::internal {
char dllFilePath[512 + 1] = {0};
}

#if _MSC_VER && !__INTEL_COMPILER
#    include <Windows.h>

extern "C" BOOL WINAPI DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        GetModuleFileNameA(hModule, dxfeed::jni::internal::dllFilePath, 512);
    }
    return TRUE;
}
#else

#    include <cstring>
#    include <dlfcn.h>

__attribute__((constructor)) extern "C" void DllMain(void) {
    Dl_info dl_info;
    dladdr((void *)DllMain, &dl_info);
    if (dl_info.dli_fname) {
        strcpy(dxfeed::jni::internal::dllFilePath, dl_info.dli_fname);
    }
}
#endif