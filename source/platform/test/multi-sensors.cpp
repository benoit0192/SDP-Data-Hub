#include <chrono>

#include "Platform.hpp"
#include "SensorInterface.hpp"
#include "SimulatedRgbCamera.hpp"


void manySensorsOneReadTest(){
    std::cout << "Sequential test start." << std::endl;
    Platform p(10);

    /* Sensor 1 */
    std::unique_ptr<sensor::AbstractSensor<cv::Mat>> s1 = std::make_unique<sensor::SimulatedRgbCamera>(sensor::Freq::HZ_10,"data/images/");
    std::unique_ptr<sensor_interface::AbstractSensorInterface> si_1 = std::make_unique<sensor_interface::SensorInterface<cv::Mat>>(std::move(s1));
    s1 = nullptr;

    /* Sensor 2 */
    std::unique_ptr<sensor::AbstractSensor<cv::Mat>> s2 = std::make_unique<sensor::SimulatedRgbCamera>(sensor::Freq::HZ_10,"data/images/");
    std::unique_ptr<sensor_interface::AbstractSensorInterface> si_2 = std::make_unique<sensor_interface::SensorInterface<cv::Mat>>(std::move(s2));
    s2 = nullptr;

    size_t id1 = p.attach(std::move(si_1));
    size_t id2 = p.attach(std::move(si_2));

    /* Start the sensors */
    p.stream();

    std::cout << "Streaming for 5s.." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    auto dataPtr_1 = p.read(id1);
    auto dataPtr_2 = p.read(id2);

    /* Optional */
    p.detach(id1);
    p.detach(id2);

    std::cout << "Sequential test end." << std::endl;
}

void oneSensorManyReadsTest(){
    std::cout << "Parallel test start." << std::endl;

    Platform p(10);

    auto readAccessFunc = [&p](const size_t& id){
        auto tStart = std::chrono::high_resolution_clock::now();
        auto tEnd   = tStart + std::chrono::seconds(3);

        while (std::chrono::high_resolution_clock::now() < tEnd) {
            auto dataPtr = p.read(id);
            if(dataPtr == nullptr){
                std::cout << "No data available yet." << '\n';
            }
            else{

                std::cout << "Received data." << '\n';
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    };


    /* Sensor 1 */
    std::unique_ptr<sensor::AbstractSensor<cv::Mat>> s1 = std::make_unique<sensor::SimulatedRgbCamera>(sensor::Freq::HZ_10,"data/images/");
    std::unique_ptr<sensor_interface::AbstractSensorInterface> si_1 = std::make_unique<sensor_interface::SensorInterface<cv::Mat>>(std::move(s1));
    s1 = nullptr;

    size_t id1 = p.attach(std::move(si_1));

    /* Start the sensors */
    p.stream();

    std::thread t1 = std::thread(readAccessFunc, std::ref(id1));
    std::thread t2 = std::thread(readAccessFunc, std::ref(id1));
    std::thread t3 = std::thread(readAccessFunc, std::ref(id1));

    if(t1.joinable())   t1.join();
    if(t2.joinable())   t2.join();
    if(t3.joinable())   t3.join();

    /* Optional */
    p.detach(id1);

    std::cout << "Parallel test end." << std::endl;
}

int main([[maybe_unused]]int argc, [[maybe_unused]]char* argv[]){
    /* manySensorsOneReadTest(); */
    oneSensorManyReadsTest();
    return 0;
}

