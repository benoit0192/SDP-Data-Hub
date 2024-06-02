#include <thread>

namespace sensor {

// =============================================================================
template <typename T>
AbstractSensor<T>::AbstractSensor(Freq freq): freq(freq) {
    this->dt = std::chrono::duration<double>(1 / static_cast<float>(freq));
}

// =============================================================================
template <typename T>
AbstractSensor<T>::~AbstractSensor(){}

// =============================================================================
template <typename T>
T AbstractSensor<T>::stream(){
    auto tStart = std::chrono::high_resolution_clock::now();

    T data = this->fetchData();

    auto tEnd = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(tEnd - tStart);
    auto tSleep = this->dt - duration;
    if(tSleep.count() > 0){
        std::this_thread::sleep_for(tSleep);
    }
    return data;
}

} // sensor