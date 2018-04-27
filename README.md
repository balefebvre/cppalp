# CppAlp

Author: Baptiste Lefebvre
Copyright: Copyright 2018, Baptiste Lefebvre
License: MIT
Date: 2018-04-27


## Description

CppAlp is a solution created with Microsoft Visual Studio Community 2017
to hold a clean version of the C++ code developed at the Vision Institute
to control digital micromirror devices (DMDs).


## Structure

- `checkerboard/`
  Project which holds the source code of the checkerboard stimulation.
- `ultimate_code_binary_no_dark_phase/`
  Project which holds the source code of the binary stimulation (without dark phase).
- `inc/`
  Header files to include in each project.
- `lib/`
  Library files to include in each project.
- `x64/`
  - `Debug/`
    Directory which holds executables generated in the debug configuration (development product).
  - `Release/`
    Directory which holds executables generated in the release configuration (final product).
- `data/`
  Directory which contains additional data necessary to be able to run some executables.


## Notes

- If you want to create a new control then you just need to create a new project:
  1. Open the `cppalp` solution with Microsoft Visual Studio Community 2017.
  2. Right click on the solution in the `Solution Explorer` window.
  3. Select `Add` and `New Project...`
  4. Follow the instructions (to name the project use lowercases and underscores).
  5. Start the development of the new control.
- To add the header file provided by ViALUX:
  1. Right click on the solution in the `Solution Explorer` window.
  2. Select `Properties`.
  3. Go to `C/C++`, `General`.
  4. Edit `Additional Include Directories`.
  5. Add path to the `inc/` directory.
  6. Apply the changes.
- To add the library file provided by ViALUX:
  1. Right click on the solution in the `Solution Explorer` window.
  2. Select `Properties`.
  3. Go to `Linker`, `General`.
  4. Edit `Additional Library Directories`.
  5. Add the path to the `lib/` directory.
  6. Go to `Linker`, `Input`.
  7. Edit `Additional Dependencies`
  8. Add `alpD41.lib`.
  9. Appply changes.
- To (re)generate the solution:
  1. Set the solution platform as `x64` (i.e. 64 bits).
  2. Set the solution configuration as either `Debug` or `Release`.
  3. Click on `(Re)Build the solution`.
- To be able to launch the executables, the corresponding DLL must be accessible.
  1. Locate the DLL (i.e. `lib/alpD41.dll`).
  2. Locate the executable (i.e. `x64/Debug/<...>.exe` or `x64/Release/<...>.exe`).
  3. Copy & paste the DLL from the first location to the last one.
- To be able to launch the executables, additional data may have to be accessible. For example,
  the checkerboard needs to access a file named `binarysource1000Mbits` which contains a
  pregenerated sequence of random binary numbers.
  1. Put additional data in `data/` (if necessary).
- To ease the use of the generated executables, you can create shortcuts and place them on
  the desktop.
