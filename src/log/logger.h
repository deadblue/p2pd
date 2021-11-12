#ifndef P2PD_LOG_LOGGER_H
#define P2PD_LOG_LOGGER_H

#include <memory>
#include <ostream>

#include "log/level.h"

namespace p2pd {
namespace log {

class Logger {

private:
    std::unique_ptr<std::streambuf> buffer_;
    std::unique_ptr<std::ostream> stream_;

public:
    Logger(
        const char * file, int line, 
        Level level=Level::INFO
    );
    ~Logger();

    std::ostream & stream() const;

};

} // namespace log
} // namespace p2pd

#endif // P2PD_LOG_LOGGER_H