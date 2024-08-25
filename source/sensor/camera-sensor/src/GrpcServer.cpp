#include <memory>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <boost/program_options.hpp>

#include <grpcpp/grpcpp.h>
#include "camera-sensor.grpc.pb.h"

#include "SimulatedRgbCamera.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;

using sensor::SensorService;
using sensor::Empty;
using sensor::ImageData;

namespace po = boost::program_options;


class SensorServiceImpl final : public SensorService::Service {
public:
    SensorServiceImpl(sensor::Freq freq, std::string dataPath): camera(freq, dataPath) {

    }

    Status StreamImageData(ServerContext* context, const Empty* request,
                           ServerWriter<ImageData>* writer) override {
        while (true) {
            ImageData imageData;
            if (getImageFromSensor(&imageData)) {
                writer->Write(imageData);
            }
            //std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        return Status::OK;
    }

private:
    sensor::SimulatedRgbCamera camera;
    bool getImageFromSensor(ImageData* imageData) {
        //TODO: See if "stream" method should run into a separate thread.
        std::vector<char> buffer = camera.stream();
        std::string strBuffer(buffer.begin(), buffer.end());
        imageData->set_data(strBuffer);
        imageData->set_timestamp(std::to_string(time(nullptr)));
        return true;
    }
};

void RunServer(std::string host, std::string port,
               sensor::Freq freq, std::string dataPath) {
    std::string serverAddr(host + ":" + port);
    SensorServiceImpl service(freq, dataPath);

    ServerBuilder builder;
    builder.AddListeningPort(serverAddr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << serverAddr << std::endl;

    server->Wait();
}

int main(int argc, char** argv) {
    po::options_description desc("Grpc Server with simulated camera sensor options.");
    desc.add_options()
        ("help", "Display available options")
        ("data-path", po::value<std::string>(), "Set the data path");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.contains("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    std::string host = "0.0.0.0";
    if (!vm.contains("host")) {
        host = vm["host"].as<std::string>();
    }

    std::string port = "50051";
    if (!vm.contains("port")) {
        port = vm["port"].as<std::string>();
    }

    // Path to the directory containing the images
    if (!vm.contains("data-path")) {
        std::cerr << "Data path is required. " 
                  << "Use --data-path option." << std::endl;
        return 1;
    }
    std::string dataPath = vm["data-path"].as<std::string>();
  
    sensor::Freq freq = sensor::Freq::HZ_10;
    if (vm.contains("freq")) {
      freq = sensor::Frequency::parseFrequency(vm["freq"].as<int>());
    }

    //RunServer(host, port, freq, dataPath);

    return 0;
}
