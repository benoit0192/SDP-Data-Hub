#include <atomic>
#include <memory>
#include <string>
#include <csignal>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <boost/program_options.hpp>

#include <grpcpp/grpcpp.h>
#include "camera-sensor.grpc.pb.h"
#include "sensor.grpc.pb.h"

#include "SimulatedRgbCamera.hpp"
#include "Utils.hpp"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using sensor_proto::SensorService;
using sensor_proto::Empty;
using sensor_proto::Data;

namespace po = boost::program_options;

std::atomic<bool> stopRequested(false);

void SignalHandler([[maybe_unused]] int signal) {
    stopRequested.store(true);
}

class SensorClient {
public:
    SensorClient(std::shared_ptr<Channel> channel,
                 int width,
                 int height,
                 int ch) : stub_(SensorService::NewStub(channel)) {
        params.width   = width;
        params.height  = height;
        params.channel = ch;
    }

    void StreamData() {
        ClientContext context;
        Empty request;
        std::unique_ptr<grpc::ClientReader<Data>> reader(
            stub_->StreamData(&context, request));

        Data imageData;
        while (reader->Read(&imageData) && !stopRequested.load() &&
                                                imageData.timestamp() != "0") {
            std::cout << "Received image at " << imageData.timestamp() << std::endl;
            if (imageData.data().empty()) {
                std::cerr << "Received empty data." << std::endl;
            }
            std::vector<char> buffer(imageData.data().data(),
                              imageData.data().data()+imageData.data().size());

            cv::Mat im = sensor::SimulatedRgbCamera::decodeDataFromByte(buffer, params);
        }

        //context.TryCancel();

        Status status = reader->Finish();
        if (!status.ok()) {
            std::cerr << "StreamData failed: " << status.error_message() << std::endl;
        }
    }

private:
    sensor::CameraParams params;
    std::unique_ptr<SensorService::Stub> stub_;
};

// =============================================================================
int main(int argc, char** argv) {
    po::options_description desc("Simulated camera sensor options.");
    desc.add_options()
        ("help", "Display available options")
        ("host", po::value<std::string>(), "Set the host address")
        ("port", po::value<std::string>(), "Set the port")
        ("width"   , po::value<int>(), "Set the frame width")
        ("height"  , po::value<int>(), "Set the frame height")
        ("channels", po::value<int>(), "Set the frame channel");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.contains("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    std::string host = "0.0.0.0";
    if (vm.contains("host")) {
        host = vm["host"].as<std::string>();
    }

    std::string port = "50051";
    if (vm.contains("port")) {
        port = vm["port"].as<std::string>();
    }

    int width = 1280;
    if (vm.contains("width")) {
        width = vm["width"].as<int>();
    }
    int height = 720;
    if (vm.contains("height")) {
        height = vm["height"].as<int>();
    }
    int channel = 3;
    if (vm.contains("channel")) {
        channel = vm["channel"].as<int>();
    }

    std::signal(SIGINT , SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    SensorClient client(grpc::CreateChannel(host+":"+port,
                                            grpc::InsecureChannelCredentials()),
                        width,
                        height,
                        channel);

    client.StreamData();
    std::cout << "gRPC client exit\n";
    return 0;
}
