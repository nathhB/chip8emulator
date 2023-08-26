#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "chip-8.h"

static void TestGetInstruction(void);
static void WriteInstructionInMemory(Chip8 *chip8, uint16_t instruction);
static void TestJPAddr(void);
static void TestCallAddr(void);
static void TestRet(void);
static void TestSeVxByte(void);
static void TestSneVxByte(void);
static void TestSeVxVy(void);
static void TestLdVxByte(void);
static void TestAddVxByte(void);
static void TestLdVxVy(void);
static void TestOr(void);
static void TestAnd(void);
static void TestXor(void);
static void TestAddVxVy(void);
static void TestSub(void);
static void TestSubn(void);
static void TestShr(void);
static void TestShl(void);
static void TestSneVxVy(void);
static void TestLdIAddr(void);
static void TestJpV0Addr(void);
static void TestDrw(void);
static void TestSkp(void);
static void TestSknp(void);
static void TestLdVxK(void);
static void TestLdDtVx(void);
static void TestLdStVx(void);
static void TestAddIVx(void);
static void TestLdFVx(void);
static void TestLdBVx(void);
static void TestLdIVx(void);
static void TestLdVxI(void);

int main(void)
{
    TestGetInstruction();
    TestJPAddr();
    TestCallAddr();
    TestRet();
    TestSeVxByte();
    TestSneVxByte();
    TestSeVxVy();
    TestLdVxByte();
    TestAddVxByte();
    TestLdVxVy();
    TestOr();
    TestAnd();
    TestXor();
    TestAddVxVy();
    TestSub();
    TestSubn();
    TestShr();
    TestShl();
    TestSneVxVy();
    TestLdIAddr();
    TestJpV0Addr();
    TestDrw();
    TestSkp();
    TestSknp();
    TestLdVxK();
    TestLdDtVx();
    TestLdStVx();
    TestAddIVx();
    TestLdFVx();
    TestLdBVx();
    TestLdIVx();
    TestLdVxI();

    return 0;
}

static void TestGetInstruction(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    Chip8_InstructionType instruction_type;
    uint16_t instruction;
    
    WriteInstructionInMemory(&chip8, 0x00E0);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == CLS);
    WriteInstructionInMemory(&chip8, 0x00EE);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == RET);
    WriteInstructionInMemory(&chip8, 0x12AB);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == JP_ADDR);
    WriteInstructionInMemory(&chip8, 0x22AB);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == CALL_ADDR);
    WriteInstructionInMemory(&chip8, 0x32AB);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == SE_VX_BYTE);
    WriteInstructionInMemory(&chip8, 0x42AB);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == SNE_VX_BYTE);
    WriteInstructionInMemory(&chip8, 0x52AB);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == SE_VX_VY);
    WriteInstructionInMemory(&chip8, 0x62AB);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == LD_VX_BYTE);
    WriteInstructionInMemory(&chip8, 0x72AB);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == ADD_VX_BYTE);
    WriteInstructionInMemory(&chip8, 0x82A0);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == LD_VX_VY);
    WriteInstructionInMemory(&chip8, 0x82A1);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == OR);
    WriteInstructionInMemory(&chip8, 0x82A2);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == AND);
    WriteInstructionInMemory(&chip8, 0x82A3);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == XOR);
    WriteInstructionInMemory(&chip8, 0x82A4);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == ADD_VX_VY);
    WriteInstructionInMemory(&chip8, 0x82A5);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == SUB);
    WriteInstructionInMemory(&chip8, 0x82A6);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == SHR);
    WriteInstructionInMemory(&chip8, 0x82A7);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == SUBN);
    WriteInstructionInMemory(&chip8, 0x82AE);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == SHL);
    WriteInstructionInMemory(&chip8, 0x91F0);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == SNE_VX_VY);
    WriteInstructionInMemory(&chip8, 0xA1F0);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == LD_I_ADDR);
    WriteInstructionInMemory(&chip8, 0xB1F2);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == JP_V0_ADDR);
    WriteInstructionInMemory(&chip8, 0xC1F2);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == RND);
    WriteInstructionInMemory(&chip8, 0xD1F2);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == DRW);
    WriteInstructionInMemory(&chip8, 0xE19E);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == SKP);
    WriteInstructionInMemory(&chip8, 0xE1A1);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == SKNP);
    WriteInstructionInMemory(&chip8, 0xF107);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == LD_VX_DT);
    WriteInstructionInMemory(&chip8, 0xF10A);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == LD_VX_K);
    WriteInstructionInMemory(&chip8, 0xFB15);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == LD_DT_VX);
    WriteInstructionInMemory(&chip8, 0xFB18);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == LD_ST_VX);
    WriteInstructionInMemory(&chip8, 0xFB1E);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == ADD_I_VX);
    WriteInstructionInMemory(&chip8, 0xFB29);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == LD_F_VX);
    WriteInstructionInMemory(&chip8, 0xFB33);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == LD_B_VX);
    WriteInstructionInMemory(&chip8, 0xFB55);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == LD_I_VX);
    WriteInstructionInMemory(&chip8, 0xFB65);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == LD_VX_I);
    WriteInstructionInMemory(&chip8, 0xFB12);
    Chip8_GetNextInstruction(&chip8, &instruction_type, &instruction);
    assert(instruction_type == UNKNOWN_INSTRUCTION);
}

