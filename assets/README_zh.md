# 拍手游戏项目

## 项目介绍

拍手游戏是成都七中育才学校的一个著名游戏。一些从该校毕业的学生以及其他对这个游戏感兴趣的人创建了这个仓库，旨在开发一个拍手游戏的应用程序。

这仍然是一个初期的项目，有许多事情需要完成，其中最重要的部分如下：

- **实现离线拍手游戏的基本元素**，包括气、生命值、走位、攻击以及其他可能的功能。编写游戏的裁判程序也非常重要。  
这部分内容在 [`src/game`](/src/game) 和 [`src/app`](/src/app) 中处理，其中 [`src/app`](/src/app) 包含所有主函数，而游戏核心部分的头文件则存储在 [`src/game`](/src/game) 中。  
***待办事项：截至2025年1月29日，游戏中已经支持了一些基本招数。但我们仍然需要更多的招数和游戏模式，例如组队模式。***

- **制作在线版本**，可能需要熟悉网络架构和渲染技术。  
  在线模式通过 [`src/online`](/src/online) 中的 Python 脚本与 C++ 模块结合构建，C++ 程序由 [`handsclapping_pylib.cpp`](/src/handsclapping_pylib.cpp) 封装。  
  ***待办事项：当前版本仅支持同一局域网内的在线游戏。移动设备客户端正在开发中。***

- **构建实用AI模型**，使其能够模拟人类玩家行为甚至击败人类。  
  首个非监督模型 *Idiot* 已在 `dev-AI_Idiot` 分支中构建并持续训练，目前部分符合预期。另一模型 *HDP（Homogeneous Dynamic Programming，同质动态规划）* 已在 `dev-AI_HDP` 分支发布，并且表现优于 *Idiot* 模型。

> [!IMPORTANT]
> 如果想了解该游戏的规则或如何在电子版本中玩该游戏，请参阅 `src` 目录中的 [README.md](/src/README.md) 文件。此文件仅负责从技术角度介绍该项目以及如何构建它，或者如何安装和使用发行版。

## 构建与编译

> [!TIP]
> 本部分介绍如何在本地 Windows 和 Linux 系统中构建和编译该项目，可能需要具备基本编程语言和相应工具链的先验知识。对于用户来说，跳过此部分并按照下一部分的说明下载该项目的发行版即可。

该项目需要以下包和库。请选择对应的操作系统查看。

<details>
<summary>Windows</summary>

