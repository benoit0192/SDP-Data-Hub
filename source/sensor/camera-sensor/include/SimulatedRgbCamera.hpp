#ifndef SIMULATED_RGB_CAMERA
#define SIMULATED_RGB_CAMERA

#include <filesystem>
#include <opencv2/opencv.hpp>

#include "AbstractCameraSensor.hpp"

namespace sensor {

class SimulatedRgbCamera : public AbstractCameraSensor<cv::Mat> {
public:
    SimulatedRgbCamera(Freq freq, std::string dataPath, double sampleRate=0.03);
    ~SimulatedRgbCamera();
    cv::Mat           fetchData() override;
    std::vector<char> encodeDataToByte(const cv::Mat& im) override;
    static cv::Mat    decodeDataFromByte(const std::vector<char>& buffer,
                                         CameraParams& params);
    void              streamForDuration(double duration, CameraParams& params);

private:
    std::vector<cv::Mat> images;  // Store the sequence
    size_t               imIx;
    duration_t           sampleRate;
    std::chrono::duration<long int, std::nano> sampleRateNs;
};

} // sensor

#endif // !SIMULATED_RGB_CAMERA
