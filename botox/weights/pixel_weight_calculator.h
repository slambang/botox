#ifndef LIGHTRICKS_PIXEL_WEIGHT_CALCULATOR_H
#define LIGHTRICKS_PIXEL_WEIGHT_CALCULATOR_H

namespace Botox {

    /**
     * Calculates the weight of a pixel.
     */
    class IPixelWeightCalculator {
    public:
        /**
         * Destructor.
         */
        virtual ~IPixelWeightCalculator() = default;

        /**
         * Gets the pixel weight.
         *
         * @param a The first pixel
         * @param b The second pixel
         * @return The calculated pixel weight
         */
        virtual float getPixelWeight(Pixel &a, Pixel &b) = 0;
    };
}

#endif // LIGHTRICKS_PIXEL_WEIGHT_CALCULATOR_H
