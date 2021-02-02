#include "one_arcus_array.h"

#include <one/arcus/c_api.h>

UOneArcusArray::UOneArcusArray() {
    one_array_create(&_array);
}

void UOneArcusArray::BeginDestroy() {
    one_array_destroy(_array);
    Super::BeginDestroy();
}

OneArrayPtr UOneArcusArray::array() const {
    return _array;
}
