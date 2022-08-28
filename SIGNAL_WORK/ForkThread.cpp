#include <string>
#include <iostream>
#include <ostream>
#include <random>
#include <thread>
#include <memory>
#include <exception>
#include <sys/wait.h>
#include <unistd.h>
#include <stdarg.h>
#include "ForkThread.hpp"

bibArtType             bibArtFork = ForkThread<__LIMIT__, __SYSTEM__>::CreateBibArt();

template<typename T, typename S>
bool    ForkThread<T, S>::TestSystemAvailability()
    {
        S   activeNb = 0;
        T   actualUsage = GetForkNb();

        if ((T) maxForkNb == (T) -1) {
            try {
                activeNb = system("ps -aux --no-header | wc -l");
                *std::cout << "Real system usage is: " << activeNb << std::endl;
                *std::cout << "Actual employment is: " << actualUsage << " on automated maximum usage: " << classLimit << " or system limt: " << systemLimit << std::endl;
            } catch (std::exception e) {
                *std::cerr << "Problem to get real system usage info! Error info is: " << e.what() << std::endl;
                return (false);
            }
            return ((T) actualUsage < (T) classLimit && (S) activeNb < (S) systemLimit);
        } else if (!maxForkNb) {
            return (actualUsage < (T) classLimit);
        } else {
            return ((T) actualUsage < (T) maxForkNb);
        }
    }

template<typename T, typename S>
ForkThread<T, S>::ForkThread(T maxFork, T classLimit, S systemLimit, /*std::function<void(void*)> fork_action_entry*/ void (*fork_action_entry)(void *), void * base_data) :
    maxForkNb(maxFork), forkNb(0), classLimit(classLimit), systemLimit(systemLimit), killSwitch(false), forkAction(fork_action_entry),
    common_data(base_data), fork_catcher(std::thread(&ForkThread<T, S>::ThreadCatcher, this))
    {
        if (sizeof(T) > sizeof(S)) {
            *std::cerr << "You cannot define the system binary size to an inferior capacity than the class is supposed to handle." << std::endl;
            throw std::invalid_argument("System size inferior to class usage specfied size");
        } if ((T) maxFork == (T) -1) {
            *std::cerr << "You used an option (-1) that means: 'manage -automaticaly/during the runtime- the max number and the limitation of available processes in the limit of the system and the instance definition." << std::endl;
        }
        PrintMsg();
        *std::cerr << "AN INSTANCE HAS BEEN CREATED" << std::endl;
    }

template<typename T, typename S>
ForkThread<T, S>::~ForkThread()
    {
        SwitchOffKillSwitch();
        while (fork_catcher.joinable()) fork_catcher.join();
        //fork_catcher.join();
    }

template<typename T, typename S>
void    ForkThread<T, S>::PrintMsg()
    {
        *std::cout << "Press Ctrl+c to test provocated ending." << std::endl;        
        *std::cout << "Now there is no signal handling to respond to the requested no signal principle." << std::endl;
    }

template<typename T, typename S>
void       ForkThread<T, S>::SetForkNb(char paritedUnit)
    {
        fork_nb_mutex.lock();
        forkNb += paritedUnit;
        fork_nb_mutex.unlock();
    }

template<typename T, typename S>
T       ForkThread<T, S>::GetForkNb()
    {
        fork_nb_mutex.lock();
        T fork_nb_got = forkNb;
        fork_nb_mutex.unlock();
        return (fork_nb_got);
    }

template<typename T, typename S>
[[optimize_for_synchronized]]
void    ForkThread<T, S>::SetForkAction(void (*action) (void *))
    {
        synchronized { forkAction = action; }
    }

template<typename T, typename S>
void    ForkThread<T, S>::SwitchOffKillSwitch()
    {
        kill_switch_mutex.lock();
        killSwitch = true;
        kill_switch_mutex.unlock();
    }

template<typename T, typename S>
bool    ForkThread<T, S>::GetKillSwitch()
    {
        kill_switch_mutex.lock();
        bool kill_switch_got = killSwitch;
        kill_switch_mutex.unlock();
        return (kill_switch_got);
    }

template<typename T, typename S>
void    ForkThread<T, S>::Fork(void * data)
    {
        pid_t       pid;
        int         random_time = (std::rand() % 10) + 1;

        if (TestSystemAvailability()) {
            pid = fork();
            if (!pid) {
                if (forkAction) forkAction(data);
                else BasicTestForkAction(random_time);
                exit(0); // Be sure you exit(?) in the child code. It's just an overload security
            } else if (pid > 0) {
                SetForkNb(1);
                *std::cout << "Child created with pid: " << pid << " for: " << random_time << " seconds." << std::endl;
            } else *std::cerr << "Fork failed." << std::endl;
        } else *std::cerr << "You have reached the max number of available processes." << std::endl;
    }

