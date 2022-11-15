#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <iostream>
#include <experimental/filesystem>
#include <boost/program_options.hpp>

#include "botox/botox.h"

using namespace std;

namespace btx = Botox;
namespace po = boost::program_options;
namespace fs = std::experimental::filesystem;

struct MainArgs {
    std::vector<std::string> inputFiles;
    std::string outputDirectory;
    std::string maskFile;
    btx::PixelConnectivity connectivity;
    float e;
    float z;

    bool proceed = false;
};

int getArgs(int argc, char *argv[], MainArgs &mainArgs) {

    po::options_description description("Botox options");
    description.add_options()
            ("help,h", "Displays this help message")
            ("input_directory,i", po::value<std::string>(), "Directory containing files to process")
            ("output_directory,o", po::value<std::string>(), "Output directory")
            ("mask,m", po::value<std::string>(), "Mask file")
            ("connectivity,c", po::value<int>(), "Boundary pixel connectivity mode (4 or 8)")
            ("E,e", po::value<float>(), "E for the lightricks weighting function. Must be greater than zero.")
            ("Z,z", po::value<float>(), "Z for the lightricks weighting function");

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(description).run(), vm);

    if (vm.count("help")) {
        cout << description;
        return 0;
    }

    if (!vm.count("input_directory")) {
        cout << "Missing input directory" << endl << description;
        return -1;
    }
    std::string inputDirectory = vm["input_directory"].as<std::string>();

    for (const auto &entry : fs::directory_iterator(inputDirectory)) {
        mainArgs.inputFiles.push_back(entry.path());
    }

    if (!vm.count("output_directory")) {
        cout << "Missing output directory" << endl << description;
        return -2;
    }

    std::string outputDirectory = vm["output_directory"].as<std::string>();
    if (outputDirectory[outputDirectory.size() - 1] != fs::path::preferred_separator) {
        outputDirectory += fs::path::preferred_separator;
    }
    mainArgs.outputDirectory = outputDirectory;

    if (!vm.count("mask")) {
        cout << "Missing mask file" << endl << description;
        return -3;
    }
    mainArgs.maskFile = vm["mask"].as<std::string>();

    if (!vm.count("connectivity")) {
        cout << "Missing connectivity mode." << endl << description;
        return -4;
    }

    auto connectivityMode = static_cast<btx::PixelConnectivity>(vm["connectivity"].as<int>());
    if (connectivityMode == 1) {
        cout << "Invalid connectivity value: " << connectivityMode << "." << endl << description << endl;
        return -5;
    }
    mainArgs.connectivity = connectivityMode;

    if (!vm.count("E")) {
        cout << "Missing e." << endl << description;
        return -6;
    }
    mainArgs.e = vm["E"].as<float>();
    if (mainArgs.e < 0.0f) {
        mainArgs.e = 0.01f;
    }

    if (!vm.count("Z")) {
        cout << "Missing z." << endl << description;
        return -7;
    }
    mainArgs.z = vm["Z"].as<float>();

    mainArgs.proceed = true;
    return 0;
}

#endif // ARGS_PARSER_H
