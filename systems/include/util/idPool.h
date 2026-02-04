#include <list>
#include <stdexcept>
#include <type_traits>

template<typename T>
    requires(std::is_integral_v<T>)
class IdPool
{

public:
    IdPool(T maxID) : m_maxId(maxID)
    {
    }

    [[nodiscard]] T getId()
    {
        if (m_nextId > m_maxId) {
            if (m_freeList.empty()) {
                throw std::runtime_error("Ran out of IDs!");
            }
            T id = m_freeList.back();
            m_freeList.pop_back();
            return id;
        }

        return m_nextId++;
    }

    void giveId(T id)
    {
        if (id > m_maxId) {
            throw std::out_of_range("giveId: ID out of range");
        }
        m_freeList.push_front(id);
    }

    bool hasIdsLeft()
    {
        if (m_nextId > m_maxId) {
            return !m_freeList.empty();
        }
        return true;
    }

private:
    T m_nextId = 1;
    T m_maxId;

    std::list<T> m_freeList;
};
