#pragma once

#include <cassert>
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iostream>

#include "state.hpp"

class CpuTestWrapper;

class Cpu {
    protected:
        Registers regs;
        Memory memory;

        void UnimplementedInstruction(uint16_t PC);

        uint16_t read16atPC(); // read next two bytes and increment PC twice

    public:
        int loadRom(std::istream &rom);
        int loadRom(const char* rom);
        int decode();
    
    friend class CpuTestWrapper;
};

class CpuTestWrapper {
    public:
        static uint8_t * getreg(int n, Cpu& cpu) {
            switch (n) {
                case 0: return &cpu.regs.B;
                case 1: return &cpu.regs.C;
                case 2: return &cpu.regs.D;
                case 3: return &cpu.regs.E;
                case 4: return &cpu.regs.H;
                case 5: return &cpu.regs.L;
                case 7: return &cpu.regs.A;
                default:
                    FAIL("Invalid register index: " << n);
                    return 0;  // Keep the compiler happy about return paths
            }
        }
        static uint16_t getRegPairValue(int rp, Cpu& cpu) {
            switch (rp) {
                case 0b00: return cpu.regs.readBC();
                case 0b01: return cpu.regs.readDE();
                case 0b10: return cpu.regs.readHL();
                case 0b11: return cpu.regs.SP;
                default:
                    FAIL("Invalid register pair index: " << rp);
                    return 0;
            }
        }
};