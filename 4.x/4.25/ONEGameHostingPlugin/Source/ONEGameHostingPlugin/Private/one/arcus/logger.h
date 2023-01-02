// Copyright i3D.net, 2021. All Rights Reserved.
#pragma once

#include <functional>

#include <one/arcus/types.h>

namespace i3d {
namespace one {

// Must match OneLogLevel in c_api.h.
enum class LogLevel { Info = 0, Error };

// Simple logger class that needs to be given the actual logging callback.
class Logger final {
public:
    Logger() : _logFn(nullptr) {}
    Logger(std::function<void(void *userdata, LogLevel, const String &)> logFn, void *userdata) : _logFn(logFn), _userdata(userdata) {}

    void Log(LogLevel level, const String &message) const {
        if (_logFn == nullptr) return;
        _logFn(_userdata, level, message);
    };

private:
    std::function<void(void *, LogLevel, const String &)> _logFn;
    void *_userdata;
};

}  // namespace one
}  // namespace i3d