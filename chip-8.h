#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define RAM_SIZE 4096
#define PROGRAM_START_ADDR 0x200
#define INSTRUCTION_COUNT 35
#define REGISTER_COUNT 16
#define STACK_SIZE 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SIZE ((DISPLAY_WIDTH * DISPLAY_HEIGHT) / 8) // display in bytes (1 pixel = 1 bit)
#define SPRITE_SIZE 5 // in bytes
#define CPU_FREQUENCY 500.0
#define CPU_TICK_SECS (1 / CPU_FREQUENCY) 
#define TIMER_TICK_SECS (1 / 60.0) // timer ticks at 60Hz

typedef struct Chip8 Chip8;

typedef uint16_t (*Chip8_InstructionHandler)(Chip8 *, uint16_t);
typedef uint16_t (*GetKeysCb)(void);

struct Chip8
{
    uint8_t v[REGISTER_COUNT];                                  // 16 8 bits general purpose registers
    uint8_t dt;                                                 // special purpose 8 bits register used for delay timer
    uint8_t st;                                                 // special purpose 8 bits register used for sound timer
    uint16_t i;                                                 // 16 bit register generally used to store memory addresses (only 12 lowest bits are used)
    uint16_t pc;                                                // program counter
    uint8_t sp;                                                 // stack pointer
    uint16_t stack[STACK_SIZE];                                 // stack
    uint8_t mem[RAM_SIZE];                                      // RAM
    uint8_t display[DISPLAY_SIZE];                              // pixels to display
    unsigned int program_len;                                   // size of the program
    double time_acc;                                            // time accumulator for timers
    Chip8_InstructionHandler instruction_handlers[INSTRUCTION_COUNT];
    GetKeysCb get_keys;                                         // is key pressed callback
};

typedef enum Chip8_InstructionType
{
    UNKNOWN_INSTRUCTION,

    // drawing

    CLS,
    DRW,

    // subroutines

    RET,
    JP_ADDR,
    JP_V0_ADDR,
    CALL_ADDR,

    // load

    LD_VX_BYTE,
    LD_VX_VY,
    LD_I_ADDR,
    LD_VX_DT,
    LD_VX_K,
    LD_DT_VX,
    LD_ST_VX,
    LD_F_VX,
    LD_B_VX,
    LD_I_VX,
    LD_VX_I,

    // arithmetic

    ADD_VX_BYTE,
    ADD_VX_VY,
    ADD_I_VX,
    SUB,
    SHR,
    SUBN,
    SHL,
    RND,

    // logic

    OR,
    AND,
    XOR,

    // branching

    SE_VX_BYTE,
    SNE_VX_BYTE,
    SE_VX_VY, 
    SNE_VX_VY,
    SKP,
    SKNP
} Chip8_InstructionType;

void Chip8_Init(Chip8 *chip8);
void Chip8_Reset(Chip8 *chip8);
int Chip8_Load(Chip8 *chip8, uint8_t *data, unsigned int len);
int Chip8_LoadFromFile(Chip8 *chip8, const char *path);
int Chip8_GetNextInstruction(Chip8 *chip8, Chip8_InstructionType *instruction_type, uint16_t *instruction);
uint16_t Chip8_ExecuteInstruction(Chip8 *chip8, Chip8_InstructionType opcode, uint16_t instruction);
unsigned int Chip8_GetPixel(Chip8 *chip8, unsigned int pos);
void Chip8_SetGetKeysCallback(Chip8 *chip8, GetKeysCb cb);
int Chip8_Tick(Chip8 *chip8);

#endif // CHIP8_H
