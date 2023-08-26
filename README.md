# Chip-8 emulator

A Chip-8 emulator written in C with raylib.

## Building and running

```
cmake -DRAYLIB_LIBRARY_PATH=<PATH TO RAYLIB LIBRARY FILE> -DRAYLIB_INCLUDE_PATH=<PATH TO RAYLIB HEADER>
make
ctest # run unit tests
./emulator ROM_PATH # run the specified ROM
```

## Test ROMS and resources

- [C8TECH10](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [chip8-test-suite](https://github.com/Timendus/chip8-test-suite)
- [chip8-test-rom](https://github.com/corax89/chip8-test-rom)
