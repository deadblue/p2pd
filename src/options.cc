#include "options.h"

#include <getopt.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>

namespace p2pd {

static struct option long_options[] = {
    {"help",    no_argument, nullptr, 'h'},
    {"version", no_argument, nullptr, 'v'},

    {"save-dir",      required_argument, nullptr, 'S'},
    {"ip",            required_argument, nullptr, 'I'},
    {"bt-port",       required_argument, nullptr, 'b'},
    {"tracker-list",  required_argument, nullptr, 'T'},
    {"dht-node-list", required_argument, nullptr, 'D'},
    {"api-port",      required_argument, nullptr, 'a'},
    {"api-addr",      required_argument, nullptr, 'A'},
    // {"cli-port",      required_argument, nullptr, 'c'},
    // {"conf-file",     required_argument, nullptr, 'C'},
    {nullptr, 0, 0, 0}
};

static const char * short_options = "hvS:I:b:T:D:a:A:";

static const char * usage = 
    "Usage: p2pd [options]\n"
    "\n"
    "Common options:\n"
    "  -h, --help     Print help message and exit.\n"
    "  -v, --version  Print version information and exit.\n"
    "\n"
    "P2P options:\n"
    "  -S, --save-dir <dir>  Directory where p2pd downloads files into.\n"
    "  -I, --ip <ip-addr>    Public IP address for incomming connections.\n"
    "\n"
    "BitTorrent options:\n"
    "  -b, --bt-port <port>         Listen port for peer connection.\n"
    "  -T, --tracker-list <file>    Tracker list file.\n"
    "  -D, --dht-node-list <file>   DHT bootstrap node list file.\n"
    "\n"
    "API server options:\n"
    "  -a, --api-port <port>  API server listening port.\n"
    "  -A, --api-addr <addr>  API server listening address.\n"
    // "CLI options:\n"
    // "  -c, --cli-port <port>  Command Line Interface listening port.\n"
    // "  -C, --conf-file <config-file>  Directory where p2pd downloads files into.\n"
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
        case 'h': help = true; break;
        case 'v': version = true; break;

        case 'S': save_dir = optarg; break;
        case 'I': ip = optarg; break;

        case 'b': bt_port = atoi(optarg); break;
        case 'T': tracker_list = optarg; break;
        case 'D': dht_node_list = optarg; break;

        case 'a': api_port = atoi(optarg); break;
        case 'A': api_addr = optarg; break;

        // case 'c': cli_port = atoi(optarg); break;
        // case 'C': conf_file = optarg; break;

        case '?': break;
        default: break;
        }
    }

    FillDefaultValue();

    return true;
}

void Options::FillDefaultValue() {
    if(api_addr.empty()) {
        api_addr = "0.0.0.0";
    }
    if(save_dir.empty()) {
        save_dir = getenv("HOME");
    }
}

void Options::PrintUsage() {
    std::cout << usage << std::endl;
}

} // namespace p2pd