# Arcus Example

It is only possible to build a dedicated game server with the Unreal Engine Editor that is built directly from sources. Also, this [guide](https://docs.unrealengine.com/en-US/InteractiveExperiences/Networking/HowTo/DedicatedServers/index.html) explain the required setup to create your own dedicated game server.

## Building Arcus Example Game Server

With the Unreal Engine Editor built from the sources, follow the following steps to build the Arcus Example dedicated game server:
1. Copy the packaged folder content of `ONEGameHostingPlugin` into `UnrealEngine/Engine/Plugins/One`.
2. In the Unreal Engine Editor select `Edit -> Plugins`.
3. Search for the `ONEGameHostingPlugin` plugin.
4. Enable the plugin and restart the Unreal Engine Editor.
5. Open the `ArcusExample` with the Unreal Engine Editor.
6. When prompted to convert the project select `More Options...` -> `Convert in-place`.
7. On the compile failed prompt select `Open in Visual Studio` and build `ArcusExample`.
8. Restart `Unreal Engine` and reopen the `ArcusExample`.
9. In `File -> Package Project -> Build Target` select: `ArcusExampleServer`.
10. Package the project to a folder of your choice by selecting: `File -> Package Project -> Windows (64-bits)`. Note: `Linux` and `Windows (32-bits)` are also supported.

## Testing Arcus Example Locally

To test locally one can use the `Fake Agent` found [here](https://github.com/i3D-net/ONE-GameHosting-SDK/tree/master/one/agent). The technical documentation for that project can be found [here](https://github.com/i3D-net/ONE-GameHosting-SDK).

To test locally the Arcus Example game server with the local agent:

1. To launch the `Fake Agent` run:
```
agent.exe
```
Located in the `one-game-sdk` build folder. The agent will repeatedly attempt to connect to the game server on the default port: `19001`. It is possible to pass an alternate port as a command line parameter, e.g. `agent.exe <port>`.

2. Package the Arcus Example Game Server with Unreal Engine Editor into a folder, in this case: `ArcusExampleWin64`.
3. Run the executable with the following arguments: `ArcusExampleWin64/WindowsServer/ArcusExampleServer.exe -ManagementPort 19001`. If the `-ManagementPort 19001` switch is omitted or isn't an integer, the game server will use the default port `19001` to communicate with the `agent`.
4. The `ArcusExampleServer` logs will show the received `Fake Agent` messages payload.

## Building Unreal Engine Editor from Sources

### Windows

1. Get the Unreal Engine sources by following this [guide](https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/DownloadingSourceCode/index.html). Note: the linux [guide] (https://docs.unrealengine.com/en-US/SharingAndReleasing/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/index.html) has detailed steps to register for an Epic Game account and seting up `git`.
2. (Optional) To be able to cross compile for `linux`, one has to install the `cross compile toolchain` following this [guide](https://docs.unrealengine.com/en-US/SharingAndReleasing/Linux/GettingStarted/index.html). Note: make sure to reboot at least once after the installation.
3. Checkout the release branch of Unreal Engine you intend to use. At the time of writting the latest release branch was: `4.25.4-release`.
4. Build the Unreal Engine from the sources following this [guide](https://docs.unrealengine.com/en-US/ProductionPipelines/DevelopmentSetup/BuildingUnrealEngine/index.html).

### Linux

1. Get the sources and build the Unreal Engine Editor following this [guide](https://docs.unrealengine.com/en-US/SharingAndReleasing/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/index.html).
