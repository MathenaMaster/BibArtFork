#include <iostream>
#include <ostream>
#include <thread>
#include <string>
#include <semaphore>
#include "osthings.hpp"
#include "bibArtOut.hpp"


ArtOut::ArtOut(std::ostream & copy): inos(copy), stream_sem(std::counting_semaphore<1>(1))
    {
    }

std::ostream &            ArtOut::operator*()
    {
        stream_sem.acquire();
        return (inos);
    }

std::ostream &         ArtOut::GetInos()
    {
        return (inos);
    }

std::string                 ArtOut::endl()
    {
        inos << std::endl;
        stream_sem.release();
        return ("");
    }

std::string                 ArtOut::flush()
    {
        inos << std::flush;
        stream_sem.release();
        return ("");
    }

ArtOut bibOut = ArtOut(std::cout);
ArtOut bibErr = ArtOut(std::cerr);
