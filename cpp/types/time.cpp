#include "gb32960.h"

namespace mj::gb32960 {
    Time::Time(const std::chrono::system_clock::time_point &time): BufferData(
        std::make_shared<std::string>(kTimeFieldSize, 0), 0, kTimeFieldSize) {
        const auto t = std::chrono::system_clock::to_time_t(time);
        auto tm = *std::localtime(&t);
        char *p = payload->data();
        p[0] = static_cast<char>(tm.tm_year - 100);
        p[1] = static_cast<char>(tm.tm_mon + 1);
        p[2] = static_cast<char>(tm.tm_mday);
        p[3] = static_cast<char>(tm.tm_hour);
        p[4] = static_cast<char>(tm.tm_min);
        p[5] = static_cast<char>(tm.tm_sec);
    }

    Time::Time(const std::shared_ptr<std::string> &data): Time(data, 0) {
    }

    Time::Time(const std::shared_ptr<std::string> &data, const size_t offset): BufferData(
        data, offset, kTimeFieldSize) {
    }

    int Time::year() const {
        return 100 + static_cast<int>(payload->c_str()[0]);
    }

    int Time::month() const {
        return payload->c_str()[1];
    }

    int Time::day() const {
        return payload->c_str()[2];
    }

    int Time::hour() const {
        return payload->c_str()[3];
    }

    int Time::minute() const {
        return payload->c_str()[4];
    }

    int Time::second() const {
        return payload->c_str()[5];
    }

    std::chrono::system_clock::time_point Time::system_clock() const {
        std::tm tm = {};
        tm.tm_year = year();
        tm.tm_mon = month() - 1;
        tm.tm_mday = day();
        tm.tm_hour = hour();
        tm.tm_min = minute();
        tm.tm_sec = second();
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }
}
