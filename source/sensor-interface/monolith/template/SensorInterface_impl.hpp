
namespace sensor_interface {

// =============================================================================
template <typename T>
SensorInterface<T>::SensorInterface(
                                 std::unique_ptr<sensor::AbstractSensor<T>>&& s)
        : AbstractSensorInterface(),
          sensor(std::move(s))
{
}

// =============================================================================
template <typename T>
SensorInterface<T>::~SensorInterface()
{
    stopStream();
}

// =============================================================================
template <typename T>
void SensorInterface<T>::stream(std::unique_ptr<AtomicPtrSwap>& memHdl)
{
    while(isStreaming.load()){
        memHdl->write(sensor->stream());
    }
}

// =============================================================================
template <typename T>
void SensorInterface<T>::startStream(std::unique_ptr<AtomicPtrSwap>& memHdl,
                                     bool blocking)
{
    if(isStreaming.load())  return;
    isStreaming.store(true);
    if(!blocking){
        streamingThread = std::thread(&SensorInterface<T>::stream,
                                      this,
                                      std::ref(memHdl));
    }
    else{
        stream(memHdl);
    }
}

// =============================================================================
template <typename T>
void SensorInterface<T>::stopStream()
{
    isStreaming.store(false);
    if(streamingThread.joinable()){
        streamingThread.join();
    }
}

} // sensor_interface
