#ifndef SIMULATED_CAMERA_SENSOR
#define SIMULATED_CAMERA_SENSOR

#include <vector>
#include <filesystem>
#include <opencv2/opencv.hpp>

#include "AbstractCameraSensor.hpp"

namespace sensor {

class SimulatedCameraSensor : public AbstractCameraSensor<cv::Mat> {
public:
    SimulatedCameraSensor(Freq freq, std::string dataPath);
    ~SimulatedCameraSensor();
    cv::Mat fetchData() override;
    void playClip();

private:
    std::vector<cv::Mat> images;  // Store the sequence
    unsigned int         imIx;
};

} // sensor

#endif // SIMULATED_CAMERA_SENSOR
