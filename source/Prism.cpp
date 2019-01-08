#include "Prism.hpp"

namespace prism {
    HSV toHsv(RGB const & rgb)
    {
        return {};
    }

    RGB toRgb(HSV const & hsv)
    {
        return {};
    }

    HSL rgbToHsl(const RGB & rgb)
    {
        return {};
    }

    RGB fromInteger(uint32_t integer)
    {
        auto r = (integer & 0x000000ff) >> 0;
        auto g = (integer & 0x0000ff00) >> 8;
        auto b = (integer & 0x00ff0000) >> 16;
        auto a = (integer & 0xff000000) >> 24;
        return {{r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f}};
    }

    RGBColorSpace::RGBColorSpace(CieXY const & white, CieXY const & red, CieXY const & green, CieXY const & blue)
    {
        mRgbToXyz << red.x(), green.x(), blue.x(), 0,
                    red.y(), green.y(), blue.y(), 0,
                    1 - red.x() - red.y(), 1 - green.x() - green.y(), 1 - blue.x() - blue.y(), 0,
                    0, 0, 0, 1;

        mXyzToRgb = mRgbToXyz.inverse();

        detail::Coefficients scale;
        scale << white.x(), white.y(), 1 - white.x() - white.y(), 1;
        scale = mXyzToRgb * (scale / white.y());
        scale[3] = 1.0f;
        Transformation scaleMatrix = Transformation::Zero();
        scaleMatrix.diagonal() = scale;

        mRgbToXyz = mRgbToXyz * scaleMatrix;
        mXyzToRgb = mRgbToXyz.inverse();
    }

    CieXYZ RGBColorSpace::transform(RGB const & source) const
    {
        return {mRgbToXyz * source.values};
    }

    RGB RGBColorSpace::transform(CieXYZ const & source, RenderingIntent intent) const
    {
        auto ret =  RGB{mXyzToRgb * source.values};
        // TODO rendering intent
        return ret;
    }
    
    RGBColorSpace const & rec2020()
    {
        static const RGBColorSpace ret(
            {0.31271, 0.32902},
            {0.708,   0.292},
            {0.170,   0.797},
            {0.131,   0.046});
        return ret;
    }

    RGBColorSpace const & ws2812()
    {
        static const RGBColorSpace ret(
            {0.28623f, 0.27455f},
            {0.68934f, 0.31051f},
            {0.13173f, 0.77457f},
            {0.13450f, 0.04598f});
        return ret;
    }

    RGBColorSpace const & sRGB()
    {
        static const RGBColorSpace ret(
            {0.31271, 0.32902f},
            {0.64,    0.33},
            {0.3,     0.6},
            {0.15,    0.06});
        return ret;
    }

    RGB linearizeSRGB(RGB rgb)
    {
        RGB ret;
        ret.values = rgb.values.array().pow(2.2f).matrix();
        return ret;
    }

    CieXYZ temperature(detail::ColorScalar t)
    {
        float t2 = t * t;
        float t3 = t2 * t;

        float x, y;
        if(t <= 4000.0f) {
            x = -0.2661239e9f / t3
                -0.2343580e6f / t2
                + 0.8776956e3f / t
                + 0.179910f;
        } else {
            x = -3.0258469e9f / t3
                + 2.1070379e6f / t2
                + 0.2226347e3f / t
                + 0.240390f;
        }

        float x2 = x * x;
        float x3 = x2 * x;

        if(t < 2222.0f) {
            y = -1.1063814f * x3
                - 1.34811020f * x2
                + 2.18555832f * x
                - 0.20219683f;
        } else if(t < 4000.0f) {
            y = -0.9549476f * x3
                - 1.37418593f * x2
                + 2.09137015f * x
                - 0.16748867f;
        } else {
            y = 3.0817580f * x3
                - 5.8733867f * x2
                + 3.75112997f * x
                - 0.37001483f;
        }

        return {{x / y, 1, (1 - x - y) / y, 0}};
    }
}