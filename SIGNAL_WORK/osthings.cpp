#include <thread>
#include "osthings.hpp"


template<std::string const &>
std::ostream & ostream::operator<<(std::ostream & hereOs, std::string const & following)
    {
        hereOs << following.c_str();
        return (hereOs);
    }

/*
template<std::ostream &>
std::ostream &  ostream::operator<<(std::ostream & hereOs, std::ostream & following)
    {
        synchronized {
            hereOs << following.rdbuf();
        }
        return (hereOs);
    }

template<std::ostream &(*)(std::ostream &)>
std::ostream &  ostream::operator<<(std::ostream & hereOs, std::ostream & (*following)(std::ostream &))
    {
        synchronized {
            hereOs.operator<<(following);
        }
        return (hereOs);
    }
*/