// SPDX-License-Identifier: MPL-2.0

#include <sstream>
#include <iostream>

#include "dxfeed/utils/LoadLibrary.hpp"

#if _MSC_VER && !__INTEL_COMPILER
#include <Windows.h>

const wchar_t JAVA_DLL_NAME[] = L"java.dll";
const wchar_t JVM_DLL_NAME[] = L"jvm.dll";

static inline std::unique_ptr<WCHAR[]> multibyteStrToUtf16(const char *s) {
  DWORD size = MultiByteToWideChar(CP_UTF8, 0, s, -1, nullptr, 0);
  std::unique_ptr<WCHAR[]> wideStr(new wchar_t[size]);
  MultiByteToWideChar(CP_UTF8, 0, s, -1, wideStr.get(), static_cast<int32_t>(size));
  return wideStr;
}

std::wstring utf8ToPlatformString(const char* str) {
  const auto& buffer = multibyteStrToUtf16(str);
  std::wstring w(buffer.get());
  return w;
}

template <typename SymbolType>
SymbolType loadSymbolPlatform(HINSTANCE libraryHandle, const char* symbolName) {
  return (SymbolType) GetProcAddress(libraryHandle, symbolName);
}

auto loadLibraryPlatform(const wchar_t* symbolName) {
  return LoadLibraryW(symbolName);
}

#else

#include <dlfcn.h>
#include <string>

const char JAVA_DLL_NAME[] = "java";
const char JVM_DLL_NAME[] = "libjvm.dylib";

std::string utf8ToPlatformString(const char* str) {
  return std::string { str };
}

auto loadLibraryPlatform(const char* path) {
  return dlopen(path, RTLD_LAZY);
}

template <typename SymbolType>
SymbolType loadSymbolPlatform(void* libraryHandle, const char* symbolName) {
  return (SymbolType) dlsym(libraryHandle, symbolName);
}

#endif

template <class TargetType, class InitialType>
constexpr inline TargetType r_cast(InitialType arg) {
  return reinterpret_cast<TargetType>(arg);
}

namespace dxfeed::internal {
  CreateJavaVM_t createJavaVM = nullptr;

  template <typename CharT>
  fs::path recursivelyLookUpLibraryByName(const fs::path& path, CharT libName) {
    for (const auto& dirOrFile : fs::recursive_directory_iterator(path)) {
      if (dirOrFile.is_regular_file() && dirOrFile.path().filename() == libName) {
        return dirOrFile.path();
      }
    }
    throw std::runtime_error("NO_PATH");
  }

  auto loadLibrary(const fs::path& path) {
    auto libraryHandle = loadLibraryPlatform(path.c_str());
    if (!libraryHandle) {
      std::string errMsg("Can't load lib from: ");
      throw std::runtime_error(errMsg + path.string());
    }
    std::cout << "Loaded library: " << path << "\n";
    return libraryHandle;
  }

  template<typename SymbolType, typename HandleT>
  SymbolType loadSymbol(HandleT libraryHandle, const char* symbolName) {
    auto symbol = loadSymbolPlatform<SymbolType>(libraryHandle, symbolName);
    if (!symbol) {
      std::string errMsg("Can't load symbol: ");
      throw std::runtime_error(errMsg + symbolName);
    }
    return symbol;
  }

  void loadJVMLibrary(const char* javaHomeUTF8) {
    auto javaHome = utf8ToPlatformString(javaHomeUTF8);
    auto javaDllPath = recursivelyLookUpLibraryByName(javaHome, JAVA_DLL_NAME);
    bool file_exists = fs::exists(javaDllPath);
    auto size = file_exists && fs::is_regular_file(javaDllPath) ? static_cast<int64_t>(fs::file_size(javaDllPath)) : 0;
    if (!size) {
      throw std::runtime_error(javaDllPath.string() + "doesn't exits");
    }

    auto jvmDllPath = recursivelyLookUpLibraryByName(javaHome, JVM_DLL_NAME);
    auto JVM_DLL = loadLibrary(jvmDllPath.c_str());
    createJavaVM = loadSymbol<CreateJavaVM_t>(JVM_DLL, "JNI_CreateJavaVM");
    auto findBootClass = loadSymbol<FindClassFromBootLoader_t>(JVM_DLL, "JVM_FindClassFromBootLoader");
    if (createJavaVM == nullptr || findBootClass == nullptr) {
      std::stringstream ss{};
      ss << "GetProcAddress failed: " << r_cast<void*>(createJavaVM) << ", " << r_cast<void*>(findBootClass);
      throw std::runtime_error(ss.str());
    }
  }
}

// todo: at least need
//      server/jvm.dll
//      java.dll
//      jimage.dll
//      jsvml.dll
//      verify.dll
//      lib/modules