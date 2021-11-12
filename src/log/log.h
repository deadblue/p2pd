#ifndef P2PD_LOG_LOG_H
#define P2PD_LOG_LOG_H

#include "logger.h"

#define LOG \
    ::p2pd::log::Logger(__FILE__, __LINE__).stream()

#define VLOG \
    ::p2pd::log::Logger(__FILE__, __LINE__, p2pd::log::VERBOSE).stream()

#define DLOG \
    ::p2pd::log::Logger(__FILE__, __LINE__, p2pd::log::DEBUG).stream()

#define WLOG \
    ::p2pd::log::Logger(__FILE__, __LINE__, p2pd::log::WARN).stream()

#define ELOG \
    ::p2pd::log::Logger(__FILE__, __LINE__, p2pd::log::ERROR).stream()

#endif // P2PD_LOG_LOG_H