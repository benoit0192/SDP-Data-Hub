#ifndef ABSTRACT_CAMERA_SENSOR
#define ABSTRACT_CAMERA_SENSOR

#include "common/include/AbstractSensor.hpp"

namespace sensor {

template <typename T>
class AbstractCameraSensor : public AbstractSensor<T>{
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

#endif // ABSTRACT_CAMERA_SENSOR
