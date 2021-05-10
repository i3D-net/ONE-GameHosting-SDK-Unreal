// Copyright i3D.net, 2021. All Rights Reserved.

#pragma once

// Provides example global allocation hooks used to override the allocations
// made within the ONE Game Hosting SDK.
namespace allocation {

void *alloc(size_t bytes) {
    return std::malloc(bytes);
}

void free(void *p) {
    std::free(p);
}

void *realloc(void *p, size_t bytes) {
    return std::realloc(p, bytes);
}

}  // namespace allocation
