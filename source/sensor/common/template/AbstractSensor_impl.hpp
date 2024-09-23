#include <thread>

namespace sensor {

// =============================================================================
template <typename T>
AbstractSensor<T>::AbstractSensor(Freq freq): freq(freq) {
    this->dt = std::chrono::duration<double>(1 / static_cast<float>(freq));
}

// =============================================================================
template <typename T>
AbstractSensor<T>::~AbstractSensor() {
}

// =============================================================================
template <typename T>
std::vector<char> AbstractSensor<T>::stream() {
    auto tStart = std::chrono::high_resolution_clock::now();

    T data                   = fetchData();
    std::vector<char> buffer = encodeDataToByte(data);

    auto tEnd     = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(tEnd - tStart);
    auto tSleep   = this->dt - duration;
    if(tSleep.count() > 0){
        std::this_thread::sleep_for(tSleep);
    }
    return buffer;
}

// =============================================================================
template <typename T>
T decodeDataFromByte(const std::vector<char>& data, SensorParams& params) {
    throw std::runtime_error("'decodeDataFromByte' not implemented.");
}

} // sensor
