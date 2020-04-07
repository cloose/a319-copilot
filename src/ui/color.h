#ifndef _COLOR_H_
#define _COLOR_H_

#include <array>

struct Color {
    std::array<float, 3> values;

    Color(float r, float g, float b) : values({ r, g, b }) { }

    operator float*() { return values.data(); }
    operator const float*() const { return values.data(); }
};

static const Color color_white { 1.0, 0.0, 0.0 };
static const Color color_red { 1.0, 0.0, 0.0 };
static const Color color_green { 0.0, 1.0, 0.0 };
static const Color color_blue { 0.0, 0.0, 1.0 };

#endif // _COLOR_H_