# Developer Setup

Follow the following steps to build the Unreal Plugin and update the Arcus SDK version. The target audiance is software developers that want to build the plugin locally without using pre-built plugin from Unreal marketplace.


## Plugin Build Steps

The following sections describe the instructions to build the plugins locally.

### Windows

1. Add `C:\Program Files\Epic Games\UE_4.25\Engine\Build\BatchFiles` in the `PATH` environment variable
2. Build the Arcus SDK plugin:
    2.1 Run the following command to build the sdk plugin:
    ```
    RunUAT.bat BuildPlugin -plugin="%USERPROFILE%/source/repos/one-gamehosting-sdk-unreal/OneArcusSDKPlugin/OneArcusSDKPlugin.uplugin" -package="%USERPROFILE%/.../Plugins/OneArcusSDKPlugin" -TargetPlatforms=Win32+Win64
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


### Linux

1. Follow the [Linux Quick Start](https://docs.unrealengine.com/en-US/SharingAndReleasing/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/index.html) for instructions on how to build the Unreal Engine locally.
2. For the following steps: the `UnrealEngine` is cloned in the folder `~/src/UnrealEngine/` and the `one-gamehosting-sdk-unreal` is clone in the folder `~/src/one-gamehosting-sdk-unreal`. Make sure that the folder is created `~/src/Arcus`.

2. Build the Arcus SDK plugin:
    2.1 Run the following command:
    ```
    ~/src/UnrealEngine/Engine/Build/BatchFiles/RunUAT.sh BuildPlugin -plugin=../one-gamehosting-sdk-unreal/OneArcusSDKPlugin/OneArcusSDKPlugin.uplugin -package=../Arcus/OneArcusSDKPlugin -TargetPlatform=Linux
    ```
    2.2 Copy the folder `~/src/Arcus/OneArcusSDKPlugin` in `~/src/UnrealEngine/Engine/Plugin/Arcus/OneArcusSDKPlugin`

3. Build the Arcus plugin:
    3.1 Run the following command:
    ```
    ~/src/UnrealEngine/Engine/Build/BatchFiles/RunUAT.sh BuildPlugin -plugin=../one-gamehosting-sdk-unreal/OneArcusPlugin/OneArcusPlugin.uplugin -package=../Arcus/OneArcusPlugin -TargetPlatform=Linux
    ```
    3.2 Copy the folder `~/src/Arcus/OneArcusPlugin` in `~/src/UnrealEngine/Engine/Plugin/Arcus/OneArcusPlugin`

## SDK update guide


Follow the following instructions to change the version of the i3D.net ONE Game Hosting SDK code used in the plugin:

1. Make sure you have both git repositories cloned locally.
    * SDK repository: `git@git.i3d.net:one/ardentblue/one-game-sdk.git`
    * SDK plugin repository: `git@git.i3d.net:one/ardentblue/one-gamehosting-sdk-unreal.git`
2. Make sure that the sdk repository has the wanted SDK version checked out.
3. Run the following script:
```bash
${PATH_OF_SDK_PLGUIN_REPOSITORY}/tools/sdk_update.sh ${PATH_OF_SDK_REPOSITORY} ${PATH_OF_SDK_PLGIN_REPOSITORY}
```
Note: make sure to omit the trailing slash at the end of the path. For example:
```bash
./tools/sdk_update.sh ~/src/one-game-sdk ~/src/one-gamehosting-sdk-unreal
```
4. Commit the changes in the SDK plugin repository.

Note that the update script will update the `${PATH_OF_SDK_PLGUIN_REPOSITORY}/docs/sdk_version.md` automatically.

## Arcus Example Dedicated Game Server


Currently, is it only possible to build a dedicated game server with the Unreal Engine Editor that is built directly from sources. Also, this [guide](https://docs.unrealengine.com/en-US/InteractiveExperiences/Networking/HowTo/DedicatedServers/index.html) explain the required setup to create your own dedicated game server.

### Building Unreal Engine Editor from Sources


#### Windows


1. Get the Unreal Engine sources by following this [guide](https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/DownloadingSourceCode/index.html). Note: the linux [guide] (https://docs.unrealengine.com/en-US/SharingAndReleasing/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/index.html) has detailed steps to register for an Epic Game account and seting up `git`.
2. (Optional) To be able to cross compile for `linux`, one has to install the `cross compile toolchain` following this [guide](https://docs.unrealengine.com/en-US/SharingAndReleasing/Linux/GettingStarted/index.html). Note: make sure to reboot at least once after the installation.
3. Checkout the release branch of Unreal Engine you intend to use. At the time of writting the latest release branch was: `4.25.4-release`.
4. Build the Unreal Engine from the sources following this [guide](https://docs.unrealengine.com/en-US/ProductionPipelines/DevelopmentSetup/BuildingUnrealEngine/index.html).

#### Linux


1. Get the sources and build the Unreal Engine Editor following this [guide](https://docs.unrealengine.com/en-US/SharingAndReleasing/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/index.html).

### Building Arcus Example Game Server


Once the Unreal Engine Editor has been built, follow the following steps to build the Arcus Example dedicated game server:
1. Copy the packaged folder content for both `OneArcusSKDPlugin` and `OneArcusPlugin` into `UnrealEngine/Engine/Plugins/Arcus`.
2. In the Unreal Engine Editor select `Edit -> Plugins`.
3. Search for `Arcus` to find both `OneArcusSKDPlugin` and `OneArcusPlugin` plugins.
4. Enable both plugins and restart the Unreal Engine Editor.
5. Open the `ArcusExample` with the Unreal Engine Editor.
6. In `File -> Package Project -> Build Target` select: `ArcusExampleServer`.
7. Package the project to a folder of your choice by selecting: `File -> Package Project -> Windows (64-bits)`. Note: `Linux` and `Windows (32-bits)` are also supported.   

### Testing Arcus Example Locally


To test locally one can use the `Fake Agent` found [here](https://git.i3d.net/one/ardentblue/one-game-sdk). The technical documentation for that project can be found [here](https://git.i3d.net/one/ardentblue/one-game-sdk/-/tree/master/docs).

To test locally the Arcus Example game server with the local agent:

1. To launch the `Fake Agent` run:
```
agent.exe 19002
```
Located in the `one-game-sdk` build folder. Where `19002` is the port that the agent connects on. If the port is omitted, it defaults to `19001`.

2. Package the Arcus Example Game Server with Unreal Engine Editor into a folder, in this case: `ArcusExampleWin64`.
3. Run the executable with the following arguments: `ArcusExampleWin64/WindowsServer/ArcusExampleServer.exe -log -ManagementPort 19001`. The `-log` switch is optional and if present will display a window with the logs. If the `-log` is omitted, the logs files can be found in `ArcusExampleWin64/WindowsServer/ArcusExample/Saved/Logs`. Also, if the `-ManagementPort 19001` switch is omitted or isn't an integer, the game server will use the default port `19001` to communicate with the `agent`.
