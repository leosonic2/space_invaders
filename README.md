# Space Invaders

A 2D desktop arcade game implemented in C17 with OpenGL 3.3, GLFW, and GLAD. The game separates its simulation from windowing and rendering, allowing the game logic and selected renderer helpers to be tested without opening a window.

## Features

- Keyboard-controlled horizontal movement and projectiles.
- Moving enemy formations, collision detection, scoring, and lives.
- Menu, pause, Game Over, victory, and restart states.
- A pixel-art player ship rendered from the reference design.
- Configurable campaign length, with five waves by default.
- A resizable 4:3 logical playfield with a minimum window size of 800 x 600.
- Headless CTest coverage for configuration, game logic, renderer helpers, and integration.

## Requirements

- CMake 3.21 or newer.
- Ninja and Git.
- A C17 compiler: GCC, Clang, or MSVC.
- Python 3 and pip; GLAD generation uses the pinned Jinja2 dependency in `requirements-build.txt`.
- OpenGL development libraries and the GLFW platform development dependencies.

CMake downloads GLFW 3.4 and GLAD 2.0.8 through FetchContent during configuration. The application requires an OpenGL-capable desktop session to run; the CTest suite does not require opening a game window.

### Linux (Ubuntu)

Install the compiler, build tools, OpenGL headers, and the GLFW X11/Wayland dependencies:

```sh
sudo apt-get update
sudo apt-get install -y build-essential cmake git ninja-build pkg-config \
  libgl1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev \
  libwayland-dev wayland-protocols libxkbcommon-dev python3 python3-venv
```

Install the Python build dependency in a virtual environment:

```sh
python3 -m venv .venv
source .venv/bin/activate
python -m pip install -r requirements-build.txt
```

`pkg-config` is used by GLFW's Linux configuration when checking platform dependencies. `libwayland-dev` provides the Wayland scanner needed by the GLFW Wayland backend.

### Windows (MSYS2 UCRT64)

Install the UCRT64 GCC, CMake, and Ninja packages in MSYS2, then use an MSYS2 UCRT64 terminal where those tools are on `PATH`:

```sh
pacman -S --needed mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```

Install the Python build dependency from PowerShell:

```powershell
py -3 -m pip install -r requirements-build.txt
```

Alternatively, build with MSVC from a Visual Studio Developer PowerShell that has CMake and Ninja available. A build directory configured for one generator cannot be reused with another generator; choose a fresh build directory when changing toolchains.

## Configure and build

Run these commands from the repository root. Keep the Python environment used to install Jinja2 active while configuring GLAD.

```sh
cmake -S . -B build -G Ninja
cmake --build build --parallel
```

CMake uses the C17 standard and disables compiler extensions. The application target is built with `/W4` on MSVC or `-Wall -Wextra -Wpedantic` on other compilers.

### Configure the campaign length

`SPACE_INVADERS_FINAL_WAVE` is a positive integer CMake cache setting and defaults to `5`:

```sh
cmake -S . -B build -G Ninja -DSPACE_INVADERS_FINAL_WAVE=8
```

## Run

The application loads shader files using paths relative to its working directory. Run it from the repository root, which contains `shaders/`, or from the build directory, where CMake copies the shaders:

```sh
./build/bin/space_invaders
```

On Windows, run `.\build\bin\space_invaders.exe` from PowerShell in the repository root.

## Controls

| Key | Action |
| --- | --- |
| ENTER | Start from the menu, resume while paused, or restart after Game Over |
| A or Left Arrow | Move left |
| D or Right Arrow | Move right |
| SPACE | Fire |
| ESC | Pause |

The game starts with three lives. Eliminating a common enemy awards 10 points. An invader that reaches the player's lower region or collides with the ship costs one life. Clearing the configured final wave displays the victory state.

## Tests

Build the project and run the headless test suite from the repository root:

```sh
ctest --test-dir build --output-on-failure
```

For details on test coverage and limitations, see [TESTING.md](TESTING.md).

## Optional frame-rate logging

Set `SPACE_INVADERS_LOG_FPS` to a non-empty value before launching the application to print average rendered frames per second over five-second sampling intervals to standard error:

```powershell
$env:SPACE_INVADERS_LOG_FPS = "1"
.\build\bin\space_invaders.exe
```

```sh
SPACE_INVADERS_LOG_FPS=1 ./build/bin/space_invaders
```

For module relationships and runtime flow, see [ARCHITECTURE.md](ARCHITECTURE.md). For the public C interfaces, see [API_REFERENCE.md](API_REFERENCE.md).
