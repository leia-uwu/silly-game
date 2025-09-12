#include "bytestream.h"

ByteStream::ByteStream(const size_t size)
    : m_size(size)
    , m_owns_data(true)
{
    m_buffer = new uint8_t[size]();
}

ByteStream::ByteStream(uint8_t* buff, const size_t size)
    : m_size(size)
    , m_buffer(buff)
    , m_owns_data(false)
{
}

ByteStream::~ByteStream()
{
    if (m_owns_data) {
        delete[] m_buffer;
    }
}

ByteStream& ByteStream::writeUint8(const uint8_t val)
{
    if (m_index >= m_size)
        throw new std::out_of_range("Trying to write past the end of the stream");
    m_buffer[m_index++] = val;
    return *this;
}

uint8_t ByteStream::readUint8()
{
    if (m_index >= m_size)
        throw new std::out_of_range("Trying to read past the end of the stream");
    return m_buffer[m_index++];
}

ByteStream& ByteStream::writeUint16(const uint16_t val)
{
    writeUint8(val);
    writeUint8(val >> 8);
    return *this;
}

uint16_t ByteStream::readUint16()
{
    return readUint8()
        | (readUint8() << 8);
}

ByteStream& ByteStream::writeUint24(const uint32_t val)
{
    writeUint8(val);
    writeUint8(val >> 8);
    writeUint8(val >> 16);
    return *this;
}

uint32_t ByteStream::readUint24()
{
    return readUint8()
        | (readUint8() << 8)
        | (readUint8() << 16);
}

ByteStream& ByteStream::writeUint32(const uint32_t val)
{
    writeUint16(val);
    writeUint16(val >> 16);
    return *this;
}

uint32_t ByteStream::readUint32()
{
    return readUint16()
        | (readUint16() << 16);
}

ByteStream& ByteStream::writeUint64(const uint64_t val)
{
    writeUint32(val);
    writeUint32(val >> 32);
    return *this;
}

uint64_t ByteStream::readUint64()
{
    return readUint32()
        | ((uint64_t)readUint32() << 32);
}

ByteStream& ByteStream::writeInt8(const int8_t val)
{
    return writeUint8(val);
}

int8_t ByteStream::readInt8()
{
    return readUint8();
}

ByteStream& ByteStream::writeInt16(const int16_t val)
{
    return writeUint16(val);
}

int16_t ByteStream::readInt16()
{
    return readUint16();
}

ByteStream& ByteStream::writeInt32(const int32_t val)
{
    return writeUint32(val);
}

int32_t ByteStream::readInt32()
{
    return readUint32();
}

ByteStream& ByteStream::writeInt64(const int64_t val)
{
    return writeUint64(val);
}

int64_t ByteStream::readInt64()
{
    return readUint64();
}

ByteStream& ByteStream::writeFloat32(const float_t val)
{
    return writeUint32(std::bit_cast<uint32_t>(val));
}

float_t ByteStream::readFloat32()
{
    return std::bit_cast<float_t>(readUint32());
}

ByteStream& ByteStream::writeFloat64(const double_t val)
{
    return writeUint64(std::bit_cast<uint64_t>(val));
}

double_t ByteStream::readFloat64()
{
    return std::bit_cast<double_t>(readUint64());
}

ByteStream& ByteStream::writeString(size_t bytes, const std::string& val)
{
    if (bytes == 0)
        return *this;
    const char* const c_str = val.c_str();

    for (int i = 0; i < bytes; i++) {
        const char c = c_str[i];
        writeUint8(c);

        if (c == 0)
            break;
    }

    return *this;
}

std::string ByteStream::readString(const size_t bytes)
{
    if (bytes == 0)
        return "";

    std::string s{};
    uint8_t c;
    size_t i = 0;

    do {
        if ((c = readUint8()) == 0)
            break;

        s += c;
    } while (++i < bytes);

    return s;
}

ByteStream& ByteStream::writeFloat(
    const double_t value,
    const double_t min,
    const double_t max,
    const uint8_t byte_count
)
{
    const uint64_t range = (1L << (8L * byte_count)) - 1L;
    const double_t val = ((value - min) / (max - min)) * range + 0.5;

    switch (byte_count) {
    case 1:
        writeUint8(val);
        break;
    case 2:
        writeUint16(val);
        break;
    case 3:
        writeUint24(val);
        break;
    case 4:
        writeUint32(val);
        break;
    case 8:
        writeUint64(val);
        break;
    default:
        throw new std::invalid_argument("Invalid byte count (must be either 1, 2, 3, 4, or 8)");
        break;
    }

    return *this;
}

double_t ByteStream::readFloat(
    const double_t min,
    const double_t max,
    const uint8_t byte_count
)
{
    const uint64_t range = (1L << (8L * byte_count)) - 1L;
    uint64_t value;

    switch (byte_count) {
    case 1:
        value = readUint8();
        break;
    case 2:
        value = readUint16();
        break;
    case 3:
        value = readUint24();
        break;
    case 4:
        value = readUint32();
        break;
    case 8:
        value = readUint64();
        break;
    default:
        throw new std::invalid_argument("Invalid byte count (must be either 1, 2, 3, 4, or 8)");
        break;
    }

    return min + (max - min) * value / range;
}

ByteStream& ByteStream::writeBooleanGroup(
    const bool b0,
    const bool b1,
    const bool b2,
    const bool b3,
    const bool b4,
    const bool b5,
    const bool b6,
    const bool b7
)
{
    writeUint8(
        (b0 ? 1 : 0)
        + (b1 ? 2 : 0)
        + (b2 ? 4 : 0)
        + (b3 ? 8 : 0)
        + (b4 ? 16 : 0)
        + (b5 ? 32 : 0)
        + (b6 ? 64 : 0)
        + (b7 ? 128 : 0)
    );
    return *this;
}

std::tuple<bool, bool, bool, bool, bool, bool, bool, bool>
ByteStream::readBooleanGroup()
{
    const uint8_t packed_group = readUint8();
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

ByteStream& ByteStream::writeBooleanGroup2(
    const bool b0, const bool b1,
    const bool b2, const bool b3,
    const bool b4, const bool b5,
    const bool b6, const bool b7,
    const bool b8, const bool b9,
    const bool bA, const bool bB,
    const bool bC, const bool bD,
    const bool bE, const bool bF
)
{
    writeUint8(
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
    return *this;
}

std::tuple<
    bool, bool, bool, bool,
    bool, bool, bool, bool,
    bool, bool, bool, bool,
    bool, bool, bool, bool>
ByteStream::readBooleanGroup2()
{
    const uint8_t packed_group = readUint16();
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
