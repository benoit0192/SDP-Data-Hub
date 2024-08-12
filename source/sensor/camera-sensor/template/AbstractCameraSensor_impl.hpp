#include <thread>

namespace sensor {

// =============================================================================
template <typename T>
AbstractCameraSensor<T>::AbstractCameraSensor(Freq freq): 
                                                AbstractSensor<T>(freq),
                                                width(0),
                                                height(0),
                                                channel(0) {
}

// =============================================================================
template <typename T>
AbstractCameraSensor<T>::~AbstractCameraSensor() {}

// =============================================================================
template <typename T>
int AbstractCameraSensor<T>::getWidth() const {
    return width;
}

// =============================================================================
template <typename T>
int AbstractCameraSensor<T>::getHeight() const {
    return height;
}

// =============================================================================
template <typename T>
int AbstractCameraSensor<T>::getChannel() const {
    return channel;
}

} // sensor

