
//==========================================================================================================
// RtlQsfpStatus.cpp - Implements an interface to the QSFP status registers
//==========================================================================================================
#include <unistd.h>
#include <string.h>
#include "RtlQsfpStatus.h"


//==========================================================================================================
// getStatus() - Returns the raw status bits for the specified QSFP channel
//==========================================================================================================
uint32_t RtlQsfpStatus::getStatus(int channel)
{
    // We only care about the bottom bit of 'channel' (i.e., channel should be 0 or 1)
    channel = channel & 1;

    // Return the status bits for this QSFP channel
    return *baseAddr_[channel];
}
//==========================================================================================================


//==========================================================================================================
// checkStatus() - Checks to see if the status bits of the QSFP channel indicate "all good"
//
// If the QSFP channel isn't up, this routine either throws a std::runtime_error, or returns false
//==========================================================================================================
bool RtlQsfpStatus::checkStatus(int channel)
{
    uint32_t statusGood;

    // Ensure that 'channel' is either 0 or 1
    channel = channel & 1;

    // Determine which bits must be on for the status of this QSFP channel to be "good"
    if (channel == 0)
        statusGood = MASK_CHANNEL_UP | MASK_PLL_LOCK; 
    else
        statusGood = MASK_CHANNEL_UP | MASK_PLL_LOCK | MASK_C2C_UP;

    // Fetch the status-bits for this QSFP channel
    uint32_t status = getStatus(channel);

    // Tell the caller whether or not this channel is up
    return (status & statusGood) == statusGood;
}
//==========================================================================================================
