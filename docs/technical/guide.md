# Developer Setup

Follow the following steps to build the Unreal Plugin and update the Arcus SDK version. The target audiance is software developers that want to build the plugin locally without using pre-built plugin from Unreal marketplace.


## Build Steps

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
./tools/sdk_update.sh ~/src/one-game-sdk ~/src one-gamehosting-sdk-unreal
```
4. Commit the changes in the SDK plugin repository.

Note that the update script will update the `${PATH_OF_SDK_PLGUIN_REPOSITORY}/docs/sdk_version.md` automatically.
