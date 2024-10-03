#ifndef _gb32960_h_
#define _gb32960_h_

#include <chrono>
#include <cstdint>
#include <exception>
#include <memory>
#include <string>


namespace mj::gb32960 {
    constexpr size_t kHeaderSize = 24;
    constexpr size_t kTimeFieldSize = 6;


    /**
     * GB32960 Error
     */
    class Gb32960Error : public std::exception {
    public:
        explicit Gb32960Error(const char *msg);

        [[nodiscard]] const char *what() const noexcept override;

    private:
        std::string msg;
    };

    /**
     * Invalid GB32960 Data Error
     */
    class InvalidData final : public Gb32960Error {
    public:
        explicit InvalidData(const char *msg);
    };

    /**
     * Invalid GB32960 Header Error
     */
    class InvalidHeader final : public Gb32960Error {
    public:
        explicit InvalidHeader(const char *msg);
    };

    /**
     * Invalid GB32960 Time Error
     */
    class InvalidTime final : public Gb32960Error {
    public:
        explicit InvalidTime(const char *msg);
    };

    /**
     * Command
     */
    enum class Command : uint8_t {
        VehicleLogin = 0x01,
        RealtimeData = 0x02,
        ReissueData = 0x03,
        VehicleLogout = 0x04,
        PlatformLogin = 0x05,
        PlatformLogout = 0x06,
        Unknown = 0xff
    };

    /**
     * Response Code
     */
    enum class ResponseCode: uint8_t {
        Success = 0x01,
        Failure = 0x02,
        DuplicateVin = 0x03,
        Command = 0xFE,
        Unknown = 0xFF
    };

    /**
     * Encryption Type
     */
    enum class EncryptionType : uint8_t {
        None = 0x00,
        RSA = 0x01,
        AES128 = 0x02,
        Error = 0xFE,
        Unknown = 0xFF
    };

    class BufferData {
    public:
        BufferData(const std::shared_ptr<std::string> &data, size_t length);

        BufferData(const std::shared_ptr<std::string> &data, size_t offset, size_t length);

        BufferData(BufferData &) = delete;

        BufferData &operator=(BufferData &) = delete;

    protected:
        std::shared_ptr<std::string> payload;
        size_t offset;
        size_t length;
    };

    class Header : public BufferData {
    public:
        /**
         * construct the header from string data
         * @param data string data
         */
        explicit Header(const std::shared_ptr<std::string> &data);

        /**
         * construct the header from string data with offset
         * @param data string data
         * @param offset offset
         */
        Header(const std::shared_ptr<std::string> &data, size_t offset);

        /**
         * get the magic number, it should be 0x2323
         * @return magic number
         */
        [[nodiscard]] uint16_t magic() const;

        /**
         * get the command value
         * @return command value
         */
        [[nodiscard]] Command command() const;

        /**
         * get the command value
         * @return command value
         */
        [[nodiscard]] uint8_t command_value() const;

        /**
         * get the response code
         * @return response code
         */
        [[nodiscard]] ResponseCode response_code() const;

        /**
         * get the response code value
         * @return response code value
         */
        [[nodiscard]] uint8_t response_code_value() const;

        /**
         * get the vin
         * @return vin
         */
        std::string_view vin();

        /**
         * get the encryption method
         * @return encryption method
         */
        [[nodiscard]] EncryptionType encryption() const;

        /**
         * get the encryption method value
         * @return encryption method value
         */
        [[nodiscard]] uint8_t encryption_value() const;

        /**
         * get the data length
         * @return data length
         */
        [[nodiscard]] uint16_t data_length() const;

        /**
         * do validation, if the header is invalid, it will throw InvalidHeader exception
         */
        void validate() const;

    private:
        std::string vin_str;
    };


    class Time : public BufferData {
    public:
        /**
         * get the year
         */
        [[nodiscard]] int year() const;

        /**
         * get the month
         */
        [[nodiscard]] int month() const;

        /**
         * get the day
         */
        [[nodiscard]] int day() const;

        /**
         * get the hour
         */
        [[nodiscard]] int hour() const;

        /**
         * get the minute
         */
        [[nodiscard]] int minute() const;

        /**
         * get the second
         */
        [[nodiscard]] int second() const;

        /**
         * get system clock
         */
        [[nodiscard]] std::chrono::system_clock::time_point system_clock() const;

        /**
         * construct the time from system clock
         * @param time
         */
        explicit Time(const std::chrono::system_clock::time_point &time);

        /**
         * construct the time from string data
         * @param data string data
         * @param offset offset
         */
        Time(const std::shared_ptr<std::string> &data, size_t offset);

        /**
         * construct the time from string data
         * @param data string data
         */
        explicit Time(const std::shared_ptr<std::string> &data);
    };

    class Packet : public BufferData {
    public:
        explicit Packet(const std::shared_ptr<std::string> &data);

        Packet(const std::shared_ptr<std::string> &data, size_t offset);

        void validate() const;

        [[nodiscard]] const Header &header() const;

    private:
        const Header header_data;
        const BufferData payload;
        uint8_t bcc;
    };

    class VehicleLoginPacket : public Packet {
    };
};


#endif // _gb32960_h_
