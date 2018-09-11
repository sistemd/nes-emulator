// vim: set shiftwidth=8 tabstop=8:

#pragma once

#include "utils.h"
#include "mirroring.h"
#include <cassert>

namespace Emulator {

std::size_t constexpr tile_width = 8;
std::size_t constexpr tile_height = 8;
using Tile = Matrix<Byte, tile_width, tile_height>;

class VRAM : public Memory {
public:
        static Address constexpr pattern_tables_start = 0x0000;
        static Address constexpr pattern_tables_end = 0x1FFF;
        static Address constexpr pattern_table_size = 0x1000;
        static Address constexpr pattern_tables_size = pattern_tables_end + 1 - pattern_tables_start;

        static Address constexpr name_table_size = 0x03C0;
        static Address constexpr attribute_table_size = 0x0040;
        static Address constexpr name_tables_start = pattern_tables_end + 1;
        static Address constexpr name_tables_end = 0x3EFF;
        static Address constexpr name_tables_size = name_tables_end + 1 - name_tables_start;
        static Address constexpr name_tables_real_size = 0x3000 - name_tables_start;

        static Address constexpr first_name_table_start = name_tables_start;
        static Address constexpr first_name_table_end = first_name_table_start + name_table_size + attribute_table_size - 1;
        static Address constexpr second_name_table_start = first_name_table_end + 1;
        static Address constexpr second_name_table_end = second_name_table_start + name_table_size + attribute_table_size - 1;
        static Address constexpr third_name_table_start = second_name_table_end + 1;
        static Address constexpr third_name_table_end = third_name_table_start + name_table_size + attribute_table_size - 1;
        static Address constexpr fourth_name_table_start = third_name_table_end + 1;
        static Address constexpr fourth_name_table_end = fourth_name_table_start + name_table_size + attribute_table_size - 1;

        static Address constexpr palette_size = 0x0010;
        static Address constexpr background_palette_start = name_tables_end + 1;
        static Address constexpr background_palette_end = background_palette_start + palette_size - 1;
        static Address constexpr sprite_palette_start = background_palette_end + 1;
        static Address constexpr sprite_palette_end = sprite_palette_start + palette_size - 1;
        static Address constexpr palettes_real_size = sprite_palette_end + 1 - background_palette_start;
        static Address constexpr palettes_start = background_palette_start;
        static Address constexpr palettes_end = 0x3FFF;
        static Address constexpr palettes_size = palettes_end + 1 - palettes_start;

        explicit VRAM(Mirroring mirroring) noexcept;

        Tile read_tile(Address address);

private:
        bool address_is_writable_impl(Address address) const noexcept override;
        bool address_is_readable_impl(Address address) const noexcept override;
        void write_byte_impl(Address address, Byte byte) override;
        Byte read_byte_impl(Address address) override;

        template <class Self>
        static auto& memory_destination(Self& self, Address address) noexcept
        {
                if (pattern_tables_start <= address && address <= pattern_tables_end)
                        return self.pattern_tables_[address];
                else if (name_tables_start <= address && address <= name_tables_end)
                        return name_table_memory_destination(self, address);
                else if (palettes_start <= address && address <= palettes_end)
                        return self.palettes_[address % palettes_real_size];
                else
                        return memory_destination(self, address % (palettes_end + 1));
        }

        template <class Self>
        static auto& name_table_memory_destination(Self& self, Address address) noexcept
        {
                address %= name_tables_real_size;
                switch (self.mirroring_) {
                        case Mirroring::horizontal:
                                if ((second_name_table_start <= address && address <= second_name_table_end) ||
                                    (fourth_name_table_start <= address && address <= fourth_name_table_end)) {
                                        address -= name_table_size + attribute_table_size;
                                }
                                break;
                        case Mirroring::vertical:
                                if ((third_name_table_start <= address && address <= third_name_table_end) ||
                                    (fourth_name_table_start <= address && address <= fourth_name_table_end)) {
                                        address -= 2 * (name_table_size + attribute_table_size);
                                }
                                break;
                        case Mirroring::four_screen:
                                break;
                        default:
                                assert(false);
                }
                return self.name_tables_[address];
        }

        Mirroring mirroring_;
        std::array<Byte, pattern_tables_size> pattern_tables_ {0};
        std::array<Byte, name_tables_real_size> name_tables_ {0};
        std::array<Byte, palettes_real_size> palettes_ {0};
};

struct Sprite {
        Byte x = 0;
        Byte y = 0;
        Byte tile_index = 0;
        ByteBitset attributes = 0;

        enum class Priority {
                beneath_background,
                above_background
        };

        Priority priority() const noexcept;
        bool flip_vertically() const noexcept;
        bool flip_horizontally() const noexcept;
        Byte color() const noexcept;
};

static Address constexpr oam_size = 0x0100;
using OAM = std::array<Byte, oam_size>;

std::size_t constexpr screen_width = 256;
std::size_t constexpr screen_height = 240;
using Screen = Matrix<Byte, screen_width, screen_height>;

class DoubleRegister {
public:
        void write_byte(Byte byte) noexcept;
        void write_address(Address value) noexcept;
        void increment(Address offset) noexcept;
        Address read_address() const noexcept;
        Byte read_low_byte() const noexcept;
        Byte read_high_byte() const noexcept;
        bool complete() const noexcept;

private:
        void write_low_byte(Byte byte) noexcept;
        void write_high_byte(Byte byte) noexcept;

        Address value_ = 0;
        bool complete_ = true;
};

class PPU : public Memory {
public:
        static Address constexpr control_register = 0x2000;
        static Address constexpr mask_register = 0x2001;
        static Address constexpr status_register = 0x2002;
        static Address constexpr oam_address_register = 0x2003;
        static Address constexpr oam_data_register = 0x2004;
        static Address constexpr scroll_register = 0x2005;
        static Address constexpr vram_address_register = 0x2006;
        static Address constexpr vram_data_register = 0x2007;
        static Address constexpr oam_dma_register = 0x4014;

        static unsigned constexpr sprite_width = 8;
        static unsigned constexpr background_square_size = 16;
        static unsigned constexpr background_tile_size = 8;

        PPU(Mirroring mirroring, ReadableMemory& dma_memory) noexcept;

        void vblank_started();
        void vblank_finished();

        Address base_name_table_address() const noexcept;
        Address address_increment_offset() const noexcept;
        Address sprite_pattern_table_address() const noexcept;
        Address background_pattern_table_address() const noexcept;
        unsigned sprite_height() const noexcept;
        bool nmi_enabled() const noexcept;
        bool greyscale() const noexcept;
        bool show_leftmost_background() const noexcept;
        bool show_leftmost_sprites() const noexcept;
        bool show_background() const noexcept;
        bool show_sprites() const noexcept;

        Screen screen();

protected:
        bool address_is_writable_impl(Address address) const noexcept override;
        bool address_is_readable_impl(Address address) const noexcept override;
        void write_byte_impl(Address address, Byte byte) override;
        Byte read_byte_impl(Address address) override;
        
private:
        void paint_background(Screen& screen) noexcept;
        Byte sprite_color(unsigned palette_index) noexcept; // Do I need this?

        void increment_oam_address() noexcept;
        void increment_vram_address() noexcept;
        void execute_dma(Byte source);

        ByteBitset control_ = 0;
        ByteBitset mask_ = 0;
        ByteBitset status_ = 0;
        Byte oam_address_ = 0;
        DoubleRegister scroll_;
        DoubleRegister vram_address_;
        Byte vram_data_buffer_ = 0;
        VRAM vram_;
        OAM oam_ {0};
        ReadableMemory& dma_memory_;
};

}