static void WriteInstructionInMemory(Chip8 *chip8, uint16_t instruction)
{
    chip8->mem[chip8->pc] = instruction >> 8;
    chip8->mem[chip8->pc + 1] = instruction & 0x00FF;
    chip8->program_len = 2;
}

static void TestJPAddr(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);
    Chip8_ExecuteInstruction(&chip8, JP_ADDR, 0x12E); 

    assert(chip8.pc == 0x12E);

    Chip8_ExecuteInstruction(&chip8, JP_ADDR, 0x3FF); 

    assert(chip8.pc == 0x3FF);
}

static void TestCallAddr(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);
    Chip8_ExecuteInstruction(&chip8, JP_ADDR, 0x12E); 

    assert(chip8.pc == 0x12E);

    Chip8_ExecuteInstruction(&chip8, CALL_ADDR, 0x3FF); 

    assert(chip8.pc == 0x3FF);
    assert(chip8.sp == 1);
    assert(chip8.stack[0] == 0x12E);

    Chip8_ExecuteInstruction(&chip8, CALL_ADDR, 0x4AB); 

    assert(chip8.pc == 0x4AB);
    assert(chip8.sp == 2);
    assert(chip8.stack[0] == 0x12E);
    assert(chip8.stack[1] == 0x3FF);
}

static void TestRet(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);
    Chip8_ExecuteInstruction(&chip8, JP_ADDR, 0x12E); 
    Chip8_ExecuteInstruction(&chip8, CALL_ADDR, 0x3FF); 
    Chip8_ExecuteInstruction(&chip8, CALL_ADDR, 0x4AB); 

    assert(chip8.pc == 0x4AB);

    Chip8_ExecuteInstruction(&chip8, RET, 0x0);

    assert(chip8.pc == 0x3FF);
    assert(chip8.sp == 1);

    Chip8_ExecuteInstruction(&chip8, RET, 0x0);

    assert(chip8.pc == 0x12E);
    assert(chip8.sp == 0);
}

static void TestSeVxByte(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x3] = 0x42;
    chip8.v[0xA] = 0x24;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, SE_VX_BYTE, 0x342);

    assert(ret == 4);

    ret = Chip8_ExecuteInstruction(&chip8, SE_VX_BYTE, 0xA42);

    assert(ret == 2);

    ret = Chip8_ExecuteInstruction(&chip8, SE_VX_BYTE, 0xA24);

    assert(ret == 4);
}

static void TestSneVxByte(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x3] = 0x42;
    chip8.v[0xA] = 0x24;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, SNE_VX_BYTE, 0x342);

    assert(ret == 2);

    ret = Chip8_ExecuteInstruction(&chip8, SNE_VX_BYTE, 0xA42);

    assert(ret == 4);

    ret = Chip8_ExecuteInstruction(&chip8, SNE_VX_BYTE, 0xA24);

    assert(ret == 2);
}

