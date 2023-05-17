// SPDX-License-Identifier: MPL-2.0

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
  // todo: get dll path for Unix
#endif