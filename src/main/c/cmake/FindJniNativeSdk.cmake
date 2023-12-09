message("--- Start DxFeed cmake for samples begin --- ")

function(LINK_LIB_FROM_DIR LINK_DIR)
  # Add DxFeedJniNativeSdk.dylib|dll|.so as SHARED dependency
  add_library(DX_FEED_JNI_NATIVE_SDK SHARED IMPORTED)

  # Specify linkage directory where to find DxFeedJniNativeSdk.dylib|dll|.so
  link_directories(${LINK_DIR})
  if (WIN32)
    set_property(TARGET DX_FEED_JNI_NATIVE_SDK PROPERTY IMPORTED_LOCATION ${LINK_DIR}/DxFeedJniNativeSdk.dll)
    set_property(TARGET DX_FEED_JNI_NATIVE_SDK PROPERTY IMPORTED_IMPLIB ${LINK_DIR}/DxFeedJniNativeSdk.lib)
  elseif (APPLE)
    set_property(TARGET DX_FEED_JNI_NATIVE_SDK PROPERTY IMPORTED_LOCATION ${LINK_DIR}/DxFeedJniNativeSdk.dylib)
  elseif (UNIX)
    set_property(TARGET DX_FEED_JNI_NATIVE_SDK PROPERTY IMPORTED_LOCATION ${LINK_DIR}/DxFeedJniNativeSdk.so)
  endif ()
endfunction()

function(SPECIFY_TARGET_COMPILE_OPTIONS)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINKER_LANGUAGE CXX
        LINK_FLAGS "-Wl,-rpath,@executable_path/"
        MACOSX_RPATH TRUE
        SKIP_BUILD_RPATH FALSE
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH_USE_LINK_PATH TRUE
        INSTALL_NAME_DIR "@rpath/")

    target_compile_options(${PROJECT_NAME} PRIVATE
        -std=c++17
        -stdlib=libc++
        -Wall
        -Wextra
        -Wno-unused-parameter
        -O3
        )
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _WIN32_WINDOWS
        _WINSOCK_DEPRECATED_NO_WARNINGS
        )
    target_compile_options(${PROJECT_NAME} PRIVATE
        /std:c++17
        /W3
        /bigobj
        /EHsc
        "$<$<CONFIG:Release>:/GL>"
        "$<$<CONFIG:Release>:/Ox>"
        "$<$<CONFIG:Release>:/Ob2>"
        "$<$<CONFIG:Release>:/Ot>"
        "$<$<CONFIG:Release>:/Oi>"
        "$<$<CONFIG:Release>:/Oy->"
        )
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -std=c++17
        -Wall
        -Wextra
        -Wno-unused-parameter
        -O3
        )
  endif ()
endfunction()