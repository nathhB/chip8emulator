#ifndef ROM_PICKER_H
#define ROM_PICKER_H

#define MAX_ROMS 16
#define ROM_PATH_MAX_LEN 255
#define ROM_NAME_MAX_LEN 25

typedef struct RomPicker
{
    char roms[MAX_ROMS][ROM_NAME_MAX_LEN];
    char roms_path[ROM_PATH_MAX_LEN];
    unsigned int rom_count;
    unsigned int cursor;
} RomPicker;

void RomPicker_Init(RomPicker *picker, const char *roms_path);
void RomPicker_Up(RomPicker *picker);
void RomPicker_Down(RomPicker *picker);
void RomPicker_GetSelectedPath(RomPicker *picker, char rom_path[ROM_PATH_MAX_LEN]);
const char *RomPicker_GetSelectedRomName(RomPicker *picker);

#endif // ROM_PICKER_H
