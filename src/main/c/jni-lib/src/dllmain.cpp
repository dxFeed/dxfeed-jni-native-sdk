// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

namespace dxfeed::jni::internal {
  char dllFilePath[512 + 1] = {0};
}

#if _MSC_VER && !__INTEL_COMPILER
#include <Windows.h>

extern "C" BOOL WINAPI DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpvReserved) {
  if (fdwReason == DLL_PROCESS_ATTACH) {
    GetModuleFileNameA(hModule, dxfeed::jni::internal::dllFilePath, 512);
  }
  return TRUE;
}
#else

#include <dlfcn.h>
#include <cstring>

__attribute__((constructor))
extern "C" void DllMain(void) {
  Dl_info dl_info;
  dladdr((void *)DllMain, &dl_info);
  if (dl_info.dli_fname) {
    strcpy(dxfeed::jni::internal::dllFilePath, dl_info.dli_fname);
  }
}
#endif