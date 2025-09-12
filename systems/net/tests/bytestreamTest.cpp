#include "../bytestream.h"
#include "systems/math/gmath.h"
#include <cassert>
#include <numbers>

// NOLINTNEXTLINE(bugprone-exception-escape)
int main()
{

    ByteStream bs{4096};

    // // i'm so funny!!!!
    bs.writeUint8(69);
    bs.writeUint16(6969);
    bs.writeUint32(42069);
    bs.writeUint64(42069999);

    bs.writeInt8(-69);
    bs.writeInt16(-6969);
    bs.writeInt32(-42069);
    bs.writeInt64(-42069999);

    bs.writeFloat64(std::numbers::pi);

    bs.writeFloat(-5, -10, 10, 2);
    bs.writeFloat(5, 0, 10, 2);

    bs.writeBooleans(
        2,
        {
            true,
            false,
            true,
            true,
            false,
            false,
            true,
            false,
            true,
            false
        }
    );

    bs.setIndex(0);

    assert(bs.readUint8() == 69);
    assert(bs.readUint16() == 6969);
    assert(bs.readUint32() == 42069);
    assert(bs.readUint64() == 42069999);

    assert(bs.readInt8() == -69);
    assert(bs.readInt16() == -6969);
    assert(bs.readInt32() == -42069);
    assert(bs.readInt64() == -42069999);

    assert(bs.readFloat64() == std::numbers::pi);

    assert(GMath::EqAbs(bs.readFloat(-10, 10, 2), -5, 0.01));
    assert(GMath::EqAbs(bs.readFloat(0, 10, 2), 5, 0.01));

    auto bools = bs.readBooleans(2);
    assert(bools[0] == true);
    assert(bools[1] == false);
    assert(bools[2] == true);
    assert(bools[3] == true);
    assert(bools[4] == false);
    assert(bools[5] == false);
    assert(bools[6] == true);
    assert(bools[7] == false);
    assert(bools[8] == true);
    assert(bools[9] == false);

    return 0;
}
