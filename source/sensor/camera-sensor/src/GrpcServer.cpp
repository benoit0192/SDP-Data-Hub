#include <atomic>
#include <memory>
#include <string>
#include <csignal>
#include <fstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <boost/program_options.hpp>

#include <grpcpp/grpcpp.h>
#include "sensor.grpc.pb.h"
#include "camera-sensor.grpc.pb.h"

#include "SimulatedRgbCamera.hpp"
#include "Utils.hpp"

using grpc::Status;
using grpc::Server;
using grpc::ServerWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;

using sensor_proto::Data;
using sensor_proto::Empty;
using sensor_proto::SensorService;

namespace po = boost::program_options;

std::atomic<bool> stopRequested(false);

void SignalHandler([[maybe_unused]] int signal) {
    stopRequested.store(true);
}

class SensorServiceImpl final : public SensorService::Service {
public:
    SensorServiceImpl(sensor::Freq freq, std::string dataPath):
                                                    camera(freq, dataPath) {

    }

    Status StreamData([[maybe_unused]] ServerContext* context,
                      [[maybe_unused]] const Empty* request,
                      ServerWriter<Data>* writer) override {
        Data imageData;

        while(getImageFromSensor(imageData) && !stopRequested.load()) {
            writer->Write(imageData);
        }
        // TODO: Create proper protos to end the stream
        Data terminationSignal;
        terminationSignal.set_timestamp(std::to_string(0));  // Signal to indicate the stream is ending
        writer->Write(terminationSignal);

        return Status::OK;
    }

private:
    sensor::SimulatedRgbCamera camera;
    bool getImageFromSensor(Data& imageData) {
        std::vector<char> buffer = camera.stream();
        for(int i=0; i < 5; i++) {
          printHex(buffer[i]);
          std::cout << " ";
        }
        std::cout << std::endl;
        std::string strBuffer(buffer.data(), buffer.size());
        imageData.set_data(strBuffer);
        imageData.set_timestamp(std::to_string(time(nullptr)));
        return true;
    }
};

// =============================================================================

void runServer(std::string host, std::string port,
               sensor::Freq freq, std::string dataPath) {
    std::string serverAddr(host + ":" + port);
    SensorServiceImpl service(freq, dataPath);

    ServerBuilder builder;
    builder.AddListeningPort(serverAddr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << serverAddr << std::endl;

    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    while (!stopRequested.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Stopping server..." << std::endl;
    server->Shutdown();
    //server->Wait();
}

int main(int argc, char** argv) {
    po::options_description desc("Grpc Server with simulated camera sensor options.");
    desc.add_options()
        ("help", "Display available options")
        ("host", po::value<std::string>(), "Set the host address")
        ("port", po::value<std::string>(), "Set the port")
        ("data-path", po::value<std::string>(), "Set the data path")
        ("freq", po::value<int>(), "Set the sensor frequency");

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

    runServer(host, port, freq, dataPath);

    return 0;
}
