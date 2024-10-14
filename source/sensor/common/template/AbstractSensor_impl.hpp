#include <thread>

namespace sensor {

// =============================================================================
template <typename T>
AbstractSensor<T>::AbstractSensor(Freq freq) :
         freq(freq),
         dt(duration_t(1.0 / static_cast<double>(freq))),
         dt_ns(std::chrono::duration_cast<std::chrono::nanoseconds>(dt)),
         tStart(std::chrono::steady_clock::now()),
         nextSampleTime(tStart + std::chrono::duration_cast<std::chrono::nanoseconds>(dt))
{
}

// =============================================================================
template <typename T>
AbstractSensor<T>::~AbstractSensor()
{
}

// =============================================================================
template <typename T>
std::vector<char> AbstractSensor<T>::stream()
{
    auto tNow = clock::now();

    auto elapsedTime     = tNow - tStart;
    auto intervalsPassed = elapsedTime / dt_ns;
    auto nextDeltaTime   = (intervalsPassed + 1) * dt_ns;
    nextSampleTime       = tStart + nextDeltaTime;

    if (tNow < nextSampleTime) {
        std::this_thread::sleep_until(nextSampleTime);
    }

    T data                   = fetchData();
    std::vector<char> buffer = encodeDataToByte(data);

    return buffer;
}

// =============================================================================
template <typename T>
T decodeDataFromByte(const std::vector<char>& data, SensorParams& params)
{
    throw std::runtime_error("'decodeDataFromByte' not implemented.");
}

} // sensor
