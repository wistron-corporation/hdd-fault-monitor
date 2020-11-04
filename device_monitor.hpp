#pragma once
#include "device.hpp"

#include <phosphor-logging/log.hpp>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/server.hpp>
#include <sdeventplus/clock.hpp>
#include <sdeventplus/event.hpp>
#include <sdeventplus/utility/timer.hpp>

namespace witherspoon
{
namespace hdd
{

using namespace phosphor::logging;

/**
 * @class DeviceMonitor
 */
class DeviceMonitor
{
  public:
    DeviceMonitor() = delete;
    ~DeviceMonitor() = default;
    DeviceMonitor(const DeviceMonitor&) = delete;
    DeviceMonitor& operator=(const DeviceMonitor&) = delete;
    DeviceMonitor(DeviceMonitor&&) = delete;
    DeviceMonitor& operator=(DeviceMonitor&&) = delete;

    /**
     * Constructor
     *
     * @param[in] d - device to monitor
     * @param[in] e - event object
     * @param[in] i - polling interval in ms
     */
    DeviceMonitor(std::unique_ptr<Device>&& d, const sdeventplus::Event& e,
                  std::chrono::milliseconds i) :
        device(std::move(d)),
        timer(e, std::bind(&DeviceMonitor::analyze, this), i)
    {
    }

    /**
     * Starts the timer to monitor the device on an interval.
     */
    virtual int run()
    {
        return timer.get_event().loop();
    }

  protected:
    /**
     * Analyzes the device for faults
     *
     * Runs in the timer callback
     *
     * Override if desired
     */
    virtual void analyze()
    {
        device->analyze();
    }

    /**
     * The device to run the analysis on
     */
    std::unique_ptr<Device> device;

    /**
     * The timer that runs fault check polls.
     */
    sdeventplus::utility::Timer<sdeventplus::ClockId::Monotonic> timer;
};

} // namespace hdd
} // namespace witherspoon

