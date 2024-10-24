#ifndef ABSTRACT_SENSOR
#define ABSTRACT_SENSOR

#include <chrono>
#include <vector>

#include "Frequency.hpp"

namespace sensor {

struct SensorParams {
};

template <typename T>
class AbstractSensor {
public:
    AbstractSensor(Freq freq=Freq::HZ_10);
    virtual                   ~AbstractSensor();
    virtual T                 fetchData() = 0;
    virtual std::vector<char> encodeDataToByte(const T& data) = 0;
    static T                  decodeDataFromByte(const std::vector<char>& data,
                                                 SensorParams& params);
    std::vector<char>         stream();

protected:
    using clock      = std::chrono::steady_clock;
    using duration_t = std::chrono::duration<double>;
    Freq              freq;
    duration_t        dt;
    std::chrono::duration<long int, std::nano> dt_ns;
    clock::time_point tStart;
    clock::time_point nextSampleTime;
};

} // sensor

#include "AbstractSensor_impl.hpp"

#endif // !ABSTRACT_SENSOR
