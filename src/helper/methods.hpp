#include <vector>

namespace peg
{
    template<typename T>
    bool in(const T& value, const std::vector<T>& vector)
    {
        for(const T& item: vector)
        {
            if(item == value)
            {
                return true;
            }
        }
        return false;
    }
}