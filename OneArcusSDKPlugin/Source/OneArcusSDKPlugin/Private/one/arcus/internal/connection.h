#pragma once

#include <functional>
#include <vector>

#include <one/arcus/error.h>
#include <one/arcus/platform.h>
#include <one/arcus/internal/accumulator.h>
#include <one/arcus/internal/health.h>
#include <one/arcus/internal/ring.h>
#include <one/arcus/internal/time.h>

namespace i3d {
namespace one {

namespace codec {
struct Header;
}
class Socket;
class Message;
template <typename T>
class RingBuffer;

namespace connection {

// Stream buffers sizes used to pump pending data from/to the connection's
// socket.
constexpr size_t stream_send_buffer_size() {
    return 1024 * 128;
}

constexpr size_t stream_receive_buffer_size() {
    return 1024 * 128;
}

}  // namespace connection

// Connection manages Arcus protocol communication between two TCP sockets.
class Connection final {
public:
    // A default that can be used for production.
    static constexpr size_t max_message_default = 48;
    static constexpr int handshake_timeout_seconds = 1;

    // Connection must be given an active socket. Socket errors encountered
    // during processing will be returned as errors, and it is the caller's
    // responsibilty to either destroy the Connection, or restore the Socket's
    // state for communication.
    // Creating the conneciton starts the handshake timeout.
    Connection(size_t max_messages_in, size_t max_messages_out);
    ~Connection() = default;

    // Init the connection with the given socket. The given socket should be
    // active. Must be called after construction and shutdown. Handshaking timers
    // start when init is called.
    void init(Socket &socket);

    // Clears Connection to construction state. Erases all pending incoming
    // and outgoing data. Unassigns the socket.
    void shutdown();

    // Marks this side of the connection as responsible for initiating the
    // handshaking process. Must be called from one side of the connection
    // only. Attempting to send a Message or any other data to other side of
    // the Connection before handshaking is complete results in an error.
    // Must be called after init, before updating.
    Error initiate_handshake();

    // Update process incoming and outgoing messges. It attempts to read
    // all incoming messages that are available. It attempts to send all
    // queued outgoing messages. Must be called after init.
    Error update();

    enum class Status {
        uninitialized,
        handshake_not_started,
        handshake_hello_received,
        handshake_hello_scheduled,
        handshake_hello_sent,
        ready,
        error
    };
    Status status() const;

    // Adds a Message to the outgoing message queue, and passes the message
    // back in a modifier function that allows the caller to configure the
    // queued message. If the outgoing message queue is full, then the
    // call fails with ONE_ERROR_INSUFFICIENT_SPACE and the queue is not
    // modified. Must be called after init.
    Error add_outgoing(const Message &message);

    // The number of incoming messages available for pop. Must be called after
    // init.
    Error incoming_count(unsigned int &count) const;

    // Removes a message from the incoming message queue, but before doing so
    // passes the message into the given callback for reading.
    // Returns ONE_ERROR_EMPTY if the incoming_count is
    // zero and there is no message to pop.
    // Note that some messages are internally consumed and do not show up in
    // the incoming count or here.
    // Must be called after init.
    Error remove_incoming(std::function<Error(const Message &message)> read_callback);

private:
    Connection() = delete;

    Error process_handshake();
    // Reads all available incoming messages from the socket and stores them in
    // the incoming message queue.
    Error process_incoming_messages();
    // Sends all outgoing messages in the queue as long as the socket is ready
    // for sending.
    Error process_outgoing_messages();

    Error process_health();

    // Message helpers.
    Error try_read_data_into_in_stream();
    Error try_read_message_from_in_stream(codec::Header &header, Message &message);

    // Handshake helpers.
    Error ensure_nothing_received();
    Error try_send_hello();  // The initial hello type.
    Error try_receive_hello();
    Error try_send_hello_message();  // Hello as a Message with opcode.
    Error try_receive_hello_message();

    Socket *_socket;
    Status _status;

    Accumulator _in_stream;
    Accumulator _out_stream;

    Ring<Message> _incoming_messages;
    Ring<Message> _outgoing_messages;

    IntervalTimer _handshake_timer;
    HealthChecker _health_checker;
};

}  // namespace one
}  // namespace i3d
