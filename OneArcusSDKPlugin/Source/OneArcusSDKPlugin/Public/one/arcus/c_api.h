// Copyright i3D.net, 2021. All Rights Reserved.
#pragma once

#include <cstddef>

/** @file c_api.h
    @brief The C API for Arcus Server.

    This is the externally facing interface for the ONE Game Hosting SDK, exposing
    management an Arcus Server to allow communication with the ONE Platform.

    It is a C API for ABI compatibility and widespread language binding support.

    The basic use pattern to support a game server integration is:
    1. Create a server and update it.
    2. Use the outgoing and incoming message functions like
       one_server_send_live_state and one_server_set_allocated_callback
       to send and receive the supported Arcus Messages, which includes providing
       the high level game state to the Arcus platform.
    3. If needed, use the Message API functions like one_message_val_int to read
    key/value pair details from the more complex messages.
    4. Properly cleanup the server as needed.

    NOTE ON THREAD SAFETY:
    > Working with server objects returned from one_server_create is thread safe.
    > Other functions like one_allocator_set_alloc, or working with message
    > object data instances like OneArrayPtr or OneObjectPtr are not thread safe
    > as these should be called either in the game's main init/shutdown code path
    > or while handling specific isolated functionality on a thread.
    >
    > Thread safe functions are identified in the function declaration comments.

    NOTE ON MULTIPLE SERVERS:
    > This Api is designed to allow for multiple game servers to be managed
    > on a single process. Each game server would need its own One Arcus Server,
    > created via one_server_create. Please check with One Documentation or
    > customer support for backend support.
*/

#include <one/arcus/c_platform.h>

#include <one/arcus/c_error.h>

