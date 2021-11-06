/***************************
 *                         *
 *  Make P2P Great Again!  *
 *                         *
 ***************************/

#include <memory>

#include "daemon.h"

int main(int argc, const char ** argv) {
    auto daemon = std::make_unique<p2pd::Daemon>(4);
    return daemon->Run();
}
