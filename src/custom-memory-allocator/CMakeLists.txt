SET(TARGET custom-memory-allocator)

FILE(GLOB_RECURSE SRC_FILES "${SRC_DIR}/${TARGET}/*.c" "${SRC_DIR}/${TARGET}/*.cpp")
ADD_LIBRARY(${TARGET} STATIC ${SRC_FILES})

ADD_DEPENDENCIES(dev ${TARGET})

SET_TARGET_PROPERTIES(${TARGET} PROPERTIES C_EXTENSIONS OFF)
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES C_STANDARD 17)
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES C_STANDARD_REQUIRED ON)
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES CXX_EXTENSIONS OFF)
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES CXX_STANDARD 17)
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES CXX_STANDARD_REQUIRED ON)

TARGET_COMPILE_OPTIONS(${TARGET} PUBLIC
  $<$<PLATFORM_ID:Linux>:
    $<$<COMPILE_LANG_AND_ID:C,GNU,Clang>:
      -Wall -Wextra -Wpedantic
      $<$<CONFIG:Debug>: -g -fsanitize=address,undefined>
      $<$<CONFIG:Release>: -O3>
    >
    $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang>:
      -Wall -Wextra -Wpedantic
      $<$<CONFIG:Debug>: -g -fsanitize=address,undefined>
      $<$<CONFIG:Release>: -O3>
    >
  >
  $<$<PLATFORM_ID:Windows>:
    $<$<COMPILE_LANG_AND_ID:C,GNU>:
      -Wall -Wextra -Wpedantic
      $<$<CONFIG:Debug>: -g>
      $<$<CONFIG:Release>: -O3>
    >
    $<$<COMPILE_LANG_AND_ID:C,Clang>:
      -Wall -Wextra -Wpedantic
      $<$<CONFIG:Debug>: -g -fsanitize=address,undefined>
      $<$<CONFIG:Release>: -O3>
    >
    $<$<COMPILE_LANG_AND_ID:C,MSVC>:
      /Wall
      $<$<CONFIG:Debug>: /DEBUG /fsanitize=address,undefined>
      $<$<CONFIG:Release>: /O2>
    >
    $<$<COMPILE_LANG_AND_ID:CXX,GNU>:
      -Wall -Wextra -Wpedantic
      $<$<CONFIG:Debug>: -g>
      $<$<CONFIG:Release>: -O3>
    >
    $<$<COMPILE_LANG_AND_ID:CXX,Clang>:
      -Wall -Wextra -Wpedantic
      $<$<CONFIG:Debug>: -g -fsanitize=address,undefined>
      $<$<CONFIG:Release>: -O3>
    >
    $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:
      /Wall
      $<$<CONFIG:Debug>: /DEBUG /fsanitize=address,undefined>
      $<$<CONFIG:Release>: /O2>
    >
  >
)

TARGET_INCLUDE_DIRECTORIES(${TARGET} PRIVATE ${SRC_DIR} ${INC_DIR})
FILE(GLOB_RECURSE LIB_FILES "${LIB_DIR}/*.a" "${LIB_DIR}/*.so" "${LIB_DIR}/*.lib")
TARGET_LINK_LIBRARIES(${TARGET} PRIVATE ${LIB_FILES})

FIND_PACKAGE(Qt5 COMPONENTS Core Quick REQUIRED)
TARGET_INCLUDE_DIRECTORIES(${TARGET} PRIVATE Qt)
TARGET_LINK_LIBRARIES(${TARGET} PRIVATE ${Qt5Core_LIBRARIES} ${Qt5Quick_LIBRARIES})

FIND_PACKAGE(Boost REQUIRED)
TARGET_INCLUDE_DIRECTORIES(${TARGET} PRIVATE ${Boost_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${TARGET} PRIVATE ${Boost_LIBRARIES})

FIND_PACKAGE(Vulkan REQUIRED)
TARGET_INCLUDE_DIRECTORIES(${TARGET} PRIVATE ${Vulkan_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${TARGET} PRIVATE ${Vulkan_LIBRARIES})

FIND_PACKAGE(GTest REQUIRED)
TARGET_INCLUDE_DIRECTORIES(${TARGET} PRIVATE ${GTest_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${TARGET} PRIVATE ${GTest_LIBRARIES})