static void TestSeVxVy(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x3] = 0x42;
    chip8.v[0xA] = 0x24;
    chip8.v[0xF] = 0x42;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, SE_VX_VY, 0x3A0);

    assert(ret == 2);

    ret = Chip8_ExecuteInstruction(&chip8, SE_VX_VY, 0x3F0);

    assert(ret == 4);

    ret = Chip8_ExecuteInstruction(&chip8, SE_VX_VY, 0xAF0);

    assert(ret == 2);
}

static void TestLdVxByte(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);
    
    uint16_t ret = Chip8_ExecuteInstruction(&chip8, LD_VX_BYTE, 0xF42);

    assert(ret == 2);
    assert(chip8.v[0xF] == 0x42);

    ret = Chip8_ExecuteInstruction(&chip8, LD_VX_BYTE, 0x2FF);

    assert(ret == 2);
    assert(chip8.v[0x2] == 0xFF);

    ret = Chip8_ExecuteInstruction(&chip8, LD_VX_BYTE, 0x212);

    assert(ret == 2);
    assert(chip8.v[0x2] == 0x12);
}

static void TestAddVxByte(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0xB] = 0x42;
    
    uint16_t ret = Chip8_ExecuteInstruction(&chip8, ADD_VX_BYTE, 0xB10);

    assert(ret == 2);
    assert(chip8.v[0xB] == 0x52);

    ret = Chip8_ExecuteInstruction(&chip8, ADD_VX_BYTE, 0xB24);

    assert(ret == 2);
    assert(chip8.v[0xB] == 0x76);
}

static void TestLdVxVy(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0xA] = 0x42;
    chip8.v[0xF] = 0x2F;
    
    uint16_t ret = Chip8_ExecuteInstruction(&chip8, LD_VX_VY, 0xBA0);

    assert(ret == 2);
    assert(chip8.v[0xB] == 0x42);

    ret = Chip8_ExecuteInstruction(&chip8, LD_VX_VY, 0xAF0);

    assert(ret == 2);
    assert(chip8.v[0xA] == 0x2F);

    ret = Chip8_ExecuteInstruction(&chip8, LD_VX_VY, 0xBA0);

    assert(ret == 2);
    assert(chip8.v[0xB] == 0x2F);
}

static void TestOr(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x1] = 0b10101010;
    chip8.v[0x2] = 0b01010101;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, OR, 0x120);

    assert(ret == 2);
    assert(chip8.v[0x1] == 0xFF);
}

static void TestAnd(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x1] = 0b10101010;
    chip8.v[0x2] = 0b01010101;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, AND, 0x120);

    assert(ret == 2);
    assert(chip8.v[0x1] == 0x0);
}

static void TestXor(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x1] = 0b10101010;
    chip8.v[0x2] = 0b01010101;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, XOR, 0x120);

    assert(ret == 2);
    assert(chip8.v[0x1] == (0b10101010 ^ 0b01010101));
}

static void TestAddVxVy(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x1] = 0x2;
    chip8.v[0x2] = 0xF0;
    chip8.v[0x3] = 0xF7;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, ADD_VX_VY, 0x120);

    assert(ret == 2);
    assert(chip8.v[0x1] == 0xF2);
    assert(chip8.v[0xF] == 0);

    ret = Chip8_ExecuteInstruction(&chip8, ADD_VX_VY, 0x130);
    uint8_t res = (0xF2 + 0xF7) & 0xFF;

    assert(ret == 2);
    assert(chip8.v[0x1] == res);
    assert(chip8.v[0xF] == 1);
}

static void TestSub(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x1] = 0x2;
    chip8.v[0x2] = 0xF0;
    chip8.v[0x3] = 0xF7;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, SUB, 0x210);

    assert(ret == 2);
    assert(chip8.v[0x2] == 0xF0 - 0x2);
    assert(chip8.v[0xF] == 1);

    ret = Chip8_ExecuteInstruction(&chip8, SUB, 0x230);
    uint8_t res = 0xEE - 0xF7;

    assert(ret == 2);
    assert(chip8.v[0x2] == res);
    assert(chip8.v[0xF] == 0);
}

