/**
 * 2020-10-20
 */

#include "argument.hpp"
#include "hdd-manager.hpp"
#include "mowgli-hdd.hpp"

#include <chrono>
#include <iostream>
#include <phosphor-logging/log.hpp>
#include <sdeventplus/event.hpp>

using namespace witherspoon::hdd;
using namespace phosphor::logging;

int main(int argc, char** argv)
{
    ArgumentParser args{argc, argv};
    auto action = args["action"];

    auto i = strtoul(args["interval"].c_str(), nullptr, 10);
    if (i == 0)
    {
        std::cerr << "Invalid interval value\n";
        exit(EXIT_FAILURE);
    }

    std::chrono::milliseconds interval{i};

    auto event = sdeventplus::Event::get_default();
    auto bus = sdbusplus::bus::new_default();
    bus.attach_event(event.get(), SD_EVENT_PRIORITY_NORMAL);

    std::unique_ptr<DeviceMonitor> monitor;

    // Mowgli HDD-monitor
    auto device = std::make_unique<mowglihdd>(0, bus);
    monitor =
        std::make_unique<HDDMonitor>(std::move(device), bus, event, interval);

    return monitor->run();
}
