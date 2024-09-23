#include "SimulatedRgbCamera.hpp"

namespace fs = std::filesystem;

namespace sensor {

template class AbstractSensor<cv::Mat>;

// =============================================================================
SimulatedRgbCamera::SimulatedRgbCamera(
                    Freq freq,
                    std::string dataPath) : AbstractCameraSensor<cv::Mat>(freq),
                                            imIx(0)
{
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
SimulatedRgbCamera::~SimulatedRgbCamera()
{
}

// =============================================================================
cv::Mat SimulatedRgbCamera::fetchData()
{
    // Loop indefinitely over the image sequence
    this->imIx = (this->imIx + 1) % this->images.size();
    return this->images[this->imIx];
}

// =============================================================================
std::vector<char> SimulatedRgbCamera::encodeDataToByte(const cv::Mat& im)
{
    if(!im.isContinuous()) {
        throw std::runtime_error("cv::Mat is not continuous.");
    }
    if ((int)im.total()*(int)im.elemSize() != height*width*channel) {
        throw std::runtime_error(
            "Image size does not match the expected size of the buffer.");
    }
    std::vector<char> buffer(height * width * channel);
    std::memcpy(buffer.data(), im.data, buffer.size());

    return buffer;
}

// =============================================================================
cv::Mat SimulatedRgbCamera::decodeDataFromByte(const std::vector<char>& buffer,
                                               CameraParams& params)
{
    int type = CV_8UC(params.channel);
    cv::Mat im(params.height, params.width, type);

    if (!im.isContinuous()) {
        throw std::runtime_error("cv::Mat is not continuous.");
    }
    int imSize = params.height * params.width * params.channel;
    if ((int)buffer.size() != imSize) {
        throw std::runtime_error(
            "Buffer size does not match the expected size of the cv::Mat.");
    }
    std::memcpy(im.data, buffer.data(), buffer.size());

    return im;
}

// =============================================================================
void SimulatedRgbCamera::playClip()
{
    //CameraParams params;
    //params.width = 1280;
    //params.height = 720;
    //params.channel = 3;
    for (const auto& im: this->images){
        //std::vector<char> bytes = encodeDataToByte(im);
        //std::string strBuffer(bytes.data(), bytes.size());
        //std::vector<char> buffer(strBuffer.data(), strBuffer.data()+strBuffer.size());
        //auto decIm = decodeDataFromByte(buffer, params);
        //cv::imshow("Image", decIm);
        cv::imshow("Image", im);
        cv::waitKey(0);
    }
}

} // namespace sensor