static void TestSubn(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x1] = 0x2;
    chip8.v[0x2] = 0xF0;
    chip8.v[0x3] = 0xF7;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, SUBN, 0x210);
    uint8_t res = 0x2 - 0xF0;

    assert(ret == 2);
    assert(chip8.v[0x2] == res);
    assert(chip8.v[0xF] == 0);

    ret = Chip8_ExecuteInstruction(&chip8, SUBN, 0x230);

    assert(ret == 2);
    assert(chip8.v[0x2] == 0xF7 - res);
    assert(chip8.v[0xF] == 1);
}

static void TestShr(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x1] = 0x4;
    chip8.v[0x2] = 0x43;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, SHR, 0x100);

    assert(ret == 2);
    assert(chip8.v[0x1] == 0x2);
    assert(chip8.v[0xF] == 0);

    ret = Chip8_ExecuteInstruction(&chip8, SHR, 0x200);

    assert(ret == 2);
    assert(chip8.v[0x2] == 0x21);
    assert(chip8.v[0xF] == 1);
}

static void TestShl(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x1] = 0x4;
    chip8.v[0x2] = 0xA2;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, SHL, 0x100);

    assert(ret == 2);
    assert(chip8.v[0x1] == 0x8);
    assert(chip8.v[0xF] == 0);

    ret = Chip8_ExecuteInstruction(&chip8, SHL, 0x200);
    uint8_t res = 0xA2 * 2;

    assert(ret == 2);
    assert(chip8.v[0x2] == res);
    assert(chip8.v[0xF] == 1);
}

static void TestSneVxVy(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x3] = 0x42;
    chip8.v[0xA] = 0x24;
    chip8.v[0xD] = 0x24;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, SNE_VX_VY, 0x3A0);

    assert(ret == 4);

    ret = Chip8_ExecuteInstruction(&chip8, SNE_VX_VY, 0xAD0);

    assert(ret == 2);

    ret = Chip8_ExecuteInstruction(&chip8, SNE_VX_VY, 0xD30);

    assert(ret == 4);
}

static void TestLdIAddr(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, LD_I_ADDR, 0xABC);

    assert(ret == 2);
    assert(chip8.i == 0xABC);
}

static void TestJpV0Addr(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0] = 0xF0;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, JP_V0_ADDR, 0xABC);

    assert(ret == 0);
    assert(chip8.pc == 0xABC + 0xF0);
}