template<typename T, typename S>
void    ForkThread<T, S>::Fork()
    {
        pid_t       pid;
        int         random_time = (std::rand() % 10) + 1;

        if (TestSystemAvailability()) {
            pid = fork();
            if (!pid) {
                if (forkAction) forkAction(common_data);
                else BasicTestForkAction(random_time);
                exit(0);
            } else if (pid > 0) {
                SetForkNb(1);
                *std::cout << "Child created with pid: " << pid << " for: " << random_time << " seconds." << std::endl;
            } else *std::cerr << "Fork failed." << std::endl;
        } else *std::cerr << "You have reached the max number of available processes." << std::endl;
    }


template<typename T, typename S>
void    ForkThread<T, S>::EndCatchLoop()
    {
        pid_t       catched_pid;
        int         return_status = 0;
        T           actual_fork_nb;

        while ((actual_fork_nb = GetForkNb()) > 0)
        {
            catched_pid = waitpid(-1, &return_status, WNOHANG);
            if (catched_pid > 0) {
                SetForkNb(-1);
                *std::cerr << "The ENDLOOP CATCHED pid: " << catched_pid << " ended with status: " << return_status << std::endl;
                *std::cerr << "There is actually: " << actual_fork_nb << " left in processing" << std::endl;
            } else *std::cerr << "ENDLOOP not over but conntinuing with a global number of child of: " << GetForkNb() << std::endl;
            catched_pid = 0;
            return_status = 0;
        }
    }

template<typename T, typename S>
void    ForkThread<T, S>::CatchLoop()
    {
        pid_t       catched_pid;
        int         return_status = 0;
        T           actual_fork_nb;

        while ((actual_fork_nb = GetForkNb()) > 0)
        {
            catched_pid = wait(&return_status);
            if (catched_pid > 0) { // | WIFEXITED(return_status) | WIFSIGNALED(return_status) | WCOREDUMP(return_status)) {
                SetForkNb(-1);
                *std::cout << "The pid: " << catched_pid << " ended with status: " << return_status << std::endl; //*this << "The pid: " << catched_pid << " ended with status: " << return_status << std::endl;
                *std::cerr << "There's stil actual_fork_nb: " << actual_fork_nb << " and: " << GetForkNb() << " real (-1) forkNb left in processing" << std::endl;
            } else {
                *std::cout << "The wait failed and the pid: " << catched_pid << " ended with status: " << return_status << std::endl;
                *std::cerr << "There's stil actual_fork_nb: " << actual_fork_nb << " and: " << GetForkNb() << " real forkNb left in processing" << std::endl;
            }
            return_status = 0;
        }
    }

template<typename T, typename S>
void     ForkThread<T, S>::ThreadCatcher()
    {
        *std::cout << "Thread catcher running. kill_switch = " << GetKillSwitch() << std::endl;
        while (!GetKillSwitch()) CatchLoop();
        //CatchLoop(); // maybe to retire if no security proof
    }

template<typename T, typename S>
void    ForkThread<T, S>::BasicTestForkAction(int time_data)
    {
        std::this_thread::sleep_for(std::chrono::seconds(time_data));
        *std::cout << "Child ended after: " << time_data << " seconds." << std::endl;
        exit(0);
    }

template<std::string const &>
std::ostream & ostream::operator<<(std::ostream & hereOs, std::string const & following)
    {
        hereOs << following.c_str();
        return (hereOs);
    }

template<std::ostream &>
std::ostream &  ostream::operator<<(std::ostream & hereOs, std::ostream & following)
    {
        hereOs << following.rdbuf();
        return (hereOs);
    }

template<std::ostream &(*)(std::ostream &)>
[[optimize_for_synchronized]]
std::ostream &  ostream::operator<<(std::ostream & hereOs, std::ostream & (*following)(std::ostream &))
    {
        synchronized {
            hereOs.operator<<(following);
        }
        return (hereOs);
    }

[[optimize_for_synchronized]]
std::ostream &  operator*(std::ostream & that)
    {
        synchronized {
            return (that);
        }
    }

template<typename T, typename S>
bibArtType    ForkThread<T, S>::CreateBibArt(T maxFork, void (*action) (void *), void * base_data)
    {
        return (std::unique_ptr<ForkThread<T, S>>(new ForkThread<T, S>(maxFork, -3, -3, action, base_data)));
    }

/*
template class ForkThread<unsigned char, unsigned char>;
template class ForkThread<unsigned char, unsigned short>;
template class ForkThread<unsigned char, unsigned int>;
template class ForkThread<unsigned char, unsigned long>;
template class ForkThread<unsigned char, unsigned long long>;

template class ForkThread<unsigned short, unsigned short>;
template class ForkThread<unsigned short, unsigned int>;
template class ForkThread<unsigned short, unsigned long>;
template class ForkThread<unsigned short, unsigned long long>;
*/
template class ForkThread<__LIMIT__, __SYSTEM__>;
/*
template class ForkThread<unsigned int, unsigned long>;
template class ForkThread<unsigned int, unsigned long long>;
template class ForkThread<unsigned long, unsigned long>;
template class ForkThread<unsigned long, unsigned long long>;

template class ForkThread<unsigned long long, unsigned long long>;
*/



