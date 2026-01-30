#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

#include "state.hpp"

TEST_CASE_METHOD(Memory, "Memory") {
    SECTION("read") {
        uint16_t address = GENERATE(take(2, random(0, 0xFFFF)));
        uint8_t value = GENERATE(take(2, random(0, 0xFF)));
        this->data[address] = value;
        REQUIRE(value == this->read(address));
    }

    SECTION("read16") {
        uint16_t address = GENERATE(take(2, random(0, 0xFFFF)));
        uint16_t value = GENERATE(take(2, random(0, 0xFFFF)));
        this->data[address] = value & 0xFF;
        this->data[address+1] = value >> 8;
        REQUIRE(value == this->read16(address));
    }

    SECTION("write") {
        uint16_t address = GENERATE(take(2, random(0, 0xFFFF)));
        uint8_t value = GENERATE(take(2, random(0, 0xFF)));
        this->write(address, value);
        REQUIRE(value == this->data[address]);
    }
}