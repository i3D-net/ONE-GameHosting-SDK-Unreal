# one-gamehosting-sdk-unreal

This Unreal Engine Plugin provides access to `i3D.net` One Platform SDK, for easy and efficient scaling of game servers!

The `i3D.net` Game Hosting SDK works on Windows and Linux.
If something doesn’t work, please [file an issue](https://github.com/i3D-net/ONE-GameHosting-SDK/issues).


## Overview

The documentation for the entire ONE Platform can be found [here](https://www.i3d.net/docs/one/). This plugin provides the required public headers files as described [here](https://git.i3d.net/one/ardentblue/one-game-sdk/-/blob/master/docs/integration_guide.md), as well, as the required internal implementation which can be found [here](https://git.i3d.net/one/ardentblue/one-game-sdk/-/tree/master/one/arcus).


## SDK Versoin

The current version of `i3D.net` One Platform SDK code used in this plugin is referenced [here](docs/sdk_version.md).

### update guide

Follow the instruction [here](docs/sdk_update_guide.md) to change the version of the SDK code used in the plugin.


## Developer Setup

Follow the following steps to build the Unreal Plugin. The target audiance is software developers that want to build the plugin locally without using pre-built plugin from Unreal marketplace.


### Windows

1. Add `C:\Program Files\Epic Games\UE_4.25\Engine\Build\BatchFiles` in the `PATH` environment variable
2. Build the Arcus SDK plugin:
    2.1 Run the following command to build the sdk plugin:
    ```
    RunUAT.bat BuildPlugin -plugin="%USERPROFILE%/source/repos/one-gamehosting-sdk-unreal/OneArcusSDKPlugin/OneArcusSDKPlugin.uplugin" -package="%USERPROFILE%/.../Plugins/OneArcusSDKPlugin"
    ```
    Where `...` is the working directory where you want to copy the plugin.
    2.2. Copy the `OneArcusSDKPlugin` into Unreal Engine plugin directory: `C:\Program Files\Epic Games\UE_4.25\Engine\Plugins\Arcus`, since it cannot be set directly via their toolchain.
3. Build the Arcus plugin:
    3.1 Run the following command to build the sdk plugin:
    ```
    RunUAT.bat BuildPlugin -plugin="%USERPROFILE%/source/repos/one-gamehosting-sdk-unreal/OneArcusPlugin/OneArcusPlugin.uplugin" -package="%USERPROFILE%/.../Plugins/OneArcusPlugin"
    ```
    Where `...` is the working directory where you want to copy the plugin.
    3.2. Copy the `OneArcusPlugin` into Unreal Engine plugin directory: `C:\Program Files\Epic Games\UE_4.25\Engine\Plugins\Arcus`, since it cannot be set directly via their toolchain.
