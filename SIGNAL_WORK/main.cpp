#include <iostream>
#include <string>
#include "ForkThread.hpp"

template<int var>
void                    troll_func(std::string msg)
    {
        std::cout << msg << var << std::endl;
    }

    void    BasicForkAction(void *time_data)
    {
        std::this_thread::sleep_for(std::chrono::seconds(*((int *) time_data)));
        std::cout << "Child BASICALY ended after: " << *((int *) time_data) << " seconds." << std::endl;
    }

int                     main(int argc, char **argv)
    {
        cout_mutex.lock();
        troll_func<10>("So ! That trolls ?: ");
        troll_func<10>("This test will repeat this number of time: ");
        cout_mutex.unlock();
        for (int i = 1; i <= 10; ++i)
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            cout_mutex.lock();
            std::cout << "This is the: " << i  << " over: " << 10 << std::endl;
            cout_mutex.unlock();
            bibArtFork.get()->Fork();
        }
        bibArtFork.get()->Set_Fork_Action(BasicForkAction);
        for (int i = 1; i <= 10; ++i)
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            cout_mutex.lock();
            std::cout << "This is the func set test: " << i  << " over: " << 10 << std::endl;
            cout_mutex.unlock();
            bibArtFork.get()->Fork();
        }
        cout_mutex.lock();
        std::cout << "This thime it's the last test" << std::endl;
        cout_mutex.unlock();
        for (int i = 1; i <= 10; ++i)
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            cout_mutex.lock();
            std::cout << "This is the func and var set test: " << i  << " over: " << 10 << std::endl;
            cout_mutex.unlock();
            int time_data = (std::rand() % 10) + 1;
            bibArtFork.get()->Fork(&time_data);
        }
        return (0);
    }