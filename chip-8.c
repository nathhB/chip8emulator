#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "chip-8.h"
#include "util.h"

#define ADDR(instr) (instr & 0xFFF)
#define NIBBLE(instr) (instr & 0xF)
#define HIGH_BYTE(instr) (instr >> 8)
#define LOW_BYTE(instr) (instr & 0xFF)
#define KEY_MASK(k) (0x1 << (0xF - k))

// --- op handlers ---
static uint16_t JpAddrHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t CallAddrHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t RetHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t SeVxByteHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t SneVxByteHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t SeVxVyHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t LdVxByteHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t AddVxByteHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t LdVxVyHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t OrHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t AndHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t XorHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t AddVxVyHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t SubHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t SubnHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t ShrHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t ShlHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t SneVxVyHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t LdIAddrHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t JpV0AddrHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t RndHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t DrwHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t ClsHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t SkpHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t SknpHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t LdVxDtHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t LdVxKHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t LdDtVxHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t LdStVxHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t AddIVxHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t LdFVxHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t LdBVxHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t LdIVxHandler(Chip8 *chip8, uint16_t instruction);
static uint16_t LdVxIHandler(Chip8 *chip8, uint16_t instruction);
// -------------------

static void StoreDigitSpritesInMemory(Chip8 *chip8);
static void PutAddrOnStack(Chip8 *chip8, uint16_t addr);
static uint16_t GetAddrFromStack(Chip8 *chip8);
static void GetInstructionRegisters(uint16_t instruction, uint8_t *reg_x, uint8_t *reg_y);
static void DrawPixel(Chip8 *chip8, unsigned int draw_pos, uint8_t sprite_pixel, unsigned int *collision);

void Chip8_Init(Chip8 *chip8)
{
    srand(time(NULL));
    memset(chip8, 0, sizeof(Chip8));

    chip8->pc = PROGRAM_START_ADDR;
    chip8->program_len = 0;

    for (int i = 0; i < INSTRUCTION_COUNT; i++)
    {
        chip8->instruction_handlers[i] = NULL;
    }

    StoreDigitSpritesInMemory(chip8); 

    chip8->instruction_handlers[RET] = RetHandler;
    chip8->instruction_handlers[JP_ADDR] = JpAddrHandler;
    chip8->instruction_handlers[JP_V0_ADDR] = JpV0AddrHandler;
    chip8->instruction_handlers[CALL_ADDR] = CallAddrHandler;

    chip8->instruction_handlers[SE_VX_BYTE] = SeVxByteHandler;
    chip8->instruction_handlers[SE_VX_VY] = SeVxVyHandler;
    chip8->instruction_handlers[SNE_VX_BYTE] = SneVxByteHandler;
    chip8->instruction_handlers[SNE_VX_VY] = SneVxVyHandler;
    chip8->instruction_handlers[SKP] = SkpHandler;
    chip8->instruction_handlers[SKNP] = SknpHandler;

    chip8->instruction_handlers[LD_VX_BYTE] = LdVxByteHandler;
    chip8->instruction_handlers[LD_VX_VY] = LdVxVyHandler;
    chip8->instruction_handlers[LD_I_ADDR] = LdIAddrHandler;
    chip8->instruction_handlers[LD_VX_DT] = LdVxDtHandler;
    chip8->instruction_handlers[LD_VX_K] = LdVxKHandler;
    chip8->instruction_handlers[LD_DT_VX] = LdDtVxHandler;
    chip8->instruction_handlers[LD_ST_VX] = LdStVxHandler;
    chip8->instruction_handlers[LD_F_VX] = LdFVxHandler;
    chip8->instruction_handlers[LD_B_VX] = LdBVxHandler;
    chip8->instruction_handlers[LD_I_VX] = LdIVxHandler;
    chip8->instruction_handlers[LD_VX_I] = LdVxIHandler;

    chip8->instruction_handlers[ADD_VX_BYTE] = AddVxByteHandler;
    chip8->instruction_handlers[ADD_VX_VY] = AddVxVyHandler;
    chip8->instruction_handlers[ADD_I_VX] = AddIVxHandler;
    chip8->instruction_handlers[SUB] = SubHandler;
    chip8->instruction_handlers[SUBN] = SubnHandler;
    chip8->instruction_handlers[SHR] = ShrHandler;
    chip8->instruction_handlers[SHL] = ShlHandler;
    chip8->instruction_handlers[RND] = RndHandler;

    chip8->instruction_handlers[OR] = OrHandler;
    chip8->instruction_handlers[AND] = AndHandler;
    chip8->instruction_handlers[XOR] = XorHandler;

    chip8->instruction_handlers[DRW] = DrwHandler;
    chip8->instruction_handlers[CLS] = ClsHandler;
}

