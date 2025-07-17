#pragma once

#include <iostream>

namespace peg
{
    class AutoFlushCout
    {
    public:
        template <typename T>
        AutoFlushCout& operator<<(const T& val)
        {
            std::cout << val << std::flush;
            return *this;
        }

        AutoFlushCout& operator<<(std::ostream& (*manip)(std::ostream&))
        {
            std::cout << manip << std::flush;
            return *this;
        }
    };

    inline AutoFlushCout cout;
}