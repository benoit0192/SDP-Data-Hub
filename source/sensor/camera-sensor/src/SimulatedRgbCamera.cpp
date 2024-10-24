#include "SimulatedRgbCamera.hpp"

namespace fs = std::filesystem;

namespace sensor {

template class AbstractSensor<cv::Mat>;

// =============================================================================
SimulatedRgbCamera::SimulatedRgbCamera(
                    Freq freq,
                    std::string dataPath,
                    double originalSampleRate) :
     AbstractCameraSensor<cv::Mat>(freq),
     imIx(0)
{
    if (originalSampleRate <= 0.0) {
        throw std::invalid_argument("Invalid sample rate");
    }

    this->originalSampleRate   = duration_t(originalSampleRate);
    this->originalSampleRateNs = std::chrono::duration_cast<std::chrono::nanoseconds>(this->originalSampleRate);

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
    auto now         = clock::now();
    auto elapsedTime = now - tStart;

    imIx = (elapsedTime / originalSampleRateNs) % images.size();
    return images[imIx];
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
void SimulatedRgbCamera::streamForDuration(double duration,
                                           CameraParams& params)
{

    using clock   = std::chrono::steady_clock;
    auto tStart_  = clock::now();
    auto dt_      = std::chrono::duration<double>(duration);
    auto dt_ns_   = std::chrono::duration_cast<std::chrono::nanoseconds>(dt_);
    auto lastTime = tStart_;
    std::string cvWindowName = "Sensor Stream";
    cv::namedWindow(cvWindowName, cv::WINDOW_NORMAL);
    cv::resizeWindow(cvWindowName, params.width, params.height);
    while(clock::now() - tStart_ < dt_ns_){
        auto t0  = clock::now();
        auto buffer = stream();
        std::chrono::duration<double, std::milli> dt0 = clock::now() - t0;
        std::cout << "Stream call dt: " << dt0 << '\n';

        /* auto t1  = clock::now(); */
        auto im     = decodeDataFromByte(buffer, params);
        /* std::chrono::duration<double, std::milli> dt1 = clock::now() - t1; */
        /* std::cout << "DecodeFromByte call dt: " << dt1 << '\n'; */

        auto currentTime = clock::now();
        std::chrono::duration<double, std::milli> deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        std::string deltaText = "Delta Time: " +
                                std::to_string(deltaTime.count()) +
                                " ms";
        /* std::cout << deltaText << '\n'; */

        cv::imshow(cvWindowName, im);
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    cv::destroyWindow(cvWindowName);
}

} // namespace sensor

