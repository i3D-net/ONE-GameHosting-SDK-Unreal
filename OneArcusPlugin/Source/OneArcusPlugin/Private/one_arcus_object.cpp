#include "one_arcus_object.h"

#include <one/arcus/c_api.h>

UOneArcusObject::UOneArcusObject() {
    one_object_create(&_object);
}

void UOneArcusObject::BeginDestroy() {
    one_object_destroy(_object);
    Super::BeginDestroy();
}

OneObjectPtr UOneArcusObject::object() const {
    return _object;
}
