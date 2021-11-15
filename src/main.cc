/***************************
 *
 *  Make P2P Great Again!
 *
 ***************************/

#include <iostream>
#include <memory>

#include "daemon.h"
#include "options.h"

int main(int argc, char * argv[]) {
    p2pd::Options options{};
    if( !options.Parse(argc, argv) || options.help ) {
        options.PrintUsage();
        return 0;
    }
    if(options.version) {
        p2pd::Daemon::PrintVersion();
        return 0;
    }

    auto daemon = std::make_unique<p2pd::Daemon>(options);
    return daemon->Run();
}