- C++ 编译器：下载并安装 [Microsoft Visual C++](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-microsoft-visual-c-redistributable-version)。（推荐安装 [Visual Studio](https://visualstudio.microsoft.com/)，尽管它体积较大。）

- CMake：下载并安装 [CMake](https://cmake.org/download/)。

- Python3：下载并安装 [Python3](https://www.python.org/download/releases/3.0/)。  
**(在 Windows 中通过安装程序安装 Python3 时，请记得勾选“将 Python 添加到环境变量”选项。)**

- pip：在大多数情况下，安装 Python 时会自动安装 pip。您可以通过在命令行中运行 `pip --version` 来检查是否安装成功。如果未成功安装，您可以重新安装 Python3 并在安装程序中选择 pip 选项。

- pybind11：在命令行中运行 `pip install pybind11`。

- git：下载并安装 [git](https://git-scm.com/downloads/)。

- PyInstaller（如果想本地生成发行版，非必需）：在命令行中运行 `pip install pyinstaller`。

- NSIS（如果想在 Windows 中生成安装程序，非必需）：下载并安装 [NSIS](https://nsis.sourceforge.io/Download)。

</details>

<details>
<summary>Linux</summary>

- C++ 编译器：运行 `apt install g++`。

- CMake：运行 `apt install cmake`。

- Python3：运行 `apt install python3`。

- pip：在大多数情况下，安装 Python 时会自动安装 pip。您可以通过在终端中运行 `pip --version` 来检查是否安装成功。如果未成功安装，您可以运行 `apt install pip`。

- pybind11：在终端中运行 `pip install pybind11`。

- git：在终端中运行 `apt install git`。

- PyInstaller（如果想本地生成发行版，非必需）：在终端中运行 `pip install pyinstaller`。

</details>

构建步骤如下。请选择对应的操作系统查看。

<details>
<summary>Windows</summary>

1. 将仓库克隆到本地文件系统：在命令行中运行 `git clone "https://github.com/YixuanFu06/HandsClapping.git"` 或 `git clone https://gitee.com/YixuanFu06/HandsClapping.git`，也可以使用 GitHub Desktop（通常与 git 一起下载）。

2. 需要将 [`CMakeLists.txt`](/CMakeLists.txt) 中的两个路径更改为您的文件路径（请参阅注释）。您可以通过运行 `where python3` 获取 Python3 的路径，通过运行 `pip show pybind11` 查找 pybind11 的路径。

3. 在命令行中，切换到项目目录并输入以下命令：

``` powershell
> cd HandsClapping
--------------------
> mkdir build
> cd build
> cmake ..
> cd ..
> cmake --build build --config Release
```

如果您已安装 PyInstaller 并希望生成该项目的发行版，可以切换到 `release-Windows` 分支并运行

``` powershell
> cd build
--------------------
> cpack
```

4. 编译完成后，您可以在目录 `build/bin` 中找到可执行文件，并可以在 `build/bin/game` 中运行程序以离线玩拍手游戏。要启用在线模式，您需要首先启动服务器程序 ([server.py](/src/online/server.py))，然后将客户端程序 ([client.py](/src/online/client.py)) 连接到它。

有关运行程序的更多详细信息，请参阅 `src` 目录中的 [README.md](/src/README.md)。

</details>

<details>
<summary>Linux</summary>

1. 将仓库克隆到本地文件系统：运行 `git clone "https://github.com/YixuanFu06/HandsClapping.git"` 或 `git clone https://gitee.com/YixuanFu06/HandsClapping.git`。

2. 需要将 [`CMakeLists.txt`](/CMakeLists.txt) 中的两个路径更改为您的文件路径（请参阅注释）。您可以通过运行 `which python3` 获取 Python3 的路径，通过运行 `pip show pybind11` 查找 pybind11 的路径。

3. 在终端中，切换到项目目录并输入以下命令：

``` bash
$ cd HandsClapping
--------------------
$ mkdir build
$ cd build
$ cmake ..
$ make
```

如果您已安装 PyInstaller 并希望生成该项目的发行版，可以切换到 `release-Linux` 分支并运行

``` bash
$ cd build
--------------------
$ cpack
```

4. 编译完成后，您可以在目录 `build/bin` 中找到可执行文件，并可以在 `build/bin/game` 中运行程序以离线玩拍手游戏。要启用在线模式，您需要首先启动服务器程序 ([server.py](/src/online/server.py))，然后将客户端程序 ([client.py](/src/online/client.py)) 连接到它。

有关运行程序的更多详细信息，请参阅 `src` 目录中的 [README.md](/src/README.md)。

</details>

## 使用发行版安装

要使用发行版安装，请在仓库中找到适合您操作系统和计算机架构的发行版。目前该项目仅支持 Windows 系统和 Linux 系统的 x86-64 架构的发行版。不过，也许 Linux 的发行版也可以在 arm64 上运行，但我们不能完全确定。

安装完成后，您可以根据 `src` 目录中的 [README.md](/src/README.md) 的介绍运行程序。

不幸的是，由于技术原因，我们目前无法支持Macos系统。如果您愿意协助构建Macos系统的发行版，我们将不胜感激。

> **联系我们**  
> 欢迎在仓库中提出问题。您也可以随时通过他在个人资料中提供的电子邮件联系 Yixuan Fu。