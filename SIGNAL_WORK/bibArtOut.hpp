#include <ostream>
//#include <iostream>
#include <string>
#include <semaphore>
#include "osthings.hpp"

#ifndef __ARTOUT__
#define __ARTOUT__

class ArtOut : public std::ostream
{
    private:
        std::ostream                        &   inos;
        std::counting_semaphore<1>              stream_sem;
    public:
        ArtOut(std::ostream & inos);
        std::ostream &      GetInos();
        std::ostream &      operator*();
        std::string         endl();
        std::string         flush();

};

extern ArtOut bibOut;
extern ArtOut bibErr;

#endif /* !__ARTOUT__*/