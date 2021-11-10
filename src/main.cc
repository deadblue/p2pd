/***************************
 *
 *  Make P2P Great Again!
 *
 ***************************/

#include <memory>

#include "options.h"
#include "daemon.h"

int main(int argc, char * argv[]) {
    auto options = p2pd::Options();
    if( !options.Parse(argc, argv) || options.help ) {
        options.PrintUsage();
        exit(0);
    }

    auto daemon = std::make_unique<p2pd::Daemon>(options);
    return daemon->Run();
}
