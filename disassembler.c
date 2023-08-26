#include <stdio.h>
#include <stdlib.h>

#include "chip-8.h"

static void Disassemble(Chip8 *chip8);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: disassembler ROM_PATH\n");
        return 1;
    }

    const char *rom_path = argv[1];
    Chip8 chip8;

    Chip8_Init(&chip8);
    
    if (Chip8_LoadFromFile(&chip8, rom_path) < 0)
    {
        printf("Failed to load ROM (path: %s)\n", rom_path);
        return 1;
    }

    printf("ROM loaded (program length: %d)\n", chip8.program_len);

    Disassemble(&chip8);

    return 0;
}

static void Disassemble(Chip8 *chip8)
{
    Chip8_InstructionType instruction_type;
    uint16_t instruction;

    while (Chip8_GetNextInstruction(chip8, &instruction_type, &instruction))
    {
        uint8_t x_reg = (instruction & 0x0F00) >> 8;
        uint8_t y_reg = (instruction & 0x00F0) >> 4;
        uint8_t low_byte = instruction & 0xFF;
        uint8_t nibble = low_byte & 0xF;

        printf("0x%x\t", chip8->pc);

        switch (instruction_type)
        {
            case UNKNOWN_INSTRUCTION:
                printf("NOP\n");
                break;

            case CLS:
                printf("CLS\n");
                break;

            case RET:
                printf("RET\n");
                break;

            case JP_ADDR:
                printf("JP 0x%X\n", instruction);
                break;

            case CALL_ADDR:
                printf("CALL 0x%X\n", instruction);
                break;

            case SE_VX_BYTE:
                printf("SE V%X, 0x%X\n", x_reg, low_byte);
                break;

            case SNE_VX_BYTE:
                printf("SNE V%X, 0x%X\n", x_reg, low_byte);
                break;

            case SE_VX_VY:
                printf("SE V%X, V%X\n", x_reg, y_reg);
                break;

            case LD_VX_BYTE:
                printf("LD V%X, 0x%X\n", x_reg, low_byte);
                break;

            case ADD_VX_BYTE:
                printf("ADD V%X, 0x%X\n", x_reg, low_byte);
                break;

            case LD_VX_VY:
                printf("LD V%X, V%X\n", x_reg, y_reg);
                break;

            case OR:
                printf("OR V%X, V%X\n", x_reg, y_reg);
                break;

            case AND:
                printf("AND V%X, V%X\n", x_reg, y_reg);
                break;

            case XOR:
                printf("XOR V%X, V%X\n", x_reg, y_reg);
                break;

            case ADD_VX_VY:
                printf("ADD V%X, V%X\n", x_reg, y_reg);
                break;

            case SUB:
                printf("SUB V%X, V%X\n", x_reg, y_reg);
                break;

            case SHR:
                printf("SHR V%X {, V%X}\n", x_reg, y_reg);
                break;

            case SUBN:
                printf("SUBN V%X, V%X\n", x_reg, y_reg);
                break;

            case SHL:
                printf("SHL V%X {, V%X}\n", x_reg, y_reg);
                break;

            case SNE_VX_VY:
                printf("SNE V%X, V%X\n", x_reg, y_reg);
                break;

            case LD_I_ADDR:
                printf("LD I, 0x%X\n", instruction);
                break;

            case JP_V0_ADDR:
                printf("JP V0, 0x%X\n", instruction);
                break;

            case RND:
                printf("RND V%X, 0x%X\n", x_reg, low_byte);
                break;

            case DRW:
                printf("DRW V%X, V%X, 0x%X\n", x_reg, y_reg, nibble);
                break;

            case SKP:
                printf("SKP V%X\n", x_reg);
                break;

            case SKNP:
                printf("SKNP V%X\n", x_reg);
                break;

            case LD_VX_DT:
                printf("LD V%X, DT\n", x_reg);
                break;

            case LD_VX_K:
                printf("LD V%X, K\n", x_reg);
                break;

            case LD_DT_VX:
                printf("LD DT, V%X\n", x_reg);
                break;

            case LD_ST_VX:
                printf("LD ST, V%X\n", x_reg);
                break;

            case ADD_I_VX:
                printf("ADD I, V%X\n", x_reg);
                break;

            case LD_F_VX:
                printf("LD F, V%X\n", x_reg);
                break;

            case LD_B_VX:
                printf("LD B, V%X\n", x_reg);
                break;

            case LD_I_VX:
                printf("LD [I], V%X\n", x_reg);
                break;

            case LD_VX_I:
                printf("LD V%X, [I]\n", x_reg);
                break;

            default:
                abort();
        }

        chip8->pc += 2;
    }
}
