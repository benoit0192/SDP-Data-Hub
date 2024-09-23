#include <atomic>
#include <memory>
#include <string>
#include <csignal>
#include <fstream>
#include <iostream>

#include <grpcpp/grpcpp.h>
#include <opencv2/opencv.hpp>
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


std::atomic<bool> stopRequested(false);

void SignalHandler([[maybe_unused]] int signal) {
    stopRequested.store(true);
}

class SensorClient {
public:
    SensorClient(std::shared_ptr<Channel> channel)
        : stub_(SensorService::NewStub(channel)) {
        //TODO: Dimensions should be passed from the CLI
        params.width   = 1280;
        params.height  = 720;
        params.channel = 3;
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
            //for(int i=0; i < 5; i++) {
            //  printHex(buffer[i]);
            //  std::cout << " ";}
            //std::cout << std::endl;

            cv::Mat im = sensor::SimulatedRgbCamera::decodeDataFromByte(buffer, params);
        }

        //context.TryCancel();

        std::cout << "Call to Finish\n";
        Status status = reader->Finish();
        std::cout << "Finished\n";
        if (!status.ok()) {
            std::cerr << "StreamData failed: " << status.error_message() << std::endl;
        }
        std::cout << "Go out of scope\n";
    }

private:
    sensor::CameraParams params;
    std::unique_ptr<SensorService::Stub> stub_;
};

// =============================================================================

int main(int argc, char** argv) {
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    SensorClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    client.StreamData();
    std::cout << "gRPC client exit\n";
    return 0;
}
