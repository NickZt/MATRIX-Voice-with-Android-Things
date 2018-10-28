# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)
# Resolve Android Things headers and libraries
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/libs/native-libandroidthings-1.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_DIR}/libandroidthings)
#find_package(FindAndroidThings REQUIRED)
# Resolve Android Things headers and libraries
find_package(AndroidThings REQUIRED)

# Create lists with all source files
file(GLOB jni_SRCS "${CMAKE_SOURCE_DIR}/src/main/cpp/*.cpp")
file(GLOB driver_SRCS "${CMAKE_SOURCE_DIR}/src/main/cpp/driver/*.cpp")
file(GLOB demos_SRCS "${CMAKE_SOURCE_DIR}/src/main/cpp/demos/*.cpp")
file(GLOB fft_SRCS "${CMAKE_SOURCE_DIR}/src/main/cpp/fft/*.c")

include_directories(${ANDROIDTHINGS_INCLUDE_DIRS})


set_property(SOURCE ${jni_SRCS}
        APPEND_STRING PROPERTY COMPILE_FLAGS "-O3")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${jni_SRCS}
        ${driver_SRCS}
        ${demos_SRCS}
        ${fft_SRCS}
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-lib
        ${ANDROIDTHINGS_LIBRARIES}
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})