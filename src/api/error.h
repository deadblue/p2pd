#ifndef P2PD_API_ERROR_H
#define P2PD_API_ERROR_H

namespace p2pd {
namespace api {

enum errors {
    // ----- Internal errors -----
    unsupported_method = -1,
    invalid_parameters = -2,

    ok = 0, // Horizon

    // ----- Service errors -----
};

} // namespace api
} // namespace p2pd

#endif // P2PD_API_ERROR_H