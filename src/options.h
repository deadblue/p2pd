#ifndef P2PD_OPTIONS_H
#define P2PD_OPTIONS_H

#include <cstdint>
#include <string>

namespace p2pd {

struct Options {
    bool help;
    uint16_t bt_port = 6881;
    uint16_t api_port = 9066;
    uint16_t cli_port = 9068;
    uint8_t pool_size = 4;

    std::string save_dir;

    bool Parse(int argc, char * argv[]);
    void PrintUsage();
};

} // namespace p2pd

#endif // P2PD_OPTIONS_H