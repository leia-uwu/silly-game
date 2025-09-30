#pragma once

#include <cmath>
#include <cstdint>
#include <format>
#include <stdexcept>
#include <string>
#include <vector>

class ByteStream
{
private:
    size_t m_size;
    uint8_t* m_buffer;
    size_t m_index = 0;

    bool m_owns_data;

public:
    [[nodiscard]] constexpr size_t size() const noexcept
    {
        return m_size;
    }

    [[nodiscard]] constexpr size_t index() const noexcept
    {
        return m_index;
    }

    constexpr ByteStream& setIndex(const size_t index)
    {
        if (index >= m_size || m_index < 0)
            throw new std::out_of_range(
                std::format("Index out of stream bounds, index: {}, size: {}", index, m_size)
            );

        m_index = index;
        return *this;
    }

    ByteStream(size_t size);
    ByteStream(uint8_t* buff, size_t size);
    ~ByteStream();

    ByteStream& writeUint8(uint8_t val);
    uint8_t readUint8();

    ByteStream& writeUint16(uint16_t val);
    uint16_t readUint16();

    ByteStream& writeUint24(uint32_t val);
    uint32_t readUint24();

    ByteStream& writeUint32(uint32_t val);
    uint32_t readUint32();

    ByteStream& writeUint64(uint64_t val);
    uint64_t readUint64();

    ByteStream& writeInt8(int8_t val);
    int8_t readInt8();

    ByteStream& writeInt16(int16_t val);
    int16_t readInt16();

    ByteStream& writeInt32(int32_t val);
    int32_t readInt32();

    ByteStream& writeInt64(int64_t val);
    int64_t readInt64();

    ByteStream& writeFloat32(float_t val);
    float_t readFloat32();

    ByteStream& writeFloat64(double_t val);
    double_t readFloat64();

    ByteStream& writeString(size_t bytes, const std::string& val);
    std::string readString(size_t bytes);

    ByteStream& writeBytes(
        uint64_t value,
        uint8_t byteCount
    );

    uint64_t readBytes(
        uint8_t byteCount
    );

    ByteStream& writeFloat(
        double_t value,
        double_t min,
        double_t max,
        uint8_t byteCount
    );
    double_t readFloat(
        double_t min,
        double_t max,
        uint8_t byteCount
    );

    ByteStream& writeBooleans(uint8_t byteCount, const std::vector<bool>& bools);

    std::vector<bool> readBooleans(uint8_t byteCount);
};
