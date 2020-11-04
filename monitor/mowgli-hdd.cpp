#include "config.h"

#include "mowgli-hdd.hpp"

#include "utility.hpp"

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <chrono>
#include <elog-errors.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <org/open_power/Witherspoon/Fault/error.hpp>
#include <phosphor-logging/elog.hpp>
#include <phosphor-logging/log.hpp>
#include <string>
#include <xyz/openbmc_project/Common/Device/error.hpp>

extern "C" {
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
}

// i2c bus & i2c slave address of Mowgli's CPLD
#define busId 3
#define slaveAddr 0x40

// SMLink Status Register(HDD 0~7 fault status Register)
const static constexpr size_t StatusReg_1 = 0x40;

// SMLink Status Register(HDD 0~7 rebuild status Register)
const static constexpr size_t StatusReg_3 = 0x43;

using namespace std;
namespace witherspoon
{
namespace hdd
{
const auto DEVICE_NAME = "mowglihdd"s;

namespace fs = std::filesystem;
using namespace phosphor::logging;

using namespace sdbusplus::org::open_power::Witherspoon::Fault::Error;

mowglihdd::mowglihdd(size_t instance, sdbusplus::bus::bus& bus) :
    Device(DEVICE_NAME, instance), bus(bus)
{
}

void mowglihdd::analyze()
{
    HDDErrorCode code;
    code = static_cast<HDDErrorCode>(checkFault(StatusReg_1));

    HDDRebuildCode code1;
    code1 = static_cast<HDDRebuildCode>(checkFault(StatusReg_3));

    if (!faultcodeMask)
    {
        switch (code)
        {
            case HDDErrorCode::_0:
                report<HDDErrorCode0>();
                faultcodeMask = 1;
                break;
            case HDDErrorCode::_1:
                report<HDDErrorCode1>();
                faultcodeMask = 1;
                break;
            default:
                faultcodeMask = 0;
                break;
        }
    }

    switch (code1)
    {
        case HDDRebuildCode::_1:
            if (!rebuildcodeMask)
            {
                report<HDDRebuildCode1>();
                rebuildcodeMask = 1;
            }
            break;
        case HDDRebuildCode::_2:
            if (!rebuildcodeMask)
            {
                report<HDDRebuildCode1>();
                rebuildcodeMask = 1;
            }
            break;
        default:
            if (rebuildcodeMask)
            {
                report<HDDRebuildCode0>();
            }
            rebuildcodeMask = 0;
            break;
    }
}

// Check for PoweronFault
int mowglihdd::checkFault(int reg)
{
    int result = 0;
    std::string i2cBus = "/dev/i2c-" + std::to_string(busId);

    // open i2c device(CPLD-PSU-register table)
    int fd = open(i2cBus.c_str(), O_RDWR | O_CLOEXEC);
    if (fd < 0)
    {
        std::cerr << "Unable to open i2c device \n";
    }

    // set i2c slave address
    if (ioctl(fd, I2C_SLAVE_FORCE, slaveAddr) < 0)
    {
        std::cerr << "Unable to set device address \n";
        close(fd);
    }

    // check whether support i2c function
    unsigned long funcs = 0;
    if (ioctl(fd, I2C_FUNCS, &funcs) < 0)
    {
        std::cerr << "Not support I2C_FUNCS \n";
        close(fd);
    }

    // check whether support i2c-read function
    if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE_DATA))
    {
        std::cerr << "Not support I2C_FUNC_SMBUS_READ_BYTE_DATA \n";
        close(fd);
    }

    int statusValue;

    statusValue = i2c_smbus_read_byte_data(fd, reg);
    close(fd);

    if (statusValue < 0)
    {
        std::cerr << "i2c_smbus_read_byte_data failed \n";
        result = 0;
    }
    else
    {
        if (statusValue & 1)
        {
            result = 1;
        }
        else if ((statusValue >> 1) & 1)
        {
            result = 2;
        }
    }

    return result;
}

} // namespace hdd
} // namespace witherspoon
