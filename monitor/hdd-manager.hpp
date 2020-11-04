#pragma once

#include "device.hpp"
#include "device_monitor.hpp"

#include <sdbusplus/bus.hpp>
#include <sdbusplus/server.hpp>
#include <sdeventplus/event.hpp>

namespace witherspoon
{
namespace hdd
{

/**
 * @class HDDMonitor
 *
 */
class HDDMonitor : public DeviceMonitor
{
  public:
    HDDMonitor() = delete;
    ~HDDMonitor() = default;
    HDDMonitor(const HDDMonitor&) = delete;
    HDDMonitor& operator=(const HDDMonitor&) = delete;
    HDDMonitor(HDDMonitor&&) = delete;
    HDDMonitor& operator=(HDDMonitor&&) = delete;

    /**
     * Constructor
     *
     * @param[in] d - the device to monitor
     * @param[in] b - D-Bus bus object
     * @param[in] e - event object
     * @param[in] t - time to allow to monitor to come up
     */
    HDDMonitor(std::unique_ptr<witherspoon::hdd::Device>&& d,
               sdbusplus::bus::bus& b, const sdeventplus::Event& e,
               std::chrono::milliseconds& t) :
        DeviceMonitor(std::move(d), e, t),
        bus(b)
    {
    }

    /**
     * Analyzes the HDD status.
     */
    void analyze() override;

    /**
     *
     */
    int run() override;

  private:
    /**
     * Enable HDD_Monitor when host poweron.
     */
    bool hddMonitoring();

    /**
     * The D-Bus object
     */
    sdbusplus::bus::bus& bus;

    /**
     * The match object for the properties changed signal
     */
    std::unique_ptr<sdbusplus::bus::match_t> match;
};

} // namespace hdd
} // namespace witherspoon
