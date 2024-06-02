#ifndef ABSTRACT_SENSOR
#define ABSTRACT_SENSOR

#include <chrono>

namespace sensor {

enum class Freq{
    HZ_10 = 10,
    HZ_20 = 20,
    HZ_30 = 30,
};

template <typename T>
class AbstractSensor{
public:
    AbstractSensor(Freq freq=Freq::HZ_10);
    virtual ~AbstractSensor();
    virtual T fetchData() = 0;
    T stream();

protected:
    Freq freq;
    std::chrono::duration<double> dt;
};

} // sensor

#include "AbstractSensor_impl.hpp"

#endif // ABSTRACT_SENSOR
