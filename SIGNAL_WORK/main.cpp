#include <iostream>
#include <string>
#include <sstream>
#include "ForkThread.hpp"

template<int var>
void                    troll_func(std::string msg)
    {
        std::stringstream ssV;

        ssV << var;
        bibArtFork << msg + ssV.str();
    }

    void    BasicForkAction(void *time_data)
    {
        std::this_thread::sleep_for(std::chrono::seconds(*((int *) time_data)));
        std::stringstream ssT;

        ssT << *((int *) time_data);
        bibArtFork << "Child BASICALY ended after: " + ssT.str() + " seconds.";
    }

int                     main(int argc, char **argv)
    {
        troll_func<10>("So ! That trolls ?: ");
        troll_func<10>("This test will repeat this number of time: ");
        for (int i = 1; i <= 10; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::stringstream ssI, ssD;

            ssI << i;
            ssD << 10;
            bibArtFork << "This is the: " + ssI.str() + " over: " + ssD.str();
            bibArtFork.get()->Fork();
        }
        bibArtFork.get()->Set_Fork_Action(BasicForkAction);
        for (int i = 1; i <= 10; ++i)
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::stringstream ssI, ssD;

            ssI << i;
            ssD << 10;
            bibArtFork << "This is the func set test: " + ssI.str() + " over: " + ssD.str();
            bibArtFork.get()->Fork();
        }
        bibArtFork << "This thime it's the last test";
        for (int i = 1; i <= 10; ++i)
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::stringstream ssI, ssD;

            ssI << i;
            ssD << 10;
            bibArtFork << "This is the func and var set test: " + ssI.str() + " over: " + ssD.str();
            int time_data = (std::rand() % 10) + 1;
            bibArtFork.get()->Fork(&time_data);
        }
        return (0);
    }