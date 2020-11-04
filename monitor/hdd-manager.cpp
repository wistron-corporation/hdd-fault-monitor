/**
 * 2020-11-04
 */
#include "config.h"

#include "hdd-manager.hpp"

#include "elog-errors.hpp"
#include "utility.hpp"

#include <iostream>
#include <org/open_power/Witherspoon/Fault/error.hpp>
#include <phosphor-logging/log.hpp>

using namespace std;
namespace witherspoon
{
namespace hdd
{

constexpr auto POWER_OBJ_PATH = "/org/openbmc/control/power0";
constexpr auto POWER_INTERFACE = "org.openbmc.control.Power";

using namespace phosphor::logging;
using namespace sdbusplus::org::open_power::Witherspoon::Fault::Error;

bool HDDMonitor::hddMonitoring()
{
    bool monitor = false;
    int32_t state = 0;

    auto service = util::getService(POWER_OBJ_PATH, POWER_INTERFACE, bus);

    util::getProperty<int32_t>(POWER_INTERFACE, "state", POWER_OBJ_PATH,
                               service, bus, state);

    // Start to monitor when host starts to power on.
    if (state)
    {
        monitor = true;
    }

    return monitor;
}

void HDDMonitor::analyze()
{
    if (hddMonitoring())
    {
        device->analyze();
    }
}

int HDDMonitor::run()
{
    try
    {
        if (!hddMonitoring())
        {
            return EXIT_SUCCESS;
        }
    }
    catch (std::exception& e)
    {
        log<level::ERR>(e.what());
        log<level::ERR>("Unexpected failure prevented HDD checking");
    }

    return DeviceMonitor::run();
}

} // namespace hdd
} // namespace witherspoon
