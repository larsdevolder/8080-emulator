#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>    

#include "cpu.hpp"

TEST_CASE("Data Transfer Group") {
    std::stringstream fakerom;
    Cpu cpu;

    SECTION("MVI r, data") {
        auto r = GENERATE(0, 1, 2, 3, 4, 5, 7);
        uint8_t instruction = 0b00000110 | (r << 3);
        fakerom.put(instruction); // MVI A, data
        auto value = GENERATE(take(20, random(0, 255)));
        fakerom.put(value); // data

        REQUIRE(cpu.loadRom(fakerom) == 0);
        cpu.decode();
        cpu.decode();
        REQUIRE(CpuTestWrapper::getreg(r, cpu) == value);
    }
}   