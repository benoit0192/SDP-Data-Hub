#ifndef SIMULATED_RGB_CAMERA
#define SIMULATED_RGB_CAMERA

#include <filesystem>
#include <opencv2/opencv.hpp>

#include "AbstractCameraSensor.hpp"

namespace sensor {

class SimulatedRgbCamera : public AbstractCameraSensor<cv::Mat> {
public:
    SimulatedRgbCamera(Freq freq, std::string dataPath);
    ~SimulatedRgbCamera();
    cv::Mat           fetchData()                                  override;
    std::vector<char> encodeDataToByte(cv::Mat im)                 override;
    cv::Mat           decodeDataFromByte(std::vector<char> buffer) override;
    void              playClip();

private:
    std::vector<cv::Mat> images;  // Store the sequence
    unsigned int         imIx;
};

} // sensor

#endif // SIMULATED_RGB_CAMERA
