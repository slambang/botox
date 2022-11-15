#include <limits>
#include <cmath>
#include <iostream>
#include "botox.h"

using namespace std;

namespace Botox {

    u_char channelAverage(const u_char *input, int frame, int channels) {
        u_char average = 0;
        for (int i = 0; i < channels; ++i) {
            average += (input[i + frame * channels] - average) / (channels + 1);
        }
        return average;
    }

    float normalise(float input) {
        return (input - std::numeric_limits<u_char>::lowest()) /
               (std::numeric_limits<u_char>::max() - std::numeric_limits<u_char>::lowest());
    }

    void denormalise(const std::vector<float> &input, std::vector<float> &output) {
        for (int i = 0; i < output.size(); ++i) {
            output[i] = std::numeric_limits<u_char>::max() * input[i];
        }
    }

    void findHole(const u_char *input, std::vector<Pixel> &output,
                  float intensityThreshold, int channels, int height, int width) {

        float thresholdValue = std::numeric_limits<u_char>::max() * intensityThreshold;

        for (int x = 0; x < height; x++) {
            for (int y = 0; y < width; y++) {

                float maskAverage = channelAverage(input, x * width + y, channels);
                if (maskAverage < thresholdValue) {
                    output.emplace_back(Pixel(x, y));
                }
            }
        }
    }

    void findBoundaries(const u_char *input, const std::vector<Pixel> &holePixels, int width, int channels,
                        float intensityThreshold, std::vector<Pixel> &boundaries, PixelConnectivity connectivity) {

        float thresholdValue = std::numeric_limits<u_char>::max() * intensityThreshold;

        for (auto &pixel : holePixels) {
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {

                    // Skip the corners
                    if (connectivity == FOUR && abs(x) + abs(y) == 2) continue;

                    float boundaryCandidate = channelAverage(input, (pixel.x + x) * width + (pixel.y + y), channels);
                    if (boundaryCandidate > thresholdValue) {

                        boundaries.emplace_back(Pixel((pixel.x + x), (pixel.y + y)));
                    }
                }
            }
        }
    }

    void fillHole(std::vector<float> &input, int width, std::vector<Pixel> &holePixels,
                  std::vector<Pixel> &boundaryPixels, IPixelWeightCalculator &pixelWeightCalculator) {

        for (auto &holePixel : holePixels) {

            float weightSum = 0.0f;
            float dotProductSum = 0.0f;

            for (auto &boundaryPixel : boundaryPixels) {
                float weight = pixelWeightCalculator.getPixelWeight(holePixel, boundaryPixel);
                weightSum += weight;
                dotProductSum += weight * input[boundaryPixel.x * width + boundaryPixel.y];
            }

            input[holePixel.x * width + holePixel.y] = dotProductSum / weightSum;
        }
    }

    float lightricksPixelWeight(Pixel &a, Pixel &b, float z, float e) {
        float distance = euclideanDistance(a, b);
        float absDistance = abs(distance);
        return 1.0f / (float) (pow(absDistance, z) + e);
    }

    float euclideanDistance(Pixel &a, Pixel &b) {
        int xPow = pow(a.x - b.x, 2);
        int yPow = pow(a.y - b.y, 2);
        return (float) sqrt(xPow + yPow);
    }

    void prepareImage(const u_char *input, int height, int channels,
                      const std::vector<Pixel> &mask, std::vector<float> &output,
                      float holeValue) {

        // Convert to greyscale and normalise
        for (int i = 0; i < output.size(); ++i) {
            output[i] = channelAverage(input, i, channels);
            output[i] = normalise(output[i]);
        }

        // Carve the hole
        for (auto &pixel : mask) {
            output[pixel.x * height + pixel.y] = holeValue;
        }
    }
}
