#include <thread>
#include "osthings.hpp"

template<std::string const &>
std::ostream & ostream::operator<<(std::ostream & hereOs, std::string const & following)
    {
        if (following.size() > 0) hereOs << following.c_str();
        return (hereOs);
    }


