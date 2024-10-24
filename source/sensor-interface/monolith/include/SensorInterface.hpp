#ifndef MONOLITH_SENSOR_INTERFACE
#define MONOLITH_SENSOR_INTERFACE

#include <memory>

#include "AbstractSensor.hpp"
#include "AbstractSensorInterface.hpp"

namespace sensor_interface {

template <typename T>
class SensorInterface: public AbstractSensorInterface {
public:
    SensorInterface(std::unique_ptr<sensor::AbstractSensor<T>>&& s);
    virtual ~SensorInterface();
    void startStream(std::unique_ptr<AtomicPtrSwap>& memHdl, bool blocking=false) override;
    void stopStream() override;

private:
    void stream(std::unique_ptr<AtomicPtrSwap>& memHdl) override;

    std::unique_ptr<sensor::AbstractSensor<T>> sensor;
};

} // sensor_interface

#include "SensorInterface_impl.hpp"

#endif // !MONOLITH_SENSOR_INTERFACE
