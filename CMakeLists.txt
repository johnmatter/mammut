cmake_minimum_required(VERSION 3.16)

project(mammut VERSION 0.61.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Platform-specific settings
if(APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")
  # Only target the current architecture to avoid linking issues with Homebrew libraries
  if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
  else()
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
  endif()
endif()

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(REQUIRED REQUIRED vorbis vorbisfile sndfile samplerate)

# Add JUCE as a subdirectory
add_subdirectory(juce)

# Source files - use wildcards for automatic discovery
file(GLOB_RECURSE SOURCES 
  src/*.cpp
  src/*.c
)

# Exclude build directory files
list(FILTER SOURCES EXCLUDE REGEX ".*build.*")

# Header files - use wildcards for automatic discovery
file(GLOB_RECURSE HEADERS 
  src/*.h
)

# Exclude build directory files
list(FILTER HEADERS EXCLUDE REGEX ".*build.*")

# Create executable
add_executable(mammut ${SOURCES} ${HEADERS})

# Set target properties for macOS bundle
if(APPLE)
  set_target_properties(mammut PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/macosx/Info.plist"
    MACOSX_BUNDLE_BUNDLE_NAME "Mammut"
    MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.mammut.audio"
    MACOSX_BUNDLE_ICON_FILE "icon.icns"
  )
endif()

# Set target properties
target_compile_definitions(mammut PRIVATE
  FOR_MACOSX
  NDEBUG=1
  MAMMUT_VERSION="${PROJECT_VERSION}"
)

# Include directories - use absolute paths
target_include_directories(mammut PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/c_core
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core/utils
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
  ${CMAKE_CURRENT_SOURCE_DIR}/src/audio
  ${CMAKE_CURRENT_SOURCE_DIR}/src/transforms
  ${REQUIRED_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(mammut
  juce::juce_audio_basics
  juce::juce_audio_devices
  juce::juce_audio_formats
  juce::juce_audio_processors
  juce::juce_audio_utils
  juce::juce_core
  juce::juce_data_structures
  juce::juce_events
  juce::juce_graphics
  juce::juce_gui_basics
  juce::juce_gui_extra
  ${REQUIRED_LIBRARIES}
)

# Add library directories
target_link_directories(mammut PRIVATE ${REQUIRED_LIBRARY_DIRS})

# Compiler flags
target_compile_options(mammut PRIVATE
  $<$<C_COMPILER_ID:Clang>:-Wall -Wextra -Wno-deprecated-declarations -Wno-unused-parameter>
  $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wno-deprecated-declarations -Wno-unused-parameter>
)

# Copy resources to build directory for macOS bundle
if(APPLE)
  add_custom_command(TARGET mammut POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_CURRENT_SOURCE_DIR}/doc"
      "$<TARGET_BUNDLE_DIR:mammut>/Contents/Resources/doc"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_CURRENT_SOURCE_DIR}/icons"
      "$<TARGET_BUNDLE_DIR:mammut>/Contents/Resources/icons"
    COMMENT "Copying resources to macOS bundle"
  )
endif()

