#ifndef MAIN_H
#define MAIN_H 1

#include <stdint.h>
#include <stdbool.h>
#include "nordic_common.h"
#include "nrf.h"

#include "stream.h"

#define STREAM_BUF_LEN 64
#define NUS_PACKET_LEN 20

extern stream_t m_txs;
extern stream_t m_rxs;

#endif
/*
** EOF
*/
