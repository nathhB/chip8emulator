#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rom_picker.h"

void RomPicker_Init(RomPicker *picker, const char *roms_path)
{
    picker->rom_count = 0;
    picker->cursor = 0;

    strncpy(picker->roms_path, roms_path, ROM_PATH_MAX_LEN - 1);

    DIR *dir = opendir(roms_path);
    struct dirent *ent;

    if (!dir)
    {
        fprintf(stderr, "Failed to read ROMs directory\n");
        abort();
    }

    while ((ent = readdir(dir)) != NULL && picker->rom_count < MAX_ROMS)
    {
        if (ent->d_type == DT_REG)
        {
            strncpy(picker->roms[picker->rom_count], ent->d_name, ROM_NAME_MAX_LEN - 1);
            picker->rom_count++;
        }
    }

    closedir(dir);
}

void RomPicker_Up(RomPicker *picker)
{
    picker->cursor = (picker->cursor - 1 + MAX_ROMS) % MAX_ROMS;
}

void RomPicker_Down(RomPicker *picker)
{
    picker->cursor = (picker->cursor + 1) % MAX_ROMS;
}

void RomPicker_GetSelectedPath(RomPicker *picker, char rom_path[ROM_PATH_MAX_LEN])
{
    const char *selected_rom = picker->roms[picker->cursor];

    snprintf(rom_path, ROM_PATH_MAX_LEN, "%s/%s", picker->roms_path, selected_rom);
}

const char *RomPicker_GetSelectedRomName(RomPicker *picker)
{
    return picker->roms[picker->cursor];
}
