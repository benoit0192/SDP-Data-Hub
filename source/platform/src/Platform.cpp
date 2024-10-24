#include "Platform.hpp"


Platform::Platform(double freq) : curId(0)
{
}

Platform::~Platform()
{
    for(auto& [id, si]: sensors){
        si->stopStream();
    }
}

int Platform::attach(std::unique_ptr<sensor_interface::AbstractSensorInterface>&& si)
{
    size_t id = ++curId;
    sensors[id] = std::move(si);
    memHdls[id] = std::make_unique<AtomicPtrSwap>();

    return id;
}

std::unique_ptr<sensor_interface::AbstractSensorInterface> Platform::detach(size_t id)
{
    if(!sensors.contains(id)){
        throw std::runtime_error(
                "Requested sensor interface ID does not exist.");
    }
    if(!memHdls.contains(id)){
        throw std::runtime_error(
                "Requested sensor interface memory handler ID does not exist.");
    }
    auto si = move(sensors[id]);
    si->stopStream();
    sensors.erase(id);
    memHdls.erase(id);
    return si;
}

void Platform::stream()
{
    for(auto& [id, si]: sensors) {
        stream(id);
    }
}

void Platform::stream(size_t id)
{
    if(!sensors.contains(id)){
        throw std::runtime_error(
                "Requested sensor interface ID does not exist.");
    }
    if(!memHdls.contains(id)){
        throw std::runtime_error(
                "Requested sensor interface memory handler ID does not exist.");
    }
    auto& si     = sensors[id];
    auto& memHdl = memHdls[id];
    si->startStream(memHdl);
}

std::shared_ptr<std::vector<char>> Platform::read(size_t id)
{
    if(!memHdls.contains(id)){
        throw std::runtime_error(
                "Requested sensor interface memory handler ID does not exist.");
    }
    return memHdls[id]->read();
}
