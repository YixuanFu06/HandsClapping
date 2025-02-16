# Handsclapping Game Tutorial

## Rules

If you are already well-versed in the rules of the Handsclapping Game, you may proceed directly to the section on **Input and Action Syntax**.

*Details coming soon.*

---

## Input and Action Syntax

This section aims to instruct you on how to enter your actions during gameplay. Each action has a formal name and several nicknames. The formal names, such as **PRODUCE**, **PISTOL**, and **SHIELD**, are used to display battlefield information, such as showing other players' actions after each round.

When entering your action, you may choose either the formal name or any of the nicknames for the desired action. For example, `.` and `qi` are nicknames for **PRODUCE**, while `dia` is a nickname for **PISTOL**, and `$` and `papapa` are nicknames for **DUPLICATOR**. Incorrect action names will be treated as **NONE**, meaning no action is performed in that round.

If an action needs to be executed multiple times, you may append the nickname with the desired repetition count. For example, `2dia` indicates **PISTOL** performed twice.

In battles involving more than two players, it is crucial to specify the target of your action; otherwise, it will be applied to all players and may consume significantly more energy than expected. You can set the target by name or by index.

Since player names are guaranteed to be unique, you can specify the target by name. For example, the command for **PISTOL** twice to a player named **name1** is `2dia->name1`.

Alternatively, you can specify the target by index, which is displayed at the beginning of each round in the order of player indices, starting from 1. For example, if players Alice, Bob, and Charley have indices 1, 2, and 3 respectively, the command for **PISTOL** three times to Bob would be `3dia->#2`.

Commands such as `dia->Alice,dia->Bob` and `2dia->#1,3dia->Charley` are also supported. Note that there should be no spaces around the commas.

The nicknames for each action are listed in the table below. You may also refer to the source code in [`src/game/define_actions.cpp`](/src/game/define_actions.cpp) for more details.

| Formal Name | Chinese Name | Nicknames |
|-------------|--------------|-----------|
| PRODUCE     | 积气         | ., qi     |
| PISTOL      | 手枪         | dia, gun  |
| PISTOL_UPPER| 上打         | ^dia, dia^, shangda |
| PISTOL_LOWER| 下打         | vdia, diav, Vdia, diaV, xiada |
| PISTOL_LEFT | 左打         | <dia, dia<, zuoda |
| PISTOL_RIGHT| 右打         | >dia, dia>, youda |
| CHOP_HORIZONTAL | 横扫       | -, hengpi, hengdun, hengsao, heng |
| CHOP_VERTICAL | 竖劈       | \|, shupi, shudun, shu |
| CHOP_INCLINE | 斜劈（左上到右下） | \\, xiepi\, xiedun\, xie\ |
| CHOP_INCLINE2 | 斜劈（右上到左下） | /, xiepi/, xiedun/, xie/ |
| CHOP_UPPER  | 上扫         | shangsao |
| CHOP_LOWER  | 下扫         | xiasao |
| CHOP_LEFT   | 左劈         | zuopi, zuosao |
| CHOP_RIGHT  | 右劈         | youpi, yousao |
| VISION      | 幻视         | T, t, huanshi |
| HAMMER      | 雷锤         | leichui, chuizi, langtou, chui |
| HAWKEYE     | 鹰眼         | o, O, yan, yingyan, ying |
| CHOP_CROSS  | 横竖扫       | +, hengshu, hengshusao, hengshupi, hengshudun |
| CHOP_X      | 双斜扫       | x, X, xpi, xdun, xsao |
| ICEAGE      | 冰天         | bing, bingtian, tianbeng |
| EARTHQUAKE  | 地裂         | dilie, liedi |
| EDITH       | 伊迪斯       | yidisi, e, E |
| BLACKHOLE   | 黑洞         | heidong |
| DOOMSDAY    | 末日         | mori |
| SHIELD      | 小防         | fang, _\|, xiaofang |
| REMOTE_SHIELD| 浪子战甲     | ~, lang, langzi |
| REBOUNDER   | 小弹         | tan, xiaotan |
| DODGE_UPLEFT | 左上躲       | ^<, <^, zuoshang, shangzuo, zuoshangduo, shangzuoduo, zuoshangshan, shangzuoshan, upleft, leftup |
| DODGE_UPMID | 上躲         | ^, shang, shangduo, shangshan, up |
| DODGE_UPRIGHT| 右上躲       | ^>, >^, youshang, shangyou, youshangduo, shangyouduo, youshangshan, shangyoushan, upright, rightup |
| DODGE_GROUNDLEFT | 左躲         | <, zuo, zuoduo, zuoshan, left |
| DODGE_GROUNDRIGHT| 右躲         | >, you, youduo, youshan, right |
| DODGE_DOWNLEFT | 左下躲       | v<, <v, V<, <V, zuoxia, xiazuo, zuoxiaduo, xiazuoduo, zuoxiashan, xiazuoshan, downleft, leftdown |
| DODGE_DOWNMID | 下躲         | v, V, xia, xiaduo, xiashan, down |
| DODGE_DOWNRIGHT| 右下躲       | v>, >v, V>, >V, youxia, xiayou, youxiaduo, xiayouduo, youxiashan, xiayoushan, downright, rightdown |
| SUICIDE     | 自杀         | Q, q, *, zisha, bong |
| ARTIFACT_SOUL| 灵魂原石 (not completed) | soul, linghun, @ |
| DUPLICATOR  | 啪啪啪啪啪 (not completed) | $, papapa, papapapapa, kexing |
| ARTIFACT_TIME| 时间原石 (not published) | time, shijian, jiu, lanihuilai |
| ARTIFACT_SPACE| 空间原石 (not published) | space, kongjian |
| ARTIFACT_MIND| 心灵原石 (not published) | mind, spirit, jingshen |
| ARTIFACT_REALITY| 现实原石 (not published) | reality, xianshi |
| ARTIFACT_STRENGTH| 力量原石 (not published) | strength, liliang |

