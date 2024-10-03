#include "gb32960.h"

namespace mj::gb32960 {
    Packet::Packet(const std::shared_ptr<std::string> &data): Packet(data, 0) {
    }

    Packet::Packet(const std::shared_ptr<std::string> &data,
                   size_t offset): BufferData(data, offset, 0),
                                   header_data(data, offset),
                                   payload(data, offset + kHeaderSize, data->size() - offset - kHeaderSize - 1),
                                   bcc(*data->rbegin()) {
    }

    const Header &Packet::header() const {
        return header_data;
    }

    void Packet::validate() const {
        header_data.validate();
    }
}
