# i3D.net ONE Game Hosting SDK Unreal Plugin #

**Version: v0.9 (Beta)**

> All v1.0 features are complete and ready for integration and use. Customer iteration will determine any final changes before labelling as v1.0.

---

The current version of `i3D.net` ONE Game Hosting SDK code used in this plugin is referenced [here](docs/sdk_version.md).


## Overview ##

The plugin provides Unreal game servers with the ability to communicate over TCP with the i3D.net ONE Platform, for easy and efficient scaling of game servers.

- [Integration guide](#integration-guide) - How to integrate the plugin into a game server.
- [Sample game](/ArcusExample) - An example of how to use the plugin.
- [Plugin package](#plugin-package) - How to package the plugin.

The i3D.net Game Hosting SDK works on Windows and Linux.
If something doesn’t work, please [file an issue](https://github.com/i3D-net/ONE-GameHosting-SDK-Unreal/issues).

The documentation for the entire ONE Platform can be found [here](https://www.i3d.net/docs/one/).

Supported platforms:
    - Windows 10 Pro
    - Ubuntu 18.04


## <a name="requirements"></a> Requirements ##

1. Minimum compatible Unreal version: 4.25.4.
2. Native SDK libraries require C++ Redistributable 2015 to be installed on Windows. Download the installer and follow its instructions:
    - [x64 and x86](https://www.microsoft.com/en-US/download/details.aspx?id=48145)


## <a name="integration-guide"></a> Integration guide ##

1. Download the `ONE Game Hosting Plugin` from the Unreal Marketplace.
2. Copy the plugin folder into `C:\Program Files\Epic Games\UE_4.25\Engine\Plugins`.
3. In Unreal Editor, select _Edit > Plugins_ menu.
4. Select the plugin from step 1 and click _Enable_.
5. Relaunch the Unreal Editor and open your game.
6. Click _Add New > Blueprint Class_ and search for class `One Arcus Server`.
7. Drag in the new `One Arcus Server` into a persistent level, so that the `One Arcus Server` persist during the whole game life cycle.
8. Refer to [the arcus example](/ArcusExample) on how to set up and use the component.
9. [Test](#how-to-test).

There is an of a dedicated game server using the `ONE Game Hosting Plugin` available [here](/ArcusExample/README.md).


## <a name="how-to-test"></a> How to test ##

There are two ways to test a Game Server that is running an `One Arcus Server`:

1. The SDK contains a Fake Agent that can connect and simulate a real deployment. Build and run instructions can be found [here](https://github.com/i3D-net/ONE-GameHosting-SDK/tree/master/one/agent).
2. The Game Server can be uploaded to a live One Development Platform Deployment. See [here](https://www.i3d.net/docs/one/).

> Testing can be performed either in Unity Editor or on a build running in headless mode.

## <a name="plugin-package"></a> Package export ##

Optional - for developers that need to build and package the plugin locally.

### Windows ###

1. Add `C:\Program Files\Epic Games\UE_4.25\Engine\Build\BatchFiles` in the `PATH` environment variable
2. Build the Arcus plugin:
    2.1 For the following steps: the `one-gamehosting-sdk-unreal` repository is cloned in the folder `%USERPROFILE%/source/repos/one-gamehosting-sdk-unreal`.
    2.2 Run the following command to build the sdk plugin:
    ```
    RunUAT.bat BuildPlugin -plugin="%USERPROFILE%/source/repos/one-gamehosting-sdk-unreal/ONEGameHostingPlugin/ONEGameHostingPlugin.uplugin" -package="%USERPROFILE%/.../Plugins/ONEGameHostingPlugin"
    ```
    Where `...` is the working directory where you want to copy the plugin.
    2.3. Copy the `ONEGameHostingPlugin` into Unreal Engine plugin directory: `C:\Program Files\Epic Games\UE_4.25\Engine\Plugins\One`, since it cannot be set directly via their toolchain.


### Linux ###

1. Follow the [Linux Quick Start](https://docs.unrealengine.com/en-US/SharingAndReleasing/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/index.html) for instructions on how to build the Unreal Engine locally.
2. For the following steps: the `UnrealEngine` is cloned in the folder `~/src/UnrealEngine/` and the `one-gamehosting-sdk-unreal` is cloned in the folder `~/src/one-gamehosting-sdk-unreal`. Make sure that the folder is created `~/src/One`.
3. Build the Arcus plugin:
    3.1 Run the following command:
    ```
    ~/src/UnrealEngine/Engine/Build/BatchFiles/RunUAT.sh BuildPlugin -plugin=../one-gamehosting-sdk-unreal/ONEGameHostingPlugin/ONEGameHostingPlugin.uplugin -package=../One/ONEGameHostingPlugin -TargetPlatform=Linux
    ```
    3.2 Copy the folder `~/src/One/ONEGameHostingPlugin` in `~/src/UnrealEngine/Engine/Plugin/One/ONEGameHostingPlugin`


## For developers using this plugin

Optional - The target audiance is software developers that want to build the plugin locally without using pre-built plugin from Unreal marketplace. The technical guide can be found [here](docs/technical/guide.md).
