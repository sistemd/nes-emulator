// vim: set shiftwidth=8 tabstop=8:

#pragma once

#include "utils.h"
#include "mirroring.h"
#include <stdexcept>
#include <vector>
#include <memory>
#include <string>

namespace Emulator {

class MemoryMapperNotSupported : public std::runtime_error {
public:
        explicit MemoryMapperNotSupported(Byte id) noexcept;
};

class InvalidCartridgeHeader : public std::runtime_error {
public:
        using runtime_error::runtime_error;
};

class InvalidCartridge : public std::runtime_error {
public:
        using runtime_error::runtime_error;
};

class Cartridge {
public: 
        static unsigned constexpr header_size = 0x10;
        static Address constexpr prg_rom_bank_size = 0x4000;
        static Address constexpr prg_rom_lower_bank_start = 0x8000;
        static Address constexpr prg_rom_lower_bank_end = prg_rom_lower_bank_start + prg_rom_bank_size - 1;
        static Address constexpr prg_rom_upper_bank_start = prg_rom_lower_bank_end + 1;
        static Address constexpr prg_rom_upper_bank_end = prg_rom_upper_bank_start + prg_rom_bank_size - 1;

        explicit Cartridge(std::string const& path);
        explicit Cartridge(std::vector<Byte> data);

        static bool is_prg_rom(Address address) noexcept;

        Byte num_prg_rom_banks() const noexcept;
        Byte num_chr_rom_banks() const noexcept;
        Byte mmc_id() const noexcept;
        bool has_sram() const noexcept;
        bool has_trainer() const noexcept;
        Mirroring mirroring() const noexcept;
        bool has_chr_ram() const noexcept;
        ByteBitset first_control_byte() const noexcept;
        ByteBitset second_control_byte() const noexcept;
        Byte read_prg_rom_byte(Address address) const;

private:
        std::vector<Byte> data_;

        Address apply_mirroring(Address address) const noexcept;
        void check_data_size() const;
        void check_header_footprint() const;
};

class MemoryMapper : public Memory {
public:
        static Address constexpr prg_ram_start = 0x6000;
        static Address constexpr prg_ram_bank_size = 0x2000;
        static Address constexpr prg_ram_end = prg_ram_start + prg_ram_bank_size - 1; 
        
        static std::unique_ptr<MemoryMapper> make(Cartridge const& cartridge);
};

class NROM : public MemoryMapper {
public:
        static Byte constexpr id = 0;
        explicit NROM(Cartridge const& cartridge);
        static bool is_prg_ram(Address address) noexcept;

protected:
        bool address_is_writable_impl(Address address) const noexcept override;
        bool address_is_readable_impl(Address address) const noexcept override;
        void write_byte_impl(Address address, Byte byte) override;
        Byte read_byte_impl(Address address) override;

private:
        Cartridge const& cartridge_;
        std::array<Byte, 0x2000> prg_ram_ {0};
};

class MMC1 : public MemoryMapper {
public:
        static Byte constexpr id = 1;
};

class MMC3 : public MemoryMapper {
public:
        static Byte constexpr id = 4;
};

}

