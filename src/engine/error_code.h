#ifndef P2PD_ENGINE_ERROR_H
#define P2PD_ENGINE_ERROR_H

#include <boost/system/error_code.hpp>

namespace p2pd {
namespace engine {

using error_code = boost::system::error_code;

enum errors {
    // Begin code of errors
    begin = 0x0100,

    invalid_magnet_uri,
    invalid_torrent_data,
    invalid_task_id,
    no_such_task,

    // End code of errors
    end
};

error_code make_error_code(errors e);

} // namespace engine
} // namespace p2pd


namespace boost {
namespace system {

template<> 
struct is_error_code_enum<p2pd::engine::errors>{
    static const bool value = true; 
};

} // namespace system
} // namespace boost

#endif // P2PD_ENGINE_ERROR_H