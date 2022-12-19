//==========================================================================================================
// RtlDataControl.cpp - Implements an interface to the data-control RTL module
//==========================================================================================================
#include <string.h>
#include "RtlDataControl.h"


//------------------------------------------------------------------
// These are the valid registers, as offsets from the base address
//------------------------------------------------------------------
enum
{

    REG_BUFFH     = 0,   // Hi 32 bits of the PC's contiguous buffer
    REG_BUFFL     = 1,   // Lo 32 bits of the PC's contiguous buffer
    REG_BUFF_SIZE = 2,   // Contiguous buffer size in 2048-byte blocks
    REG_START     = 3,   // A write to this register starts data transfer
};
//------------------------------------------------------------------



//==========================================================================================================
// start() - Pre-loads the RTL FIFO with data from our ping-pong buffers, and prepares the RTL design
//           to start sending data over QSFP as requested by the ECD
//
// Passed: physAddr = Physical address of the PC's contiguous buffer
//         buffSize = The size of each buffer in units of 2048 byte blocks
//==========================================================================================================
void RtlDataControl::start(uint64_t physAddr, uint32_t buffSize)
{
    // Give our AXI slave the physical addresses of our ping-pong buffers
    baseAddr_[REG_BUFFH] = (physAddr >> 32) & 0xFFFFFFFF;
    baseAddr_[REG_BUFFL] = (physAddr      ) & 0xFFFFFFFF;    

    // Give our AXI slave the size of those buffers, in units of 2048 byte blocks
    baseAddr_[REG_BUFF_SIZE] = buffSize;

    // Start the data transfer
    baseAddr_[REG_START] = 1;
}
//==========================================================================================================

