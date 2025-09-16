#include "bytestream.h"
#include <cstdint>

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

ByteStream& ByteStream::writeBytes(uint64_t value, uint8_t byteCount)
{
    switch (byteCount) {
    case 1:
        return writeUint8(value);
    case 2:
        return writeUint16(value);
    case 3:
        return writeUint24(value);
    case 4:
        return writeUint32(value);
    case 8:
        return writeUint64(value);
    default:
        throw new std::invalid_argument("Invalid byte count (must be either 1, 2, 3, 4, or 8)");
    }
};

uint64_t ByteStream::readBytes(uint8_t byteCount)
{
    switch (byteCount) {
    case 1:
        return readUint8();
    case 2:
        return readUint16();
    case 3:
        return readUint24();
    case 4:
        return readUint32();
    case 8:
        return readUint64();
    default:
        throw new std::invalid_argument("Invalid byte count (must be either 1, 2, 3, 4, or 8)");
    }
};

ByteStream& ByteStream::writeFloat(
    const double_t value,
    const double_t min,
    const double_t max,
    const uint8_t byteCount
)
{
    if (value < min || value > max) {
        throw new std::out_of_range(
            std::format("writeFloat: Value out of range, received {}, range: [{}, {}]", value, min, max)
        );
    }
    const uint64_t range = (1 << (8 * byteCount)) - 1;

    const double_t t = (value - min) / (max - min);
    const double_t val = t * range + 0.5;
    writeBytes(val, byteCount);

    return *this;
}

double_t ByteStream::readFloat(
    const double_t min,
    const double_t max,
    const uint8_t byteCount
)
{
    const uint64_t range = (1 << (8 * byteCount)) - 1;
    double_t value = readBytes(byteCount);

    return min + (max - min) * value / range;
}

ByteStream& ByteStream::writeBooleans(uint8_t byteCount, const std::vector<bool>& bools)
{
    const uint64_t size = byteCount * 8L;
    if (bools.size() > size) {
        throw new std::out_of_range(std::format("Can only write {} booleans, received {}", size, bools.size()));
    }

    uint64_t val = 0;

    for (size_t i = 0; i < bools.size(); i++) {
        if (bools[i]) {
            val |= 1 << i;
        }
    }

    writeBytes(val, byteCount);

    return *this;
};

std::vector<bool> ByteStream::readBooleans(uint8_t byteCount)
{
    const uint64_t size = byteCount * 8L;

    std::vector<bool> bools{};
    bools.resize(size);

    const uint64_t value = readBytes(byteCount);

    for (size_t i = 0; i < size; i++) {
        bools[i] = (value & (1 << i)) != 0;
    }

    return bools;
};
