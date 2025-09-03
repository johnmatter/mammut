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

# JUCE setup
add_subdirectory(juce)

# Source files
set(SOURCES
  src/globals.c
  src/load.c
  src/fft.c
  src/analysett.c
  src/save.c
  src/phaseswap.c
  src/crossover.c
  src/loadmult.c
  src/undo.c
  src/jackplay.c
  src/c_interface.c
  src/ApplicationStartup.cpp
  src/MainAppWindow.cpp
  src/Interface.cpp
  src/gui.cpp
  src/Stretch.cpp
  src/Wobble.cpp
  src/MultiplyPhase.cpp
  src/DerivativeAmp.cpp
  src/Filter.cpp
  src/Invert.cpp
  src/Threshold.cpp
  src/SpectrumShift.cpp
  src/AmplitudeToPhase.cpp
  src/Gain.cpp
  src/CombSplit.cpp
  src/SplitRealImag.cpp
  src/KeepPeaks.cpp
  src/BlockSwap.cpp
  src/Mirror.cpp
  src/Stereo.cpp
  src/juceplay.cpp
  src/tempfile.cpp
  src/Progressbar.cpp
  src/Zoom.cpp
  src/oggsoundholder.cpp
  src/Prefs.cpp
  src/error.cpp
  src/PictureHolder.cpp
  src/transform/t_stretch.c
  src/transform/t_wobble.c
  src/transform/t_sshift.c
  src/transform/t_phadd.c
  src/transform/t_pderiv.c
  src/transform/t_filter.c
  src/transform/t_invert.c
  src/transform/t_threshold.c
  src/transform/t_peaks.c
  src/transform/t_blockmov.c
  src/transform/t_gain.c
  src/transform/t_combsplit.c
  src/transform/t_reimsplit.c
  src/transform/t_mirror.c
  src/transform/t_ampphas.c
)

# Header files
set(HEADERS
  src/mammut.h
  src/transformheader.h
  src/transform/mammut.h
  src/MainHeader.h
  src/GraphComponent.h
  src/MainAppWindow.h
  src/Interface.h
  src/PictureHolder.h
  src/Prefs.h
  src/Stretch.h
  src/Wobble.h
  src/MultiplyPhase.h
  src/DerivativeAmp.h
  src/Filter.h
  src/Invert.h
  src/Threshold.h
  src/SpectrumShift.h
  src/AmplitudeToPhase.h
  src/Gain.h
  src/CombSplit.h
  src/SplitRealImag.h
  src/KeepPeaks.h
  src/BlockSwap.h
  src/Mirror.h
  src/Stereo.h
  src/juceplay.h
  src/tempfile.h
  src/undo.h
  src/jackplay.h
  src/oggsoundholder.h
  src/AppSettings.h
  src/DasButtons.h
  src/DasTabbedComponent.h
  src/gui.h
  src/mysndfile.h
  src/Zoom.h
)

# Create executable
add_executable(mammut ${SOURCES} ${HEADERS})

# Set target properties
target_compile_definitions(mammut PRIVATE
  FOR_MACOSX
  NDEBUG=1
  MAMMUT_VERSION="${PROJECT_VERSION}"
)

# Include directories
target_include_directories(mammut PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
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

# Install target
install(TARGETS mammut
  RUNTIME DESTINATION bin
  BUNDLE DESTINATION Applications
)

# Copy resources
install(FILES
  doc/mammuthelp.html
  NEW
  DESTINATION share/mammut
)

# Copy icons
install(FILES
  icons/icon.ico
  icons/icon.png
  DESTINATION share/mammut/icons
)
