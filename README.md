# README

## Project Introduction

Handsclapping is a famous game in Chengdu No.7 Yucai Middle School. Some students who graduate from it, and other people who are interested in this game found this repository in order to develop an application for Handsclapping.

This is still a naive project and there're many things to do, in which the most essential ones are listed below:

- Implement the basic element of offline Handsclapping, including energy, health, dodging, attack and other possible functions. It is also important to write a referee program for the game.\
This part is dealt in [`src/game`](src/game) and [`src/app`](src/app), where [`src/app`](src/app) contains all the main programs while all the include files for the core part of the game are stored in [`src/game`](src/game).\
***TODO: Till Jun 29th, 2025, some basic actions are already supported in the game. But we still need more actions and game modes such as teaming.***

- Make the online version of this game, this may require the familiarity with the structure of the Internet and some rendering technique.\
Online mode is constructed through python script in [`src/online`](src/online) with module from C++ program, which is capsulized by [`handsclapping_pylib.cpp`](src/handsclapping_pylib.cpp).\
***TODO: The current version can only support the online game within the same LAN. Also, the client program for mobile devices is in development.***

- Build a practical AI model for this game which can play with human players and acts like a human player, or can even do better, beat the human in Handsclapping.\
The first non-supervised model for Handsclapping, *Idiot*, has been constructed in `dev-AI_Idiot` branch of the project and is still on training (training data is not published yet). It reaches part of our expectation and will be released fairly soon (maybe after the Chinese Spring Festival holiday :) ).\
***TODO: The next AI model named by HDP (which stands for hybrid dynamic programming model) has already been designed mathematically. It may achieve better performance in playing with human.***

> **IMPORTANT**\
> To get the rule of this game or learn how to play it in the electronic version, please refer to [README.md](src/README.md) in `src` directory. This file is only responsible for introduce this project technically and show how to construct it or how to install and use the released version.

## Construct and Compile

> **ATTENTION**\
> This part introduces how to construct and compile this project in local Windows and Linux system and may require the previous knowledge about basic programming language and the corresponding toolchains. For users, it's OK to simply skip this part and download the released version of this project, following the instruction in the next part.

The following packages and libraries are required by this project:
- The C++ compiler: run `apt install g++` in Linux or download [Microsoft Visual C++](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-microsoft-visual-c-redistributable-version) ([Visual Studio](https://visualstudio.microsoft.com/) is also recommended, although much larger.) in Windows.

- CMake: run `apt install cmake` in Linux or download [CMake](https://cmake.org/download/) in Windows.

- Python3: run `apt install python3` in Linux or download [Python3](https://www.python.org/download/releases/3.0/) in Windows.\
**(When installing python3 in Windows by setup file, remember to click the option "Add Python to environment variables".)**

- pip: in most cases, pip is installed automatically when installing python. You may check it by running `pip --version` in command line. If it's not installed successfully, you may run `apt install pip` in Linux or reinstall python3 in Windows with pip option in the setup program selected.

- pybind11: run `pip install pybind11` in command line in both Linux and Windows system.

- git: run `apt install git` in Linux or download [git](https://git-scm.com/downloads/) in Windows.

- PyInstaller (if you want to generate released version locally, not necessary): run `pip install pyinstaller` in command line in both Linux and Windows system.

The construction steps are as following:

1. Clone the repository into local file system: running `git clone "https://github.com/YixuanFu06/HandsClapping.git"` or `git clone https://gitee.com/YixuanFu06/HandsClapping.git` in Linux, or use GitHub Desktop (often download together with git) in Windows.

2. Two path in [`CMakeLists.txt`](./CMakeLists.txt) needs to be changed into your file path (see it in comments). You may get your Python3's path by `which python3` in Linux or `where python3` in Windows. The path for pybind11 can be find through `pip show pybind11` in both Linux and Windows.

3. In command line, change into the project directory and type the following commands:
```
$ cd HandsClapping
--------------------
$ mkdir build
$ cd build
$ cmake ..
```
Then, run
```
$ make
```
in Linux, or
```
$ cd ..
$ cmake --build build --config Release
```
in Windows. If you have installed PyInstaller and wants to generate released version of this project, you may switch to branch `release-Linux` or `release-Windows` and run
```
$ cd build
--------------------
$ cpack
```
in Linux or Windows respectively,

4. After compiling, you may find executable files in directory `build/bin` and can run programs in `build/bin/game` to play Handsclapping game offline. To enable the online mode, you should first launch a server program ([server.py](src/online/server.py)) and connect client programs ([client.py](src/online/client.py)) to it.

For more information of running the program, please see [README.md](src/README.md) in `src` directory.

## Install with Released Versions

To install with released versions, just find the released version which fits your operating system and computer structure provided by us in the repository. The project now only supports released versions for Windows system and Linux system on x86_64 structure. However, maybe released version for Linux can also work on arm64 but we're not really sure.

After installing, you can run the program according to the introduction in [README.md](src/README.md) in `src` directory.

Sadly, we can't support Macos currently for technical reasons. We'll be glad if you are willing to offer assistance to construct released versions for Macos.

> **TOUCH US**\
> Rasing issues in the repository is welcomed. Also, feel free to touch Yixuan Fu at the email provided in his profile.