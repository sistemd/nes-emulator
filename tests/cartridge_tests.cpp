// vim: set shiftwidth=8 tabstop=8:

#include "catch.hpp"
#include "../src/cartridge.h"
#include <array>
#include <string>

using namespace std::string_literals;

TEST_CASE("NEStress header test")
{
        Emulator::Cartridge cartridge("../roms/NEStress.nes"s);

        CHECK(cartridge.num_prg_rom_banks() == 2);
        CHECK(cartridge.num_chr_rom_banks() == 1);
        CHECK(cartridge.has_sram() == false);
        CHECK(cartridge.has_trainer() == false);
        CHECK(cartridge.mirroring() == Emulator::Mirroring::vertical);
        CHECK(cartridge.mmc_id() == Emulator::NROM::id);
        CHECK(cartridge.has_chr_ram() == false);
}

TEST_CASE("The Legend of Zelda header test")
{
        Emulator::Cartridge cartridge("../roms/The Legend of Zelda.nes"s);

        CHECK(cartridge.num_prg_rom_banks() == 8);
        CHECK(cartridge.num_chr_rom_banks() == 0);
        CHECK(cartridge.has_sram() == true);
        CHECK(cartridge.has_trainer() == false);
        CHECK(cartridge.mirroring() == Emulator::Mirroring::horizontal);
        CHECK(cartridge.mmc_id() == Emulator::MMC1::id);
        CHECK(cartridge.has_chr_ram() == true);
}

TEST_CASE("Super Mario Bros. 1 header test")
{
        Emulator::Cartridge cartridge("../roms/Super Mario Bros. 1.nes"s);

        CHECK(cartridge.num_prg_rom_banks() == 2);
        CHECK(cartridge.num_chr_rom_banks() == 1);
        CHECK(cartridge.has_sram() == false);
        CHECK(cartridge.has_trainer() == false);
        CHECK(cartridge.mirroring() == Emulator::Mirroring::vertical);
        CHECK(cartridge.mmc_id() == Emulator::NROM::id);
        CHECK(cartridge.has_chr_ram() == false);
}

TEST_CASE("Super Mario Bros. 2 header test")
{
        Emulator::Cartridge cartridge("../roms/Super Mario Bros. 2.nes"s);

        CHECK(cartridge.num_prg_rom_banks() == 8);
        CHECK(cartridge.num_chr_rom_banks() == 16);
        CHECK(cartridge.has_sram() == false);
        CHECK(cartridge.has_trainer() == false);
        CHECK(cartridge.mirroring() == Emulator::Mirroring::horizontal);
        CHECK(cartridge.mmc_id() == Emulator::MMC3::id);
        CHECK(cartridge.has_chr_ram() == false);
}

TEST_CASE("Super Mario Bros. 3 header test") 
{
        Emulator::Cartridge cartridge("../roms/Super Mario Bros. 3.nes"s);

        CHECK(cartridge.num_prg_rom_banks() == 16);
        CHECK(cartridge.num_chr_rom_banks() == 16);
        CHECK(cartridge.has_sram() == false);
        CHECK(cartridge.has_trainer() == false);
        CHECK(cartridge.mirroring() == Emulator::Mirroring::horizontal);
        CHECK(cartridge.mmc_id() == Emulator::MMC3::id);
        CHECK(cartridge.has_chr_ram() == false);
}

TEST_CASE("Loading a cartridge with a bad footprint should fail")
{
        REQUIRE_THROWS_AS(Emulator::Cartridge("../roms/NEStress bad footprint.nes"s),
                          Emulator::InvalidCartridgeHeader);
}

TEST_CASE("Loading a cartridge that doesn't exist on disk should fail")
{
        REQUIRE_THROWS_AS(Emulator::Cartridge("this shouldn't exist"s),
                          Emulator::CantOpenFile);
}

