#include "engine/error_code.h"

namespace p2pd {
namespace engine {

struct ErrorCategory final : boost::system::error_category {
    const char * name() const BOOST_SYSTEM_NOEXCEPT final;
    std::string message(int code) const BOOST_SYSTEM_NOEXCEPT final;
};
const char * ErrorCategory::name() const BOOST_SYSTEM_NOEXCEPT {
    return "p2pd.engine";
}
std::string ErrorCategory::message(int code) const BOOST_SYSTEM_NOEXCEPT {
    static const char * messages[] = {
        "Unknown",
        "Invalid magnet URI",
        "Invalid torrent data",
        "Invalid task ID",
        "No such task"
    };
    if(code <= errors::begin || code >= errors::end) {
        return messages[0];
    }
    return messages[code >> 8];
}

boost::system::error_category& engine_error_category() {
    static ErrorCategory category;
    return category;
}

error_code make_error_code(errors e) {
    return { e, engine_error_category() };
}

} // namespace engine
} // namespace p2pd