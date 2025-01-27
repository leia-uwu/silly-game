#include <bit>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include "bytestream.h"

ByteStream::ByteStream(size_t size)
    : m_size(size)
{
    m_buffer = (uint8_t*)malloc(size);
    memset(m_buffer, 0, size);
}

ByteStream::~ByteStream()
{
    free(m_buffer);
}

ByteStream* ByteStream::write_uint8(const uint8_t& val)
{
    if (this->m_index == m_size)
        throw new std::out_of_range("Trying to write past the end of the stream");
    this->m_buffer[this->m_index++] = val;
    return this;
}

uint8_t ByteStream::read_uint8()
{
    if (this->m_index == m_size)
        throw new std::out_of_range("Trying to read past the end of the stream");
    return this->m_buffer[this->m_index++];
}

ByteStream* ByteStream::write_uint16(const uint16_t& val)
{
    this->write_uint8(val)
        ->write_uint8(val >> 8);
    return this;
}

uint16_t ByteStream::read_uint16()
{
    return this->read_uint8()
        | (this->read_uint8() << 8);
}

ByteStream* ByteStream::write_uint24(const uint32_t& val)
{
    this->write_uint8(val)
        ->write_uint8(val >> 8)
        ->write_uint8(val >> 16);
    return this;
}

uint32_t ByteStream::read_uint24()
{
    return this->read_uint8()
        | (this->read_uint8() << 8)
        | (this->read_uint8() << 16);
}

ByteStream* ByteStream::write_uint32(const uint32_t& val)
{
    this->write_uint16(val)
        ->write_uint16(val >> 16);
    return this;
}

uint32_t ByteStream::read_uint32()
{
    return this->read_uint16()
        | (this->read_uint16() << 16);
}

ByteStream* ByteStream::write_uint64(const uint64_t& val)
{
    this->write_uint32(val)
        ->write_uint32(val >> 32);
    return this;
}

uint64_t ByteStream::read_uint64()
{
    return this->read_uint32()
        | ((uint64_t)this->read_uint32() << 32);
}

ByteStream* ByteStream::write_int8(const int8_t& val)
{
    return this->write_uint8(val);
}

int8_t ByteStream::read_int8()
{
    return (uint8_t)this->read_uint8();
}

ByteStream* ByteStream::write_int16(const int16_t& val)
{
    return this->write_uint16(val);
}

int16_t ByteStream::read_int16()
{
    return (uint16_t)this->read_uint16();
}

ByteStream* ByteStream::write_int32(const int32_t& val)
{
    return this->write_uint32(val);
}

int32_t ByteStream::read_int32()
{
    return (uint32_t)this->read_uint32();
}

ByteStream* ByteStream::write_int64(const int64_t& val)
{
    return this->write_uint64(val);
}

int64_t ByteStream::read_int64()
{
    return (uint64_t)this->read_uint64();
}

ByteStream* ByteStream::write_float32(const float_t& val)
{
    return this->write_uint32(std::bit_cast<uint32_t>(val));
}

float_t ByteStream::read_float32()
{
    return std::bit_cast<float_t>(this->read_uint32());
}

ByteStream* ByteStream::write_float64(const double_t& val)
{
    return this->write_uint64(std::bit_cast<uint64_t>(val));
}

double_t ByteStream::read_float64()
{
    return std::bit_cast<double_t>(this->read_uint64());
}

ByteStream* ByteStream::write_string(const size_t& bytes, const std::string& val)
{
    if (bytes == 0)
        return this;
    const char* const c_str = val.c_str();

    for (int i = 0; i < bytes; i++) {
        const char c = c_str[i];
        this->write_uint8(c);

        if (c == 0)
            break;
    }

    return this;
}

std::string ByteStream::read_string(const size_t& bytes)
{
    if (bytes == 0)
        return "";

    std::string s{};
    uint8_t c;
    size_t i = 0;

    do {
        if ((c = this->read_uint8()) == 0)
            break;

        s += c;
    } while (++i < bytes);

    return s;
}

