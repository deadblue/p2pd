#include "log/logger.h"

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace p2pd {
namespace log {

#define ENV_LOG_LEVEL "P2PD_LOG_LEVEL"

Level get_log_level() {
    Level ret = Level::INFO;

    // Get log level from environment
    char * value = std::getenv(ENV_LOG_LEVEL);
    if(value != nullptr) {
        switch (value[0]) {
        case 'V': case 'v': ret = Level::VERBOSE; break;
        case 'D': case 'd': ret = Level::DEBUG; break;
        case 'W': case 'w': ret = Level::WARN; break;
        case 'E': case 'e': ret = Level::ERROR; break;
        case 'X': case 'x': ret = Level::OFF; break;
        default: break;
        }
    }
    return ret;
}

static Level log_level = get_log_level();

class NullBuffer : public std::streambuf {
protected:
    int_type overflow(int_type c) override {
        return c;
    }
};

Logger::Logger(const char * file, int line, Level level) {
    if (level < log_level) {
        buffer_ = std::make_unique<NullBuffer>();
        stream_ = std::make_unique<std::ostream>(buffer_.get());
    } else {
        stream_ = std::make_unique<std::ostringstream>();
        // Prepend datetime
        auto now = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()
        );
        (*stream_) << "[" << std::put_time(std::localtime(&now), "%F %T") 
            << "]";
        // Prepend log level
        switch (level) {
        case VERBOSE: (*stream_) << "[V]"; break;
        case DEBUG:   (*stream_) << "[D]"; break;
        case WARN:    (*stream_) << "[W]"; break;
        case ERROR:   (*stream_) << "[E]"; break;
        default:      (*stream_) << "[I]"; break;
        }
        // Prefix file name and line number
#if defined(SOURCE_ROOT)
        auto * ptr = strstr(file, SOURCE_ROOT);
        if(ptr != nullptr) {
            (*stream_) << "[" << ptr + strlen(SOURCE_ROOT) + 1;
        } else {
            (*stream_) << "[" << file;
        }
#else
        (*stream_) << "[" << file;
#endif
        (*stream_) << "(" << line << ")] ";
    }
}

Logger::~Logger() {
    if(buffer_) {
        return;
    }
    auto * ss = static_cast<std::ostringstream *>(stream_.get());
    std::cout << ss->str() << std::endl;
}

std::ostream & Logger::stream() const {
    return *stream_;
}

} // namespace log
} // namespace p2pd