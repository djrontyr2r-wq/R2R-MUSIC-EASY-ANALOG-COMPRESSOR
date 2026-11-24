# R2R MUSIC — Easy Analog Compressor

A fresh JUCE 8 / C++ VST3 compressor project by DJ R2R Music.

## Features
- 64-bit Windows VST3 (CMake + Visual Studio 2022)
- Threshold, Ratio, Attack, Release, Knee, Makeup, Mix and Output controls
- Stereo-linked peak detector, soft-knee compression, smoothed envelope
- Subtle analog-style saturation, gain-reduction meter and preset state
- Black interface with orange accents

## Build (Windows)
1. Install Visual Studio 2022 with **Desktop development with C++**.
2. Install CMake 3.22+.
3. Open this repository folder in a terminal.
4. Run:
   ```powershell
   cmake -S . -B build -A x64
   cmake --build build --config Release
   ```
5. The VST3 is generated under `build/R2R MUSIC_artefacts/Release/VST3/`.
6. Copy `R2R MUSIC.vst3` to `C:\Program Files\Common Files\VST3\`, then rescan plugins in FL Studio.

JUCE 8.0.6 is fetched by CMake during configuration; an internet connection is required for the first build.

## Important
This is a brand-new compressor project. It does not depend on or modify the separate DJ-R2R-LIMITER repository.
