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
    [[nodiscard]] constexpr size_t get_size() const noexcept
    {
        return m_size;
    }

    [[nodiscard]] constexpr size_t get_index() const noexcept
    {
        return m_index;
    }

    constexpr ByteStream& set_index(const size_t index)
    {
        if (index >= m_size || m_index < 0)
            throw new std::out_of_range("Index out of stream bounds");

        m_index = index;
        return *this;
    }

    ByteStream(size_t size);
    ~ByteStream();

    ByteStream& write_uint8(uint8_t val);
    uint8_t read_uint8();

    ByteStream& write_uint16(uint16_t val);
    uint16_t read_uint16();

    ByteStream& write_uint24(uint32_t val);
    uint32_t read_uint24();

    ByteStream& write_uint32(uint32_t val);
    uint32_t read_uint32();

    ByteStream& write_uint64(uint64_t val);
    uint64_t read_uint64();

    ByteStream& write_int8(int8_t val);
    int8_t read_int8();

    ByteStream& write_int16(int16_t val);
    int16_t read_int16();

    ByteStream& write_int32(int32_t val);
    int32_t read_int32();

    ByteStream& write_int64(int64_t val);
    int64_t read_int64();

    ByteStream& write_float32(float_t val);
    float_t read_float32();

    ByteStream& write_float64(double_t val);
    double_t read_float64();

    ByteStream& write_string(size_t bytes, const std::string& val);
    std::string read_string(size_t bytes);

    ByteStream& write_float(
        double_t value,
        double_t min,
        double_t max,
        uint8_t byte_count
    );
    double_t read_float(
        double_t min,
        double_t max,
        uint8_t byte_count
    );

    ByteStream& write_boolean_group(
        bool b0 = false,
        bool b1 = false,
        bool b2 = false,
        bool b3 = false,
        bool b4 = false,
        bool b5 = false,
        bool b6 = false,
        bool b7 = false
    );
    std::tuple<bool, bool, bool, bool, bool, bool, bool, bool> read_boolean_group();

    ByteStream& write_boolean_group2(
        bool b0 = false, bool b1 = false,
        bool b2 = false, bool b3 = false,
        bool b4 = false, bool b5 = false,
        bool b6 = false, bool b7 = false,
        bool b8 = false, bool b9 = false,
        bool bA = false, bool bB = false,
        bool bC = false, bool bD = false,
        bool bE = false, bool bF = false
    );
    std::tuple<
        bool, bool, bool, bool,
        bool, bool, bool, bool,
        bool, bool, bool, bool,
        bool, bool, bool, bool>
    read_boolean_group2();
};
