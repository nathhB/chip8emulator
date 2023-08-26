#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "chip-8.h"

#define GAME_WIDTH 640
#define GAME_HEIGHT 320
#define HUD_HEIGHT 30
#define SCREEN_WIDTH GAME_WIDTH
#define SCREEN_HEIGHT (GAME_HEIGHT + HUD_HEIGHT)

static void DrawGameScreen(RenderTexture2D display_render_texture);
static void DrawHUD(void);
static void UpdateScreen(Chip8 *chip8, RenderTexture2D display_render_texture, void *pixels);
static void UpdateKeys(void);
static uint16_t GetKeys(void);

// key mappings from 0 to 0xF (16 keys)
static int key_mappings[16] = {
    KEY_X,      // 0
    KEY_ONE,    // 1
    KEY_TWO,    // 2
    KEY_THREE,  // 3
    KEY_Q,      // 4
    KEY_W,      // 5
    KEY_E,      // 6
    KEY_A,      // 7
    KEY_S,      // 8
    KEY_D,      // 9
    KEY_Z,      // A
    KEY_C,      // B
    KEY_FOUR,   // C
    KEY_R,      // D
    KEY_F,      // E
    KEY_V       // F
};

static uint16_t keys = 0;

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
    Chip8_SetGetKeysCallback(&chip8, GetKeys);

    if (Chip8_LoadFromFile(&chip8, rom_path) < 0)
    {
        printf("Failed to load ROM (path: %s)\n", rom_path);
        return 1;
    }

    printf("ROM loaded (program length: %d)\n", chip8.program_len);

    void *pixels = malloc(sizeof(Color) * DISPLAY_WIDTH * DISPLAY_HEIGHT);
    memset(pixels, 0, sizeof(Color) * DISPLAY_WIDTH * DISPLAY_HEIGHT);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chip-8 Emulator");

    RenderTexture2D display_render_texture = LoadRenderTexture(DISPLAY_WIDTH, DISPLAY_HEIGHT);

    double last_time = 0;
    double time_acc = 0;
    int exit = 0;

    while (!WindowShouldClose())
    {
        double dt_secs = GetTime() - last_time;
        last_time = GetTime();
        time_acc += dt_secs;

        while (time_acc >= CPU_TICK_SECS)
        {
            if (!Chip8_Tick(&chip8))
            {
                exit = 1;
                break;
            }

            time_acc -= CPU_TICK_SECS;
        }

        if (exit) break;

        if (chip8.st)
        {
            // TODO: play sound
        }

        UpdateScreen(&chip8, display_render_texture, pixels); 
        UpdateKeys();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGameScreen(display_render_texture); 
        DrawHUD();
        EndDrawing();
    }

    CloseWindow();
    UnloadRenderTexture(display_render_texture);
    free(pixels);

    return 0;
}

static void DrawGameScreen(RenderTexture2D display_render_texture)
{
    DrawTexturePro(
            display_render_texture.texture,
            (Rectangle){0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT},
            (Rectangle){0, 0, GAME_WIDTH, GAME_HEIGHT},
            (Vector2){0, 0},
            0,
            WHITE);
}

static void DrawHUD(void)
{
    DrawText(TextFormat("Frequency: %.1f", CPU_FREQUENCY), 10, GAME_HEIGHT + 8, 15, BLACK);
}

static void UpdateScreen(Chip8 *chip8, RenderTexture2D display_render_texture, void *pixels)
{
    for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++)
    {
        Color color = Chip8_GetPixel(chip8, i) ? BLACK : WHITE;

        memcpy(pixels + (i * sizeof(Color)), &color, sizeof(Color));
    }

    UpdateTexture(display_render_texture.texture, pixels);
}

static void UpdateKeys(void)
{
    keys = 0;

    for (int i = 0; i <= 0xF; i++)
    {
        if (IsKeyDown(key_mappings[i]))
        {
            keys |= (1 << (0xF - i));
        }
    }
}

static uint16_t GetKeys(void)
{
    return keys;
}