void Chip8_Reset(Chip8 *chip8)
{
    memset(chip8->v, 0, sizeof(chip8->v));
    memset(chip8->stack, 0, sizeof(chip8->stack));
    memset(chip8->display, 0, sizeof(chip8->display));

    chip8->dt = 0;
    chip8->st = 0;
    chip8->i = 0;
    chip8->pc = PROGRAM_START_ADDR;
    chip8->sp = 0;
    chip8->time_acc = 0;
}

int Chip8_Load(Chip8 *chip8, uint8_t *data, unsigned int len)
{
    if (len > RAM_SIZE - PROGRAM_START_ADDR)
    {
        return -1;
    }

    // load the program in RAM
    memcpy(chip8->mem + PROGRAM_START_ADDR, data, len);

    chip8->program_len = len;

    return 0;
}

int Chip8_LoadFromFile(Chip8 *chip8, const char *path)
{
    FILE *f = fopen(path, "rb");

    if (!f)
    {
        return -1;
    }

    uint8_t data[RAM_SIZE - PROGRAM_START_ADDR];
    size_t len = fread(data, 1, sizeof(data), f);

    fclose(f);

    if (!len)
    {
        return -1;
    }

    return Chip8_Load(chip8, data, len);
}

int Chip8_GetNextInstruction(Chip8 *chip8, Chip8_InstructionType *instruction_type, uint16_t *instruction)
{
    if (chip8->pc >= PROGRAM_START_ADDR + chip8->program_len)
    {
        return 0;
    }

    uint8_t high_byte = chip8->mem[chip8->pc];
    uint8_t low_byte = chip8->mem[chip8->pc + 1];
    uint8_t opcode = high_byte >> 4;

    *instruction_type = UNKNOWN_INSTRUCTION;
    *instruction = ((high_byte & 0x0F) << 8) | (uint16_t)low_byte;

    switch (opcode)
    {
        case 0x00:
            if (*instruction == 0xE0)
            {
                *instruction_type = CLS;
                break;
            }
            else if (*instruction == 0xEE)
            {
                *instruction_type = RET;
                break;
            }
            break;

        case 0x01:
            *instruction_type = JP_ADDR;
            break;

        case 0x02:
            *instruction_type = CALL_ADDR;
            break;

        case 0x03:
            *instruction_type = SE_VX_BYTE;
            break;

        case 0x04:
            *instruction_type = SNE_VX_BYTE;
            break;

        case 0x05:
            *instruction_type = SE_VX_VY;
            break;

        case 0x06:
            *instruction_type = LD_VX_BYTE;
            break;

        case 0x07:
            *instruction_type = ADD_VX_BYTE;
            break;

        case 0x08:
            switch (low_byte & 0x0F)
            {
                case 0x00:
                    *instruction_type = LD_VX_VY;
                    break;

                case 0x01:
                    *instruction_type = OR;
                    break;

                case 0x02:
                    *instruction_type = AND;
                    break;

                case 0x03:
                    *instruction_type = XOR;
                    break;

                case 0x04:
                    *instruction_type = ADD_VX_VY;
                    break;

                case 0x05:
                    *instruction_type = SUB;
                    break;

                case 0x06:
                    *instruction_type = SHR;
                    break;

                case 0x07:
                    *instruction_type = SUBN;
                    break;

                case 0x0E:
                    *instruction_type = SHL;
                    break;
            }
            break;

        case 0x09:
            *instruction_type = SNE_VX_VY;
            break;

        case 0x0A:
            *instruction_type = LD_I_ADDR;
            break;

        case 0x0B:
            *instruction_type = JP_V0_ADDR;
            break;

        case 0x0C:
            *instruction_type = RND;
            break;

        case 0x0D:
            *instruction_type = DRW;
            break;

        case 0x0E:
            switch (low_byte)
            {
                case 0x9E:
                    *instruction_type = SKP;
                    break;

                case 0xA1:
                    *instruction_type = SKNP;
                    break;
            }
            break;

        case 0x0F:
            switch (low_byte)
            {
                case 0x07:
                    *instruction_type = LD_VX_DT;
                    break;

                case 0x0A:
                    *instruction_type = LD_VX_K;
                    break;

                case 0x15:
                    *instruction_type = LD_DT_VX;
                    break;

                case 0x18:
                    *instruction_type = LD_ST_VX;
                    break;

                case 0x1E:
                    *instruction_type = ADD_I_VX;
                    break;

                case 0x29:
                    *instruction_type = LD_F_VX;
                    break;

                case 0x33:
                    *instruction_type = LD_B_VX;
                    break;

                case 0x55:
                    *instruction_type = LD_I_VX;
                    break;

                case 0x65:
                    *instruction_type = LD_VX_I;
                    break;
            }
            break;
    }

    return 1;
}

