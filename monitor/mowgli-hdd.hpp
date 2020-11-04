#pragma once

#include "device.hpp"
//#include "pmbus.hpp"

#include <algorithm>
#include <filesystem>
#include <sdbusplus/bus.hpp>

namespace witherspoon
{
namespace hdd
{

/**
 * @class mowglihdd
 *
 * This class implements fault analysis for Mihawk's CPLD
 * power sequencer device.
 *
 */
class mowglihdd : public Device
{
  public:
    mowglihdd() = delete;
    ~mowglihdd() = default;
    mowglihdd(const mowglihdd&) = delete;
    mowglihdd& operator=(const mowglihdd&) = delete;
    mowglihdd(mowglihdd&&) = default;
    mowglihdd& operator=(mowglihdd&&) = default;

    /**
     * Constructor
     *
     * @param[in] instance - the device instance number
     * @param[in] bus - D-Bus bus object
     */
    mowglihdd(size_t instance, sdbusplus::bus::bus& bus);

    /**
     * Checks the device for errors and only creates a log
     * if one is found.
     */
    void analyze() override;

    /**
     * Clears faults in the device
     */
    void clearFaults() override
    {
    }

  private:
    /**
     * Confirm the HDD status(fault or rebuild)
     */
    int checkFault(int statusReg);

    /**
     * The D-Bus bus object
     */
    sdbusplus::bus::bus& bus;

    /**
     * The flag of HDD-fault status
     */
    bool faultcodeMask;

    /**
     * The flag of HDD-rebuild status
     */
    bool rebuildcodeMask;

    enum class HDDErrorCode : int
    {
        /**
         * The definition of CPLD-HDD-Fault-code:
         * HDD_0 fail.
         */
        _0 = 1,

        /**
         * The definition of CPLD-HDD-Fault-code:
         * HDD_1 fail.
         */
        _1 = 2
    };

    enum class HDDRebuildCode : int
    {
        /**
         * The definition of CPLD-HDD-Rebuild-code:
         * HDD_0 or HDD_1 ends to rebuild.
         */
        _0 = 0,

        /**
         * The definition of CPLD-HDD-Rebuild-code:
         * HDD_0 starts to rebuild.
         */
        _1 = 1,

        /**
         * The definition of CPLD-HDD-Rebuild-code:
         * HDD_1 starts to rebuild.
         */
        _2 = 2
    };
};

} // namespace hdd
} // namespace witherspoon
