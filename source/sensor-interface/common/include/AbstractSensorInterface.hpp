#ifndef ABSTRACT_SENSOR_INTERFACE
#define ABSTRACT_SENSOR_INTERFACE

#include <thread>
#include <atomic>

#include "AtomicPtrSwap.hpp"

namespace sensor_interface {

class AbstractSensorInterface {
public:
    AbstractSensorInterface();
    virtual ~AbstractSensorInterface();
    virtual void startStream(std::unique_ptr<AtomicPtrSwap>& memHdl, bool blocking=false) = 0;
    virtual void stopStream() = 0;
    bool getIsStreaming() const;

protected:
    virtual void stream(std::unique_ptr<AtomicPtrSwap>& memHdl) = 0;

    std::atomic<bool> isStreaming;
    std::thread streamingThread;
};

} // sensor_interface

#endif // !ABSTRACT_SENSOR_INTERFACE

