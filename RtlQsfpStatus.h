//==========================================================================================================
// RtlQsfpStatus.h - Defines an interface to the QSFP status registers
//==========================================================================================================
#pragma once
#include <stdint.h>
#include <string>
using std::string;

class RtlQsfpStatus
{
public:

    // This are the bit-mask values for the value returned by getStatus()
    enum
    {
        MASK_LANE_UP    = 0x0000F,
        MASK_CHANNEL_UP = 0x00010,
        MASK_PLL_LOCK   = 0x00020
    };

    // Call this to set the base address of the RTL module for each channel
    void     setBaseAddress(uint8_t* p) {baseAddr_ = (volatile uint32_t*)p;}

    // Call this to return the status bits, with no automatic error checking
    uint32_t getStatus(int channel);

    // Call this to determine whether the specified QSFP channel is up and functioning
    bool     checkStatus(int channel);

protected:

    volatile uint32_t* baseAddr_;
};

