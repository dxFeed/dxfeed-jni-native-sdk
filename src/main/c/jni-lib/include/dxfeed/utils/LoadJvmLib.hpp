// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_LOADLIBRARYUTILS_H
#define DXFEED_JNI_NATIVE_SDK_LOADLIBRARYUTILS_H

#include <string>

#if _MSC_VER && !__INTEL_COMPILER
#    include <Windows.h>

const wchar_t JAVA_DLL_NAME[] = L"java.dll";
const wchar_t JVM_DLL_NAME[] = L"jvm.dll";

#else
#    include <dlfcn.h>

const char JAVA_DLL_NAME[] = "java";
const char JVM_DLL_NAME[] = "libjvm.dylib";
#endif

#include "JNICommon.hpp"

namespace dxfeed::jni::internal {

#if _MSC_VER && !__INTEL_COMPILER

std::unique_ptr<WCHAR[]> multibyteStrToUtf16(const char *s) {
    DWORD size = MultiByteToWideChar(CP_UTF8, 0, s, -1, nullptr, 0);
    std::unique_ptr<WCHAR[]> wideStr(new wchar_t[size]);
    MultiByteToWideChar(CP_UTF8, 0, s, -1, wideStr.get(), static_cast<int32_t>(size));
    return wideStr;
}

std::wstring utf8ToPlatformString(const char *str) {
    const auto &buffer = multibyteStrToUtf16(str);
    std::wstring w(buffer.get());
    return w;
}

template <typename SymbolType> SymbolType loadSymbolPlatform(HINSTANCE libraryHandle, const char *symbolName) {
    return (SymbolType)GetProcAddress(libraryHandle, symbolName);
}

auto loadLibraryPlatform(const wchar_t *symbolName) {
    return LoadLibraryW(symbolName);
}

#else

std::string utf8ToPlatformString(const char *str) {
    return std::string{str};
}

auto loadLibraryPlatform(const char *path) {
    return dlopen(path, RTLD_LAZY);
}

template <typename SymbolType> SymbolType loadSymbolPlatform(void *libraryHandle, const char *symbolName) {
    return (SymbolType)dlsym(libraryHandle, symbolName);
}

#endif
} // namespace dxfeed::jni::internal

#endif // DXFEED_JNI_NATIVE_SDK_LOADLIBRARYUTILS_H
