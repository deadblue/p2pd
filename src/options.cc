#include "options.h"

#include <cstdlib>
#include <getopt.h>
#include <iostream>

namespace p2pd {

static struct option long_options[] = {
    {"help", no_argument, nullptr, 'h'},
    {"version", no_argument, nullptr, 'v'},
    {"bt-port",  required_argument, nullptr, 'b'},
    {"api-port", required_argument, nullptr, 'a'},
    {"cli-port", required_argument, nullptr, 'c'},
    {"pool-size", required_argument, nullptr, 'p'},
    {"save-dir", required_argument, nullptr, 'S'},
    {nullptr, 0, 0, 0}
};
static const char * short_options = "hb:a:c:p:";

static const char * usage = 
    "Usage: p2pd [options]\n"
    "options:\n"
    "  -h, --help     Print help message and exit.\n"
    //"  -v, --version  Print version information and exit.\n"
    "\n"
    "  -b, --bt-port <port>   BitTorrent listening port for peer connection.\n"
    "  -a, --api-port <port>  API server listening port.\n"
    "  -c, --cli-port <port>  Command Line Interface listening port.\n"
    "\n"
    "  -S, --save-dir <dir>   Directory where p2pd downloads files into.\n"
    ;

bool Options::Parse(int argc, char * argv[]) {
    // Disable error output
    opterr = 0;

    int option_index;
    while (true) {
        int c = getopt_long(argc, argv, 
            short_options, long_options, &option_index
        );
        if(c == -1) { break; }

        switch (c) {
        case 'h':
            help = true;
            break;
        case 'a':
            api_port = atoi(optarg);
            break;
        case 'b':
            bt_port = atoi(optarg);
            break;
        case 'c':
            cli_port = atoi(optarg);
            break;
        case 'p':
            pool_size = atoi(optarg);
            break;
        case 'S':
            save_dir = optarg;
            break;
        case '?':
            break;
        default:
            break;
        }
    }

    return true;
}

void Options::PrintUsage() {
    std::cout << usage << std::endl;
}

} // namespace p2pd