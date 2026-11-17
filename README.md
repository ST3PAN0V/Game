# Game

My first C++ game: a Linux project built with OpenGL and GLUT. It includes keyboard-controlled movement, a grid-based world, and levels loaded from text files.

**Status: unfinished.** This repository preserves the current implementation as a personal game-development project.

## Features

- Window rendering and keyboard input through GLUT.
- Game logic and object rendering in C++.
- Three text-based map files that can be edited independently of the source code.
- A Makefile that builds the `MyGame` executable.

## Build and run

Requirements:

- Linux with a graphical desktop session.
- `g++` and Make.
- OpenGL development libraries.
- FreeGLUT development headers and library, including `GL/freeglut.h`.

Clone the repository, build it, and run the executable from the repository root:

```bash
git clone https://github.com/ST3PAN0V/Game.git
cd Game
make
./MyGame
```

Keep `FirstMap.txt`, `SecondMap.txt`, and `ThirdMap.txt` in the working directory. The game opens them by relative filename.

The repository also contains a Linux binary named `MyGame`. To try that existing binary from the repository root:

```bash
chmod +x MyGame
./MyGame
```

Building from source lets you use your own system's compiler and OpenGL/GLUT libraries.

## Controls

| Key | Movement |
| --- | --- |
| `w` | Up |
| `a` | Left |
| `s` | Down |
| `d` | Right |

## Project layout

| File | Purpose |
| --- | --- |
| [`main.cpp`](main.cpp) | Window setup, rendering callbacks, frame timer, and keyboard input |
| [`game.cpp`](game.cpp) | Game logic, object rendering, and map loading |
| [`classes.h`](classes.h) | Shared constants, declarations, and the FreeGLUT include |
| [`FirstMap.txt`](FirstMap.txt), [`SecondMap.txt`](SecondMap.txt), [`ThirdMap.txt`](ThirdMap.txt) | Initial map layouts |
| [`Makefile`](Makefile) | Compilation and linking against GLUT and OpenGL |
| `MyGame` | Game executable |

## Contact

Questions or feedback: [Telegram](https://t.me/ArtemS101).

