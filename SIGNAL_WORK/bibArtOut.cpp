#include <ostream>
#include <thread>
#include "osthings.hpp"
#include "bibArtOut.hpp"

ArtOut::ArtOut(std::ostream & copy): inos(copy) //: ostream(copy)
    {
        //std::ostream(copy);
        //*this = copy;
        //link(copy);
    }

[[optimize_for_synchronized]]
std::ostream &            ArtOut::operator*()
    {
        synchronized {
            return (inos);
        }
    }

std::ostream &         ArtOut::GetInos()
    {
        return (inos);
    }
/*
[[optimize_for_synchronized]]
std::ostream &            ArtOut::operator<<(std::ostream & to) //std::ostream & that)
    {
        return (inos << to.rdbuf());
    }

*/

std::ostream &      operator<<(std::ostream & toOut, ArtOut & that) // to static
    {
        return (toOut << that.GetInos().rdbuf());    
    }

ArtOut bibOut = ArtOut(std::cout);
ArtOut bibErr = ArtOut(std::cerr);
