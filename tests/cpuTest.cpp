#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>    

#include "cpu.hpp"

#define GEN_REGS GENERATE(0, 1, 2, 3, 4, 5, 7) // 8 bit registers B, C, D, E, H, L, A
#define GEN_REG_PAIR GENERATE(0b00, 0b01, 0b10, 0b11) // 16 bit register (pairs) BC, DE, HL, SP
#define GEN_ADDRESS GENERATE(take(2, random(0, 0xFFFF))) // 2 random 16 bit addresses
#define GEN_VALUE GENERATE(take(2, random(0, 0xFF))); // 2 random 8 bit values

TEST_CASE_METHOD(Cpu, "CPU") {
    std::stringstream fakerom;

    SECTION("Data Transfer Group") {
        SECTION("MVI") {
            SECTION("MVI r, data") {
                auto r = GEN_REGS;
                uint8_t instruction = 0b00000110 | (r << 3);
                fakerom.put(instruction); // MVI A, data
                auto value = GEN_VALUE;
                fakerom.put(value); // data

                REQUIRE(this->loadRom(fakerom) == 0);
                this->decode();
                REQUIRE(*CpuTestWrapper::getreg(r, *this) == value);
            }

            SECTION("MVI M, data") {
                uint16_t address = GEN_ADDRESS;
                this->regs.setHL(address);
                
                uint8_t instruction = 0b00110110;
                fakerom.put(instruction); // MVI M
                uint8_t data = GEN_VALUE;
                fakerom.put(data);

                REQUIRE(this->loadRom(fakerom) == 0);
                this->decode();
                REQUIRE(this->memory.read(address) == data);
            }
        }
        
        SECTION("MOV") {
            SECTION("MOV r1, r2") {
                auto r1 = GEN_REGS;
                auto r2 = GEN_REGS;
                uint8_t value = GEN_VALUE;

                *CpuTestWrapper::getreg(r2, *this) = value;
                
                uint8_t instruction = 0b01000000 | (r1 << 3) | r2;
                fakerom.put(instruction);
                this->loadRom(fakerom);
                this->decode();
                
                REQUIRE(*CpuTestWrapper::getreg(r1, *this) == value);
            }
            SECTION("MOV r, M") {
                auto r = GEN_REGS;

                uint16_t address = GEN_ADDRESS;
                uint8_t value = GEN_VALUE;
                this->memory.write(address, value);
                this->regs.setHL(address);
                
                uint8_t instruction = 0b01000110 | (r << 3);
                fakerom.put(instruction);
                this->loadRom(fakerom);
                this->decode();

                REQUIRE(value == *CpuTestWrapper::getreg(r, *this));
            }
            SECTION("MOV M, r") {
                auto r = GEN_REGS;
                uint16_t address = GEN_ADDRESS;
                uint8_t value = GEN_VALUE

                *CpuTestWrapper::getreg(r, *this) = value;
                this->regs.setHL(address);

                uint8_t instruction = 0b01110000 | r;
                fakerom.put(instruction);
                this->loadRom(fakerom);
                this->decode();
                
                CAPTURE(r, address, (uint16_t)value);
                // H and L are changed after setting the register to value,
                // so we can't compare the memory location to value we need to get the register value.
                REQUIRE(*CpuTestWrapper::getreg(r, *this) == this->memory.read(address));
            }
        }
        
        SECTION("LXI") {
            auto rp = GEN_REG_PAIR;
            uint8_t byte2 = GEN_VALUE;
            uint8_t byte3 = GEN_VALUE;

            uint16_t instruction = 0b00000001 | (rp << 4);
            fakerom.put(instruction);
            fakerom.put(byte2);
            fakerom.put(byte3);
            this->loadRom(fakerom);
            this->decode();

            uint16_t value = (byte3 << 8) | byte2;
            REQUIRE(value == CpuTestWrapper::getRegPairValue(rp, *this));
        }
        
        SECTION("LDA") {
            uint16_t address = GEN_ADDRESS;
            uint8_t value = GEN_VALUE;
            
            this->memory.write(address, value);
            uint8_t byte2 = address & 0xFF;
            uint8_t byte3 = address >> 8;

            uint8_t instruction = 0b00111010;
            fakerom.put(instruction);
            fakerom.put(byte2);
            fakerom.put(byte3);
            this->loadRom(fakerom);
            this->decode();

            REQUIRE(value == this->regs.A);
        }

        SECTION("STA") {
            uint8_t value = GEN_VALUE;
            uint16_t address = GEN_ADDRESS;

            this->regs.A = value;
            uint8_t byte2 = address & 0xFF;
            uint8_t byte3 = address >> 8;

            uint8_t instruction = 0b00110010;
            fakerom.put(instruction);
            fakerom.put(byte2);
            fakerom.put(byte3);
            this->loadRom(fakerom);
            this->decode();

            REQUIRE(value == this->memory.read(address));
        }

        SECTION("LHLD") {
            uint16_t address = GEN_ADDRESS;
            uint8_t value1 = GEN_VALUE;
            uint8_t value2 = GEN_VALUE;

            this->memory.write(address, value1);
            this->memory.write(address+1, value2);
            uint8_t byte2 = address & 0xFF;
            uint8_t byte3 = address >> 8;

            uint8_t instruction = 0b00101010;
            fakerom.put(instruction);
            fakerom.put(byte2);
            fakerom.put(byte3);
            this->loadRom(fakerom);
            this->decode();

            REQUIRE(value1 == this->regs.L);
            REQUIRE(value2 == this->regs.H);
        }

        SECTION("SHLD") {
            uint16_t address = GEN_ADDRESS;
            uint8_t value1 = GEN_VALUE;
            uint8_t value2 = GEN_VALUE;

            this->regs.L = value1;
            this->regs.H = value2;
            uint8_t byte2 = address & 0xFF;
            uint8_t byte3 = address >> 8;

            uint8_t instruction = 0b00100010;
            fakerom.put(instruction);
            fakerom.put(byte2);
            fakerom.put(byte3);
            this->loadRom(fakerom);
            this->decode();

            REQUIRE(value1 == this->memory.read(address));
            REQUIRE(value2 == this->memory.read(address+1));
        }
    }
}   