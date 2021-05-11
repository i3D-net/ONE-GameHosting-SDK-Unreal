# Ping Example

The Ping Example use the `ONEGameClientPlugin` to gather ping information about `I3D` data center locations. It is meant to be running on the game client, not on a dedicated game server. 

## Building Ping Example Game Client

With the Unreal Engine Editor:
1. Copy the packaged folder content of `ONEGameClientPlugin` into `UnrealEngine/Engine/Plugins/One`.
2. In the Unreal Engine Editor select `Edit -> Plugins`.
3. Search for the `ONEGameClientPlugin` plugin.
4. Enable the plugin and restart the Unreal Engine Editor.
5. Open the `PingExample` with the Unreal Engine Editor.
6. Restart `Unreal Engine` and reopen the `PingExample`.
7. Package the project to a folder of your choice by selecting: `File -> Package Project -> Windows (64-bits)`. Note: `Linux` and `Windows (32-bits)` are also supported.

## Testing Ping Example Locally

Simply run the `Ping Example` game. The ping statistics will be updated in the logs.

## Building Unreal Engine Editor from Sources

### Windows

1. Get the Unreal Engine sources by following this [guide](https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/DownloadingSourceCode/index.html). Note: the linux [guide] (https://docs.unrealengine.com/en-US/SharingAndReleasing/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/index.html) has detailed steps to register for an Epic Game account and seting up `git`.
2. (Optional) To be able to cross compile for `linux`, one has to install the `cross compile toolchain` following this [guide](https://docs.unrealengine.com/en-US/SharingAndReleasing/Linux/GettingStarted/index.html). Note: make sure to reboot at least once after the installation.
3. Checkout the release branch of Unreal Engine you intend to use. At the time of writting the latest release branch was: `4.25.4-release`.
4. Build the Unreal Engine from the sources following this [guide](https://docs.unrealengine.com/en-US/ProductionPipelines/DevelopmentSetup/BuildingUnrealEngine/index.html).

### Linux

1. Get the sources and build the Unreal Engine Editor following this [guide](https://docs.unrealengine.com/en-US/SharingAndReleasing/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/index.html).
