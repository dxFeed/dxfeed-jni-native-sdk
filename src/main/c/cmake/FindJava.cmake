# Find include directories for Java headers
message("-- Setting up: found JNI and JRE ---")
set(JAVA_HOME $ENV{JAVA_HOME})
message("  JAVA_HOME = " ${JAVA_HOME})

set(JNI_COMMON_INCLUDE ${JAVA_HOME}/include)
message("  JNI_COMMON_INCLUDE = " ${JNI_COMMON_INCLUDE})

if (WIN32)
  set(JNI_INCLUDE ${JNI_COMMON_INCLUDE} ${JNI_COMMON_INCLUDE}/win32)
elseif (APPLE)
  set(JNI_INCLUDE ${JNI_COMMON_INCLUDE} ${JNI_COMMON_INCLUDE}/darwin)
elseif (UNIX)
  set(JNI_INCLUDE ${JNI_COMMON_INCLUDE} ${JNI_COMMON_INCLUDE}/linux)
endif ()

message("  JNI_INCLUDE: ${JNI_INCLUDE}")
include_directories(${PROJECT_NAME} INTERFACE ${JNI_INCLUDE})

set(JAVA_LIB_PATH "${JAVA_HOME}/lib/server")
message("  JAVA_LIB_PATH: ${JAVA_LIB_PATH}")