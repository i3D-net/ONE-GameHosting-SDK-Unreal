#!/bin/bash

set -euo pipefail

# http://redsymbol.net/articles/unofficial-bash-strict-mode/
# -e
# The set -e option instructs bash to immediately exit if any command [1] has a non-zero exit status.
# -u
# Treat unset variables and parameters other than the special parameters "@" and "*" as an error
# when performing parameter expansion. If expansion is attempted on an unset variable or parameter,
# the shell prints an error message, and, if not interactive, exits with a non-zero status.
# set -o pipefail
# This setting prevents errors in a pipeline from being masked. If any command in a pipeline fails,
# that return code will be used as the return code of the whole pipeline.

ONE_UNREAL_ENGINE_VERSION=${1}
ONE_SDK_REPO_DIR=${2}
ONE_PLUGIN_REPO_DIR=${3}

ONE_SDK_SOURCE_DIR=${ONE_SDK_REPO_DIR}/one/ping

ONE_PLUGIN_DIR=${ONE_PLUGIN_REPO_DIR}/${ONE_UNREAL_ENGINE_VERSION}
ONE_PLUGIN_NAME=ONEGameClientPlugin
ONE_PLUGIN_SOURCE_PRIVATE_DIR=${ONE_PLUGIN_REPO_DIR}/${ONE_UNREAL_ENGINE_VERSION}/${ONE_PLUGIN_NAME}/Source/${ONE_PLUGIN_NAME}/Private/one/ping
ONE_PLUGIN_SOURCE_PUBLIC_DIR=${ONE_PLUGIN_REPO_DIR}/${ONE_UNREAL_ENGINE_VERSION}/${ONE_PLUGIN_NAME}/Source/${ONE_PLUGIN_NAME}/Public/one/ping
ONE_PLUGIN_SOURCE_THIRD_PARTY_DIR=${ONE_PLUGIN_REPO_DIR}/${ONE_UNREAL_ENGINE_VERSION}/${ONE_PLUGIN_NAME}/Source/ThirdParty/one/ping/internal
ONE_PLUGIN_TEMPLATE_DIR=${ONE_PLUGIN_REPO_DIR}/template/${ONE_PLUGIN_NAME}
ONE_PLUGIN_DOC_DIR=${ONE_PLUGIN_REPO_DIR}/docs
ONE_PLUGIN_DOC_VERSION=${ONE_PLUGIN_DOC_DIR}/sdk_version_client_${ONE_UNREAL_ENGINE_VERSION}.md
ONE_PLUGIN_UPLUGIN_FILE=${ONE_PLUGIN_DIR}/${ONE_PLUGIN_NAME}/${ONE_PLUGIN_NAME}.uplugin

mkdir -p ${ONE_PLUGIN_DIR}
mkdir -p ${ONE_PLUGIN_SOURCE_PRIVATE_DIR}
mkdir -p ${ONE_PLUGIN_SOURCE_PUBLIC_DIR}
mkdir -p ${ONE_PLUGIN_TEMPLATE_DIR}
mkdir -p ${ONE_PLUGIN_DOC_DIR}

# To avoid mv failed because directory is not empty.
if [ -d "${ONE_PLUGIN_SOURCE_THIRD_PARTY_DIR}" ]; then rm -Rf ${ONE_PLUGIN_SOURCE_THIRD_PARTY_DIR}; fi
mkdir -p ${ONE_PLUGIN_SOURCE_THIRD_PARTY_DIR}

# Copying the Plugin template file layout.
cp -r ${ONE_PLUGIN_TEMPLATE_DIR} ${ONE_PLUGIN_DIR}

# Updating the engine version in the config files.
sed -i s/ONE_ENGINE_VERSION/${ONE_UNREAL_ENGINE_VERSION}/g ${ONE_PLUGIN_UPLUGIN_FILE}

# Copying the sources and headers files from the ONEGameHostingSDK repository.

# Filtering cmake, markdown, internal & public headers.
# See https://git.i3d.net/one/ardentblue/one-game-sdk/-/blob/master/docs/integration_guide.md#using-the-arcus-server-api
# for more details.
ls ${ONE_SDK_SOURCE_DIR} \
    | grep -v internal | grep -v CMakeLists.txt | grep -v .md \
    | grep -v c_api.h | grep -v c_error.h | grep -v c_platform.h \
    | xargs -I args cp ${ONE_SDK_SOURCE_DIR}/args ${ONE_PLUGIN_SOURCE_PRIVATE_DIR}/args

# Copying the internal folder in a second step to avoid any uninteded filtering.
cp -r ${ONE_SDK_SOURCE_DIR}/internal ${ONE_PLUGIN_SOURCE_PRIVATE_DIR}
mv ${ONE_PLUGIN_SOURCE_PRIVATE_DIR}/internal/rapidjson ${ONE_PLUGIN_SOURCE_THIRD_PARTY_DIR}

# Copying the public headers
cp ${ONE_SDK_SOURCE_DIR}/c_api.h ${ONE_PLUGIN_SOURCE_PUBLIC_DIR}
cp ${ONE_SDK_SOURCE_DIR}/c_error.h ${ONE_PLUGIN_SOURCE_PUBLIC_DIR}
cp ${ONE_SDK_SOURCE_DIR}/c_platform.h ${ONE_PLUGIN_SOURCE_PUBLIC_DIR}

# Adding copyright notice
sed -i '1i// Copyright i3D.net, 2021. All Rights Reserved.' ${ONE_PLUGIN_SOURCE_PRIVATE_DIR}/*.h
sed -i '1i// Copyright i3D.net, 2021. All Rights Reserved.' ${ONE_PLUGIN_SOURCE_PRIVATE_DIR}/*.cpp
sed -i '1i// Copyright i3D.net, 2021. All Rights Reserved.' ${ONE_PLUGIN_SOURCE_PRIVATE_DIR}/internal/*.h
sed -i '1i// Copyright i3D.net, 2021. All Rights Reserved.' ${ONE_PLUGIN_SOURCE_PRIVATE_DIR}/internal/*.cpp
sed -i '1i// Copyright i3D.net, 2021. All Rights Reserved.' ${ONE_PLUGIN_SOURCE_PUBLIC_DIR}/*.h

# Update the documentation to the lastest SDK version.
ONE_GIT_HASH=`git --git-dir ${ONE_SDK_REPO_DIR}/.git rev-parse HEAD`

echo '# ONE GAME Client SDK Version' > ${ONE_PLUGIN_DOC_VERSION}
echo '' >> ${ONE_PLUGIN_DOC_VERSION}
echo "This plugin uses this [version](https://git.i3d.net/one/ardentblue/one-game-sdk/-/commit/${ONE_GIT_HASH}) of the One Platform SDK repository." >> ${ONE_PLUGIN_DOC_VERSION}
