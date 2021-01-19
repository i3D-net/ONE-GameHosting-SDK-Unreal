# one-gamehosting-sdk-unreal

This Unreal Engine Plugin provides access to `i3D.net` One Platform SDK, for easy and efficient scaling of game servers!

The `i3D.net` Game Hosting SDK works on Windows and Linux.
If something doesn’t work, please [file an issue](https://github.com/i3D-net/ONE-GameHosting-SDK/issues).


## Overview

The documentation for the entire ONE Platform can be found [here](https://www.i3d.net/docs/one/). This plugin provides the required public headers files as described [here](https://git.i3d.net/one/ardentblue/one-game-sdk/-/blob/master/docs/integration_guide.md), as well, as the required internal implementation which can be found [here](https://git.i3d.net/one/ardentblue/one-game-sdk/-/tree/master/one/arcus).


## Setup

Follow the following steps to build the Unreal Plugin.


### Windows

1. add `C:\Program Files\Epic Games\UE_4.25\Engine\Build\BatchFiles` in the `PATH` environment variable
2. run the following command to build:
```
RunUAT.bat BuildPlugin -plugin="%USERPROFILE%\source\repos\one-gamehosting-sdk-unreal\OneArcusPlugin\OneArcusPlugin.uplugin" -package="%USERPROFILE%\Documents\Unreal Projects\Game\Plugins\OneArcusPlugin2"
```


## SDK Version

This plugin use the One Platform SDK version [sources](https://git.i3d.net/one/ardentblue/one-game-sdk/-/commit/30652c0d50be9f4e67a2c5d45e8367c6091a2e29). Which is currently a work in progress: v0.9 (Beta).
