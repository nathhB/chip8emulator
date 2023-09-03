#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "chip-8.h"
#include "rom_picker.h"

#define GAME_WIDTH 640
#define GAME_HEIGHT 320
#define HUD_TOP_HEIGHT 25
#define HUD_BOTTOM_HEIGHT 25
#define SCREEN_WIDTH GAME_WIDTH
#define SCREEN_HEIGHT (GAME_HEIGHT + HUD_TOP_HEIGHT + HUD_BOTTOM_HEIGHT)
#define ROM_PICKER_FONT_SIZE 20
#define HUD_FONT_SIZE 15

typedef enum EmulatorStateType
{
    STATE_ROM_SELECTION,
    STATE_GAME
} EmulatorStateType;

typedef struct EmulatorState
{
    EmulatorStateType type;
    int (*init)(void *);
    void (*deinit)(void);
    void (*update)(void);
} EmulatorState;

typedef struct GameStateData
{
    Chip8 chip8;
    void *pixels;
    RenderTexture2D display_render_texture;
    double last_time;
    double time_acc;
} GameStateData;

typedef struct RomSelectionData
{
    RomPicker picker;
} RomSelectionData;

typedef struct EmulatorSkin
{
    Color colors[4];
} EmulatorSkin;

static int ChangeState(EmulatorStateType new_state_type, void *data);
static void DrawGameScreen(RenderTexture2D display_render_texture);
static void DrawHUD(void);
static void UpdateScreen(Chip8 *chip8, RenderTexture2D display_render_texture, void *pixels);
static void UpdateKeys(void);
static uint16_t GetKeys(void);
static int InitGameState(void *data);
static void DeinitGameState(void);
static void UpdateGameState(void);
static int InitRomSelectionState(void *data);
static void DeinitRomSelectionState(void);
static void UpdateRomSelectionState(void);

static EmulatorState states[2] = {
    (EmulatorState){STATE_ROM_SELECTION, InitRomSelectionState, DeinitRomSelectionState, UpdateRomSelectionState},
    (EmulatorState){STATE_GAME, InitGameState, DeinitGameState, UpdateGameState},
};

static EmulatorSkin skin = {
    .colors = {
        (Color){64, 60, 52, 255},
        (Color){140, 122, 105, 255},
        (Color){217, 199, 184, 255},
        (Color){13, 0, 0, 255}
    }
};

static RomSelectionData rom_selection_data;
static GameStateData game_state_data;

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
static EmulatorState *current_state = NULL;
static bool rom_picker_enabled = false;

int main(int argc, char **argv)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chip-8 Emulator");

    if (argc == 1)
    {
        RomPicker_Init(&rom_selection_data.picker, ROMS_DIR_PATH);

        rom_picker_enabled = true;

        if (ChangeState(STATE_ROM_SELECTION, NULL) < 0)
        {
            goto error;
        }
    }
    else if (argc == 2)
    {
        if (ChangeState(STATE_GAME, argv[1]) < 0)
        {
            goto error;
        }
    }
    else
    {
        printf("Usage: emulator [ROM_PATH]\n");
        goto error;
    }

    while (!WindowShouldClose())
    {
        current_state->update();
    }

    current_state->deinit();
    CloseWindow();
    return 0;

error:
    fprintf(stderr, "Something went wrong!\n");
    CloseWindow();
    return 1; 
}

static int ChangeState(EmulatorStateType new_state_type, void *data)
{
    if (current_state) current_state->deinit();

    current_state = &states[new_state_type];
    return current_state->init(data);
}

static int InitGameState(void *data)
{
    char *rom_path = data;

    memset(&game_state_data, 0, sizeof(GameStateData));
    game_state_data.last_time = GetTime();

    Chip8_Init(&game_state_data.chip8);
    Chip8_SetGetKeysCallback(&game_state_data.chip8, GetKeys);

    if (Chip8_LoadFromFile(&game_state_data.chip8, rom_path) < 0)
    {
        fprintf(stderr, "ERROR: Failed to load ROM (path: %s)\n", rom_path);
        return -1;
    }

    printf("ROM loaded (program length: %d)\n", game_state_data.chip8.program_len);

    game_state_data.pixels = malloc(sizeof(Color) * DISPLAY_WIDTH * DISPLAY_HEIGHT);
    memset(game_state_data.pixels, 0, sizeof(Color) * DISPLAY_WIDTH * DISPLAY_HEIGHT);

    game_state_data.display_render_texture = LoadRenderTexture(DISPLAY_WIDTH, DISPLAY_HEIGHT);

    return 0;
}