uint16_t Chip8_ExecuteInstruction(Chip8 *chip8, Chip8_InstructionType opcode, uint16_t instruction)
{
    Chip8_InstructionHandler handler = chip8->instruction_handlers[opcode];

    if (!handler)
    {
        return 0;
    }

    return handler(chip8, instruction);
}

unsigned int Chip8_GetPixel(Chip8 *chip8, unsigned int pos)
{
    uint8_t byte = chip8->display[pos / 8];
    unsigned int offset = 7 - (pos % 8);

    return (byte & (1 << offset)) >> offset;
}

void Chip8_SetGetKeysCallback(Chip8 *chip8, GetKeysCb cb)
{
    chip8->get_keys = cb;
}

int Chip8_Tick(Chip8 *chip8)
{
    Chip8_InstructionType instruction_type;
    uint16_t instruction;

    if (!Chip8_GetNextInstruction(chip8, &instruction_type, &instruction))
    {
        return 0;
    }

    chip8->pc += Chip8_ExecuteInstruction(chip8, instruction_type, instruction);

    // Update timers

    chip8->time_acc += CPU_TICK_SECS;

    if (chip8->time_acc >= TIMER_TICK_SECS)
    {
        if (chip8->dt > 0)
        {
            chip8->dt--;
        }

        if (chip8->st > 0)
        {
            chip8->st--;
        }

        chip8->time_acc = 0;
    }

    return chip8->pc;
}

static void StoreDigitSpritesInMemory(Chip8 *chip8)
{
    static uint8_t sprites[16][SPRITE_SIZE] = {
        {0xF0, 0x90, 0x90, 0x90, 0xF0},
        {0x20, 0x60, 0x20, 0x20, 0x70},
        {0xF0, 0x10, 0xF0, 0x80, 0xF0},
        {0xF0, 0x10, 0xF0, 0x10, 0xF0},
        {0x90, 0x90, 0xF0, 0x10, 0x10},
        {0xF0, 0x80, 0xF0, 0x10, 0xF0},
        {0xF0, 0x80, 0xF0, 0x90, 0xF0},
        {0xF0, 0x10, 0x20, 0x40, 0x40},
        {0xF0, 0x90, 0xF0, 0x90, 0xF0},
        {0xF0, 0x90, 0xF0, 0x10, 0xF0},
        {0xF0, 0x90, 0xF0, 0x90, 0x90},
        {0xE0, 0x90, 0xE0, 0x90, 0xE0},
        {0xF0, 0x80, 0x80, 0x80, 0xF0},
        {0xE0, 0x90, 0x90, 0x90, 0xE0},
        {0xF0, 0x80, 0xF0, 0x80, 0xF0},
        {0xF0, 0x80, 0xF0, 0x80, 0x80},
    };

    for (size_t i = 0; i < sizeof(sprites); i++)
    {
        chip8->mem[i] = sprites[i / SPRITE_SIZE][i % SPRITE_SIZE];
    }
}

static void PutAddrOnStack(Chip8 *chip8, uint16_t addr)
{
    if (chip8->sp >= STACK_SIZE)
    {
        // stack overflow
        abort();
    }

    chip8->stack[chip8->sp] = addr;
    chip8->sp++;
}

static uint16_t GetAddrFromStack(Chip8 *chip8)
{
    if (chip8->sp == 0)
    {
        // nothing on the stack
        abort();
    }

    chip8->sp--;

    return chip8->stack[chip8->sp];
}

static void GetInstructionRegisters(uint16_t instruction, uint8_t *reg_x, uint8_t *reg_y)
{
    *reg_x = HIGH_BYTE(instruction) & 0x0F;

    if (reg_y) *reg_y = (LOW_BYTE(instruction) & 0xF0) >> 4;
}

