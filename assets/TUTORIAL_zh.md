# 拍手游戏教程

## 规则

如果您已经非常熟悉拍手游戏的规则，可以直接跳转到 **输入与招数语法** 部分。

即将推出。

## 输入与招数语法

本部分旨在教您如何在战斗中输入您的招数。每个招数都有一个正式名称和几个昵称。正式名称（如 **PRODUCE**、**PISTOL**、**SHIELD**）将用于显示战场信息，例如在每轮结束后显示其他玩家的招数。

输入招数时，您可以选择招数的任意昵称或正式名称，例如，`.` 和 `qi` 对应 **PRODUCE**，`dia` 对应 **PISTOL**，`$` 和 `papapa` 对应 **DUPLICATOR** 等。同时，作为惩罚，任何错误的招数名称输入将被视为招数 **NONE**，即本轮您什么都不做。

如果某个招数需要多次执行，您可以在招数昵称前附加重复次数。例如，`2dia` 表示 **PISTOL** 执行 2 次。

在超过 2 名玩家的战斗中，设置招数的目标非常重要，否则将被视为对所有人执行，可能会消耗比您预期更多的气数。提供了两种设置目标的方法：按名称或按索引。

由于游戏中用户的名称是唯一的，您可以通过玩家的名称设置目标。例如，对玩家 **name1** 执行 **PISTOL** 2 次的命令是 `2dia->name1`。

当玩家的名称过长或复杂时，您也可以通过其在游戏中的索引设置目标。在每轮开始时，存活玩家将按索引顺序列出，从 1 开始。例如，在下图中，玩家 Alice、Bob 和 Charley 的索引分别是 1、2 和 3。

![index_example](/assets/index_example.png "index_example")

于是，对玩家 Bob 执行 **PISTOL** 招数3 次的命令可以是 `3dia->#2`。

类似 `dia->Alice,dia->Bob` 和 `2dia->#1,3dia->Charley` 的输入也是支持的。注意不要在 `,` 周围留有空格。

每个招数的昵称如下表所示。您也可以在 [`src/game/define_actions.cpp`](/src/game/define_actions.cpp) 中查找它们。

| 正式名称 | 中文名称 | 昵称 |
|----------|----------|------|
| PRODUCE  | 积气     | ., qi |
| PISTOL   | 手枪     | dia, gun |
| PISTOL_UPPER | 上打   | ^dia, dia^, shangda |
| PISTOL_LOWER | 下打   | vdia, diav, Vdia, diaV, xiada |
| PISTOL_LEFT  | 左打   | <dia, dia<, zuoda |
| PISTOL_RIGHT | 右打   | >dia, dia>, youda |
| CHOP_HORIZONTAL | 横扫 | -, hengpi, hengdun, hengsao, heng |
| CHOP_VERTICAL | 竖劈 | \|, shupi, shudun, shu |
| CHOP_INCLINE | 斜劈（左上到右下） | \\, xiepi\, xiedun\, xie\ |
| CHOP_INCLINE2 | 斜劈（右上到左下） | /, xiepi/, xiedun/, xie/ |
| CHOP_UPPER | 上扫 | shangsao |
| CHOP_LOWER | 下扫 | xiasao |
| CHOP_LEFT | 左劈 | zuopi, zuosao |
| CHOP_RIGHT | 右劈 | youpi, yousao |
| VISION | 幻视 | T, t, huanshi |
| HAMMER | 雷锤 | leichui, chuizi, langtou, chui |
| HAWKEYE | 鹰眼 | o, O, yan, yingyan, ying |
| CHOP_CROSS | 横竖扫 | +, hengshu, hengshusao, hengshupi, hengshudun |
| CHOP_X | 双斜扫 | x, X, xpi, xdun, xsao |
| ICEAGE | 冰天 | bing, bingtian, tianbeng |
| EARTHQUAKE | 地裂 | dilie, liedi |
| EDITH | 伊迪斯 | yidisi, e, E |
| BLACKHOLE | 黑洞 | heidong |
| DOOMSDAY | 末日 | mori |
| SHIELD | 小防 | fang, _\|, xiaofang |
| REMOTE_SHIELD | 浪子战甲 | ~, lang, langzi |
| REBOUNDER | 小弹 | tan, xiaotan |
| DODGE_UPLEFT | 左上躲 | ^<, <^, zuoshang, shangzuo, zuoshangduo, shangzuoduo, zuoshangshan, shangzuoshan, upleft, leftup |
| DODGE_UPMID | 上躲 | ^, shang, shangduo, shangshan, up |
| DODGE_UPRIGHT | 右上躲 | ^>, >^, youshang, shangyou, youshangduo, shangyouduo, youshangshan, shangyoushan, upright, rightup |
| DODGE_GROUNDLEFT | 左躲 | <, zuo, zuoduo, zuoshan, left |
| DODGE_GROUNDRIGHT | 右躲 | >, you, youduo, youshan, right |
| DODGE_DOWNLEFT | 左下躲 | v<, <v, V<, <V, zuoxia, xiazuo, zuoxiaduo, xiazuoduo, zuoxiashan, xiazuoshan, downleft, leftdown |
| DODGE_DOWNMID | 下躲 | v, V, xia, xiaduo, xiashan, down |
| DODGE_DOWNRIGHT | 右下躲 | v>, >v, V>, >V, youxia, xiayou, youxiaduo, xiayouduo, youxiashan, xiayoushan, downright, rightdown |
| SUICIDE | 自杀 | Q, q, *, zisha, bong |
| ARTIFACT_SOUL | 灵魂原石（未完善） | soul, linghun, @ |
| DUPLICATOR | 啪啪啪啪啪（未完善） | $, papapa, papapapapa, kexing |
| ARTIFACT_TIME | 时间原石（未发布） | time, shijian, jiu, lanihuilai |
| ARTIFACT_SPACE | 空间原石（未发布） | space, kongjian |
| ARTIFACT_MIND | 心灵原石（未发布） | mind, spirit, jingshen |
| ARTIFACT_REALITY | 现实原石（未发布） | reality, xianshi |
| ARTIFACT_STRENGTH | 力量原石（未发布） | strength, liliang |

