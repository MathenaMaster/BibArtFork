#include <string>
#include <ostream>

#ifndef __OSTHINGS__
#define __OSTHINGS__


namespace ostream {
    template<std::string const &>
    static std::ostream &           operator<<(std::ostream &, std::string const &);
}

#endif /* !__OSTHINGS__ */
