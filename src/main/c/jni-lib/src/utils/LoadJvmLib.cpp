// SPDX-License-Identifier: MPL-2.0

#include <filesystem>
#include <iostream>
#include <sstream>

#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/LoadJvmLib.hpp"

namespace fs = std::filesystem;

namespace dxfeed::jni::internal {
  CreateJavaVM_t fCreateJavaVM = nullptr;

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
    fCreateJavaVM = loadSymbol<CreateJavaVM_t>(JVM_DLL, "JNI_CreateJavaVM");
    auto findBootClass = loadSymbol<FindClassFromBootLoader_t>(JVM_DLL, "JVM_FindClassFromBootLoader");
    if (fCreateJavaVM == nullptr || findBootClass == nullptr) {
      std::stringstream ss{};
      ss << "GetProcAddress failed: " << r_cast<void*>(fCreateJavaVM) << ", " << r_cast<void*>(findBootClass);
      throw std::runtime_error(ss.str());
    }
  }
}