#ifdef __cplusplus
extern "C" {
#endif

// See the c_error.h file for error values.

///
/// Status of a One Arcus Server.
typedef enum OneServerStatus {
    ONE_SERVER_STATUS_UNINITIALIZED = 0,
    ONE_SERVER_STATUS_INITIALIZED,
    ONE_SERVER_STATUS_WAITING_FOR_CLIENT,
    ONE_SERVER_STATUS_HANDSHAKE,
    ONE_SERVER_STATUS_READY,
    ONE_SERVER_STATUS_ERROR
} OneServerStatus;

/// Status of a game server. As defined in:
/// https://www.i3d.net/docs/one/odp/Game-Integration/Management-Protocol/Arcus-V2/request-response/#applicationinstance-set-status-request
typedef enum OneApplicationInstanceStatus {
    ONE_SERVER_STARTING = 3,
    ONE_SERVER_ONLINE = 4,
    ONE_SERVER_ALLOCATED = 5
} OneeApplicationInstanceStatus;

//------------------------------------------------------------------------------
///@name Opaque types.
/// The API uses the pointer handles to represent internal objects.
///@{

/// Opaque type and handle to a One Arcus Server.
struct OneServer;
typedef OneServer *OneServerPtr;

/// Opaque type and handle to a One Array value used in messages.
struct OneArray;
typedef OneArray *OneArrayPtr;

// Opaque type and handle to a One Object value used in messages.
struct OneObject;
typedef OneObject *OneObjectPtr;

//------------------------------------------------------------------------------
///@}
///@name Environment.
///@{

/// Optional custom memory alloc override.
/// If set, must be set at init time, before using any other APIs. If this is called,
/// then one_allocator_set_free must also be called. If this is called, then
/// one_allocator_set_free and one_allocator_set_realloc must also be called.
/// @param callback A function that takes a size in bytes of memory to allocate.
/// @sa one_allocator_set_free
/// @sa one_allocator_set_realloc
ONE_EXPORT void one_allocator_set_alloc(void *(*callback)(unsigned int size));

/// Optional custom memory free override.
/// If set, must be set at init time, before using any other APIs.
/// @param callback An existing pointer p to free.
ONE_EXPORT void one_allocator_set_free(void (*callback)(void *));

/// Optional custom memory realloc override.
/// If set, must be set at init time, before using any other APIs.
/// @param callback A function taking an existing pointer and a new size. See
/// the standard c realloc requirements for behavior.
ONE_EXPORT void one_allocator_set_realloc(void *(*callback)(void *, unsigned int size));

//------------------------------------------------------------------------------
///@}
///@name Server interface.
/// The Server is the main object of the C API. A single server should be created
/// per "game server". The server needs to be updated often to send and receive
/// messages from an Arcus Client.
///@{

/// Logging level that is passed to the optional log callback function.
/// \sa one_server_create
typedef enum OneLogLevel { ONE_LOG_LEVEL_INFO = 0, ONE_LOG_LEVEL_ERROR } OneLogLevel;

/// Creates a new Arcus Server. Each Game Server must have one corresponding
/// Arcus Server. Listen, update, shutdown and destroy should be called to complete the
/// life cycle. Thread-safe. A pointer to the OneServerPtr is passed in and the
/// pointer will be set to newly created OneServerPtr.
/// Use Example:
///     OneServerPtr server;
///     auto err = one_server_create(&server);
///     if (one_is_error(err)) {
///         myLogger.log(one_error_text(err));
///     }
///     // Use the server, e.g. one_server_update, send/receive messages.
///     one_server_destroy(server);
/// @param port The port to bind to and listen on for incoming Client connections.
/// @param server A null server pointer, which will be set to a new server.
/// \sa one_server_destroy
/// \sa one_server_update
/// \sa one_server_status
ONE_EXPORT OneError one_server_create(unsigned int port, OneServerPtr *server);

/// Log callback function to allow the integration to handle internal ONE Server
/// logs with its own logger.
/// @param level The severity of the logged information.
/// @param message The actual message.
/// \sa one_server_create
typedef void (*OneLogFn)(void *userdata, OneLogLevel level, const char *message);

/// Sets a custom logger that can handle logs from inside of the server. By
/// default the server will log to standard out.
/// @param server A non-null server pointer.
/// @param log_cb Optional log callback function. Can be null.
/// @param userdata Optional user data that will be passed back to the callback.
ONE_EXPORT OneError one_server_set_logger(OneServerPtr server, OneLogFn log_cb,
                                          void *userdata);

/// Destroys a server instance created via one_server_create. Destroy will
/// shutdown the server first, if it is active. Note although other server functions
/// are thread safe, this one is not. A server must not be destroyed or interacted
/// with on other threads.
/// @param server A non-null server pointer.
ONE_EXPORT void one_server_destroy(OneServerPtr server);

/// Update the server. This must be called frequently (e.g. each frame) to
/// process incoming and outgoing communications. Incoming messages trigger
/// their respective incoming callbacks during the call to update. If the
/// callback for a message is not set then the message is ignored.
/// If binding to the listen port fails then either ONE_ERROR_SOCKET_BIND_FAILED
/// or ONE_ERROR_SERVER_RETRYING_LISTEN will be returned. The server in this
/// case will periodically retry binding the listen port during update.
/// @param server A non-null server pointer. Thread-safe.
ONE_EXPORT OneError one_server_update(OneServerPtr server);

/// Obtains the status of the server. Thread-safe. The passed in pointer is set
/// to the status value.
/// @param server A non-null server pointer.
/// @param status A pointer to a status enum value to be set.
ONE_EXPORT OneError one_server_status(OneServerPtr const server, OneServerStatus *status);

//------------------------------------------------------------------------------
///@}
///@name Array main interface
///@{

/// Creates a new array. Must be freed with one_array_destroy. Thread-safe.
/// @param array A null pointer to a OneArrayPtr, to be set with new array.
ONE_EXPORT OneError one_array_create(OneArrayPtr *array);

/// Destroys array.
/// @param array A non-null OneArrayPtr, to be deleted.
ONE_EXPORT void one_array_destroy(OneArrayPtr array);

/// Makes a copy of the array. The destination must have been created via
/// one_array_create.
ONE_EXPORT OneError one_array_copy(OneArrayPtr source, OneArrayPtr destination);

/// Clears the array to an empty initialized state.
/// @param array A non-null OneArrayPtr.
ONE_EXPORT OneError one_array_clear(OneArrayPtr array);

/// Reserves array space.
/// @param array A non-null OneArrayPtr  .
/// @param size Number of total elements the array should contain.
ONE_EXPORT OneError one_array_reserve(OneArrayPtr array, unsigned int size);

/// Sets the given value to true if the array is empty.
/// @param array A non-null OneArrayPtr.
/// @param empty A non-null bool pointer to set the result on.
ONE_EXPORT OneError one_array_is_empty(OneArrayPtr array, bool *empty);

/// Returns the number of elements pushed to the array.
/// @param array A non-null OneArrayPtr.
/// @param size A non-null unsigned int pointer to set the result on.
ONE_EXPORT OneError one_array_size(OneArrayPtr array, unsigned int *size);

/// Returns the total size, allocated via one_array_reserve, of the array.
/// @param array A non-null OneArrayPtr.
/// @param capacity A non-null unsigned int pointer to set the result on.
ONE_EXPORT OneError one_array_capacity(OneArrayPtr array, unsigned int *capacity);

//------------------------------------------------------------------------------
///@}
///@name Array element addition and removal
///@{

/// Adds an element value of the given type to the back of the array. The array
/// must have sufficient free space, that is the capacity must be greater than
/// the size.
/// @param array A pointer that will be set to point to the new OneArrayPtr.
/// @param val The value to add as new element.
ONE_EXPORT OneError one_array_push_back_bool(OneArrayPtr array, bool val);
ONE_EXPORT OneError one_array_push_back_int(OneArrayPtr array, int val);
ONE_EXPORT OneError one_array_push_back_string(OneArrayPtr array, const char *val);
ONE_EXPORT OneError one_array_push_back_array(OneArrayPtr array, OneArrayPtr val);
ONE_EXPORT OneError one_array_push_back_object(OneArrayPtr array, OneObjectPtr val);

/// Removes last element of the array, if any.
/// @param array A pointer that will be set to point to the new OneArrayPtr.
ONE_EXPORT OneError one_array_pop_back(OneArrayPtr array);

//------------------------------------------------------------------------------
///@}
///@name Array element value type checkers
///@{

/// Checks whether the given key is of the given type.
/// @param array A pointer that will be set to point to the new OneArrayPtr.
/// @param pos The element index to inspect.
/// @param result A non-null bool pointer to set the result to.
/// @return May return ONE_ERROR_ARRAY_* or ONE_ERROR_NONE.
ONE_EXPORT OneError one_array_is_val_bool(OneArrayPtr array, unsigned int pos,
                                          bool *result);
ONE_EXPORT OneError one_array_is_val_int(OneArrayPtr array, unsigned int pos,
                                         bool *result);
ONE_EXPORT OneError one_array_is_val_string(OneArrayPtr array, unsigned int pos,
                                            bool *result);
ONE_EXPORT OneError one_array_is_val_array(OneArrayPtr array, unsigned int pos,
                                           bool *result);
ONE_EXPORT OneError one_array_is_val_object(OneArrayPtr array, unsigned int pos,
                                            bool *result);

//------------------------------------------------------------------------------
///@}
///@name Array Getters
///@{

/// Retrieves and sets the value of the given type from the array. The given value
/// pointer must be non-null and will have the return value set on it.
/// @return May return of ONE_ERROR_ARRAY_*.
/// @param array A valid array created via one_array_create.
/// @param pos The index of the value to retrieve. Must be less than one_array_size.
/// @param val A non-null pointer to set the value on.
ONE_EXPORT OneError one_array_val_bool(OneArrayPtr array, unsigned int pos, bool *val);
ONE_EXPORT OneError one_array_val_int(OneArrayPtr array, unsigned int pos, int *val);
/// Returns the number of characters in the string. This does not include a trailing null
/// character.
/// @return May return of ONE_ERROR_ARRAY_*.
/// @param array A valid array created via one_array_create.
/// @param pos The index of the value to retrieve. Must be less than one_array_size.
/// @param size A non-null unsigned int pointer to set the size on.
ONE_EXPORT OneError one_array_val_string_size(OneArrayPtr array, unsigned int pos,
                                              unsigned int *size);
/// Writes the key value to the given character buffer.
/// @return May return of ONE_ERROR_ARRAY_*.
/// @param array A valid array created via one_array_create.
/// @param pos The index of the value to retrieve. Must be less than one_array_size.
/// @param val A non-null pointer to set the value on. In the case of OneArrayPtr
/// or OneObjectPtr, these objects must have been created previously via their
/// respective creation functions and subsequently destroyed when no longer
/// needed.
/// @param size Size of the value buffer that can be written to. Must be equal
/// to size obtained via one_array_val_string_size.
ONE_EXPORT OneError one_array_val_string(OneArrayPtr array, unsigned int pos, char *val,
                                         unsigned int size);
ONE_EXPORT OneError one_array_val_array(OneArrayPtr array, unsigned int pos,
                                        OneArrayPtr val);
ONE_EXPORT OneError one_array_val_object(OneArrayPtr array, unsigned int pos,
                                         OneObjectPtr val);

//------------------------------------------------------------------------------
///@}
///@name Array Setters
///@{

/// Allows setting a sub key/value pair on the array of the given type.
/// @param array A valid object created via one_array_create.
/// @param pos The position in the array to set the value in. Must be less than
/// one_array_size.
/// @param val The value to set.
ONE_EXPORT OneError one_array_set_val_bool(OneArrayPtr array, unsigned int pos, bool val);
ONE_EXPORT OneError one_array_set_val_int(OneArrayPtr array, unsigned int pos, int val);
ONE_EXPORT OneError one_array_set_val_string(OneArrayPtr array, unsigned int pos,
                                             const char *val);
ONE_EXPORT OneError one_array_set_val_array(OneArrayPtr array, unsigned int pos,
                                            OneArrayPtr val);
ONE_EXPORT OneError one_array_set_val_object(OneArrayPtr array, unsigned int pos,
                                             OneObjectPtr val);

//------------------------------------------------------------------------------
///@}
///@name Object main interface
///@{

/// Create a new object that can be used as a key value in a One protocol message.
/// one_object_destroy must be called to free the object. Thread-safe.
/// @param object A pointer that will be set to point to the new OneObjectPtr.
ONE_EXPORT OneError one_object_create(OneObjectPtr *object);

/// Must be called to free an object created by one_object_create.
/// @param object A non-null object pointer created via one_object_create.
ONE_EXPORT void one_object_destroy(OneObjectPtr object);

/// Makes a copy of the object. The destination must have been created via
/// one_object_create.
ONE_EXPORT OneError one_object_copy(OneObjectPtr source, OneObjectPtr destination);

//------------------------------------------------------------------------------
///@}
///@name Object Key Value type checkers
///@{

/// Checks whether the given key is of the given type.
/// @param object A pointer that will be set to point to the new OneObjectPtr.
/// @param key The key to lookup.
/// @param result A non-null bool pointer to set the result to.
/// @return May return ONE_ERROR_OBJECT_* or ONE_ERROR_NONE.
ONE_EXPORT OneError one_object_is_val_bool(OneObjectPtr object, const char *key,
                                           bool *result);
ONE_EXPORT OneError one_object_is_val_int(OneObjectPtr object, const char *key,
                                          bool *result);
ONE_EXPORT OneError one_object_is_val_string(OneObjectPtr object, const char *key,
                                             bool *result);
ONE_EXPORT OneError one_object_is_val_array(OneObjectPtr object, const char *key,
                                            bool *result);
ONE_EXPORT OneError one_object_is_val_object(OneObjectPtr object, const char *key,
                                             bool *result);

//------------------------------------------------------------------------------
///@}
///@name Object Getters
///@{

/// Retrieves and sets the value of the given type from the object. The given value
/// pointer must be non-null and will have the return value set on it.
/// @return May return of ONE_ERROR_OBJECT_*.
/// @param object A valid object created via one_object_create.
/// @param key The key of the value to return.
/// @param val Non-null pointer to set the value on.
ONE_EXPORT OneError one_object_val_bool(OneObjectPtr object, const char *key, bool *val);
ONE_EXPORT OneError one_object_val_int(OneObjectPtr object, const char *key, int *val);
/// Returns the number of characters in the string. This does not include a trailing null
/// character.
/// @return May return of ONE_ERROR_OBJECT_*.
/// @param object A valid object created via one_object_create.
/// @param key The key of the value to return.
/// @param size A non-null unsigned int pointer to set the size on.
ONE_EXPORT OneError one_object_val_string_size(OneObjectPtr object, const char *key,
                                               unsigned int *size);
/// Writes the key value to the given character buffer.
/// @return May return of ONE_ERROR_OBJECT_*.
/// @param object A valid object created via one_object_create.
/// @param key The key of the value to return.
/// @param val Non-null pointer to set the value on. In the case of OneArrayPtr
/// or OneObjectPtr, these objects must have been created previously via their
/// respective creation functions and subsequently destroyed when no longer
/// needed.
/// @param size Size of the value buffer that can be written to. Must be equal
/// to size obtained via one_object_val_string_size.
ONE_EXPORT OneError one_object_val_string(OneObjectPtr object, const char *key, char *val,
                                          unsigned int size);
ONE_EXPORT OneError one_object_val_array(OneObjectPtr object, const char *key,
                                         OneArrayPtr val);
ONE_EXPORT OneError one_object_val_object(OneObjectPtr object, const char *key,
                                          OneObjectPtr val);

//------------------------------------------------------------------------------
///@}
///@name Object Setters
///@{

/// Allows setting a sub key/value pair on the object of the given type.
/// @param object A valid object created via one_object_create.
/// @param key The key of the value to return.
/// @param val The value to set.
ONE_EXPORT OneError one_object_set_val_bool(OneObjectPtr object, const char *key,
                                            bool val);
ONE_EXPORT OneError one_object_set_val_int(OneObjectPtr object, const char *key, int val);
ONE_EXPORT OneError one_object_set_val_string(OneObjectPtr object, const char *key,
                                              const char *val);
ONE_EXPORT OneError one_object_set_val_array(OneObjectPtr object, const char *key,
                                             OneArrayPtr val);
ONE_EXPORT OneError one_object_set_val_object(OneObjectPtr object, const char *key,
                                              OneObjectPtr val);

//------------------------------------------------------------------------------
///@}
///@name Arcus outgoing property setters.
/// See the [One Arcus protocol documentation
/// website](https://www.i3d.net/docs/one/odp/Game-Integration/Management-Protocol/Arcus-V2/request-response/)
/// for more information.
///@{

/// Set the live game state information about the game server. This should be
/// called at the least when the state changes, but it is safe to call more
/// often if it is more convenient to do so - data is only sent out if there are
/// changes from the previous call. Thread-safe.
/// @param server A non-null server pointer.
/// @param players Current player count.
/// @param max_players Max player count allowed in current match.
/// @param name Friendly server name.
/// @param map Actively hosted map.
/// @param mode Actively hosted game mode.
/// @param version The version of the game software.
/// @param additional_data Any key/value pairs set on this object will be added.
ONE_EXPORT OneError one_server_set_live_state(OneServerPtr server, int players,
                                              int max_players, const char *name,
                                              const char *map, const char *mode,
                                              const char *version,
                                              OneObjectPtr additional_data);

/// This should be called at the least when the state changes, but it is safe to
/// call more often if it is more convenient to do so - data is only sent out if
/// there are changes from the previous call. Thread-safe.
/// @param server A non-null server pointer.
/// @param status The current status of the game server application instance.
ONE_EXPORT OneError one_server_set_application_instance_status(
    OneServerPtr server, OneApplicationInstanceStatus status);

//------------------------------------------------------------------------------
///@}
///@name Arcus incoming message handlers.
/// See the [One Arcus protocol documentation
/// website](https://www.i3d.net/docs/one/odp/Game-Integration/Management-Protocol/Arcus-V2/request-response/)
/// for more information.
///@{

/// Registers a callback to be called when a soft stop message is received. The
/// process should stop at its earliest convenience. If the server process is
/// still active after the given timeout (seconds), then One will terminate the
/// process directly. Thread-safe.
/// @param server Non-null server pointer.
/// @param callback Callback to be called during a call to one_server_update, if
///                 the message is received from the Client.
/// @param userdata Optional user data that will be passed back to the callback.
ONE_EXPORT OneError one_server_set_soft_stop_callback(
    OneServerPtr server, void (*callback)(void *userdata, int timeout), void *userdata);

/// Register the callback to be called when an allocated message is received.
/// The game server must read the given array data and be ready to accept
/// players in the directed gameplay environment (e.g. map, mode), if specified
/// and required by the game's ONE Platform configuration.
/// @param server Non-null server pointer. Thread-safe.
/// @param callback Callback to be called during a call to one_server_update, if
///                 the message is received from the Client.
/// @param userdata Optional user data that will be passed back to the callback.
ONE_EXPORT OneError one_server_set_allocated_callback(
    OneServerPtr server, void (*callback)(void *userdata, void *array), void *userdata);

/// Register the callback to be notified of a metadata. Thread-safe.
/// The `void *array` will be of type OneArrayPtr or the callback will error out.
/// @param server Non-null server pointer.
/// @param callback Callback to be called during a call to one_server_update, if
///                 the message is received from the Client.
/// @param userdata Optional user data that will be passed back to the callback.
ONE_EXPORT OneError one_server_set_metadata_callback(
    OneServerPtr server, void (*callback)(void *userdata, void *array), void *userdata);

/// Register the callback to be notified of host_information. Thread-safe.
/// The `void *object` will be of type OneObjectPtr or the callback will error out.
/// @param server Non-null server pointer.
/// @param callback Callback to be called during a call to one_server_update, if
///                 the message is received from the Client.
/// @param userdata Optional user data that will be passed back to the callback.
ONE_EXPORT OneError one_server_set_host_information_callback(
    OneServerPtr server, void (*callback)(void *userdata, void *object), void *userdata);

/// Register the callback to be notified of a application_instance_information.
/// Thread-safe.
/// The `void *object` will be of type OneObjectPtr or the callback will error out.
/// @param server Non-null server pointer.
/// @param callback Callback to be called during a call to one_server_update, if
///                 the message is received from the Client.
/// @param userdata Optional user data that will be passed back to the callback.
ONE_EXPORT OneError one_server_set_application_instance_information_callback(
    OneServerPtr server, void (*callback)(void *userdata, void *object), void *userdata);

///@}

#ifdef __cplusplus
};
#endif
