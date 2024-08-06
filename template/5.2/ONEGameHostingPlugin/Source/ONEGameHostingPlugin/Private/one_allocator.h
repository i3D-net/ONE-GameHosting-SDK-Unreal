// Copyright i3D.net, 2021. All Rights Reserved.

#pragma once

namespace one_integration {

// Provides example global allocation hooks used to override the allocations
// made within the ONE Game Hosting SDK.
namespace allocation {

void *alloc(unsigned int bytes) {
    return std::malloc(bytes);
}

void free(void *p) {
    std::free(p);
}

void *realloc(void *p, unsigned int bytes) {
    return std::realloc(p, bytes);
}

}  // namespace allocation

}  // namespace one_integration
