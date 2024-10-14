#ifndef ABSTRACT_CAMERA_SENSOR
#define ABSTRACT_CAMERA_SENSOR

#include "common/include/AbstractSensor.hpp"

namespace sensor {

struct CameraParams: public SensorParams {
    CameraParams() : SensorParams() {};
    int width   = 0;
    int height  = 0;
    int channel = 0;
};

template <typename T>
class AbstractCameraSensor : public AbstractSensor<T> {
public:
    AbstractCameraSensor(Freq freq=Freq::HZ_10);
    virtual ~AbstractCameraSensor();
    virtual T fetchData() = 0;
    int getWidth()   const;
    int getHeight()  const;
    int getChannel() const;

protected:
    int width;
    int height;
    int channel;
};

} // sensor

#include "AbstractCameraSensor_impl.hpp"

#endif // !ABSTRACT_CAMERA_SENSOR
