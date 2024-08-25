#ifndef ABSTRACT_SENSOR
#define ABSTRACT_SENSOR

#include <chrono>
#include <vector>

#include "Frequency.hpp"

namespace sensor {

template <typename T>
class AbstractSensor{
public:
    AbstractSensor(Freq freq=Freq::HZ_10);
    virtual                   ~AbstractSensor();
    virtual T                 fetchData() = 0;
    virtual std::vector<char> encodeDataToByte(T data) = 0;
    virtual T                 decodeDataFromByte(std::vector<char> data) = 0;
    std::vector<char>         stream();

protected:
    Freq freq;
    std::chrono::duration<double> dt;
};

} // sensor

#include "AbstractSensor_impl.hpp"

#endif // ABSTRACT_SENSOR
