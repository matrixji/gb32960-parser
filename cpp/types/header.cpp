#include <vector>
#include <cstring>

#include "gb32960.h"

namespace mj::gb32960 {
    struct HeaderData {
        uint16_t magic;
        uint8_t command;
        uint8_t response_code;
        uint8_t vin[17];
        uint8_t encryption;
        uint16_t data_length;
    } __attribute__((packed));

    Header::Header(const std::shared_ptr<std::string> &data): Header(data, 0) {
    }

    Header::Header(const std::shared_ptr<std::string> &data,
                   const size_t offset): BufferData(data, offset, kHeaderSize) {
        if (data->size() < offset + sizeof(HeaderData)) {
            throw InvalidHeader("data size is too small");
        }
    }

    uint16_t Header::magic() const {
        return reinterpret_cast<HeaderData *>(payload->data() + offset)->magic;
    }

    Command Header::command() const {
        switch (const auto v = command_value(); v) {
            case 0x01:
                return Command::VehicleLogin;
            case 0x02:
                return Command::RealtimeData;
            case 0x03:
                return Command::ReissueData;
            case 0x04:
                return Command::VehicleLogout;
            case 0x05:
                return Command::PlatformLogin;
            case 0x06:
                return Command::PlatformLogout;
            default:
                return Command::Unknown;
        }
    }

    uint8_t Header::command_value() const {
        return reinterpret_cast<HeaderData *>(payload->data() + offset)->command;
    }

    ResponseCode Header::response_code() const {
        switch (const auto v = response_code_value(); v) {
            case 0x01:
                return ResponseCode::Success;
            case 0x02:
                return ResponseCode::Failure;
            case 0x03:
                return ResponseCode::DuplicateVin;
            case 0xFE:
                return ResponseCode::Command;
            default:
                return ResponseCode::Unknown;
        }
    }

    uint8_t Header::response_code_value() const {
        return reinterpret_cast<HeaderData *>(payload->data() + offset)->response_code;
    }

    EncryptionType Header::encryption() const {
        switch (const auto v = encryption_value(); v) {
            case 0x00:
                return EncryptionType::None;
            case 0x01:
                return EncryptionType::RSA;
            case 0x02:
                return EncryptionType::AES128;
            case 0xFE:
                return EncryptionType::Error;
            default:
                return EncryptionType::Unknown;
        }
    }

    uint8_t Header::encryption_value() const {
        return reinterpret_cast<HeaderData *>(payload->data() + offset)->encryption;
    }

    uint16_t Header::data_length() const {
        return reinterpret_cast<HeaderData *>(payload->data() + offset)->data_length;
    }

    std::string_view Header::vin() {
        if (vin_str.empty()) {
            const HeaderData *header_data = reinterpret_cast<HeaderData *>(payload->data() + offset);
            vin_str.assign(reinterpret_cast<const char *>(header_data->vin), 17);
        }
        return vin_str;
    }

    void Header::validate() const {
        if (magic() != 0x2323) {
            throw InvalidHeader("magic number is invalid");
        }

        if (const auto resp = response_code_value(); resp < 1 || (resp > 3 && resp != 0xfe)) {
            throw InvalidHeader("response code is invalid");
        }

        if (const auto enc = encryption_value(); enc < 1 || (enc > 3 && enc < 0xfe)) {
            throw InvalidHeader("response code is invalid");
        }

        const HeaderData *header_data = reinterpret_cast<HeaderData *>(payload->data() + offset);
        if (auto len = strnlen(reinterpret_cast<const char *>(header_data->vin), 17); len != 17) {
            throw InvalidHeader("vin code length is invalid");
        }
    }
}
