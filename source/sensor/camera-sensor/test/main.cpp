#include <iostream>
#include <boost/program_options.hpp>

#include "SimulatedRgbCamera.hpp"

namespace po = boost::program_options;


// =============================================================================
int main(int argc, char* argv[]) {
    po::options_description desc("Simulated camera sensor options.");
    desc.add_options()
        ("help", "Display available options")
        ("data-path", po::value<std::string>(), "Set the data path")
        ("width"   , po::value<int>(), "Set the frame width")
        ("height"  , po::value<int>(), "Set the frame height")
        ("channels", po::value<int>(), "Set the frame channel")
        ("duration", po::value<int>(), "Set the duration in second");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.contains("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    if (!vm.contains("data-path")) {
        std::cerr << "Data path is required. " 
                  << "Use --data-path option." << std::endl;
        return 1;
    }
    // Path to the directory containing the frames
    std::string dataPath = vm["data-path"].as<std::string>();

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

    int duration = 5;
    if (vm.contains("duration")) {
        duration = vm["duration"].as<int>();
    }

    sensor::Freq freq = sensor::Freq::HZ_30;
    if (vm.contains("freq")) {
      freq = sensor::Frequency::parseFrequency(vm["freq"].as<int>());
    }

    sensor::SimulatedRgbCamera simCam = sensor::SimulatedRgbCamera(
                                                                freq, dataPath);
    sensor::CameraParams params;
    params.width   = width;
    params.height  = height;
    params.channel = channel;
    simCam.streamForDuration(duration, params);

    return 0;
}
