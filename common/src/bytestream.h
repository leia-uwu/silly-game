#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <string>

class ByteStream
{
private:
    size_t m_size;
    uint8_t* m_buffer;
    size_t m_index = 0;

public:
    inline constexpr size_t get_size() const noexcept
    {
        return m_size;
    }

    inline constexpr size_t get_index() const noexcept
    {
        return m_index;
    }

    inline constexpr ByteStream* set_index(const size_t index)
    {
        if (index >= this->m_size || this->m_index < 0)
            throw new std::out_of_range("Index out of stream bounds");

        this->m_index = index;
        return this;
    }

    ByteStream(const size_t size);
    ~ByteStream();

    ByteStream* write_uint8(const uint8_t val);
    uint8_t read_uint8();

    ByteStream* write_uint16(const uint16_t val);
    uint16_t read_uint16();

    ByteStream* write_uint24(const uint32_t val);
    uint32_t read_uint24();

    ByteStream* write_uint32(const uint32_t val);
    uint32_t read_uint32();

    ByteStream* write_uint64(const uint64_t val);
    uint64_t read_uint64();

    ByteStream* write_int8(const int8_t val);
    int8_t read_int8();

    ByteStream* write_int16(const int16_t val);
    int16_t read_int16();

    ByteStream* write_int32(const int32_t val);
    int32_t read_int32();

    ByteStream* write_int64(const int64_t val);
    int64_t read_int64();

    ByteStream* write_float32(const float_t val);
    float_t read_float32();

    ByteStream* write_float64(const double_t val);
    double_t read_float64();

    ByteStream* write_string(const size_t& bytes, const std::string& val);
    std::string read_string(const size_t bytes);

    ByteStream* write_float(
        const double_t value,
        const double_t min,
        const double_t max,
        const uint8_t byte_count
    );
    double_t read_float(
        const double_t min,
        const double_t max,
        const uint8_t byte_count
    );

    ByteStream* write_boolean_group(
        const bool& b0 = false,
        const bool& b1 = false,
        const bool& b2 = false,
        const bool& b3 = false,
        const bool& b4 = false,
        const bool& b5 = false,
        const bool& b6 = false,
        const bool& b7 = false
    );
    std::tuple<bool, bool, bool, bool, bool, bool, bool, bool> read_boolean_group();

    ByteStream* write_boolean_group2(
        const bool& b0 = false, const bool& b1 = false,
        const bool& b2 = false, const bool& b3 = false,
        const bool& b4 = false, const bool& b5 = false,
        const bool& b6 = false, const bool& b7 = false,
        const bool& b8 = false, const bool& b9 = false,
        const bool& bA = false, const bool& bB = false,
        const bool& bC = false, const bool& bD = false,
        const bool& bE = false, const bool& bF = false
    );
    std::tuple<
        bool, bool, bool, bool,
        bool, bool, bool, bool,
        bool, bool, bool, bool,
        bool, bool, bool, bool>
    read_boolean_group2();
};