static uint16_t CallAddrHandler(Chip8 *chip8, uint16_t instruction)
{
    PutAddrOnStack(chip8, chip8->pc);
    chip8->pc = ADDR(instruction);

    return 0;
}

static uint16_t JpAddrHandler(Chip8 *chip8, uint16_t instruction)
{
    chip8->pc = ADDR(instruction);

    return 0;
}

static uint16_t RetHandler(Chip8 *chip8, uint16_t instruction)
{
    (void)instruction;
    chip8->pc = GetAddrFromStack(chip8); 

    return 2;
}

static uint16_t SeVxByteHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    return chip8->v[reg_x] == LOW_BYTE(instruction) ? 4 : 2;
}

static uint16_t SneVxByteHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    return chip8->v[reg_x] != LOW_BYTE(instruction) ? 4 : 2;
}

static uint16_t SeVxVyHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x, reg_y;

    GetInstructionRegisters(instruction, &reg_x, &reg_y);

    // skip next instruction if the value in register X == the value in register Y
    return chip8->v[reg_x] == chip8->v[reg_y] ? 4 : 2;
}

static uint16_t LdVxByteHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    chip8->v[reg_x] = LOW_BYTE(instruction);

    return 2;
}

static uint16_t AddVxByteHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    chip8->v[reg_x] += LOW_BYTE(instruction);

    return 2;
}

static uint16_t LdVxVyHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x, reg_y;

    GetInstructionRegisters(instruction, &reg_x, &reg_y);

    chip8->v[reg_x] = chip8->v[reg_y];

    return 2;
}

static uint16_t OrHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x, reg_y;

    GetInstructionRegisters(instruction, &reg_x, &reg_y);

    chip8->v[reg_x] |= chip8->v[reg_y];

    return 2;
}

static uint16_t AndHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x, reg_y;

    GetInstructionRegisters(instruction, &reg_x, &reg_y);

    chip8->v[reg_x] &= chip8->v[reg_y];

    return 2;
}

static uint16_t XorHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x, reg_y;

    GetInstructionRegisters(instruction, &reg_x, &reg_y);

    chip8->v[reg_x] ^= chip8->v[reg_y];

    return 2;
}

static uint16_t AddVxVyHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x, reg_y;

    GetInstructionRegisters(instruction, &reg_x, &reg_y);

    uint16_t res = chip8->v[reg_x] + chip8->v[reg_y];

    chip8->v[0xF] = res > 0xFF ? 1 : 0;
    chip8->v[reg_x] = res & 0xFF;

    return 2;
}

static uint16_t SubHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x, reg_y;

    GetInstructionRegisters(instruction, &reg_x, &reg_y);

    chip8->v[0xF] = chip8->v[reg_x] > chip8->v[reg_y] ? 1 : 0;
    chip8->v[reg_x] -= chip8->v[reg_y];

    return 2;
}

static uint16_t SubnHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x, reg_y;

    GetInstructionRegisters(instruction, &reg_x, &reg_y);

    chip8->v[0xF] = chip8->v[reg_y] > chip8->v[reg_x] ? 1 : 0;
    chip8->v[reg_x] = chip8->v[reg_y] - chip8->v[reg_x];

    return 2;
}

static uint16_t ShrHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    chip8->v[0xF] = chip8->v[reg_x] & 0x1;
    chip8->v[reg_x] /= 2;

    return 2;
}

static uint16_t ShlHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    chip8->v[0xF] = (chip8->v[reg_x] & (0x1 << 7)) > 0;
    chip8->v[reg_x] *= 2;

    return 2;
}

static uint16_t SneVxVyHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x, reg_y;

    GetInstructionRegisters(instruction, &reg_x, &reg_y);

    return chip8->v[reg_x] != chip8->v[reg_y] ? 4 : 2;
}

static uint16_t LdIAddrHandler(Chip8 *chip8, uint16_t instruction)
{
    chip8->i = ADDR(instruction);

    return 2;
}

static uint16_t JpV0AddrHandler(Chip8 *chip8, uint16_t instruction)
{
    chip8->pc = ADDR(instruction) + chip8->v[0x0];

    return 0;
}

static uint16_t RndHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;
    uint8_t random = rand() % 256;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    chip8->v[reg_x] = random & LOW_BYTE(instruction);

    return 2;
}

