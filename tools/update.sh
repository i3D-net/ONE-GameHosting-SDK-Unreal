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

${ONE_PLUGIN_REPO_DIR}/tools/sdk_update_client_plugin.sh ${ONE_UNREAL_ENGINE_VERSION} ${ONE_SDK_REPO_DIR} ${ONE_PLUGIN_REPO_DIR}
${ONE_PLUGIN_REPO_DIR}/tools/sdk_update_server_plugin.sh ${ONE_UNREAL_ENGINE_VERSION} ${ONE_SDK_REPO_DIR} ${ONE_PLUGIN_REPO_DIR}
${ONE_PLUGIN_REPO_DIR}/tools/example_update_client.sh ${ONE_UNREAL_ENGINE_VERSION} ${ONE_PLUGIN_REPO_DIR}
${ONE_PLUGIN_REPO_DIR}/tools/example_update_server.sh ${ONE_UNREAL_ENGINE_VERSION} ${ONE_PLUGIN_REPO_DIR}
