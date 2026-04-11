#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <stdarg.h>

class Logger {
public:
    enum Level { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };

    // Singleton instance
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void begin(long baud = 115200) {
        Serial.begin(baud);
        while (!Serial) {;} // Ensure Serial is ready (useful for some boards)
    }

    void setLevel(Level level) { _currentLevel = level; }

    // Main log function (simple message)
    void log(Level level, const char* module, const char* message) {
        if (level < _currentLevel) return;

        printPrefix(level, module);
        Serial.println(message);
    }

    // Formatted log (like printf)
    void logf(Level level, const char* module, const char* format, ...) {
        if (level < _currentLevel) return;

        printPrefix(level, module);

        char buffer[128]; // Adjust size if needed
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        Serial.println(buffer);
    }

private:
    Logger() : _currentLevel(INFO) {}
    Level _currentLevel;

    void printPrefix(Level level, const char* module) {
        Serial.print("[");
        Serial.print(millis());  // timestamp
        Serial.print("][");
        Serial.print(getLevelString(level));
        Serial.print("][");
        Serial.print(module);
        Serial.print("] ");
    }

    const char* getLevelString(Level level) {
        switch (level) {
            case TRACE: return "TRACE";
            case DEBUG: return "DEBUG";
            case INFO:  return "INFO";
            case WARN:  return "WARN";
            case ERROR: return "ERROR";
            case FATAL: return "FATAL";
            default:    return "LOG";
        }
    }

    // Prevent copy
    Logger(Logger const&) = delete;
    void operator=(Logger const&) = delete;
};

#endif