#ifndef PLATFORM
#define PLATFORM

#include <memory>
#include <unordered_map>

#include "AtomicPtrSwap.hpp"
#include "AbstractSensorInterface.hpp"


class Platform {
public:
    Platform(double freq);
    virtual ~Platform();
    int attach(std::unique_ptr<sensor_interface::AbstractSensorInterface>&& si);
    std::unique_ptr<sensor_interface::AbstractSensorInterface> detach(size_t id);
    void stream();
    void stream(size_t id);
    std:shared_ptr<std::vector<char>> read(size_t id);
private:
    size_t curId;
    std::unordered_map<size_t,std::unique_ptr<sensor_interface::AbstractSensorInterface>> sensors;
    std::unordered_map<size_t,std::unique_ptr<AtomicPtrSwap>> memHdls;
};

#endif // !PLATFORM

