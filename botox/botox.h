#ifndef LIGHTRICKS_BOTOX_H
#define LIGHTRICKS_BOTOX_H

#include <vector>
#include "pixel.h"
#include "weights/pixel_weight_calculator.h"

namespace Botox {
    /**
     * The default masking value.
     */
    const float DEFAULT_MASK_VALUE = -1.0f;

    /**
     * Pixel connectivity mode.
     */
    enum PixelConnectivity {
        FOUR = 4,
        EIGHT = 8
    };

    /**
     * Calculates the average value of all channels for a given frame.
     *
     * @param input The input array
     * @param frame the frame at which to calculate the average
     * @param channels Number of channels per frame
     * @return The average value for all channels
     */
    u_char channelAverage(const u_char *input, int frame, int channels);

    /**
     * Normalises the input value to the range [0.0f..1.0f].
     *
     * @param input The input value
     * @return The normalised value
     */
    float normalise(float input);

    /**
     * Denormalises the input value to the range [-128..127].
     *
     * @param input The input value
     * @param output The denormalised value
     */
    void denormalise(const std::vector<float> &input, std::vector<float> &output);

    /**
     * Finds hole pixels in the given input data, storing the result in the holePixels vector.
     *
     * @param input The input data to search
     * @param holePixels The output vector to store hole pixels
     * @param intensityThreshold The threshold of a hole pixel (range [0.0..1.0])
     * @param channels The number of channels in the input data
     * @param height The height of the image input data
     * @param width The width of the image input data
     */
    void findHole(const u_char *input, std::vector<Pixel> &holePixels,
                  float intensityThreshold, int channels, int height, int width);

    /**
     * Finds boundary pixels around given a list of hole pixels.
     *
     * @param input The input data to search
     * @param holePixels The list of hole pixels
     * @param width The width of the image input data
     * @param channels The number of channels in the input data
     * @param intensityThreshold The threshold of a hole pixel (range [0.0..1.0])
     * @param boundaries The output vector to store boundary pixels
     * @param connectivity The pixel connectivity between hole and boundary pixels
     */
    void findBoundaries(const u_char *input, const std::vector<Pixel> &holePixels, int width, int channels,
                        float intensityThreshold, std::vector<Pixel> &boundaries, PixelConnectivity connectivity);

    /**
     * Fills a hole in an image using the given IPixelWeightCalculator for estimation.
    *
     * @param input The input image in which to fll the hole
     * @param width The width of the input image
     * @param holePixels THe list of hole pixels in the image
     * @param boundaryPixels The list of boundary pixels in the image
     * @param pixelWeightCalculator The weight calculator for estimation
     */
    void fillHole(std::vector<float> &input, int width, std::vector<Pixel> &holePixels,
                  std::vector<Pixel> &boundaryPixels, IPixelWeightCalculator &pixelWeightCalculator);

    /**
     * Fills a hole in an image using the connected pixels.
    *
     * @param input The input image in which to fll the hole
     * @param width The width of the input image
     * @param holePixels THe list of hole pixels in the image
     */
    void fillHole2(std::vector<float> &input, int width, std::vector<Pixel> &holePixels);

    /**
     * Calculates the Euclidean Distance between 2 pixels.
     *
     * @param a The first pixel
     * @param b The second pixel
     * @return The Euclidean Distance
     */
    float euclideanDistance(Pixel &a, Pixel &b);

    /**
     * Calculates a pixel weight using absolute value of an Euclidean Distance to the power of a given value.
     *
     * @param a The first pixel
     * @param b The second pixel
     * @param z The power to raise the Euclidean Distance to
     * @param e Small value to avoid divide by zero errors
     * @return The estimated weight between the 2 pixels
     */
    float lightricksPixelWeight(Pixel &a, Pixel &b, float z, float e);

    /**
     * A very convenient method to convert multi-channel input data
     * to greyscale as normalised data in the range [0.0f..1.0f].
     *
     * Also merges mask data over the output result using the specified value (default is -1.0f).
     *
     * @param input The input multi-channel data
     * @param height The height of the input data
     * @param channels The number of channels in the input data
     * @param mask The mask to apply
     * @param output The output result data
     * @param holeValue The value to assign to each hole pixel in the output data
     */
    void prepareImage(const u_char *input, int height, int channels, const std::vector<Pixel> &mask,
                      std::vector<float> &output, float holeValue = DEFAULT_MASK_VALUE);
}

#endif // LIGHTRICKS_BOTOX_H