static void TestDrw(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    // sprite in memory (0 digit)
    chip8.mem[0x100] = 0xF0;
    chip8.mem[0x101] = 0x90;
    chip8.mem[0x102] = 0x90;
    chip8.mem[0x103] = 0x90;
    chip8.mem[0x104] = 0xF0;

    // sprite in memory (1 digit)
    chip8.mem[0x105] = 0x20;
    chip8.mem[0x106] = 0x60;
    chip8.mem[0x107] = 0x20;
    chip8.mem[0x108] = 0x20;
    chip8.mem[0x109] = 0x70;

    chip8.v[0x1] = 0x10;
    chip8.v[0x2] = 0x10;
    chip8.v[0x3] = 0x20;

    chip8.i = 0x100; // address to load the 0 digit sprite from

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, DRW, 0x125);

    assert(ret == 2);
    assert(chip8.v[0xF] == 0); // no collision

    // should draw the 0 digit
    unsigned int pos = (0x10 * DISPLAY_WIDTH + 0x10) / 8;

    assert(chip8.display[pos] == 0xF0);
    pos = (0x11 * DISPLAY_WIDTH + 0x10) / 8;
    assert(chip8.display[pos] == 0x90);
    pos = (0x12 * DISPLAY_WIDTH + 0x10) / 8;
    assert(chip8.display[pos] == 0x90);
    pos = (0x13 * DISPLAY_WIDTH + 0x10) / 8;
    assert(chip8.display[pos] == 0x90);
    pos = (0x14 * DISPLAY_WIDTH + 0x10) / 8;
    assert(chip8.display[pos] == 0xF0);

    chip8.i = 0x105; // address to load the 1 digit sprite from

    ret = Chip8_ExecuteInstruction(&chip8, DRW, 0x325);

    assert(ret == 2);
    assert(chip8.v[0xF] == 0); // no collision

    pos = (0x10 * DISPLAY_WIDTH + 0x20) / 8;

    // should draw the 1 digit
    assert(chip8.display[pos] == 0x20);
    pos = (0x11 * DISPLAY_WIDTH + 0x20) / 8;
    assert(chip8.display[pos] == 0x60);
    pos = (0x12 * DISPLAY_WIDTH + 0x20) / 8;
    assert(chip8.display[pos] == 0x20);
    pos = (0x13 * DISPLAY_WIDTH + 0x20) / 8;
    assert(chip8.display[pos] == 0x20);
    pos = (0x14 * DISPLAY_WIDTH + 0x20) / 8;
    assert(chip8.display[pos] == 0x70);

    // should still draw the 0 digit
    pos = (0x10 * DISPLAY_WIDTH + 0x10) / 8;

    assert(chip8.display[pos] == 0xF0);
    pos = (0x11 * DISPLAY_WIDTH + 0x10) / 8;
    assert(chip8.display[pos] == 0x90);
    pos = (0x12 * DISPLAY_WIDTH + 0x10) / 8;
    assert(chip8.display[pos] == 0x90);
    pos = (0x13 * DISPLAY_WIDTH + 0x10) / 8;
    assert(chip8.display[pos] == 0x90);
    pos = (0x14 * DISPLAY_WIDTH + 0x10) / 8;
    assert(chip8.display[pos] == 0xF0);

    // draw another 1 digit on top of the 0 digit
    ret = Chip8_ExecuteInstruction(&chip8, DRW, 0x125);

    assert(ret == 2);
    assert(chip8.v[0xF] == 1); // collision

    pos = (0x10 * DISPLAY_WIDTH + 0x10) / 8;
    assert(chip8.display[pos] == (0xF0 ^ 0x20));
    pos = (0x11 * DISPLAY_WIDTH + 0x10) / 8;
    assert(chip8.display[pos] == (0x90 ^ 0x60));
    pos = (0x12 * DISPLAY_WIDTH + 0x10) / 8;
    assert(chip8.display[pos] == (0x90 ^ 0x20));
    pos = (0x13 * DISPLAY_WIDTH + 0x10) / 8;
    assert(chip8.display[pos] == (0x90 ^ 0x20));
    pos = (0x14 * DISPLAY_WIDTH + 0x10) / 8;
    assert(chip8.display[pos] == (0xF0 ^ 0x70));
}

static uint8_t keys[0xF] = {0};

static uint16_t TestGetKeys(void)
{
    uint16_t keys_bits = 0;

    for (int i = 0; i < 0xF; i++)
    {
        keys_bits |= (keys[i] << (0xF - i));
    }

    return keys_bits;
}

static void TestSkp(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.get_keys = TestGetKeys;
    chip8.v[0x3] = 0xA;
    chip8.v[0x4] = 0xB;

    keys[0xB] = 1;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, SKP, 0x300);

    assert(ret == 2);

    ret = Chip8_ExecuteInstruction(&chip8, SKP, 0x400);

    assert(ret == 4);
}

static void TestSknp(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.get_keys = TestGetKeys;
    chip8.v[0x3] = 0xA;
    chip8.v[0x4] = 0xB;

    keys[0xB] = 1;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, SKNP, 0x300);

    assert(ret == 4);

    ret = Chip8_ExecuteInstruction(&chip8, SKNP, 0x400);

    assert(ret == 2);
}

static void TestLdVxK(void)
{
    memset(keys, 0, sizeof(keys));

    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.get_keys = TestGetKeys;

    assert(Chip8_ExecuteInstruction(&chip8, LD_VX_K, 0xE00) == 0);

    keys[0x2] = 1;

    assert(Chip8_ExecuteInstruction(&chip8, LD_VX_K, 0xE00) == 2);
}

