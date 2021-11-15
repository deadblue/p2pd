#ifndef P2PD_OPTIONS_H
#define P2PD_OPTIONS_H

#include <cstdint>
#include <string>

namespace p2pd {

struct Options {
    bool help;
    bool version;

    std::string save_dir;
    std::string ip;

    uint16_t bt_port = 6881;
    std::string tracker_list;
    std::string dht_node_list;

    uint16_t api_port = 9066;
    std::string api_addr;

    /**
     * Parse options from command line.
     * 
     * @param argc
     * @param argv
     * 
     * @return Operation result.
     */
    bool Parse(int argc, char * argv[]);

    /**
     * Print command line usage.
     */
    void PrintUsage();

private:
    void FillDefaultValue();

};

} // namespace p2pd

#endif // P2PD_OPTIONS_H