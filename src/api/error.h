#ifndef P2PD_API_ERROR_H
#define P2PD_API_ERROR_H

namespace p2pd {
namespace api {

enum Error {
    // ----- Internal errors -----
    UnsupportedMethod = -1,
    InvalidParameters = -2,

    OK = 0, // Horizon

    // ----- Service errors -----
};

} // namespace api
} // namespace p2pd

#endif // P2PD_API_ERROR_H