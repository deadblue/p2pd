#ifndef P2PD_API_SERVER_SESSION_HOST_H
#define P2PD_API_SERVER_SESSION_HOST_H

#include <cstdint>
#include <string>

namespace p2pd {
namespace api {

// Declare seesion ID type.
using session_id = uintptr_t;

/**
 * @brief   SessionHost is the onwer of Session, which receives events 
 *          from session.
 * 
 * @author  deadblue
 */
class SessionHost {
public:
    /**
     * @brief Fired when session received a text message from client.
     * 
     * @param id       Session ID
     * @param message  Message received from client.
     */
    virtual void OnSessionMessage(
        session_id id, std::string message) = 0;

    /**
     * @brief Fired when a session has been closed.
     * 
     * @param id  Session ID
     */
    virtual void OnSessionClose(session_id id) = 0;
};

} // namespace api
} // namespace p2pd

#endif // P2PD_API_SERVER_SESSION_HOST_H