static void DeinitGameState(void)
{
    UnloadRenderTexture(game_state_data.display_render_texture);
    free(game_state_data.pixels);
}

static void UpdateGameState(void)
{
    if (rom_picker_enabled && IsKeyPressed(KEY_BACKSPACE))
    {
        ChangeState(STATE_ROM_SELECTION, NULL);
        return;
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        // reset the ROM
        Chip8_Reset(&game_state_data.chip8);
    }

    double dt_secs = GetTime() - game_state_data.last_time;
    game_state_data.last_time = GetTime();
    game_state_data.time_acc += dt_secs;

    while (game_state_data.time_acc >= CPU_TICK_SECS)
    {
        if (!Chip8_Tick(&game_state_data.chip8))
        {
            break;
        }

        game_state_data.time_acc -= CPU_TICK_SECS;
    }

    if (game_state_data.chip8.st)
    {
        // TODO: play sound
    }

    UpdateScreen(&game_state_data.chip8, game_state_data.display_render_texture, game_state_data.pixels); 
    UpdateKeys();

    BeginDrawing();
    ClearBackground(skin.colors[2]);
    DrawGameScreen(game_state_data.display_render_texture); 
    DrawHUD();
    EndDrawing();
}

static int InitRomSelectionState(void *data)
{
    (void)data;

    return 0;
}

static void DeinitRomSelectionState(void) {}

static void UpdateRomSelectionState(void)
{
    if (IsKeyPressed(KEY_DOWN))
    {
        RomPicker_Down(&rom_selection_data.picker);
    }

    if (IsKeyPressed(KEY_UP))
    {
        RomPicker_Up(&rom_selection_data.picker);
    }

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
    {
        char rom_path[ROM_PATH_MAX_LEN];

        RomPicker_GetSelectedPath(&rom_selection_data.picker, rom_path);
        ChangeState(STATE_GAME, rom_path);
    }

    BeginDrawing();
    ClearBackground(skin.colors[2]);

    // draw cursor

    int y = HUD_TOP_HEIGHT + (rom_selection_data.picker.cursor * ROM_PICKER_FONT_SIZE);

    DrawRectangle(0, y, SCREEN_WIDTH, ROM_PICKER_FONT_SIZE, skin.colors[0]);

    // draw rom list

    for (unsigned int i = 0; i < rom_selection_data.picker.rom_count; i++)
    {
        char *rom = rom_selection_data.picker.roms[i];
        int rom_text_width = MeasureText(rom, ROM_PICKER_FONT_SIZE);
        Color color = i == rom_selection_data.picker.cursor ? skin.colors[2] : skin.colors[0];

        DrawText(rom, SCREEN_WIDTH / 2 - rom_text_width / 2, HUD_TOP_HEIGHT + ROM_PICKER_FONT_SIZE * i, ROM_PICKER_FONT_SIZE, color);
    }

    DrawHUD();
    EndDrawing();
}

static void DrawGameScreen(RenderTexture2D display_render_texture)
{
    DrawTexturePro(
            display_render_texture.texture,
            (Rectangle){0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT},
            (Rectangle){0, HUD_TOP_HEIGHT, GAME_WIDTH, GAME_HEIGHT},
            (Vector2){0, 0},
            0,
            WHITE);
}

static void DrawHUD(void)
{
    const char *text;

    if (current_state->type == STATE_ROM_SELECTION)
    {
        text = "Select ROM (Up/Down + ENTER)";
    }
    else if (current_state->type == STATE_GAME)
    {
        text = RomPicker_GetSelectedRomName(&rom_selection_data.picker);
    }

    int text_w = MeasureText(text, HUD_FONT_SIZE);

    DrawRectangle(0, 0, SCREEN_WIDTH, HUD_TOP_HEIGHT, skin.colors[1]);
    DrawRectangle(0, SCREEN_HEIGHT - HUD_BOTTOM_HEIGHT, SCREEN_WIDTH, HUD_BOTTOM_HEIGHT, skin.colors[1]);
    DrawText(text, SCREEN_WIDTH / 2 - text_w / 2, 5, HUD_FONT_SIZE, skin.colors[2]);
    DrawText(TextFormat("Frequency: %.1f", CPU_FREQUENCY), 10, SCREEN_HEIGHT - 18, HUD_FONT_SIZE, skin.colors[2]);
}

static void UpdateScreen(Chip8 *chip8, RenderTexture2D display_render_texture, void *pixels)
{
    for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++)
    {
        Color color = Chip8_GetPixel(chip8, i) ? skin.colors[3] : skin.colors[2];

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
