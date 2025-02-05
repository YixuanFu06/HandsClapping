# Game Tutorial

Coming out soon.

## Rules and Action Syntax

## Offline Mode

Three modes for offline game are provided if you construct the project locally by the code provided in the repository, including **game mode**, **debug mode**, and **referee mode**, in which game mode implements the basic function of Handsclapping game (multi-player battle is supported), while the debug mode provides more detailed information about the battle field and players which is nontransparent for players in game mode, such as each player's energy and health. The referee mode, instead of providing a battle environment, judges the outcome of two against actions, with no restriction on energy.

However, only **game mode** and **referee mode** are provided in the releases.

To run these modes, please run the corresponding executable files in `bin/game` in your installation path.

## Online Mode

> **REQUIREMENT**\
> The device must have its 38921 port available, or recompiling locally with port editing is needed.

To launch online mode, you can either run files `python/server.py`, `python/server.py` by your local python3 interpreter or directly run executable files `bin/online/server.exe`, `bin/online/client.exe`. More detailed introduction to online mode is given below.

The online mode bases on the client-server structure, in which each player runs a client program on their device which are connected to an identical server program. Typically, one of the players needs to run client and server program at the same time.

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

or simply click the executable file `bin/online/server.exe`.

Then you are required to set the arguments for server, including maximum player number, maximum players number from the same IP address, waiting time for players to decide their action, and the maximum length of usernames. The default value is provided as the image shows. You can use them directly by enter `y` or set custom value by enter `n`.

![server](../assets/server.png "server")

The program will require you to enter the custom argument respectively if `y` is entered. The arguments are all `int` type.

There's another convenient way to launch the server with few custom arguments, through the command line. In the command line, you are expected to launch the server as below:

``` powershell
> python3 ./python/server.py -player <max_players> -ip <max_ip_player> -wait <waiting_time> -name <max_name_length>
```

or

``` powershell
> ./bin/online/server.exe -player <max_players> -ip <max_ip_player> -wait <waiting_time> -name <max_name_length>
```

Where the four argument can in arbitrary order and any one can be omitted. For instance, if a server with argument `<max_players>`=5 is needed, the corresponding command is like

``` powershell
> ./bin/online/server.exe -player 5
```

After launching the server program successfully, it will shows it's IP address, which will be used by the clients to get connection with it.