## 离线模式

> [!IMPORTANT]
> 本应用不会在桌面创建快捷方式。所有可执行文件均需在安装目录下查找，例如 `C:/Program Files/HandsClappingGame 1.0.0`。

离线游戏提供三种模式：**游戏模式**、**调试模式** 和 **裁判模式**。游戏模式实现了拍手游戏的基本功能（支持多人战斗），调试模式提供了更多关于战场和玩家的详细信息（这些信息在游戏模式中对玩家是不可见的），例如每个玩家的气数和生命值。裁判模式则不提供战斗环境，而是判断两个对抗招数的结果，且不受气数限制。

要运行这些模式，请在安装路径的 `HandsClapping/bin/game` 中启动对应的可执行文件。

## 在线模式

> **要求**\
> 设备必须有可用的 38921 端口，否则需要本地重新编译并修改端口。
> 当前在线模式仅支持局域网内的互联网连接。

要启动在线模式，您可以运行 `HandsClapping/python/server.py` 和 `HandsClapping/python/client.py` 文件，或者直接运行可执行文件 `HandsClapping/bin/online/server.exe` 和 `HandsClapping/bin/online/client.exe`。在线模式的详细介绍如下。

在线模式基于客户端-服务器架构，每个玩家在其设备上运行客户端程序，这些客户端程序连接到同一个服务器程序。通常，其中一名玩家需要同时运行客户端和服务器程序。

> [!WARNING]
> 根据游戏规则，所有玩家的活动（包括招数、气数、血量）对服务器程序是可见的，但对客户端程序则不是。因此，如果启动服务器的人也参与战斗，您需要找到一个您信任的人，并确保他们不会通过服务器程序的信息作弊。

### 如何启动服务器

如上所述，运行 `.py` 文件和可执行文件均被支持。例如（在安装目录下）：

```powershell  
> python3 ./python/server.py  
```

或者

```powershell  
> ./bin/online/server.exe  
```

或者直接点击可执行文件 `HandsClapping/bin/online/server.exe`。

然后，您需要设置服务器的参数，包括最大玩家数量、同一 IP 地址的最大玩家数量、玩家决定招数的等待时间以及用户名的最大长度。默认值如图所示。您可以直接输入 `y` 使用默认值，或者输入 `n` 设置自定义值。

![server](/assets/server.png "server")

如果输入 `n`，程序将依次要求您输入自定义参数。所有参数均为 `int` 类型。

还有一种更便捷的方法可以通过命令行启动服务器并设置少量自定义参数。在命令行中，启动服务器的命令如下：

```powershell  
> python3 ./python/server.py -player <max_players> -ip <max_ip_player> -wait <waiting_time> -name <max_name_length>  
```

或者

```powershell  
> ./bin/online/server.exe -player <max_players> -ip <max_ip_player> -wait <waiting_time> -name <max_name_length>  
```

其中四个参数可以任意顺序排列，且可以省略任意一个。例如，如果需要一个参数 `<max_players>`=5 的服务器，对应的命令是：

```powershell  
> ./bin/online/server.exe -player 5  
```

成功启动服务器程序后，它将显示其 IP 地址，客户端将使用该 IP 地址与服务器建立连接。

### 如何启动客户端

与启动服务器类似，运行客户端的 `.py` 文件和可执行文件均被支持：

```powershell  
> python3 ./python/client.py  
```

或者

```powershell  
> ./bin/online/client.exe  
```

或者直接点击可执行文件 `HandsClapping/bin/online/client.exe`。

客户端程序需要两个参数：服务器的 IP 地址和您的用户名，如下图所示。

![client](/assets/client.png "client")

您也可以通过命令行方便地设置参数。

```powershell  
> python3 ./python/client.py 192.168.3.92 username  
```

或者

```powershell  
> ./bin/online/client.exe 192.168.3.92 username  
```

注意客户端参数的顺序不能改变。

成功建立连接后，将显示当前玩家数量，并且服务器将要求您确认加入游戏。输入 `y` 或直接按回车键表示您已准备好，且该消息无法撤回。所有玩家发送确认后，游戏将开始。

最后，输入您的招数并享受游戏！

![online_game](/assets/online_game.png "online_game")

## 人工智能模式

拍手游戏项目目前支持玩家与 AI 的 1v1 对战。我们提供了两种 AI 结构：*Idiot* 和 *HDP*。基于 *Idiot* 结构的四个模型（**Idiot-alpha**、**Idiot-beta**、**Idiot-gamma** 和 **Idiot-delta**）以及基于 *HDP* 结构的一个模型（**HDP-alpha**）已发布，目前 *HDP* 结构的表现优于 *Idiot* 结构。

由于时间有限，我们尚未提供训练您自己的 AI 的接口。此功能将很快推出。

要与 AI 对战，请在安装目录下运行 `HandsClapping/bin/AI/api/ai_1v1.exe` 文件。

![ai_1v1](/assets/ai_1v1.png "ai_1v1")

您也可以通过运行 `HandsClapping/bin/AI/api/ai_demo.exe` 文件观看两个 AI 之间的演示战斗，以熟悉它们。

![ai_demo](/assets/ai_demo.png "ai_demo")

更强大的 AI 正在训练中。感谢您对我们工作的耐心和支持。
