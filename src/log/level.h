#ifndef LOG_LEVEL_H
#define LOG_LEVEL_H

namespace p2pd {
namespace log {

enum Level {
    VERBOSE = 0,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    OFF
};

} // namespace log
} // namespace p2pd

#endif // LOG_LEVEL_H