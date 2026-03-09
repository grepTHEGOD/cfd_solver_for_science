#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

namespace cfd {

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    static Logger& instance() {
        static Logger instance;
        return instance;
    }

    void set_level(LogLevel level) { min_level_ = level; }
    LogLevel level() const { return min_level_; }

    void debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }
    void info(const std::string& msg) { log(LogLevel::INFO, msg); }
    void warning(const std::string& msg) { log(LogLevel::WARNING, msg); }
    void error(const std::string& msg) { log(LogLevel::ERROR, msg); }

    template<typename... Args>
    void log(LogLevel level, const std::string& fmt, Args&&... args) {
        if (level < min_level_) return;
        
        std::ostringstream oss;
        ((oss << std::forward<Args>(args)), ...);
        log(level, oss.str());
    }

    void log(LogLevel level, const std::string& msg) {
        if (level < min_level_) return;
        
        std::ostringstream oss;
        oss << "[" << level_string(level) << "] " << msg;
        
        std::cout << oss.str() << std::endl;
    }

private:
    Logger() = default;
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string level_string(LogLevel level) const {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARN";
            case LogLevel::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }

    LogLevel min_level_ = LogLevel::INFO;
};

#define LOG_DEBUG(...) cfd::Logger::instance().log(cfd::LogLevel::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) cfd::Logger::instance().log(cfd::LogLevel::INFO, __VA_ARGS__)
#define LOG_WARNING(...) cfd::Logger::instance().log(cfd::LogLevel::WARNING, __VA_ARGS__)
#define LOG_ERROR(...) cfd::Logger::instance().log(cfd::LogLevel::ERROR, __VA_ARGS__)

}
