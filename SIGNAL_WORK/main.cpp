#include <iostream>
#include <string>
#include <exception>

//#define __MAKE_BIBART__
//#define FT_8

#include "bibArtOut.hpp"
#include "ForkThread.hpp"

/*
template<int var>
void                    Troll_func(std::string msg)
    {
        bibArtFork << msg << var << "." << std::endl;
    }
*/

int                     main(int argc, char **argv)
    {
        int             max_fork = 1000;
        std::size_t     junk_var;

        //Troll_func<10>("Whats that 10: ");
        (*bibOut) << "You can specify a number as first argument of this program to set the number of fork to launch during the test. (not mandatory and set to: " << max_fork << ")" << bibOut.endl();
        try {
            if (argc == 2) max_fork = std::stoi(std::string(argv[1]), &junk_var);
            if (max_fork <= 0) throw ("The number of fork to do must not be null or negative!");
        } catch (std::exception e) {
            (*bibErr) << "You specified an entry to the program that is NaN or too extreme! Error is: " << e.what() << bibErr.endl();
            exit(0);
        }
        for (int i = 0; i <= max_fork; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            (*bibOut) << "This is the: " << i << " over: " << max_fork << bibOut.endl();
            bibArtFork->Fork();
        }
        return (0);
    }
