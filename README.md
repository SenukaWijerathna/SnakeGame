<h1 align="center">🐍 Console Snake Game</h1>

<p align="center"> This is a feature-rich implementation of the classic Snake Game with a <b>Text-based User Interface (TUI)</b> written in <b>C++</b>.</p>

<p align="center">
   <img src="./screenshots/SnakeGame-1.gif"alt="Main menu" width="20%">
   <img src="./screenshots/SnakeGame-2.gif" alt="Gameplay" width="20%">
</p>

## 📑 Table of Contents
- [📖 About](#-about)
- [✨ Features](#-features)
- [🛠️ Tech Stack and Requirements](#️-tech-stack-and-requirements)
- [⚙️ Installation Steps](#️-installation-steps)
- [🎮 Controls](#-controls)
- [🏗️ Project Architecture](#️-project-architecture)
- [⚠️ Known Issues and Limitations](#️-known-issues-and-limitations)
---


## 📖 About
* This project is a modern C++ take on the classic game.
* Unlike other standard console applications that use simple output loops, this project utilizes the Windows API to manage colors and screen refreshing.
* This was designed to demonstrate:
    * Object-Oriented Programming principles,
    * File handling,
    * and the use of algorithm to handle and display grid-based movement.


## ✨ Features

* 📂 **Permanent Save System:** Save your progress and resume later. The game reads from and writes to `.txt` files that hold records with data separated by commas.
  
* 🕹️ **3 Difficulty Levels:** Easy, Medium and Hard.
  
* 💯 **HighScore Tracking:** Automatically tracks and sorts the top score for each difficulty level.
  
* 💻 **Responsive TUI:** *Text-based User Interface (TUI)* that uses dynamic coloring for the UI elements.
  




## 🛠️ Tech Stack and Requirements
|**Language Used**|[![Language](https://skillicons.dev/icons?i=cpp)](https://skillicons.dev)|C++ (**Recommended:** C++17 and up) |
|:---:|:---:|:---:|
|**Platform Used**|[![Platforms](https://skillicons.dev/icons?i=visualstudio)](https://skillicons.dev)|Visual Studio Community 2022/26 |
|**Compatible OS**|[![OS](https://skillicons.dev/icons?i=windows)](https://skillicons.dev)|Windows 10/11 |
>[!CAUTION]
> This game can only be run on **Windows** operating systems due to the usage of OS-specific libraries such as *'Windows.h'* and *'Conio.h'*.



## ⚙️ Installation Steps
1. **Clone the Repository**

    ```bash
    git clone https://github.com/SenukaWijerathna/SnakeGame.git
    ```
    
2. **Compile**
* **Visual Studio (Recommended):** Open the `.sln` file (if available) or create a new Console Project and import all `.h` and `.cpp` files.
* **MSVC (Using the Developer Command Prompt for VS)**
    ```bash
    cl /EHsc /std:c++17 src\*.cpp /I include /Fe:SnakeGame.exe && del *.obj
    ```
    * C++ version can be changed, however minimum C++11 is required due to the usage of ```std::thread``` and ```std::chrono``` etc.
    * *C++17 is recommended.*
      
* **G++ (MinGW):**
    ```bash
    g++ src/*.cpp -I include -o SnakeGame.exe -static
    ```
    
3. **Run:**
   
    * Launch the `SnakeGame.exe` file.



## 🎮 Controls
In the Game Menus:
|Action|Key(s)|
|:--|:--|
|**Navigate Up**|`Arrow Up (↑)` or `Arrow Left (←)`|
|**Navigate Down**|`Arrow Down (↓)` or `Arrow Right (→)`|
|**Select Option**|`Enter`|
|**Return to Main Menu/Cancel Operation**|`Esc`|

During the gameplay:
|Action|Key(s)|
|:--|:--|
|**Move Up**|`Arrow Up (↑)` or `W`|
|**Move Down**|`Arrow Down (↓)` or `S`|
|**Move Left**|`Arrow Left (←)` or `A`|
|**Move Right**|`Arrow Right (→)` or `D`|
|**Pause Game**|`Spacebar` or `Esc`|
|**Select (Menus)**|`Enter`|
|**Navigate (Menus)**|`Arrow Keys`|




## 🏗️ Project Architecture
The project is structured using strict OOP principles
* **Game Loop & Rendering:** The game uses `refreshScreen()` function to refresh the screen and check the console window size, and the gameplay grid is rendered by a 2D deque that contains Pixel objects, where each `Pixel` determines its color based on its nature (snake's head, body, fruit,or a blank area).
  
* **Snake Logic:** The snake is represented as a `std::deque<std::pair<int,int>>` which contains the coordinates of each snake segment.
   
* **File I/O:**
  
    * `savedGames.txt`: Stores the game records in the form of:
        `name,difficulty,score,direction,snake-coordinates,fruit-coordinates`
    * **highScores.txt**: Stores high scores in the form of:
        `difficulty,name,score`
    * Data is parsed using `stringstream` to convert raw text back into `GameRecord` objects.

  



## ⚠️ Known Issues and Limitations
1. **Window Resizing:** Having a width or height lower than the minimum window size (`VISUAL_WIDTH` and `VISUAL_HEIGHT`) causes the GUI to glitch. As a preventive measure, `windowSizeCheck` function was added to temporarily stop the gameplay.

    Therefore, if you see a message like this:
    <p align="center">
    <img src="./screenshots/SnakeGame-3.png" alt="Warning Message" width="50%"/>
   </p>

    Simply resize the console window according to the given width and height requirements and continue.
To prevent accidental snake collisions after resizing the window, the game is set to pause so the player has time to return to the control keys. 


3. **Input:** Rapid inputs might occasionally buffer due to the `_kbhit()` implementation speed relative to the frame refresh rate.

---
*Created by [Senuka Wijerathna](https://github.com/SenukaWijerathna)*
