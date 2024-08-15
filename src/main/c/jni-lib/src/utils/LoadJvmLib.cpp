// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include <filesystem>
#include <iostream>
#include <sstream>

#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/LoadJvmLib.hpp"

namespace fs = std::filesystem;

namespace dxfeed::jni::internal {
CreateJavaVM_t fCreateJavaVM = nullptr;

template <typename CharT> fs::path recursivelyLookUpLibraryByName(const fs::path &path, CharT libName) {
    for (const auto &dirOrFile : fs::recursive_directory_iterator(path)) {
        if (dirOrFile.is_regular_file() && dirOrFile.path().filename() == libName) {
            return dirOrFile.path();
        }
    }
    auto errMsg = "NO_PATH";
    std::cerr << errMsg << std::endl;
    throw std::runtime_error(errMsg);
}

auto loadLibrary(const fs::path &path) {
    auto libraryHandle = loadLibraryPlatform(path.c_str());
    if (!libraryHandle) {
        auto errMsg = "Can't load lib from: " + path.string();
        std::cerr << errMsg << std::endl;
        throw std::runtime_error(errMsg);
    }
    return libraryHandle;
}

template <typename SymbolType, typename HandleT> SymbolType loadSymbol(HandleT libraryHandle, const char *symbolName) {
    auto symbol = loadSymbolPlatform<SymbolType>(libraryHandle, symbolName);
    if (!symbol) {
        auto errMsg = std::string("Can't load symbol: ") + symbolName;
        std::cerr << errMsg << std::endl;
        throw std::runtime_error(errMsg);
    }
    return symbol;
}

void loadJVMLibrary(const char *javaHomeUTF8) {
    auto javaHome = utf8ToPlatformString(javaHomeUTF8);
    auto javaDllPath = recursivelyLookUpLibraryByName(javaHome, JAVA_DLL_NAME);
    bool file_exists = fs::exists(javaDllPath);
    auto size = file_exists && fs::is_regular_file(javaDllPath) ? static_cast<int64_t>(fs::file_size(javaDllPath)) : 0;
    if (!size) {
        auto errMsg = javaDllPath.string() + "doesn't exits";
        std::cerr << errMsg << std::endl;
        throw std::runtime_error(errMsg);
    }

    auto jvmDllPath = recursivelyLookUpLibraryByName(javaHome, JVM_DLL_NAME);
    auto JVM_DLL = loadLibrary(jvmDllPath.c_str());
    fCreateJavaVM = loadSymbol<CreateJavaVM_t>(JVM_DLL, "JNI_CreateJavaVM");
    auto findBootClass = loadSymbol<FindClassFromBootLoader_t>(JVM_DLL, "JVM_FindClassFromBootLoader");
    if (fCreateJavaVM == nullptr || findBootClass == nullptr) {
        std::stringstream ss{};
        ss << "GetProcAddress failed: " << r_cast<void *>(fCreateJavaVM) << ", " << r_cast<void *>(findBootClass);
        auto errMsg = ss.str();
        std::cerr << errMsg << std::endl;
        throw std::runtime_error(errMsg);
    }
}
} // namespace dxfeed::jni::internal