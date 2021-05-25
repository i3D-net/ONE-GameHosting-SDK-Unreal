// Copyright i3D.net, 2021. All Rights Reserved.

#pragma once

#include <one/server/one_parsing.h>

#include <functional>
#include <mutex>
#include <array>

//----------------------------------------------
// One SDK object types forward declarations

struct OneServer;
typedef OneServer *OneServerPtr;

struct OneArray;
typedef OneArray *OneArrayPtr;

struct OneObject;
typedef OneObject *OneObjectPtr;

namespace one_integration {

/// OneServerWrapper encapsulates the integration for the One Arcus Server and
/// provides a game interface that hides the One Arcus Server API implementation
/// from the game. Errors are handled directly in the implementation of the
/// wrapper.
///
/// This is provided as a potential copy-paste initial integration solution for
/// users that would like to hide the c-api under a game-specific C++ class, and
/// also as a complete example of an integration.
class OneServerWrapper final {
public:
    OneServerWrapper();
    OneServerWrapper(const OneServerWrapper &) = delete;
    OneServerWrapper &operator=(const OneServerWrapper &) = delete;
    ~OneServerWrapper();

    //------------
    // Life cycle.

    bool init(unsigned int port);
    void shutdown();

    // Must called often (e.g. each frame). Updates the Arcus Server, which
    // processes incoming and outgoing messages.
    void update(bool quiet);

    enum class Status {
        uninitialized = 0,
        initialized,
        waiting_for_client,
        handshake,
        ready,
        error,
        unknown
    };
    static const char *status_to_string(Status status);
    Status status() const;

    bool quiet() const;

    //---------------
    // Arcus setters.

    struct GameState {
        GameState() : players(0), max_players(0), name(), map(), mode(), version() {}

        int players;      // Game number of players.
        int max_players;  // Game max number of players.
        std::array<char, codec::value_max_size_null_terminated()> name;  // Server name.
        std::array<char, codec::value_max_size_null_terminated()> map;   // Game map.
        std::array<char, codec::value_max_size_null_terminated()> mode;  // Game mode.
        std::array<char, codec::value_max_size_null_terminated()>
            version;  // Game version.

        // Add extra custom game-specific properties here, if needed. Then
        // these need to be added to a OneObjectPtr in the implementation
        // and passed to the c_api. See the cpp file for an example.
    };

    // Set the game state to the current value. The wrapper uses this to send
    // the current state to the ONE Platform, when requested to do so.
    void set_game_state(const GameState &);

    // As defined in:
    // https://www.i3d.net/docs/one/odp/Game-Integration/Management-Protocol/Arcus-V2/request-response/#applicationinstance-set-status-request
    enum class ApplicationInstanceStatus { starting = 3, online = 4, allocated = 5 };

    // Sets the Arcus application instance status. The game server must set
    // the status to starting during initialization, online once the server is
    // ready for matchmaking. Allocated is optional when directed for allocation
    // by Arcus, depending on the matchmaking design and features used.
    void set_application_instance_status(ApplicationInstanceStatus status);

    //------------------------
    // Incoming Arcus Messages

    // Sets a callback that is triggered when the remote client has notified the
    // server that it must gracefully exit the entire process. A timeout in
    // seconds is passed to allow the server to wait for as much active gameplay
    // (e.g. an active match) as possible to finish before shutting down. If the
    // timeout expires before the process is closed, the process will be
    // terminated forcefully.
    // userdata may be nullptr.
    void set_soft_stop_callback(std::function<void(int timeout, void *userdata)> callback,
                                void *userdata);

    // Allows the game server to be notified of an incoming Allocated message.
    // The allocation request has a optional JSON body. The keys and values are definable
    // by the customer. The current values are matching the payload shown in the
    // documentation at:
    // https://www.i3d.net/docs/one/odp/Game-Integration/Management-Protocol/Arcus-V2/request-response/#allocated-request
    void set_allocated_callback(
        std::function<void(const OneArrayPtr data, void *userdata)> callback,
        void *userdata);

    // The metadate response has a payload as defined at:
    // https://www.i3d.net/docs/one/odp/Game-Integration/Management-Protocol/Arcus-V2/request-response/#meta-data-request
    void set_metadata_callback(
        std::function<void(const OneArrayPtr data, void *userdata)> callback,
        void *userdata);

    // Allows the game server to be notified of an incoming Host Information message.
    // The host information response has a payload as defined at:
    // https://www.i3d.net/docs/one/odp/Game-Integration/Management-Protocol/Arcus-V2/request-response/#host-information-response
    void set_host_information_callback(
        std::function<void(const OneObjectPtr data, void *userdata)> callback,
        void *userdata);

    // Allows the game server to be notified of an incoming Application
    // Instance Information message.
    // The application instance information response has a payload as defined at:
    // https://www.i3d.net/docs/one/odp/Game-Integration/Management-Protocol/Arcus-V2/request-response/#applicationinstance-information-response
    void set_application_instance_information_callback(
        std::function<void(const OneObjectPtr data, void *userdata)> callback,
        void *userdata);

private:
    // Callbacks potentially called by the arcus server.
    static void soft_stop(void *userdata, int timeout_seconds);
    static void allocated(void *userdata, void *allocated);
    static void metadata(void *userdata, void *metadata);
    static void host_information(void *userdata, void *information);
    static void application_instance_information(void *userdata, void *information);

    // The Arcus Server itself.
    mutable std::mutex _wrapper;
    OneServerPtr _server;
    bool _quiet;

    //--------------------------------------------------------------------------
    // Callbacks.

    std::function<void(int, void *)> _soft_stop_callback;
    void *_soft_stop_userdata;

    std::function<void(const OneArrayPtr, void *)> _allocated_callback;
    void *_allocated_userdata;

    std::function<void(const OneArrayPtr, void *)> _metadata_callback;
    void *_metadata_userdata;

    std::function<void(const OneObjectPtr, void *)> _host_information_callback;
    void *_host_information_userdata;

    std::function<void(const OneObjectPtr &, void *)>
        _application_instance_information_callback;
    void *_application_instance_information_userdata;
};

}  // namespace one_integration
