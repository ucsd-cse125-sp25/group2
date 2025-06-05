# 🐷 Barnyard Breakout 🐮

The farmer is off to town for the day. It's the perfect time for a breakout! Work together as the pig, cow, chicken, and sheep to solve puzzles, parkour, and get off the dinner table.

This game was our project for [CSE 125](https://cse125.ucsd.edu/) at UCSD in Spring 2025. Check out our development process [here](https://luck-bird-6c2.notion.site/CSE-125-SP25-Group-2-1cb063108a72803098f4cc812307c0e7)!

Made By:

- Jonathon Duong
- Skyler Goh
- Kristhian Ortiz
- Luffy Saito
- Reese Whitlocker
- JC Yan
- Grace Yang

## Game Preview

## Controls

- **Mouse Movement**

  - Move the mouse up or down to look up and down.
  - Move the mouse left or right to look around and rotate the player's facing direction.
  - **Left-Click** to pickup/drop objects, only certain objects can be picked up

- **Keyboard Movement**

  - Use **W** to move forward.
  - Use **S** to move backward.
  - Use **A** to strafe left.
  - Use **D** to strafe right.
  - Press **Space** to jump.
  - Chicken can glide by holding **Space**
  - Sheep can double jump by pressing **Space** while jumping

- **Animal Abilities**

  - The Cow is strong and can push items 💪🐮
  - The Pig is smart and can read notes and numbers 📖🐖
  - The Chicken can slow down its fall by gliding 🪽🐤
  - The Sheep's wool is bouncy and can bouce other players or double jump itself 🐑

- **Reset Key**
  - If you get lost / stuck / fall through the floor 😅, hit **U** to reset player position

## Installation and Setup

Start by cloning the repository and following these steps. If you only wish to play the game, you just have to complete initial setup and can ignore the resut

### Requirements

Only works on Windows (sorry Mac and Linux users 😞)

- WSL & Ubuntu
- CMake Version 3.x.x
- Visual Studio

### 1. Initial Setup

#### WSL Ubuntu

Run these commands in the root directory:

```bash
mkdir wsl
cd wsl
make server
./server # Runs the server
```

If you run into errors, you may need to install these packages

```
sudo apt-get install libglm-dev
sudo apt-get install libglfw3-dev
sudo apt-get install libglfw3
sudo apt-get install libglfw3-dev
sudo apt-get install libxinerama-dev
sudo apt-get install libxcursor-dev
sudo apt-get install libxi-dev
```

#### Windows (Visual Studio)

Run these commands in the root directory:

```bash
mkdir build
cd build
cmake ..
cmake --build .
./Debug/client.exe    # Runs client (open a separate terminal for each client)
```

⚠️ Important: Do not run executables from inside the Debug/ folder. Always run them from the **build/** directory–shader paths are resolved relative to the working directory, which is where you run the executable.

### 2. If You Add New .cpp or .hpp Files or Modify CMakeLists.txt

#### WSL Ubuntu

Run these commands from the **wsl directory**:

```bash
make server
./server # Runs the server
```

#### Windows (Visual Studio)

Run these commands from the **build directory**:

```bash
cmake ..
cmake --build .
./Debug/client.exe    # Runs client (open a separate terminal for each client)
```

### 3. If You Just Modified Existing Files

#### WSL Ubuntu

Run these commands from the **wsl directory**:

```bash
make server
./server # Runs the server
```

#### Windows (Visual Studio)

Run these commands from the **build directory**:

```bash
cmake --build .
./Debug/client.exe    # Runs client (open a separate terminal for each client)
```

Link to Game Art Assets (models, ui, obj files, etc):  
https://drive.google.com/drive/folders/11j-Le37h2sPZk4vPApFcZuR77SX-XgGy?usp=sharing
