//==========================================================================================================
// ecdproxy.h - Defines a class that manages the ECD hardware
//
// General flow of operations:
//
//   [1] init()                - To read the configuration file
//   [2] loadMasterBitstream() - To load the bitstream into the FPGA
//   [3] startPCI()            - Initialize the PCI subsystem
//==========================================================================================================
#pragma once
#include <string>
#include <vector>
#include <map>

class CECDProxy
{
public:

    // Default constructor
    CECDProxy();
    ~CECDProxy();

    // Call this once to read in the configuration file.  Can throw std::runtime_error
    void init(std::string filename);

    // Loads the bitstream into the master FPGA
    bool loadMasterBitstream();

    // Loads the bitstream into the ECD FPGA
    bool loadEcdBitstream();

    // If loading a bitstream fails, this will return an error message
    std::string getLoadError() {return loadError_;}

    // Perform a PCI hot-reset and map PCI resource regions into userspace.
    // Can throw std:runtime_error
    void startPCI();

    // Preload the RTL FIFOs and prepare for data transfer to begin
    void prepareDataTransfer(uint64_t physAddress, uint32_t buffSize);    

    // Call these to fetch the version number or date of the master bitstream
    // after it has been loaded
    std::string getMasterBitstreamVersion();
    std::string getMasterBitstreamDate();

    // Returns the status bits for a QSFP channel.  Channel should be 0 or 1
    uint32_t getQsfpStatus(int channel);

    // Call this to determine if the specified QSFP channel is up
    bool checkQsfpStatus(int channel, bool throwOnFail = false);

    // Override this to receive interrupt notifications
    virtual void onInterrupt(int irq, uint64_t irqCounter) {};

protected:

    // This is the low-level function that loads a bitstream into an FPGA
    bool loadBitstream(bool loadMaster);

    // This is the number of PCI interrupt sources we are managing
    enum {IRQ_COUNT = 3};

    // Keeps track of how many times an interrupt has been called since "prepareDataTransfer()"
    uint64_t interruptCounter_[IRQ_COUNT];

    // Spawns "monitorInterrupts()" in its own detached thread
    void spawnTopLevelInterruptHandler(int uioDevice);

    // Monitors PCI interrupts and notifies handlers by writing to FIFOs
    void monitorInterrupts(int uioDevice);

    // If loading a bitstream fails, the error will be stored here
    std::string loadError_;

    // These constants serve as indices into axiMap_
    enum
    {   
        AM_MASTER_REVISION,
        AM_IRQ_MANAGER,
        AM_RESTART_MANAGER,
        AM_DATA_CONTROL,
        AM_QSFP_STATUS,
        AM_MAX
    };

    // One address per AM_xxxx constant
    uint32_t axiMap_[AM_MAX];

    // These values are read in from the config file durint init()
    struct
    {
        std::string tmpDir;
        std::string vivado;
        std::string pciDevice;
        std::vector<std::string> masterProgrammingScript;
        std::vector<std::string> ecdProgrammingScript;

    } config_;
    
};

