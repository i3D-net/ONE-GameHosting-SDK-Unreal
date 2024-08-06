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

ONE_UNREAL_TEMPLATE=${1}
ONE_UNREAL_ENGINE_VERSION=${2}
ONE_PLUGIN_REPO_DIR=${3}

ONE_PLUGIN_NAME=ONEGameHostingPlugin
ONE_EXAMPLE_NAME=ArcusExample

ONE_TEMPLATE_EXAMPLE_DIR=${ONE_PLUGIN_REPO_DIR}/template/${ONE_UNREAL_TEMPLATE}/${ONE_EXAMPLE_NAME}

ONE_PLUGIN_DIR=${ONE_PLUGIN_REPO_DIR}/${ONE_UNREAL_TEMPLATE}/${ONE_UNREAL_ENGINE_VERSION}/${ONE_PLUGIN_NAME}
ONE_UPLUGIN_FILE=${ONE_PLUGIN_DIR}/${ONE_PLUGIN_NAME}.uplugin

ONE_TARGET_DIR=${ONE_PLUGIN_REPO_DIR}/${ONE_UNREAL_TEMPLATE}/${ONE_UNREAL_ENGINE_VERSION}
ONE_TARGET_EXAMPLE_DIR=${ONE_TARGET_DIR}/${ONE_EXAMPLE_NAME}
ONE_UPROJECT_FILE=${ONE_TARGET_EXAMPLE_DIR}/${ONE_EXAMPLE_NAME}.uproject

mkdir -p ${ONE_TARGET_DIR}
mkdir -p ${ONE_TARGET_EXAMPLE_DIR}

# Copying the template example and the template plugin in the target directory.
cp -r ${ONE_TEMPLATE_EXAMPLE_DIR} ${ONE_TARGET_DIR}

sed -i s/ONE_ENGINE_VERSION/${ONE_UNREAL_ENGINE_VERSION}/g ${ONE_UPLUGIN_FILE}
sed -i s/ONE_ENGINE_VERSION/${ONE_UNREAL_ENGINE_VERSION}/g ${ONE_UPROJECT_FILE}
