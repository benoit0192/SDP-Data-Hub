#include <thread>

namespace sensor {

// =============================================================================
template <typename T>
AbstractCameraSensor<T>::AbstractCameraSensor(Freq freq): 
                                                AbstractSensor<T>(freq) {}

// =============================================================================
template <typename T>
AbstractCameraSensor<T>::~AbstractCameraSensor(){}

} // sensor

