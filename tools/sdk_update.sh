#!/bin/bash -u

# http://redsymbol.net/articles/unofficial-bash-strict-mode/
# -u
# Treat unset variables and parameters other than the special parameters "@" and "*" as an error
# when performing parameter expansion. If expansion is attempted on an unset variable or parameter,
# the shell prints an error message, and, if not interactive, exits with a non-zero status.

ONE_SDK_REPO_DIR=${1}
ONE_PLUGIN_REPO_DIR=${2}

ONE_SDK_SOURCE_DIR=${ONE_SDK_REPO_DIR}/one/arcus

ONE_PLUGIN_SOURCE_PRIVATE_DIR=${ONE_PLUGIN_REPO_DIR}/ONEGameHostingPlugin/Source/ONEGameHostingPlugin/Private/one/arcus
ONE_PLUGIN_SOURCE_PUBLIC_DIR=${ONE_PLUGIN_REPO_DIR}/ONEGameHostingPlugin/Source/ONEGameHostingPlugin/Public/one/arcus
ONE_PLUGIN_DOC_VERSION=${ONE_PLUGIN_REPO_DIR}/docs/sdk_version.md

# Filtering cmake, markdown, internal & public headers.
# See https://git.i3d.net/one/ardentblue/one-game-sdk/-/blob/master/docs/integration_guide.md#using-the-arcus-server-api
# for more details.
ls ${ONE_SDK_SOURCE_DIR} \
    | grep -v internal | grep -v CMakeLists.txt | grep -v .md \
    | grep -v c_api.h | grep -v c_error.h | grep -v c_platform.h \
    | xargs -I args cp ${ONE_SDK_SOURCE_DIR}/args ${ONE_PLUGIN_SOURCE_PRIVATE_DIR}/args

# Copying the internal folder in a second setp to avoid any uninteded filtering.
cp -r ${ONE_SDK_SOURCE_DIR}/internal ${ONE_PLUGIN_SOURCE_PRIVATE_DIR}

# Copying the public headers
cp -r ${ONE_SDK_SOURCE_DIR}/c_api.h ${ONE_PLUGIN_SOURCE_PUBLIC_DIR}
cp -r ${ONE_SDK_SOURCE_DIR}/c_error.h ${ONE_PLUGIN_SOURCE_PUBLIC_DIR}
cp -r ${ONE_SDK_SOURCE_DIR}/c_platform.h ${ONE_PLUGIN_SOURCE_PUBLIC_DIR}

# Adding copyright notice
sed -i '1i// Copyright i3D.net, 2021. All Rights Reserved.' ${ONE_PLUGIN_SOURCE_PRIVATE_DIR}/*.h
sed -i '1i// Copyright i3D.net, 2021. All Rights Reserved.' ${ONE_PLUGIN_SOURCE_PRIVATE_DIR}/*.cpp
sed -i '1i// Copyright i3D.net, 2021. All Rights Reserved.' ${ONE_PLUGIN_SOURCE_PRIVATE_DIR}/internal/*.h
sed -i '1i// Copyright i3D.net, 2021. All Rights Reserved.' ${ONE_PLUGIN_SOURCE_PRIVATE_DIR}/internal/*.cpp
sed -i '1i// Copyright i3D.net, 2021. All Rights Reserved.' ${ONE_PLUGIN_SOURCE_PUBLIC_DIR}/*.h

# Update the documentation to the lastest SDK version.
ONE_GIT_HASH=`git --git-dir ${ONE_SDK_REPO_DIR}/.git rev-parse HEAD`

echo '# ONE GAME SDK Version' > ${ONE_PLUGIN_DOC_VERSION}
echo '' >> ${ONE_PLUGIN_DOC_VERSION}
echo "This plugin uses this [version](https://git.i3d.net/one/ardentblue/one-game-sdk/-/commit/${ONE_GIT_HASH}) of the One Platform SDK repository." >> ${ONE_PLUGIN_DOC_VERSION}
