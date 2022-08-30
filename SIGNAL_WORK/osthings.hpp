#include <string>
#include <ostream>

#ifndef __OSTHINGS__
#define __OSTHINGS__


namespace ostream {
    template<std::string const &>
    static std::ostream &           operator<<(std::ostream &, std::string const &);
    //template<std::ostream &>
    //static std::ostream &           operator<<(std::ostream &, std::ostream &);
    //template<std::ostream &(*)(std::ostream)>
    //static std::ostream &           operator<<(std::ostream &, std::ostream &(*)(std::ostream &));
}

#endif /* !__OSTHINGS__ */
