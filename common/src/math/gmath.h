// you might think this means "Game Math" since this is inside a game engine code
//
// WRONG!
// Gay Math.

namespace GMath
{

template<typename T>
inline int Sign(T val)
{
    return (T(0) < val) - (val < T(0));
}
};
