#ifndef MAMMUT_LOGGING_H
#define MAMMUT_LOGGING_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <chrono>
#endif

// Compile-time configuration
#ifndef MAMMUT_LOG_ENABLED
#define MAMMUT_LOG_ENABLED 1
#endif

#ifndef MAMMUT_LOG_COLOR_ENABLED
#define MAMMUT_LOG_COLOR_ENABLED 1
#endif

#ifndef MAMMUT_LOG_TIMESTAMP_ENABLED
#define MAMMUT_LOG_TIMESTAMP_ENABLED 1
#endif

#ifndef MAMMUT_LOG_FILE_INFO_ENABLED
#define MAMMUT_LOG_FILE_INFO_ENABLED 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Log levels
typedef enum {
  MAMMUT_LOG_LEVEL_ERROR = 0,
  MAMMUT_LOG_LEVEL_WARN = 1,
  MAMMUT_LOG_LEVEL_INFO = 2,
  MAMMUT_LOG_LEVEL_DEBUG = 3,
  MAMMUT_LOG_LEVEL_TRACE = 4
} mammut_log_level_t;


// Global log level - can be set at runtime
extern mammut_log_level_t mammut_current_log_level;

// High-resolution timestamp function - consistent across C and C++
static inline uint64_t mammut_get_timestamp_us(void) {
  // Use a static program start time for consistency across C and C++
  static uint64_t program_start_us = 0;
  static bool initialized = false;
  
  if (!initialized) {
#ifdef __cplusplus
    // Initialize with C++ high resolution clock - convert to microseconds
    auto start_time = std::chrono::high_resolution_clock::now();
    auto epoch = start_time.time_since_epoch();
    auto epoch_us = std::chrono::duration_cast<std::chrono::microseconds>(epoch);
    program_start_us = static_cast<uint64_t>(epoch_us.count());
#else
    // Initialize with C monotonic clock
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    program_start_us = (uint64_t)ts.tv_sec * 1000000ULL + (uint64_t)ts.tv_nsec / 1000ULL;
#endif
    initialized = true;
  }
  
  // Get current time and subtract program start time
#ifdef __cplusplus
  auto now = std::chrono::high_resolution_clock::now();
  auto epoch = now.time_since_epoch();
  auto epoch_us = std::chrono::duration_cast<std::chrono::microseconds>(epoch);
  uint64_t current_us = static_cast<uint64_t>(epoch_us.count());
#else
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  uint64_t current_us = (uint64_t)ts.tv_sec * 1000000ULL + (uint64_t)ts.tv_nsec / 1000ULL;
#endif
  
  return current_us - program_start_us;
}

// Log level names
static const char* mammut_log_level_names[] = {
  "ERROR",
  "WARN ",
  "INFO ",
  "DEBUG",
  "TRACE"
};

// Colors for terminal output (ANSI escape codes)
static const char* mammut_log_colors[] = {
  "\033[31m", // ERROR - Red
  "\033[33m", // WARN  - Yellow
  "\033[32m", // INFO  - Green
  "\033[36m", // DEBUG - Cyan
  "\033[35m"  // TRACE - Magenta
};

static const char* mammut_log_color_reset = "\033[0m";

// Internal logging function
static inline void mammut_log_internal(mammut_log_level_t level, const char* file, int line, const char* func, const char* format, ...) {
#if MAMMUT_LOG_ENABLED
  if (level > mammut_current_log_level) {
    return;
  }

  // Print high-resolution timestamp if enabled
#if MAMMUT_LOG_TIMESTAMP_ENABLED
  uint64_t timestamp_us = mammut_get_timestamp_us();
  printf("[%8llu] ", (unsigned long long)timestamp_us);
#endif

  // Print log level with color if enabled
#if MAMMUT_LOG_COLOR_ENABLED
  printf("%s%s%s ", mammut_log_colors[level], mammut_log_level_names[level], mammut_log_color_reset);
#else
  printf("%s ", mammut_log_level_names[level]);
#endif

  // Print file info if enabled
#if MAMMUT_LOG_FILE_INFO_ENABLED
  const char* filename = strrchr(file, '/');
  filename = filename ? filename + 1 : file;
  printf("%s:%d %s() ", filename, line, func);
#endif

  // Print the actual message
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  
  printf("\n");
  fflush(stdout);
#endif // MAMMUT_LOG_ENABLED
}

// Public logging macros
#if MAMMUT_LOG_ENABLED
#define MAMMUT_LOG_ERROR(format, ...) \
  mammut_log_internal(MAMMUT_LOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#define MAMMUT_LOG_WARN(format, ...) \
  mammut_log_internal(MAMMUT_LOG_LEVEL_WARN, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#define MAMMUT_LOG_INFO(format, ...) \
  mammut_log_internal(MAMMUT_LOG_LEVEL_INFO, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#define MAMMUT_LOG_DEBUG(format, ...) \
  mammut_log_internal(MAMMUT_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#define MAMMUT_LOG_TRACE(format, ...) \
  mammut_log_internal(MAMMUT_LOG_LEVEL_TRACE, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#else
// When logging is disabled, these macros do nothing
#define MAMMUT_LOG_ERROR(format, ...) ((void)0)
#define MAMMUT_LOG_WARN(format, ...) ((void)0)
#define MAMMUT_LOG_INFO(format, ...) ((void)0)
#define MAMMUT_LOG_DEBUG(format, ...) ((void)0)
#define MAMMUT_LOG_TRACE(format, ...) ((void)0)
#endif

// Convenience macros for common use cases
#define MAMMUT_LOG_ENTRY() MAMMUT_LOG_DEBUG("Entering function")
#define MAMMUT_LOG_EXIT() MAMMUT_LOG_DEBUG("Exiting function")
#define MAMMUT_LOG_CALLED() MAMMUT_LOG_DEBUG("Function called")

// Conditional logging macros - only log if condition is true
// Examples:
//   MAMMUT_LOG_DEBUG_IF(error_count > 0, "Found %d errors", error_count);
//   MAMMUT_LOG_INFO_IF(is_verbose_mode, "Verbose mode enabled");
//   MAMMUT_LOG_DEBUG_IF(buffer_size > 1024, "Large buffer: %d bytes", buffer_size);
#define MAMMUT_LOG_DEBUG_IF(condition, format, ...) \
  do { if (condition) MAMMUT_LOG_DEBUG(format, ##__VA_ARGS__); } while(0)

#define MAMMUT_LOG_INFO_IF(condition, format, ...) \
  do { if (condition) MAMMUT_LOG_INFO(format, ##__VA_ARGS__); } while(0)

// Function to set log level at runtime
static inline void mammut_set_log_level(mammut_log_level_t level) {
  mammut_current_log_level = level;
}

// Function to get current log level
static inline mammut_log_level_t mammut_get_log_level(void) {
  return mammut_current_log_level;
}

// Initialize logging system (sets default log level)
static inline void mammut_log_init(void) {
  mammut_current_log_level = MAMMUT_LOG_LEVEL_DEBUG;
}

#ifdef __cplusplus
}
#endif

#endif // MAMMUT_LOGGING_H
