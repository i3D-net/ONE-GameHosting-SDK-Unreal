// Copyright i3D.net, 2021. All Rights Reserved.
#include <one/arcus/error.h>

#include <one/arcus/allocator.h>

#include <unordered_map>

// Util that replaces the given symbol with a pair of it and its string
// representation.
#define ONE_SYMBOL_STRING_PAIR(e) e, #e

namespace i3d {
namespace one {

const char *error_text(OneError err) {
    static std::unordered_map<OneError, const char *> lookup = {
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_NONE)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_ARRAY_ALLOCATION_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_ARRAY_POSITION_OUT_OF_BOUNDS)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_ARRAY_WRONG_TYPE_IS_EXPECTING_ARRAY)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_ARRAY_WRONG_TYPE_IS_EXPECTING_BOOL)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_ARRAY_WRONG_TYPE_IS_EXPECTING_INT)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_ARRAY_WRONG_TYPE_IS_EXPECTING_OBJECT)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_ARRAY_WRONG_TYPE_IS_EXPECTING_STRING)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CLIENT_NOT_INITIALIZED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CODEC_HEADER_LENGTH_TOO_SMALL)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CODEC_HEADER_LENGTH_TOO_BIG)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CODEC_DATA_LENGTH_TOO_SMALL_FOR_HEADER)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CODEC_DATA_LENGTH_TOO_SMALL_FOR_PAYLOAD)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CODEC_EXPECTED_DATA_LENGTH_TOO_BIG)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CODEC_INVALID_MESSAGE_PAYLOAD_SIZE_TOO_BIG)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CODEC_INVALID_HEADER)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CODEC_TRYING_TO_ENCODE_UNSUPPORTED_OPCODE)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_UNINITIALIZED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_HANDSHAKE_TIMEOUT)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_HEALTH_TIMEOUT)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_HELLO_INVALID)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_HELLO_MESSAGE_HEADER_TOO_BIG)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_HELLO_MESSAGE_RECEIVE_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_HELLO_MESSAGE_REPLY_INVALID)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_HELLO_MESSAGE_SEND_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_HELLO_RECEIVE_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_HELLO_SEND_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_HELLO_TOO_BIG)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_HELLO_VERSION_MISMATCH)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_MESSAGE_RECEIVE_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_READ_TOO_BIG_FOR_STREAM)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_OUT_MESSAGE_TOO_BIG_FOR_STREAM)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_QUEUE_EMPTY)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_QUEUE_INSUFFICIENT_SPACE)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_OUTGOING_QUEUE_INSUFFICIENT_SPACE)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_INCOMING_QUEUE_INSUFFICIENT_SPACE)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_RECEIVE_BEFORE_SEND)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_RECEIVE_FAIL)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_SEND_FAIL)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_TRY_AGAIN)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_UNKNOWN_STATUS)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_UPDATE_AFTER_ERROR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_CONNECTION_UPDATE_READY_FAIL)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_MESSAGE_ALLOCATION_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_MESSAGE_CALLBACK_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_MESSAGE_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(
            ONE_ERROR_MESSAGE_OPCODE_NOT_MATCHING_EXPECTING_ALLOCATED)},
        {ONE_SYMBOL_STRING_PAIR(
            ONE_ERROR_MESSAGE_OPCODE_NOT_MATCHING_EXPECTING_LIVE_STATE)},
        {ONE_SYMBOL_STRING_PAIR(
            ONE_ERROR_MESSAGE_OPCODE_NOT_MATCHING_EXPECTING_METADATA)},
        {ONE_SYMBOL_STRING_PAIR(
            ONE_ERROR_MESSAGE_OPCODE_NOT_MATCHING_EXPECTING_SOFT_STOP)},
        {ONE_SYMBOL_STRING_PAIR(
            ONE_ERROR_MESSAGE_OPCODE_NOT_MATCHING_EXPECTING_HOST_INFORMATION)},
        {ONE_SYMBOL_STRING_PAIR(
            ONE_ERROR_MESSAGE_OPCODE_NOT_MATCHING_EXPECTING_APPLICATION_INSTANCE_INFORMATION)},
        {ONE_SYMBOL_STRING_PAIR(
            ONE_ERROR_MESSAGE_OPCODE_NOT_MATCHING_EXPECTING_APPLICATION_INSTANCE_STATUS)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_MESSAGE_OPCODE_NOT_SUPPORTED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_MESSAGE_OPCODE_PAYLOAD_NOT_EMPTY)},
        {ONE_SYMBOL_STRING_PAIR(
            ONE_ERROR_MESSAGE_OPCODE_NOT_MATCHING_EXPECTING_REVERSE_METADATA)},
        {ONE_SYMBOL_STRING_PAIR(
            ONE_ERROR_MESSAGE_OPCODE_NOT_MATCHING_EXPECTING_CUSTOM_COMMAND)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_OBJECT_ALLOCATION_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_OBJECT_KEY_NOT_FOUND)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_OBJECT_KEY_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_OBJECT_WRONG_TYPE_IS_EXPECTING_ARRAY)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_OBJECT_WRONG_TYPE_IS_EXPECTING_BOOL)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_OBJECT_WRONG_TYPE_IS_EXPECTING_INT)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_OBJECT_WRONG_TYPE_IS_EXPECTING_OBJECT)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_OBJECT_WRONG_TYPE_IS_EXPECTING_STRING)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_PAYLOAD_KEY_NOT_FOUND)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_PAYLOAD_KEY_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_PAYLOAD_PARSE_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_PAYLOAD_VAL_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_PAYLOAD_WRONG_TYPE_IS_EXPECTING_ARRAY)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_PAYLOAD_WRONG_TYPE_IS_EXPECTING_BOOL)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_PAYLOAD_WRONG_TYPE_IS_EXPECTING_INT)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_PAYLOAD_WRONG_TYPE_IS_EXPECTING_OBJECT)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_PAYLOAD_WRONG_TYPE_IS_EXPECTING_STRING)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SERVER_ALLOCATION_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SERVER_ALREADY_INITIALIZED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SERVER_ALREADY_LISTENING)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SERVER_RETRYING_LISTEN)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SERVER_CALLBACK_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SERVER_CONNECTION_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SERVER_CONNECTION_NOT_READY)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SERVER_MESSAGE_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SERVER_SOCKET_ALLOCATION_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SERVER_SOCKET_NOT_INITIALIZED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SERVER_SOCKET_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SERVER_OBJECT_ALLOCATION_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_ACCEPT_NON_BLOCKING_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_ACCEPT_UNINITIALIZED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_ADDRESS_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_AVAILABLE_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_BIND_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_CLOSE_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_CONNECT_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_CONNECT_NON_BLOCKING_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_CONNECT_UNINITIALIZED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_CREATE_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_LISTEN_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_LISTEN_NON_BLOCKING_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_RECEIVE_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_SELECT_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_SELECT_UNINITIALIZED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_SEND_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_SOCKET_OPTIONS_FAILED)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_SYSTEM_CLEANUP_FAIL)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_SOCKET_SYSTEM_INIT_FAIL)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_ARRAY_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_CALLBACK_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_CAPACITY_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_CODE_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_CONNECTION_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_DATA_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_DESTINATION_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_EMPTY_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_KEY_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_MAP_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_MESSAGE_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_MODE_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_NAME_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_OBJECT_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_RESULT_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_SERVER_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_SOCKET_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_SOURCE_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_STATUS_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_SIZE_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_VAL_IS_NULLPTR)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_VAL_SIZE_IS_TOO_SMALL)},
        {ONE_SYMBOL_STRING_PAIR(ONE_ERROR_VALIDATION_VERSION_IS_NULLPTR)}};
    auto it = lookup.find(err);
    if (it == lookup.end()) {
        return "";
    }
    return it->second;
}

}  // namespace one
}  // namespace i3d
