#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>

#include <grpcpp/grpcpp.h>
#include "camera-sensor.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;
using sensor::SensorService;
using sensor::Empty;
using sensor::ImageData;

//class SensorServiceImpl final : public SensorService::Service {
//public:
//    Status StreamImageData(ServerContext* context, const Empty* request,
//                           ServerWriter<ImageData>* writer) override {
//        while (true) {
//            ImageData image_data;
//            if (getImageFromSensor(&image_data)) {
//                writer->Write(image_data);
//            }
//            std::this_thread::sleep_for(std::chrono::seconds(1));
//        }
//        return Status::OK;
//    }
//
//private:
//    bool getImageFromSensor(ImageData* image_data) {
//        std::ifstream file("example_image.jpg", std::ios::binary);
//        if (!file) {
//            std::cerr << "Failed to open image file." << std::endl;
//            return false;
//        }
//        std::string data((std::istreambuf_iterator<char>(file)),
//                          std::istreambuf_iterator<char>());
//        image_data->set_data(data);
//        image_data->set_timestamp(std::to_string(time(nullptr)));
//        return true;
//    }
//};
//
//void RunServer() {
//    std::string server_address("0.0.0.0:50051");
//    SensorServiceImpl service;
//
//    ServerBuilder builder;
//    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
//    builder.RegisterService(&service);
//    std::unique_ptr<Server> server(builder.BuildAndStart());
//    std::cout << "Server listening on " << server_address << std::endl;
//
//    server->Wait();
//}
//
//int main(int argc, char** argv) {
//    RunServer();
//    return 0;
//}
