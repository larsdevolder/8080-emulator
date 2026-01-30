#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

#include "state.hpp"

TEST_CASE_METHOD(Registers, "Registers") {
    SECTION("Read and write register pairs") {
        SECTION("SetPair") {
            uint8_t a;
            uint8_t b;
            uint16_t value = GENERATE(take(3, random(0, 0xFFFF)));
            this->setPair(&a, &b, value);
            REQUIRE(value == (a << 8) + b);
        }

        SECTION("ReadPair") {
            uint8_t a;
            uint8_t b;
            uint16_t value = GENERATE(take(3, random(0, 0xFFFF)));
            a = (value & 0xFF00) >> 8;
            b = value & 0x00FF;
            REQUIRE(value == this->readPair(a, b));
        }

        SECTION("setBC") {
            uint16_t address = GENERATE(take(3, random(0, 0xFFFF)));
            this->setBC(address);
            REQUIRE(this->B == (address & 0xFF00) >> 8);
            REQUIRE(this->C == (address & 0x00FF));
        }

        SECTION("readBC") {
            uint8_t v1 = GENERATE(take(3, random(0, 0xFF)));
            uint8_t v2 = GENERATE(take(3, random(0, 0xFF)));
            this->B = v1;
            this->C = v2;
            uint16_t expected_value = (v1 << 8) + v2;
            REQUIRE(expected_value == this->readBC());
        }

        SECTION("setDE") {
            uint16_t address = GENERATE(take(3, random(0, 0xFFFF)));
            this->setDE(address);
            REQUIRE(this->D == (address & 0xFF00) >> 8);
            REQUIRE(this->E == (address & 0x00FF));
        }

        SECTION("readHL") {
            uint8_t v1 = GENERATE(take(3, random(0, 0xFF)));
            uint8_t v2 = GENERATE(take(3, random(0, 0xFF)));
            this->D = v1;
            this->E = v2;
            uint16_t expected_value = (v1 << 8) + v2;
            REQUIRE(expected_value == this->readDE());
        }

        SECTION("setHL") {
            uint16_t address = GENERATE(take(3, random(0, 0xFFFF)));
            this->setHL(address);
            REQUIRE(this->H == (address & 0xFF00) >> 8);
            REQUIRE(this->L == (address & 0x00FF));
        }

        SECTION("readHL") {
            uint8_t v1 = GENERATE(take(3, random(0, 0xFF)));
            uint8_t v2 = GENERATE(take(3, random(0, 0xFF)));
            this->H = v1;
            this->L = v2;
            uint16_t expected_value = (v1 << 8) + v2;
            REQUIRE(expected_value == this->readHL());
        }
    }
}
