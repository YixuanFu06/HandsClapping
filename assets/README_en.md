# Handsclapping Game Project

## Introduction

The Handsclapping game is a widely recognized activity at Chengdu No.7 Yucai Middle School. A group of alumni and other enthusiasts of the game have come together to develop a digital version of Handsclapping.

This project is currently in its early stages, with several critical components still under development. The key remaining tasks include:

- **Core Gameplay Implementation:** This involves the integration of essential game mechanics such as energy, health, dodging, and attacking. Additionally, a referee program for regulating gameplay is required.  
  This work is being carried out in the [`src/game`](/src/game) and [`src/app`](/src/app) directories, where [`src/app`](/src/app) houses the main program logic, and the game’s core functions are managed in [`src/game`](/src/game).  
  **TODO:** As of January 29, 2025, basic actions have been implemented, though additional actions and game modes, including teaming, are still in development.

- **Online Mode Development:** This requires a solid understanding of network architecture and rendering techniques.  
  The online functionality is implemented in Python, located in the [`src/online`](/src/online/) directory, with core modules in C++ encapsulated within [`handsclapping_pylib.cpp`](/src/handsclapping_pylib.cpp).  
  **TODO:** The current implementation supports multiplayer gameplay over a local area network (LAN). Development of a mobile client is ongoing.

- **AI Development:** A key goal of this project is to create an AI capable of competing with human players, potentially exceeding human performance in the game.  
  The first unsupervised AI model, *Idiot*, is currently under training in the `dev-AI_Idiot` branch, showing promising results. Additionally, an improved AI model, *HDP (Homogeneous Dynamic Programming)*, is available in the `dev-AI_HDP` branch, offering superior performance.

> [!IMPORTANT]
> Please be aware that this repository only contains the training code for the AI models. The data for the pre-trained AI models is stored separately and can be accessed via [Hugging Face](https://huggingface.co/HandsClapping/HandsClappingAI). You can download the trained models from there and store in directory `data/AI/Idiot` and `data/AI/HDP`.

> [!IMPORTANT]
> For the rules of the game and instructions on how to play the electronic version, please refer to [README.md](/src/README.md) in the `src` directory. This document serves as the technical guide for the project, detailing the setup, installation, and usage processes.

## Build and Compile

> [!TIP]
> This section explains how to build and compile the project on both Windows and Linux systems. It assumes basic programming knowledge and familiarity with the necessary toolchains. If you prefer not to build the project locally, you can skip this section and download pre-built releases as outlined below.

### Required Packages

Select the relevant instructions based on your operating system.

<details>
<summary>Windows</summary>

- **C++ Compiler:** Install [Microsoft Visual C++](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-microsoft-visual-c-redistributable-version). Visual Studio is recommended, although it is much larger.
- **CMake:** Install [CMake](https://cmake.org/download/).
- **Python3:** Install [Python3](https://www.python.org/download/releases/3.0/).  
  **Note:** During installation, ensure you select the option to "Add Python to environment variables."
- **pip:** This is typically installed with Python. You can verify by running `pip --version`. If pip is not installed, reinstall Python and select the pip option during setup.
- **pybind11:** Install via `pip install pybind11`.
- **Git:** Install [Git](https://git-scm.com/downloads).
- **PyInstaller (Optional, for local release generation):** Install via `pip install pyinstaller`.
- **NSIS (Optional, for creating setup files):** Install [NSIS](https://nsis.sourceforge.io/Download).

</details>

<details>
<summary>Linux</summary>

- **C++ Compiler:** Install via `apt install g++`.
- **CMake:** Install via `apt install cmake`.
- **Python3:** Install via `apt install python3`.
- **pip:** This is typically installed with Python. You can verify by running `pip --version`. If pip is not installed, use `apt install pip`.
- **pybind11:** Install via `pip install pybind11`.
- **Git:** Install via `apt install git`.
- **PyInstaller (Optional, for local release generation):** Install via `pip install pyinstaller`.

</details>

### Build Steps

Follow these instructions depending on your operating system.

<details>
<summary>Windows</summary>

1. Clone the repository:

    ```powershell
    git clone "https://github.com/YixuanFu06/HandsClapping.git"
    ```

    Alternatively:

    ```powershell
    git clone https://gitee.com/YixuanFu06/HandsClapping.git
    ```

2. Update the paths in [`CMakeLists.txt`](/CMakeLists.txt) (see comments). You can find your Python3 path using `where python3`, and the pybind11 path via `pip show pybind11`.

3. In the command prompt, execute the following commands:

    ```powershell
    cd HandsClapping
    mkdir build
    cd build
    cmake ..
    cd ..
    cmake --build build --config Release
    ```

4. To generate a release, switch to the `release-Windows` branch and run:

    ```powershell
    cd build
    cpack
    ```

5. After compiling, the executable can be found in the `build/bin` directory. To play the offline game, run the program in `build/bin/game`. To enable the online mode, launch the server program (`server.py`) and connect clients (`client.py`).

For detailed instructions on running the game, please refer to [README.md](/src/README.md) in the `src` directory.

</details>

<details>
<summary>Linux</summary>

1. Clone the repository:

    ```bash
    git clone "https://github.com/YixuanFu06/HandsClapping.git"
    ```

    Alternatively:

    ```bash
    git clone https://gitee.com/YixuanFu06/HandsClapping.git
    ```

2. Update the paths in [`CMakeLists.txt`](/CMakeLists.txt) (see comments). You can find your Python3 path using `which python3`, and the pybind11 path via `pip show pybind11`.

3. In the terminal, execute the following commands:

    ```bash
    cd HandsClapping
    mkdir build
    cd build
    cmake ..
    make
    ```

4. To generate a release, switch to the `release-Linux` branch and run:

    ```bash
    cd build
    cpack
    ```

5. After compiling, the executable can be found in the `build/bin` directory. To play the offline game, run the program in `build/bin/game`. To enable the online mode, launch the server program (`server.py`) and connect clients (`client.py`).

For detailed instructions on running the game, please refer to [README.md](/src/README.md) in the `src` directory.

</details>

## Install with Releases

To install using pre-built releases, download the appropriate release for your operating system from the repository. Currently, releases are available for Windows and Linux (x86-64). Linux releases may also work on arm64, though this is unconfirmed.

> [!NOTE]  
> When downloading the releases, your browser may display a warning about potential security risks. Please be assured that these warnings are often due to the use of unsigned executables or the source of the download. You can safely ignore these warnings as the releases are thoroughly tested and verified. If you are still concerned, you may manually verify the integrity of the files or check with trusted sources before proceeding with the installation.


Once the release is installed, follow the instructions in the [README.md](/src/README.md) in the `src` directory for how to run the game.

Unfortunately, macOS is not supported at this time due to technical limitations. We welcome contributions to build macOS releases.

> **Contact Us**  
> If you encounter any issues or have questions, feel free to open an issue in the repository or contact Yixuan Fu via the email listed in his profile.
