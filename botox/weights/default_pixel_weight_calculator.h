#ifndef LIGHTRICKS_DEFAULT_PIXEL_WEIGHT_CALCULATOR_H
#define LIGHTRICKS_DEFAULT_PIXEL_WEIGHT_CALCULATOR_H

#include "../botox.h"
#include "pixel_weight_calculator.h"

namespace Botox {

    /**
     * Calculates a pixel weight using the Lightricks algorithm.
     */
    class DefaultPixelWeightCalculator : virtual public IPixelWeightCalculator {
    public:
        /**
         * Constructor. Stores the value of z and e.
         *
         * @param z The power to raise the Euclidean Distance to
         * @param e Small value to avoid divide by zero errors
         */
        DefaultPixelWeightCalculator(float z, float e) : z(z), e(e) {}

        /**
         * Destructor.
         */
        ~DefaultPixelWeightCalculator() override = default;

        /**
         * Gets the pixel weight using the Lightricks algorithm.
         *
         * @param a The first pixel
         * @param b The second pixel
         * @return The calculated pixel weight
         */
        float getPixelWeight(Pixel &a, Pixel &b) override {
            return lightricksPixelWeight(a, b, z, e);
        }

    private:
        float z;
        float e;
    };
}

#endif // LIGHTRICKS_DEFAULT_PIXEL_WEIGHT_CALCULATOR_H
