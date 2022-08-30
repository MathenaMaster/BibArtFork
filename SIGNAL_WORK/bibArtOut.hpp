#include <ostream>
#include <iostream>
#include "osthings.hpp"

#ifndef __ARTOUT__
#define __ARTOUT__

class ArtOut : public std::ostream
{
    protected:
        std::ostream        & inos;
    public:
        ArtOut(std::ostream &);
        std::ostream &      GetInos();
        std::ostream &      operator*();
};

std::ostream & operator<<(std::ostream &, ArtOut &);

extern ArtOut bibOut;
extern ArtOut bibErr;

#endif /* !__ARTOUT__*/