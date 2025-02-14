# Handsclapping Game Tutorial

## Rules

If you already know well about the rule of Handsclapping game, you can go on to the next part about **Input and Action Syntax**.

Coming out soon.

## Input and Action Syntax

This part aims to teaches you how to enter your actions in the battle. Each action has one formal name and a few nicknames. The formal name, such as **PRODUCE**, **PISTOL**, **SHIELD** will be used when showing the battle field information, such as showing you others' action after each round.

When input your action, you can choose one arbitrary nickname or formal name of the action you want, for example, `.` and `qi` for **PRODUCE**, `dia` for **PISTOL**, `$` and `papapa` for **DUPLICATOR** and so on. Meanwhile, as a punishment, any wrong action name input will be treat as action **NONE**, which means you do nothing in this round.

If an action needs to be implemented more than one time, you are permitted to attach the nickname behind the repeated time number. For example, `2dia` means **PISTOL** for 2 times.

In battle with more that 2 players, it's important to set the target of your action, otherwise it will be treated as to all and may consume much more energy than you expected. Two approaches of setting target is provided, including by name or by index.

Since users' names are guaranteed to be unique in the game, you may set the target by players' names. For example, the command for **PISTOL** 2 times to player **name1** is `2dia->name1`.

When the players' names are too long or complex to type, you can also set the target by its index in the game. At the beginning of each round, the players alive will be printed in the order of index, starting from 1. For example, in the following image, the indices for player Alice, Bob, and Charley are 1, 2, and 3 respectively.

![index_example](/assets/index_example.png "index_example")

Thus, the command for **PISTOL** 3 times to player Bob can be `3dia->#2`.

Input such as `dia->Alice,dia->Bob` and `2dia->#1,3dia->Charley` is also supported. Attention that there should not be space around `,`.

The nicknames for each action is given by the table below. You may also look them up in [`src/game/define_actions.cpp`](/src/game/define_actions.cpp).

| Formal Name | Chinese Name | Nicknames |
|-------------|--------------|----------|
| PRODUCE     | 积气         | ., qi    |
| PISTOL      | 手枪         | dia, gun |
| PISTOL_UPPER|   上打       | ^dia, dia^, shangda |
| PISTOL_LOWER|  下打     | vdia, diav, Vdia, diaV, xiada |
| PISTOL_LEFT |左打       | <dia, dia<, zuoda |
| PISTOL_RIGHT| 右打     | >dia, dia>, youda |
| CHOP_HORIZONTAL |   横扫| -, hengpi, hengdun, hengsao, heng |
| CHOP_VERTICAL | 竖劈 | \|, shupi, shudun, shu |
| CHOP_INCLINE |    斜劈（左上到右下）     | \\, xiepi\, xiedun\, xie\ |
| CHOP_INCLINE2 |   斜劈（右上到左下）    | /, xiepi/, xiedun/, xie/ |
| CHOP_UPPER  |  上扫     | shangsao |
| CHOP_LOWER  |   下扫    | xiasao |
| CHOP_LEFT   |     左劈      | zuopi, zuosao |
| CHOP_RIGHT  |    右劈       | youpi, yousao |
| VISION      |     幻视      | T, t, huanshi |
| HAMMER      |     雷锤      | leichui, chuizi, langtou, chui |
| HAWKEYE     |       鹰眼    | o, O, yan, yingyan, ying |
| CHOP_CROSS  |     横竖扫      | +, hengshu, hengshusao, hengshupi, hengshudun |
| CHOP_X      |      双斜扫     | x, X, xpi, xdun, xsao |
| ICEAGE      |    冰天       | bing, bingtian, tianbeng |
| EARTHQUAKE  |     地裂      | dilie, liedi |
| EDITH       |    伊迪斯       | yidisi, e, E |
| BLACKHOLE   |   黑洞        | heidong |
| DOOMSDAY    |      末日     | mori |
| SHIELD      |    小防       | fang, _\|, xiaofang |
| REMOTE_SHIELD|    浪子战甲    | ~, lang, langzi |
| REBOUNDER   |      小弹     | tan, xiaotan |
| DODGE_UPLEFT |    左上躲     | ^<, <^, zuoshang, shangzuo, zuoshangduo, shangzuoduo, zuoshangshan, shangzuoshan, upleft, leftup |
| DODGE_UPMID |    上躲       | ^, shang, shangduo, shangshan, up |
| DODGE_UPRIGHT|     右上躲   | ^>, >^, youshang, shangyou, youshangduo, shangyouduo, youshangshan, shangyoushan, upright, rightup |
| DODGE_GROUNDLEFT |   左躲 | <, zuo, zuoduo, zuoshan, left |
| DODGE_GROUNDRIGHT| 右躲 | >, you, youduo, youshan, right |
| DODGE_DOWNLEFT |   左下躲   | v<, <v, V<, <V, zuoxia, xiazuo, zuoxiaduo, xiazuoduo, zuoxiashan, xiazuoshan, downleft, leftdown |
| DODGE_DOWNMID |    下躲   | v, V, xia, xiaduo, xiashan, down |
| DODGE_DOWNRIGHT|   右下躲  | v>, >v, V>, >V, youxia, xiayou, youxiaduo, xiayouduo, youxiashan, xiayoushan, downright, rightdown |
| SUICIDE     |    自杀       | Q, q, *, zisha, bong |
| ARTIFACT_SOUL|     灵魂原石(not completed)    | soul, linghun, @ |
| DUPLICATOR  |   啪啪啪啪啪(not completed)  | $, papapa, papapapapa, kexing |
| ARTIFACT_TIME| 时间原石(not published)    | time, shijian, jiu, lanihuilai |
| ARTIFACT_SPACE|   空间原石(not published)     | space, kongjian |
| ARTIFACT_MIND|    心灵原石(not published)      | mind, spirit, jingshen |
| ARTIFACT_REALITY|  现实原石(not published)       | reality, xianshi |
| ARTIFACT_STRENGTH|   力量原石(not published)     | strength, liliang |

