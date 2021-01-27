#include <one/game/parsing.h>

#include <stdio.h>

#include <CoreMinimal.h>

namespace one_integration {

bool Parsing::extract_key_value_payload(
    const OneArrayPtr array, std::function<bool(const size_t,
                           const std::array<char, codec::key_max_size_null_terminated()> &,
                           const std::array<char, codec::value_max_size_null_terminated()> &)>
                                 callback) {
    if (callback == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: callback is null"));
        return false;
    }

    if (array == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: array is null"));
        return false;
    }

    bool empty = true;
    auto err = one_array_is_empty(array, &empty);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    if (empty) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: array is empty"));
        return false;
    }

    unsigned number_of_keys = 0;
    err = one_array_size(array, &number_of_keys);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    OneObjectPtr pair = nullptr;
    err = one_object_create(&pair);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    for (unsigned int pos = 0; pos < number_of_keys; ++pos) {
        err = one_array_val_object(array, pos, pair);
        if (one_is_error(err)) {
            UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
            one_object_destroy(pair);
            return false;
        }

        if (!extract_key_value_pair(pair, _key, _value)) {
            UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
            one_object_destroy(pair);
            return false;
        }

        if (!callback(number_of_keys, _key, _value)) {
            UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: callback unable to extract key value pair"));
            one_object_destroy(pair);
            return false;
        }
    }

    one_object_destroy(pair);
    return true;
}

bool Parsing::extract_key_value_pair(
    const OneObjectPtr pair, std::array<char, codec::key_max_size_null_terminated()> &key,
    std::array<char, codec::value_max_size_null_terminated()> &value) {
    if (pair == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: pair is null"));
        return false;
    }

    unsigned int key_size = 0;
    auto err = one_object_val_string_size(pair, "key", &key_size);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    // Because buffer must add the '\0' explicitly.
    if (codec::key_max_size() < key_size + 1) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: key size is bigger than max key size"));
        return false;
    }

    err = one_object_val_string(pair, "key", key.data(), static_cast<unsigned int>(codec::key_max_size()));
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    // Ensure that the string is `\0` terminated.
    _key[key_size] = '\0';

    unsigned int value_size = 0;
    err = one_object_val_string_size(pair, "value", &value_size);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    // Because buffer must add the '\0' explicitly.
    if (codec::value_max_size() < value_size + 1) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: value size is bigger than max value size"));
        return false;
    }

    err = one_object_val_string(pair, "value", value.data(), static_cast<unsigned int>(codec::value_max_size()));
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    // Ensure that the string is `\0` terminated.
    _value[value_size] = '\0';
    return true;
}

bool Parsing::extract_string(const OneObjectPtr object, const char *key,
                             std::function<bool(const std::array<char, codec::value_max_size_null_terminated()>&)> callback) {
    if (object == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: object is null"));
        return false;
    }

    if (key == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: key is null"));
        return false;
    }

    unsigned int size = 0;
    auto err = one_object_val_string_size(object, key, &size);
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    // Because buffer must add the '\0' explicitly.
    if (_string_buffer.size() < size + 1) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: string buffer size is too small"));
        return false;
    }

    err =
        one_object_val_string(object, key, _string_buffer.data(), static_cast<unsigned int>(_string_buffer.size()));
    if (one_is_error(err)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: %s"), *FString(one_error_text(err)));
        return false;
    }

    // Ensure that the string is `\0` terminated.
    _string_buffer[size] = '\0';

    if (!callback(_string_buffer)) {
        UE_LOG(LogTemp, Error, TEXT("ONE ARCUS: callback unable to extract pair"));
        return false;
    }

    return true;
}

std::array<char, codec::key_max_size_null_terminated()> Parsing::_key = {};
std::array<char, codec::value_max_size_null_terminated()> Parsing::_value = {};
std::array<char, codec::string_buffer_null_terminated()> Parsing::_string_buffer = {};

}  // namespace one_integration
