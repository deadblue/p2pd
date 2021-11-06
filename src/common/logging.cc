#include "common/logging.h"

#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>

namespace p2pd {
namespace common {

Logging::Logging(const char *file, int line) {
    std::string filename(file);
#if defined(SOURCE_ROOT)
    auto index = filename.find(SOURCE_ROOT, 0);
    if(index >= 0) {
        filename.erase(0, index + strlen(SOURCE_ROOT) + 1);
    }
#else
    auto index = filename.find_last_of('/');
    if (index >= 0) {
        filename.erase(0, index + 1);
    }
#endif
    auto now = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
    );
    
    stream_ << "[" << std::put_time(std::localtime(&now), "%F %T") 
        << "]["<< filename << "(" << line << ")] ";
}

Logging::~Logging() {
    std::cerr << stream_.str() << std::endl;
}

} // namespace common
} // namespace p2pd