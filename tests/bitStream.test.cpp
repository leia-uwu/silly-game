/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include <cstdint>
#include <doctest/doctest.h>
#include <numbers>

#include "fc/core/bitStream.h"

uint8_t BUFF[1024];

TEST_CASE("BitStream tests")
{
    BitStream bs{BUFF, sizeof(BUFF)};

    SUBCASE("Byte aligned integers")
    {
        bs.writeUint8(99);
        bs.writeUint16(999);
        bs.writeUint32(999999);

        bs.writeInt8(-99);
        bs.writeInt16(-999);
        bs.writeInt32(-9999);

        bs.setBitIndex(0);

        CHECK(bs.readUint8() == 99);
        CHECK(bs.readUint16() == 999);
        CHECK(bs.readUint32() == 999999);

        CHECK(bs.readInt8() == -99);
        CHECK(bs.readInt16() == -999);
        CHECK(bs.readInt32() == -9999);
    }

    SUBCASE("Byte unaligned integers")
    {
        bs.writeBits<uint8_t>(9, 5);
        bs.writeBits<uint16_t>(999, 11);
        bs.writeBits<int16_t>(-99, 13);
        bs.writeBits<uint32_t>(99999, 20);

        bs.setBitIndex(0);

        CHECK(bs.readBits<uint8_t>(5) == 9);
        CHECK(bs.readBits<uint16_t>(11) == 999);
        CHECK(bs.readBits<int16_t>(13) == -99);
        CHECK(bs.readBits<uint32_t>(20) == 99999);
    }

    SUBCASE("Floats and doubles")
    {
        bs.writeFloat32(99.99);
        bs.writeFloat32(std::numbers::pi_v<float>);
        bs.writeFloat32(-std::numbers::pi_v<float>);
        bs.writeFloat64(std::numbers::pi_v<double>);
        bs.writeFloat64(-std::numbers::pi_v<double>);

        bs.setBitIndex(0);

        CHECK(bs.readFloat32() == doctest::Approx(99.99));
        CHECK(bs.readFloat32() == std::numbers::pi_v<float>);
        CHECK(bs.readFloat32() == -std::numbers::pi_v<float>);
        CHECK(bs.readFloat64() == std::numbers::pi_v<double>);
        CHECK(bs.readFloat64() == -std::numbers::pi_v<double>);
    }

    SUBCASE("Clamped floats")
    {
        bs.writeFloat(9.9, 8, 10, 16);
        bs.writeFloat(-6.6, -10, 1, 16);

        bs.setBitIndex(0);

        CHECK(bs.readFloat(8, 10, 16) == doctest::Approx(9.9).epsilon(0.1));
        CHECK(bs.readFloat(-10, 1, 16) == doctest::Approx(-6.6).epsilon(0.1));
    }

    SUBCASE("Strings")
    {
        bs.writeString("Deers are cool🦌");
        bs.writeString("meow meow meow", 9);

        bs.setBitIndex(0);

        CHECK(bs.readString() == std::string("Deers are cool🦌"));
        CHECK(bs.readString(9) == std::string("meow meow"));

        bs.setBitIndex(0);
        bs.writeString("AAAAAAAAA", 5);
        CHECK(bs.byteIndex() == 5); // check it didn't write past the string max size
        bs.setBitIndex(0);
        CHECK(bs.readString(5) == std::string("AAAAA"));

        bs.setBitIndex(0);
        bs.writeString("meow");
        CHECK(bs.byteIndex() == 5); // string size + null terminator
        bs.setBitIndex(0);
        CHECK(bs.readString(5) == std::string("meow"));

        bs.setBitIndex(0);
        bs.writeString("meow", 6);
        CHECK(bs.byteIndex() == 5);
        bs.setBitIndex(0);
        CHECK(bs.readString(6) == std::string("meow"));
    }

    SUBCASE("Exceptions")
    {
        CHECK_THROWS_WITH_AS(bs.setBitIndex(9999), "setBitIndex: index out of range, Bit Size: 8192, index: 9999", std::out_of_range);

        bs.setBitIndex(bs.bitSize() - 32);
        bs.writeUint32(1);

        CHECK_THROWS_WITH_AS(bs.writeUint8(1), "writeBits: trying to write past the end of the buffer, buff size: 8192, buff index: 8192, bits to write: 8", std::out_of_range);
    }
}
