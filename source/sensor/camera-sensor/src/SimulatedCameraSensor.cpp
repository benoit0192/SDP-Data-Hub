#include "SimulatedCameraSensor.hpp"

namespace fs = std::filesystem;

namespace sensor {

template class AbstractSensor<cv::Mat>;

// =============================================================================
SimulatedCameraSensor::SimulatedCameraSensor(
                    Freq freq,
                    std::string dataPath) : AbstractCameraSensor<cv::Mat>(freq),
                                                    imIx(0) {
    // Check if the directory exists
    if (!fs::exists(dataPath)) {
        throw std::runtime_error("Directory not found: " + dataPath);
    }

    std::set<fs::path> sortedImPaths;
    for (const auto& entry : fs::directory_iterator(dataPath)) {
        if (entry.path().extension() != ".jpg") continue;
        sortedImPaths.insert(entry.path());
    }
    // Iterate over all files in the directory
    for (const auto& entry : sortedImPaths){

        // Read the image using OpenCV
        cv::Mat image = cv::imread(entry.string());
        if (image.empty()) {
            throw std::runtime_error("Failed to read image:" + entry.string());
        }
        this->images.push_back(image);
    }
    if(!images.empty()){
        this->width   = images.back().cols;
        this->height  = images.back().rows;
        this->channel = images.back().channels();
    }
}

// =============================================================================
SimulatedCameraSensor::~SimulatedCameraSensor(){}

// =============================================================================
cv::Mat SimulatedCameraSensor::fetchData(){
    // Loop indefinitely over the image sequence
    this->imIx = (this->imIx + 1) % this->images.size();
    return this->images[this->imIx];
}

// =============================================================================
void SimulatedCameraSensor::playClip(){
    for (const auto& im: this->images){
        cv::imshow("Image", im);
        cv::waitKey(0);
    }
}

} // namespace sensor

