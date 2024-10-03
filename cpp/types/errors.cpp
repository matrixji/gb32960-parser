#include "gb32960.h"

namespace mj::gb32960 {
    Gb32960Error::Gb32960Error(const char *m): msg(m) {
    }

    const char *Gb32960Error::what() const noexcept {
        return msg.c_str();
    }

    InvalidData::InvalidData(const char *msg): Gb32960Error(msg) {
    }

    InvalidHeader::InvalidHeader(const char *msg): Gb32960Error(msg) {
    }

    InvalidTime::InvalidTime(const char *msg): Gb32960Error(msg) {
    }
}