static void TestLdDtVx(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x1] = 1;
    chip8.v[0x2] = 2;

    assert(chip8.dt == 0);

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, LD_DT_VX, 0x100);

    assert(ret == 2);
    assert(chip8.dt == 1);

    ret = Chip8_ExecuteInstruction(&chip8, LD_DT_VX, 0x200);

    assert(ret == 2);
    assert(chip8.dt == 2);
}

static void TestLdStVx(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x1] = 1;
    chip8.v[0x2] = 2;

    assert(chip8.st == 0);

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, LD_ST_VX, 0x100);

    assert(ret == 2);
    assert(chip8.st == 1);

    ret = Chip8_ExecuteInstruction(&chip8, LD_ST_VX, 0x200);

    assert(ret == 2);
    assert(chip8.st == 2);
}

static void TestAddIVx(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x1] = 0x10;
    chip8.v[0x2] = 0x20;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, ADD_I_VX, 0x100);

    assert(ret == 2);
    assert(chip8.i == 0x10);

    ret = Chip8_ExecuteInstruction(&chip8, ADD_I_VX, 0x200);

    assert(ret == 2);
    assert(chip8.i == 0x30);
}

static void TestLdFVx(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.v[0x1] = 0x1;
    chip8.v[0x3] = 0xA;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, LD_F_VX, 0x100);

    assert(ret == 2);
    assert(chip8.i == SPRITE_SIZE);
    
    ret = Chip8_ExecuteInstruction(&chip8, LD_F_VX, 0x200);

    assert(ret == 2);
    assert(chip8.i == 0);

    ret = Chip8_ExecuteInstruction(&chip8, LD_F_VX, 0x300);

    assert(ret == 2);
    assert(chip8.i == SPRITE_SIZE * 0xA);
}

static void TestLdBVx(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.i = 0x250;

    chip8.v[0xB] = 242;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, LD_B_VX, 0xB00);

    assert(ret == 2);
    assert(chip8.mem[0x250] == 2);
    assert(chip8.mem[0x251] == 4);
    assert(chip8.mem[0x252] == 2);
}

static void TestLdIVx(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.i = 0x250;

    chip8.v[0x0] = 0x10;
    chip8.v[0x1] = 0x20;
    chip8.v[0x2] = 0x30;
    chip8.v[0x3] = 0x40;
    chip8.v[0x4] = 0x50;
    chip8.v[0x5] = 0x60;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, LD_I_VX, 0x200);

    assert(ret == 2);
    assert(chip8.mem[0x250] == 0x10);
    assert(chip8.mem[0x251] == 0x20);
    assert(chip8.mem[0x252] == 0x30);
    assert(chip8.mem[0x253] == 0x0);
    assert(chip8.mem[0x254] == 0x0);
    assert(chip8.mem[0x255] == 0x0);

    ret = Chip8_ExecuteInstruction(&chip8, LD_I_VX, 0x400);

    assert(ret == 2);
    assert(chip8.mem[0x250] == 0x10);
    assert(chip8.mem[0x251] == 0x20);
    assert(chip8.mem[0x252] == 0x30);
    assert(chip8.mem[0x253] == 0x40);
    assert(chip8.mem[0x254] == 0x50);
    assert(chip8.mem[0x255] == 0x0);
}

static void TestLdVxI(void)
{
    Chip8 chip8;

    Chip8_Init(&chip8);

    chip8.i = 0x250;

    chip8.mem[0x250] = 0x10;
    chip8.mem[0x251] = 0x20;
    chip8.mem[0x252] = 0x30;
    chip8.mem[0x253] = 0x40;
    chip8.mem[0x254] = 0x50;
    chip8.mem[0x255] = 0x60;

    uint16_t ret = Chip8_ExecuteInstruction(&chip8, LD_VX_I, 0x200);

    assert(ret == 2);
    assert(chip8.v[0x0] == 0x10);
    assert(chip8.v[0x1] == 0x20);
    assert(chip8.v[0x2] == 0x30);
    assert(chip8.v[0x3] == 0x0);
    assert(chip8.v[0x4] == 0x0);
    assert(chip8.v[0x5] == 0x0);
}