## Offline Mode

> [!IMPORTANT]
> This application doesn't provide a shortcut on the desktop. All the executable files should be looking in the installation directory, for example, `C:/Program Files/HandsClappingGame 1.0.0`.

Three modes for offline game are provided if you construct the project locally by the code provided in the repository, including **game mode**, **debug mode**, and **referee mode**, in which game mode implements the basic function of Handsclapping game (multi-player battle is supported), while the debug mode provides more detailed information about the battle field and players which is nontransparent for players in game mode, such as each player's energy and health. The referee mode, instead of providing a battle environment, judges the outcome of two against actions, with no restriction on energy.

To run these modes, please launch the corresponding executable files in `HandsClapping/bin/game` in your installation path.

## Online Mode

> **REQUIREMENT**\
> The device must have its 38921 port available, or recompiling locally with port editing is needed.
> Current online mode can only support internet connection within LAN.

To launch online mode, you can either run files `HandsClapping/python/server.py`, `HandsClapping/python/server.py` by your local python3 interpreter or directly run executable files `HandsClapping/bin/online/server.exe`, `HandsClapping/bin/online/client.exe`. More detailed introduction to online mode is given below.

The online mode is based on the client-server structure, in which each player runs a client program on their device which are connected to an identical server program. Typically, one of the players needs to run client and server program at the same time.

> [!WARNING]
> The current activities of all players, including their actions, energies, healths is transparent to the server program but not to client program, according to the rule of the game. So if the person who launches the server is also in the battle with another client program, you'd better find the one you trust and make sure that they doesn't cheat by the information from the server program.

### How to launch the server

As mentioned above, running `.py` file and executable file are both supported. For example, (in the installation directory)

``` powershell
> python3 ./python/server.py
```

or

``` powershell
> ./bin/online/server.exe
```

or simply click the executable file `HandsClapping/bin/online/server.exe`.

Then you are required to set the arguments for server, including maximum player number, maximum players number from the same IP address, waiting time for players to decide their action, and the maximum length of usernames. The default value is provided as the image shows. You can use them directly by enter `y` or set custom value by enter `n`.

![server](/assets/server.png "server")

The program will require you to enter the custom argument respectively if `y` is entered. The arguments are all `int` type.

There's another convenient way to launch the server with few custom arguments, through the command line. In the command line, you are expected to launch the server as below:

``` powershell
> python3 ./python/server.py -player <max_players> -ip <max_ip_player> -wait <waiting_time> -name <max_name_length>
```

or

``` powershell
> ./bin/online/server.exe -player <max_players> -ip <max_ip_player> -wait <waiting_time> -name <max_name_length>
```

Where the four argument can in arbitrary order and any one can be omitted. For instance, if a server with argument `<max_players>`=5 is needed, the corresponding command is

``` powershell
> ./bin/online/server.exe -player 5
```

After launching the server program successfully, it will shows it's IP address, which will be used by the clients to get connection with it.

### How to launch the client

Similar to launching the server, running `.py` file and executable file for client are both supported:

``` powershell
> python3 ./python/client.py
```

or

``` powershell
> ./bin/online/client.exe
```

or simply click the executable file `HandsClapping/bin/online/client.exe`.

The client program requires two arguments, which are the IP address of server and your username, as shown in the image below.

![client](/assets/client.png "client")

You can also set the arguments conveniently in the command line.

``` powershell
> python3 ./python/client.py 192.168.3.92 username
```

or

``` powershell
> ./bin/online/client.exe 192.168.3.92 username
```

Attention that the order of arguments for client can't be changed.

After the connection is established successfully, in-time player number will be shown and the serve will require you to confirm to join the game. Enter `y` or type backspace directly means that you have get prepared for the game and this message can't withdraw. The game will start after all the players send their confirmation.

Finally, enter your actions and have fun!

![online_game](/assets/online_game.png "online_game")

## AI Mode

HandsClapping project now supports 1v1 game between player and AI. We provide 2 AI structures, which are *Idiot* and *HDP*. Four models based on *Idiot* structure (**Idiot-alpha**, **Idiot-beta**, **Idiot-gamma**, and **Idiot-delta**) and one model based on *HDP* structure (**HDP-alpha**) are published and it's shown that HDP structure works better than Idiot structure currently.

Due to the limited time, we still not provide the interfaces to train your own AI. This function will coming out soon.

To battle with AI, please run `HandsClapping/bin/AI/api/ai_1v1.exe` file in your installation directory.

![ai_1v1](/assets/ai_1v1.png "ai_1v1")

You can also watch the demonstration battle between two AIs to know get familiar with them by running `HandsClapping/bin/AI/api/ai_demo.exe` file.

![ai_demo](/assets/ai_demo.png "ai_demo")

More powerful AI is still in training. Thanks for your patience and support on our work.