ByteStream* ByteStream::write_float(
    const double_t& value,
    const double_t& min,
    const double_t& max,
    const uint8_t& byte_count
)
{
    const uint64_t range = (1L << (8L * byte_count)) - 1L;
    const double_t val = ((value - min) / (max - min)) * range + 0.5;

    switch (byte_count) {
    case 1:
        this->write_uint8(val);
        break;
    case 2:
        this->write_uint16(val);
        break;
    case 3:
        this->write_uint24(val);
        break;
    case 4:
        this->write_uint32(val);
        break;
    case 8:
        this->write_uint64(val);
        break;
    default:
        throw new std::invalid_argument("Invalid byte count (must be either 1, 2, 3, 4, or 8)");
        break;
    }

    return this;
}

double_t ByteStream::read_float(
    const double_t& min,
    const double_t& max,
    const uint8_t& byte_count
)
{
    const uint64_t range = (1L << (8L * byte_count)) - 1L;
    uint64_t value;

    switch (byte_count) {
    case 1:
        value = this->read_uint8();
        break;
    case 2:
        value = this->read_uint16();
        break;
    case 3:
        value = this->read_uint24();
        break;
    case 4:
        value = this->read_uint32();
        break;
    case 8:
        value = this->read_uint64();
        break;
    default:
        throw new std::invalid_argument("Invalid byte count (must be either 1, 2, 3, 4, or 8)");
        break;
    }

    return min + (max - min) * value / range;
}

ByteStream* ByteStream::write_boolean_group(
    const bool& b0,
    const bool& b1,
    const bool& b2,
    const bool& b3,
    const bool& b4,
    const bool& b5,
    const bool& b6,
    const bool& b7
)
{
    this->write_uint8(
        (b0 ? 1 : 0)
        + (b1 ? 2 : 0)
        + (b2 ? 4 : 0)
        + (b3 ? 8 : 0)
        + (b4 ? 16 : 0)
        + (b5 ? 32 : 0)
        + (b6 ? 64 : 0)
        + (b7 ? 128 : 0)
    );
    return this;
}

std::tuple<bool, bool, bool, bool, bool, bool, bool, bool>
ByteStream::read_boolean_group()
{
    const uint8_t packed_group = this->read_uint8();
    return {
        (packed_group & 1) != 0,
        (packed_group & 2) != 0,
        (packed_group & 4) != 0,
        (packed_group & 8) != 0,
        (packed_group & 16) != 0,
        (packed_group & 32) != 0,
        (packed_group & 64) != 0,
        (packed_group & 128) != 0
    };
}

ByteStream* ByteStream::write_boolean_group2(
    const bool& b0, const bool& b1,
    const bool& b2, const bool& b3,
    const bool& b4, const bool& b5,
    const bool& b6, const bool& b7,
    const bool& b8, const bool& b9,
    const bool& bA, const bool& bB,
    const bool& bC, const bool& bD,
    const bool& bE, const bool& bF
)
{
    this->write_uint8(
        (b0 ? 1 : 0)
        + (b1 ? 2 : 0)
        + (b2 ? 4 : 0)
        + (b3 ? 8 : 0)
        + (b4 ? 16 : 0)
        + (b5 ? 32 : 0)
        + (b6 ? 64 : 0)
        + (b7 ? 128 : 0)
        + (b8 ? 256 : 0)
        + (b9 ? 512 : 0)
        + (bA ? 1024 : 0)
        + (bB ? 2048 : 0)
        + (bC ? 4096 : 0)
        + (bD ? 8192 : 0)
        + (bE ? 16384 : 0)
        + (bF ? 32768 : 0)
    );
    return this;
}

std::tuple<
    bool, bool, bool, bool,
    bool, bool, bool, bool,
    bool, bool, bool, bool,
    bool, bool, bool, bool>
ByteStream::read_boolean_group2()
{
    const uint8_t packed_group = this->read_uint16();
    return {
        (packed_group & 1) != 0,
        (packed_group & 2) != 0,
        (packed_group & 4) != 0,
        (packed_group & 8) != 0,
        (packed_group & 16) != 0,
        (packed_group & 32) != 0,
        (packed_group & 64) != 0,
        (packed_group & 128) != 0,
        (packed_group & 256) != 0,
        (packed_group & 512) != 0,
        (packed_group & 1024) != 0,
        (packed_group & 2048) != 0,
        (packed_group & 4096) != 0,
        (packed_group & 8192) != 0,
        (packed_group & 16384) != 0,
        (packed_group & 32768) != 0
    };
}