static uint16_t DrwHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x, reg_y;

    GetInstructionRegisters(instruction, &reg_x, &reg_y); 

    unsigned int start_x = chip8->v[reg_x];
    unsigned int start_y = chip8->v[reg_y];
    unsigned int sprite_height = NIBBLE(instruction); // sprite height
    unsigned int collision = 0;

    // printf("Draw sprite at (%d,%d)\n", start_x, start_y);

    for (unsigned int i = 0; i < sprite_height; i++)
    {
        uint8_t sprite_byte = chip8->mem[chip8->i + i];

        for (unsigned int j = 0; j < 8; j++)
        {
            unsigned int x = (start_x + j) % DISPLAY_WIDTH;
            unsigned int y = (start_y + i) % DISPLAY_HEIGHT;
            unsigned int draw_pos = (y * DISPLAY_WIDTH) + x;
            unsigned int sprite_offset = 7 - j;
            uint8_t sprite_pixel = (sprite_byte & (0x1 << sprite_offset)) >> sprite_offset; // 0 or 1

            // printf("Draw pixel %d, %d (%d) = %d\n", x, y, draw_pos, sprite_pixel);
            DrawPixel(chip8, draw_pos, sprite_pixel, &collision);
        }
    }

    chip8->v[0xF] = collision;

    return 2;
}

static uint16_t ClsHandler(Chip8 *chip8, uint16_t instruction)
{
    (void)instruction;
    memset(chip8->display, 0, DISPLAY_SIZE);
    return 2;
}

static uint16_t SkpHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    return (chip8->get_keys() & KEY_MASK(chip8->v[reg_x])) > 0 ? 4 : 2;
}

static uint16_t SknpHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    return (chip8->get_keys() & KEY_MASK(chip8->v[reg_x])) > 0 ? 2 : 4;
}

static uint16_t LdVxDtHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    chip8->v[reg_x] = chip8->dt;

    return 2;
}

static uint16_t LdVxKHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;
    uint16_t keys = chip8->get_keys();

    GetInstructionRegisters(instruction, &reg_x, NULL);

    // don't advance the PC until a key is pressed
    if (keys > 0)
    {
        for (int k = 0; k <= 0xF; k++)
        {
            if (keys & KEY_MASK(k))
            {
                chip8->v[reg_x] = k;
                break;
            }
        }

        return 2;
    }

    return  0;
}

static uint16_t LdDtVxHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    chip8->dt = chip8->v[reg_x];

    return 2;
}

static uint16_t LdStVxHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    chip8->st = chip8->v[reg_x];

    return 2;
}

static uint16_t AddIVxHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    chip8->i += chip8->v[reg_x];

    return 2;
}

static uint16_t LdFVxHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    uint8_t digit = chip8->v[reg_x] & 0x0F;

    chip8->i = digit * SPRITE_SIZE;

    return 2;
}

static uint16_t LdBVxHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);

    uint8_t val = chip8->v[reg_x];
    uint8_t hundreds_digit = val / 100;

    chip8->mem[chip8->i] = hundreds_digit;
    val -= hundreds_digit * 100;

    uint8_t tens_digit = val / 10;

    chip8->mem[chip8->i + 1] = tens_digit;
    val -= tens_digit * 10;

    chip8->mem[chip8->i + 2] = val;

    return 2;
}

static uint16_t LdIVxHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);
    memcpy(chip8->mem + chip8->i, chip8->v, reg_x + 1);

    return 2;
}

static uint16_t LdVxIHandler(Chip8 *chip8, uint16_t instruction)
{
    uint8_t reg_x;

    GetInstructionRegisters(instruction, &reg_x, NULL);
    memcpy(chip8->v, chip8->mem + chip8->i, reg_x + 1);

    return 2;
}

static void DrawPixel(Chip8 *chip8, unsigned int draw_pos, uint8_t sprite_pixel, unsigned int *collision)
{
    unsigned int display_index = draw_pos / 8;
    unsigned int offset = 7 - (draw_pos % 8);
    uint8_t display_byte = chip8->display[display_index];
    uint8_t display_pixel = (display_byte & (0x1 << offset)) >> offset; // 0 or 1
    uint8_t draw_pixel = display_pixel ^ sprite_pixel; // 0 or 1
    uint8_t draw_mask = 0x1 << offset;

    if (*collision == 0 && display_pixel == 1 && draw_pixel == 0) *collision = 1;

    assert(display_index >= 0 && display_index < DISPLAY_SIZE);

    if (draw_pixel)
    {
        chip8->display[display_index] |= draw_mask;
    }
    else
    {
        chip8->display[display_index] &= ~draw_mask;
    }
}