---

## Offline Mode

> [!NOTE]
> This application does not provide a desktop shortcut. All executable files must be located in the installation directory, for example, `C:/Program Files/HandsClappingGame 1.0.0`.

Three modes are available for offline gameplay: **game mode**, **debug mode**, and **referee mode**. Game mode implements the basic functionality of the Handsclapping Game (multi-player battles are supported). Debug mode provides detailed information about the battlefield and players that is not visible to players in game mode, such as each player's energy and health. Referee mode, instead of providing a battle environment, judges the outcomes of two opposing actions without energy restrictions.

To run these modes, please launch the corresponding executable files in `HandsClapping/bin/game` in your installation path.

---

## Online Mode

> [!IMPORTANT]
> The device must have port 38921 available. If not, local recompilation with port editing is required. The current online mode only supports LAN connections.

To launch the online mode, you can run the files `HandsClapping/python/server.py` and `HandsClapping/python/client.py` using your local Python 3 interpreter or directly execute the files `HandsClapping/bin/online/server.exe` and `HandsClapping/bin/online/client.exe`. A detailed introduction to the online mode is provided below.

The online mode is based on a client-server architecture, where each player runs a client program on their device, connected to a single server program. Typically, one player needs to run both the client and server programs simultaneously.

> [!WARNING]
> The activities of all players, including their actions, energy levels, and health, are visible to the server program but not to the client program, according to the game rules. If the person launching the server is also participating in the battle, it is advisable to ensure that they are trustworthy and do not cheat using information from the server program.

### How to Launch the Server

As mentioned above, running `.py` files and executable files is supported. For example, in the installation directory:

``` powershell
> python3 ./python/server.py
```

or

``` powershell
> ./bin/online/server.exe
```

or simply click the executable file `HandsClapping/bin/online/server.exe`.

You will then be prompted to set the server arguments, including the maximum number of players, the maximum number of players from the same IP address, the waiting time for players to decide their actions, and the maximum length of usernames. Default values are provided, as shown in the image. You can accept the default values by entering `y` or set custom values by entering `n`.

![server](/assets/server.png "server")

If you choose to set custom arguments, you will be prompted to enter them one by one. All arguments are of type `int`.

Alternatively, you can launch the server with custom arguments via the command line. For example:

``` powershell
> python3 ./python/server.py -player <max_players> -ip <max_ip_player> -wait <waiting_time> -name <max_name_length>
```

or

``` powershell
> ./bin/online/server.exe -player <max_players> -ip <max_ip_player> -wait <waiting_time> -name <max_name_length>
```

The order of the arguments can be arbitrary, and any argument can be omitted. For instance, to launch a server with `<max_players>` set to 5, the corresponding command is:

``` powershell
> ./bin/online/server.exe -player 5
```

After successfully launching the server program, it will display its IP address, which clients will use to connect.

### How to Launch the Client

Similar to launching the server, running `.py` files and executable files is supported for the client:

``` powershell
> python3 ./python/client.py
```

or

``` powershell
> ./bin/online/client.exe
```

or simply click the executable file `HandsClapping/bin/online/client.exe`.

The client program requires two arguments: the server's IP address and your username, as shown in the image below.

![client](/assets/client.png "client")

You can also conveniently set the arguments via the command line:

``` powershell
> python3 ./python/client.py 192.168.3.92 username
```

or

``` powershell
> ./bin/online/client.exe 192.168.3.92 username
```

Note that the order of the client arguments cannot be changed.

After successfully establishing a connection, the current number of players will be displayed, and the server will prompt you to confirm your readiness to join the game. Entering `y` or pressing backspace directly signifies that you are ready, and this action cannot be undone. The game will commence once all players have confirmed.

Finally, enter your actions and enjoy the game!

![online_game](/assets/online_game.png "online_game")

---

## AI Mode

The HandsClapping project now supports 1v1 games between a player and an AI. Two AI structures are provided: *Idiot* and *HDP*. Four models based on the *Idiot* structure (**Idiot-alpha**, **Idiot-beta**, **Idiot-gamma**, and **Idiot-delta**) and one model based on the *HDP* structure (**HDP-alpha**) are currently available, with the *HDP* structure demonstrating superior performance.

Due to time constraints, we have not yet provided interfaces for training your own AI. This functionality will be released in the future.

To play against an AI, please run the `HandsClapping/bin/AI/api/ai_1v1.exe` file in your installation directory.

![ai_1v1](/assets/ai_1v1.png "ai_1v1")

You can also watch demonstration battles between two AIs by running the `HandsClapping/bin/AI/api/ai_demo.exe` file.

![ai_demo](/assets/ai_demo.png "ai_demo")

More powerful AI models are currently in development. Thank you for your patience and support.
