#include <iostream>
#include <boost/program_options.hpp>

#include "SimulatedRgbCamera.hpp"

namespace po = boost::program_options;

// =============================================================================
int main(int argc, char* argv[]) {
    po::options_description desc("Simulated camera sensor options.");
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

    if (!vm.contains("data-path")) {
        std::cerr << "Data path is required. " 
                  << "Use --data-path option." << std::endl;
        return 1;
    }

    // Path to the directory containing the images
    std::string dataPath = vm["data-path"].as<std::string>();

    sensor::SimulatedRgbCamera simCam = sensor::SimulatedRgbCamera(
                                                          sensor::Freq::HZ_10,
                                                          dataPath);
    simCam.playClip();

    return 0;
}
