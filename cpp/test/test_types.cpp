#include <gtest/gtest.h>

#include "gb32960.h"


class TestHeader : public ::testing::Test {
};

TEST_F(TestHeader, test_header_success) {
    auto data = std::make_shared<std::string>(
        "\x23\x23"
        "\x01\x01"
        "VIN12345671234567"
        "\x01"
        "\x12\x12"
    );
    mj::gb32960::Header header(data);
    EXPECT_EQ(header.magic(), 0x2323);
    EXPECT_EQ(header.command_value(), 0x01);
    EXPECT_EQ(header.response_code_value(), 0x01);
    EXPECT_EQ(header.vin(), "VIN12345671234567");
    EXPECT_EQ(header.encryption_value(), 0x01);
    EXPECT_EQ(header.data_length(), 0x1212);
    header.validate();
}

TEST_F(TestHeader, test_header_too_small) {
    auto data = std::make_shared<std::string>(
        "\x23\x23"
        "\x01\x01"
        "VIN12345671234567"
        "\x01"
    );
    EXPECT_THROW(mj::gb32960::Header header(data), mj::gb32960::InvalidData);
}

TEST_F(TestHeader, test_header_invalid_magic) {
    auto data = std::make_shared<std::string>(
        "\x23\x24"
        "\x01\x01"
        "VIN12345671234567"
        "\x01"
        "\x12\x12"
    );
    mj::gb32960::Header header(data);
    EXPECT_THROW(header.validate(), mj::gb32960::InvalidHeader);
}

class TestHeaderWithResponseCode : public ::testing::TestWithParam<std::pair<uint8_t, bool> > {
};

TEST_P(TestHeaderWithResponseCode, testing) {
    auto data = std::make_shared<std::string>(
        "\x23\x23"
        "\x01\x01"
        "VIN12345671234567"
        "\x01"
        "\x12\x12"
    );
    auto [response_code, success] = GetParam();
    mj::gb32960::Header header(data);
    header.validate();

    (*data)[3] = static_cast<char>(response_code);
    if (success) {
        header.validate();
    } else {
        EXPECT_THROW(header.validate(), mj::gb32960::InvalidHeader);
    }
}

class TestHeaderWithEncryption : public ::testing::TestWithParam<std::pair<uint8_t, bool> > {
};

TEST_P(TestHeaderWithEncryption, testing) {
    auto data = std::make_shared<std::string>(
        "\x23\x23"
        "\x01\x01"
        "VIN12345671234567"
        "\x01"
        "\x12\x12"
    );
    auto [enc, success] = GetParam();
    mj::gb32960::Header header(data);
    header.validate();

    (*data)[21] = static_cast<char>(enc);
    if (success) {
        header.validate();
    } else {
        EXPECT_THROW(header.validate(), mj::gb32960::InvalidHeader);
    }
}

INSTANTIATE_TEST_SUITE_P(
    TestHeaderWithResponseCode,
    TestHeaderWithResponseCode,
    ::testing::Values(
        std::make_pair(0x00, false),
        std::make_pair(0x01, true),
        std::make_pair(0x02, true),
        std::make_pair(0x03, true),
        std::make_pair(0x04, false),
        std::make_pair(0x0fd, false),
        std::make_pair(0x0fe, true),
        std::make_pair(0x0ff, false)
    )
);

INSTANTIATE_TEST_SUITE_P(
    TestHeaderWithEncryption,
    TestHeaderWithEncryption,
    ::testing::Values(
        std::make_pair(0x00, false),
        std::make_pair(0x01, true),
        std::make_pair(0x02, true),
        std::make_pair(0x03, true),
        std::make_pair(0x04, false),
        std::make_pair(0x0fd, false),
        std::make_pair(0x0fe, true),
        std::make_pair(0x0ff, true)
    )
);

