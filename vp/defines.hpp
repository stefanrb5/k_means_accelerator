#ifndef TYPEDEFS_HPP
#define TYPEDEFS_HPP

#define int64 opencv_int64
#define uint64 opencv_uint64

#include <opencv2/core.hpp>
#undef int64
#undef uint64

#define SC_INCLUDE_FX
#include <tlm>
#include <sysc/datatypes/fx/sc_fixed.h>


/*
#define W 16
#define I_1 10
#define I_2 16

#define PRINTS

typedef sc_dt::sc_fixed <16,10> hard_1t;
typedef sc_dt::sc_fixed <16,16> hard_2t;*/

typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
typedef tlm::tlm_base_protocol_types::tlm_phase_type ph_t;

//registers in Hard
#define ADDR_ROWS 0x00
#define ADDR_COLS 0x01
#define ADDR_CLUSTER_NUMBERS 0x02
#define ADDR_START 0x03
#define ADDR_READY 0x04

//bram size 240KB
#define BRAM_SIZE 0x3A980

//macro for offset
#define DELAY 10

//32-bit data bus, 4 bytes
#define BUS_WIDTH 4

#define VP_ADDR_BRAM_L 0x00000000
#define VP_ADDR_BRAM_H 0x00000000 + BRAM_SIZE

#define VP_ADDR_IP_HARD_L 0x40000000
#define VP_ADDR_IP_HARD_H 0x4000000F

#endif	 //TYPEDEFS_HPP
	
