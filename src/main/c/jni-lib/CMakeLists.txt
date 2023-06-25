cmake_minimum_required(VERSION 3.15)
#set(CMAKE_OSX_ARCHITECTURES arm64)
project(native_jni)

set(CMAKE_CXX_STANDARD 17)

message("JNI_INCLUDE: ${JNI_INCLUDE}")
include_directories(${PROJECT_NAME} INTERFACE ${JNI_INCLUDE})

set(API_INCLUDE include)
include_directories(${PROJECT_NAME} INTERFACE ${API_INCLUDE})

message("-- Linking native_lib")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CRITICAL_SECTION_IMPL src/utils/vm/CriticalSectionWindows.cpp)
else ()
  set(CRITICAL_SECTION_IMPL src/utils/vm/CriticalSectionMacOS.cpp)
endif ()

set(SOURCE_FILES
    src/dllmain.cpp
    src/DxEndpoint.cpp
    src/DxEndpointBuilder.cpp
    src/DxFeed.cpp
    src/DxIndexedEventSource.cpp
    src/DxEventT.cpp
    src/DxSubscription.cpp
    src/DxSymbol.cpp
    src/api/dxfg_api.cpp
    src/api/dxfg_dxfeed.cpp
    src/api/dxfg_endpoint.cpp
    src/api/dxfg_events.cpp
    src/api/dxfg_subscription.cpp
    src/api/dxfg_system.cpp
    src/api/graal_isolate.cpp
    src/listeners/DxEventListener.cpp
    src/listeners/DxStateChangeListener.cpp
    src/utils/JNIBridgeImpl.cpp
    src/utils/JNIUtils.cpp
    src/utils/LoadJvmLib.cpp
    src/utils/NativeEventData.cpp
    src/utils/NativeEventReader.cpp
    src/utils/java/DxFeedJniClass.cpp
    src/utils/java/JavaLangClass.cpp
    src/utils/java/JavaLangSystem.cpp
    src/utils/vm/InitJavaVm.cpp
    src/utils/vm/JavaVmInstance.cpp
    ${CRITICAL_SECTION_IMPL}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

set(JAVA_LIB_PATH "${JAVA_HOME}/lib/server")
message("JAVA_LIB_PATH: ${JAVA_LIB_PATH}")

link_directories(${JAVA_LIB_PATH})

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
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