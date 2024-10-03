#include "gb32960.h"

namespace mj::gb32960 {
    BufferData::BufferData(const std::shared_ptr<std::string> &data, size_t length): BufferData(data, 0, length) {
    }

    BufferData::BufferData(const std::shared_ptr<std::string> &d,
                           const size_t off, size_t len): payload(d), offset(off), length(len) {
        if (payload->size() < offset + length) {
            throw InvalidData("data size is too small");
        }
    }
}
