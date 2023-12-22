#ifndef LOGGER_H
#define LOGGER_H

struct timeval;

typedef enum LogLevel {
    LogLevelDebug,
    LogLevelInfo,
    LogLevelWarn,
    LogLevelError
} LogLevel;

typedef void (*LogHandler)(LogLevel level, struct timeval *time, const char* file, int line, const char* message);

#define LogDebug(...) LoggerLog(LogLevelDebug, __FILE__, __LINE__, __VA_ARGS__);
#define LogInfo(...) LoggerLog(LogLevelInfo, __FILE__, __LINE__, __VA_ARGS__);
#define LogWarn(...) LoggerLog(LogLevelWarn, __FILE__, __LINE__, __VA_ARGS__);
#define LogError(...) LoggerLog(LogLevelError, __FILE__, __LINE__, __VA_ARGS__);

void LoggerSetHandler(LogHandler handler, LogLevel minLevel, int addPrefix);

void LoggerLog(LogLevel level, const char* file, int line, const char* format, ...);

#endif //LOGGER_H