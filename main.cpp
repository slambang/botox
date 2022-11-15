#include <opencv2/opencv.hpp>
#include "opencv2/core/utils/logger.hpp"

#include "args_parser.h"
#include "botox/botox.h"
#include "botox/weights/default_pixel_weight_calculator.h"

namespace btx = Botox;

const float HOLE_PIXEL_INTENSITY_THRESHOLD = 0.5f;

int main(int argc, char *argv[]) {

#ifdef DISABLE_OPENCV_DEBUGGING
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
#endif

    // 1. Get the input parameters
    MainArgs mainArgs;
    int argsResult = getArgs(argc, argv, mainArgs);
    if (!mainArgs.proceed) return argsResult;

    // 2. Setup, pre-allocate and pre-process
    if (!fs::exists(mainArgs.outputDirectory) && !fs::create_directory(mainArgs.outputDirectory)) {
        cout << "Failed to create output directory: " << mainArgs.outputDirectory << "." << endl;
        return 1;
    }

    cv::Mat maskImage = cv::imread(mainArgs.maskFile);
    if (maskImage.empty()) {
        cout << "Failed to open mask file: " << mainArgs.maskFile << "." << endl;
        return 2;
    }

    std::vector<float> buffer(maskImage.rows * maskImage.cols);
    cv::Mat bufferMat(maskImage.rows, maskImage.cols, CV_32F, &buffer[0]);

    btx::DefaultPixelWeightCalculator pixelWeightCalculator(mainArgs.z, mainArgs.e); // (w)

    std::vector<btx::Pixel> holePixels; // (H)
    btx::findHole(maskImage.data, holePixels, HOLE_PIXEL_INTENSITY_THRESHOLD,
                  maskImage.channels(), maskImage.rows, maskImage.cols);

    std::vector<btx::Pixel> boundaryPixels; // (B)
    btx::findBoundaries(maskImage.data, holePixels, maskImage.cols, maskImage.channels(),
                        HOLE_PIXEL_INTENSITY_THRESHOLD, boundaryPixels, mainArgs.connectivity);

    // 3. Process each file
    for (int i = 0; i < mainArgs.inputFiles.size(); ++i) {

        std::string &inputFilePath = mainArgs.inputFiles[i];
        cout << (i + 1) << "/" << mainArgs.inputFiles.size() << ": " << inputFilePath << "." << endl;

        cv::Mat inputImage = cv::imread(inputFilePath); // (I) Optimisation: can we pre-allocate this?
        if (inputImage.empty()) {
            cout << "\tError: Failed to open, skipping." << endl;
            continue;
        } else if (inputImage.size != maskImage.size) {
            cout << "\tError: Size mismatch, skipping." << endl;
            continue;
        }

        // 4. Processing pipeline
        memset(&buffer[0], 0, buffer.size() * sizeof(float));
        btx::prepareImage(inputImage.data, inputImage.cols, inputImage.channels(), holePixels, buffer);
        btx::fillHole(buffer, maskImage.cols, holePixels, boundaryPixels, pixelWeightCalculator);
        btx::denormalise(buffer, buffer);

        // 5. Save the output
        std::string outputFilePath = mainArgs.outputDirectory + fs::path(inputFilePath).filename().generic_string();
        cv::imwrite(outputFilePath, bufferMat);
    }

    return 0;
}
