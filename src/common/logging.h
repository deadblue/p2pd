#ifndef P2PD_COMMON_LOGGING_H
#define P2PD_COMMON_LOGGING_H

#include <sstream>

#define LOG \
    ::p2pd::common::Logging(__FILE__, __LINE__).Stream()

namespace p2pd {
namespace common {

class Logging {
public:
    Logging(const char * file, int line);
    ~Logging();

    std::ostringstream& Stream() { return stream_; }

private:
    std::ostringstream stream_;

};

} // namespace common
} // namespace p2pd

#endif // P2PD_COMMON_LOGGING_H
