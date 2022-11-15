#ifndef LIGHTRICKS_PIXEL_H
#define LIGHTRICKS_PIXEL_H

namespace Botox {
    /**
     * Convenient structure representing a single pixel.
     */
    struct Pixel {
        int x;
        int y;

        Pixel() = default;

        Pixel(int x, int y) : x(x), y(y) {}
    };
}

#endif // LIGHTRICKS_PIXEL_H
