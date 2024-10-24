#include "AbstractSensorInterface.hpp"

namespace sensor_interface {

AbstractSensorInterface::AbstractSensorInterface() : isStreaming(false)
{
}

AbstractSensorInterface::~AbstractSensorInterface()
{
}

bool AbstractSensorInterface::getIsStreaming() const
{
    return isStreaming.load();
}

} // sensor_interface
