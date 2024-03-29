// vim: set shiftwidth=8 tabstop=8:

#include "rendering.h"

namespace Emulator {

namespace {

unsigned constexpr pixel_width = 2;
unsigned constexpr pixel_height = 2;

void render_pixel(Sdl::Renderer& renderer, Sdl::Color color, unsigned x, unsigned y)
{
        Sdl::Rect const rect {
                .x = static_cast<int>(x * pixel_width),
                .y = static_cast<int>(y * pixel_height),
                .w = pixel_width,
                .h = pixel_height
        };
        Sdl::render_filled_rect(renderer, rect, color);
}

}

UnknownColor::UnknownColor(Byte nes_color) noexcept
        : runtime_error("Unknown NES color " + format_hex(nes_color))
{}

void render_screen(Sdl::Renderer& renderer, Screen const& screen)
{
        for (unsigned y = 0; y < screen_height; ++y) {
                for (unsigned x = 0; x < screen_width; ++x) {
                        auto const nes_color = screen[x][y];
                        auto const rgb_color = nes_color_to_rgb(nes_color);
                        render_pixel(renderer, rgb_color, x, y);
                }
        }
}

Sdl::Color nes_color_to_rgb(Byte nes_color)
{
    switch (nes_color) {
            case 0x00: return {.r = 0x6D, .g = 0x6D, .b = 0x6D, .a = 0xFF};
            case 0x01: return {.r = 0x00, .g = 0x24, .b = 0x91, .a = 0xFF};
            case 0x02: return {.r = 0x00, .g = 0x00, .b = 0xDA, .a = 0xFF};
            case 0x03: return {.r = 0x6D, .g = 0x48, .b = 0xDA, .a = 0xFF};
            case 0x04: return {.r = 0x91, .g = 0x00, .b = 0x6D, .a = 0xFF};
            case 0x05: return {.r = 0xB6, .g = 0x00, .b = 0x6D, .a = 0xFF};
            case 0x06: return {.r = 0xB6, .g = 0x24, .b = 0x00, .a = 0xFF};
            case 0x07: return {.r = 0x91, .g = 0x48, .b = 0x00, .a = 0xFF};
            case 0x08: return {.r = 0x6D, .g = 0x48, .b = 0x00, .a = 0xFF};
            case 0x09: return {.r = 0x24, .g = 0x48, .b = 0x00, .a = 0xFF};
            case 0x0A: return {.r = 0x00, .g = 0x6D, .b = 0x24, .a = 0xFF};
            case 0x0B: return {.r = 0x00, .g = 0x91, .b = 0x00, .a = 0xFF};
            case 0x0C: return {.r = 0x00, .g = 0x48, .b = 0x48, .a = 0xFF};
            case 0x0D: return {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF};
            case 0x0E: return {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF};
            case 0x0F: return {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF};
            case 0x10: return {.r = 0xB6, .g = 0xB6, .b = 0xB6, .a = 0xFF};
            case 0x11: return {.r = 0x00, .g = 0x6D, .b = 0xDA, .a = 0xFF};
            case 0x12: return {.r = 0x00, .g = 0x48, .b = 0xFF, .a = 0xFF};
            case 0x13: return {.r = 0x91, .g = 0x00, .b = 0xFF, .a = 0xFF};
            case 0x14: return {.r = 0xB6, .g = 0x00, .b = 0xFF, .a = 0xFF};
            case 0x15: return {.r = 0xFF, .g = 0x00, .b = 0x91, .a = 0xFF};
            case 0x16: return {.r = 0xFF, .g = 0x00, .b = 0x00, .a = 0xFF};
            case 0x17: return {.r = 0xDA, .g = 0x6D, .b = 0x00, .a = 0xFF};
            case 0x18: return {.r = 0x91, .g = 0x6D, .b = 0x00, .a = 0xFF};
            case 0x19: return {.r = 0x24, .g = 0x91, .b = 0x00, .a = 0xFF};
            case 0x1A: return {.r = 0x00, .g = 0x91, .b = 0x00, .a = 0xFF};
            case 0x1B: return {.r = 0x00, .g = 0xB6, .b = 0x6D, .a = 0xFF};
            case 0x1C: return {.r = 0x00, .g = 0x91, .b = 0x91, .a = 0xFF};
            case 0x1D: return {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF};
            case 0x1E: return {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF};
            case 0x1F: return {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF};
            case 0x20: return {.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF};
            case 0x21: return {.r = 0x6D, .g = 0xB6, .b = 0xFF, .a = 0xFF};
            case 0x22: return {.r = 0x91, .g = 0x91, .b = 0xFF, .a = 0xFF};
            case 0x23: return {.r = 0xDA, .g = 0x6D, .b = 0xFF, .a = 0xFF};
            case 0x24: return {.r = 0xFF, .g = 0x00, .b = 0xFF, .a = 0xFF};
            case 0x25: return {.r = 0xFF, .g = 0x6D, .b = 0xFF, .a = 0xFF};
            case 0x26: return {.r = 0xFF, .g = 0x91, .b = 0x00, .a = 0xFF};
            case 0x27: return {.r = 0xFF, .g = 0xB6, .b = 0x00, .a = 0xFF};
            case 0x28: return {.r = 0xDA, .g = 0xDA, .b = 0x00, .a = 0xFF};
            case 0x29: return {.r = 0x6D, .g = 0xDA, .b = 0x00, .a = 0xFF};
            case 0x2A: return {.r = 0x00, .g = 0xFF, .b = 0x00, .a = 0xFF};
            case 0x2B: return {.r = 0x48, .g = 0xFF, .b = 0xDA, .a = 0xFF};
            case 0x2C: return {.r = 0x00, .g = 0xFF, .b = 0xFF, .a = 0xFF};
            case 0x2D: return {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF};
            case 0x2E: return {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF};
            case 0x2F: return {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF};
            case 0x30: return {.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF};
            case 0x31: return {.r = 0xB6, .g = 0xDA, .b = 0xFF, .a = 0xFF};
            case 0x32: return {.r = 0xDA, .g = 0xB6, .b = 0xFF, .a = 0xFF};
            case 0x33: return {.r = 0xFF, .g = 0xB6, .b = 0xFF, .a = 0xFF};
            case 0x34: return {.r = 0xFF, .g = 0x91, .b = 0xFF, .a = 0xFF};
            case 0x35: return {.r = 0xFF, .g = 0xB6, .b = 0xB6, .a = 0xFF};
            case 0x36: return {.r = 0xFF, .g = 0xDA, .b = 0x91, .a = 0xFF};
            case 0x37: return {.r = 0xFF, .g = 0xFF, .b = 0x48, .a = 0xFF};
            case 0x38: return {.r = 0xFF, .g = 0xFF, .b = 0x6D, .a = 0xFF};
            case 0x39: return {.r = 0xB6, .g = 0xFF, .b = 0x48, .a = 0xFF};
            case 0x3A: return {.r = 0x91, .g = 0xFF, .b = 0x6D, .a = 0xFF};
            case 0x3B: return {.r = 0x48, .g = 0xFF, .b = 0xDA, .a = 0xFF};
            case 0x3C: return {.r = 0x91, .g = 0xDA, .b = 0xFF, .a = 0xFF};
            case 0x3D: return {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF};
            case 0x3E: return {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF};
            case 0x3F: return {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF};
            //default: throw UnknownColor(nes_color);
            default: return {.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF};
    }
}

}

