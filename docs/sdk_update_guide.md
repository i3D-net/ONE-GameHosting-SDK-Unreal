# SDK update guide

To update the i3D.net ONE Game Hosting SDK code into the Unreal Plugin follow the following steps:

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