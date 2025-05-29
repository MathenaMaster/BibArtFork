#include <string>
#include <iostream>
#include <ostream>
#include <random>
#include <thread>
#include <memory>
#include <exception>
#include <functional>
#include <sys/wait.h>
#include <unistd.h>
#include "osthings.hpp"
#include "bibArtOut.hpp"
#include "ForkThread.hpp"

bibArtType             bibArtFork = CreateBibArt<__LIMIT__, __SYSTEM__>();

void                    sigkillToSigint(int sigkill)
{
    kill(getpid(), SIGINT);
}

template<typename T, typename S>
bool    ForkThread<T, S>::TestSystemAvailability()
    {
        S   activeNb = 0;
        T   actualUsage = GetForkNb();

        if ((T) maxForkNb == (T) -1) {
            try {
                activeNb = system("ps -aux --no-header | wc -l");
                (*bibOut) << "Real system usage is: " << activeNb << bibOut.endl();
                (*bibOut) << "Actual employment is: " << actualUsage << " on automated maximum usage: " << classLimit << " or system limit: " << systemLimit << bibOut.endl();
            } catch (std::exception e) {
                (*bibErr) << "Problem to get real system usage info! Error info is: " << e.what() << bibErr.endl();
                return (false);
            }
            return ((T) actualUsage < (T) classLimit && (S) activeNb < (S) systemLimit);
        } else if (!maxForkNb) return ((T) actualUsage < (T) classLimit);
        else return ((T) actualUsage < (T) maxForkNb);
    }

template<typename T, typename S>
ForkThread<T, S>::ForkThread(T maxFork, /* std::function<void(const void*)> fork_action_entry, */ void (*fork_action_entry)(void *), void * base_data, T classLimit, S systemLimit, bool setSigKill) :
    maxForkNb(maxFork), forkNb(0), classLimit(classLimit), systemLimit(systemLimit), killSwitch(false),
    common_data(base_data), /* forkAction(fork_action_entry), */ forkAction(std::function<void(void *)>(fork_action_entry)), fork_catcher(std::thread(&ForkThread<T, S>::ThreadCatcher, this))
    {
        if (sizeof(T) > sizeof(S)) {
            (*bibErr) << "You cannot define the system binary size to an inferior capacity than the class is supposed to handle." << bibErr.endl();
            throw std::invalid_argument("System size inferior to class usage specfied size");
        } if ((T) maxFork == (T) -1)
            (*bibErr) << "You used an option (-1) that means: 'manage -automaticaly/during the runtime- the max number and the limitation of available processes in the limit of the system and the instance definition." << bibErr.endl();
        if (setSigKill)
            signal(SIGKILL, sigkillToSigint);
        PrintMsg();
        (*bibErr) << "AN INSTANCE HAS BEEN CREATED" << bibErr.endl();
    }

template<typename T, typename S>
ForkThread<T, S>::~ForkThread()
    {
        SwitchOffKillSwitch();
        fork_catcher.join();
    }

template<typename T, typename S>
void    ForkThread<T, S>::PrintMsg()
    {
        (*bibOut) << "Press Ctrl+c to test provocated ending." << bibOut.endl();
        (*bibOut) << "Now there is no signal handling to respond to the requested no signal principle." << bibOut.endl();
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

    // Next function is problematic. Its not usefull for now, and doesn't works. Will be worked later
/*
template<typename T, typename S>
[[optimize_for_synchronized]]
void    ForkThread<T, S>::SetForkAction(void (*action) (void const *))
    {
        //synchronized { forkAction = std::function<void(const void *)>(action); }
        forkAction = *(new std::function<void(const void *)>(action));
    }*/

template<typename T, typename S>
void    ForkThread<T, S>::SetForkAction(void (*action) (void const *))
    {
        //synchronized { forkAction = std::function<void(const void *)>(action); }
        //forkAction = *(new std::function<void(const void *)>(action));
        fork_action_mutex.lock();
        forkAction = action;
        fork_action_mutex.unlock();
    }

template<typename T, typename S>
std::function<void(void*)>  ForkThread<T, S>::GetForkAction()
    {
        //void    (*action)(const void *);
        std::function<void(void*)> action;
        fork_action_mutex.lock();
        action = forkAction;
        fork_action_mutex.unlock();
        return (action);
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
            //void    (*action)(const void *) = GetForkAction();
            std::function<void(void *)> action = GetForkAction();
            pid = fork();
            if (!pid) {
                //if (forkAction) forkAction(data);
                if (action) action(data);
                else BasicTestForkAction(random_time);
                exit(0); // Be sure you exit(?) in the child code. It's just an overload security
            } else if (pid > 0) {
                SetForkNb(1);
                (*bibOut) << "Child created with pid: " << pid << " for: " << random_time << " seconds." << bibOut.endl();
            } else (*bibErr) << "Fork failed." << bibErr.endl();
        } else (*bibErr) << "You have reached the max number of available processes." << bibErr.endl();
    }

template<typename T, typename S>
void    ForkThread<T, S>::Fork()
    {
        pid_t       pid;
        int         random_time = (std::rand() % 10) + 1;

        if (TestSystemAvailability()) {
            //void    (*action)(const void *) = GetForkAction();
            std::function<void(void *)> action = GetForkAction();
            pid = fork();
            if (!pid) {
                //if (forkAction) forkAction(common_data);
                if (action) action(common_data);
                else BasicTestForkAction(random_time);
                exit(0);
            } else if (pid > 0) {
                SetForkNb(1);
                (*bibOut) << "Child created with pid: " << pid << " for: " << random_time << " seconds." << bibOut.endl();
            } else (*bibErr) << "Fork failed." << bibErr.endl();
        } else (*bibErr) << "You have reached the max number of available processes." << bibErr.endl();
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
                (*bibErr) << "The ENDLOOP CATCHED pid: " << catched_pid << " ended with status: " << return_status << bibErr.endl();
                (*bibErr) << "There is actually: " << actual_fork_nb << " left in processing" << bibErr.endl();
            } else (*bibErr) << "ENDLOOP not over but conntinuing with a global number of child of: " << GetForkNb() << bibErr.endl();
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
            if (catched_pid > 0) {
                SetForkNb(-1);
                (*bibOut) << "The pid: " << catched_pid << " ended with status: " << return_status << bibOut.endl();
                (*bibErr) << "There's stil actual_fork_nb: " << actual_fork_nb << " and: " << GetForkNb() << " real (-1) forkNb left in processing" << bibErr.endl();
            } else {
                (*bibOut) << "The wait failed and the pid: " << catched_pid << " ended with status: " << return_status << bibOut.endl();
                (*bibErr) << "There's stil actual_fork_nb: " << actual_fork_nb << " and: " << GetForkNb() << " real forkNb left in processing" << bibErr.endl();
            }
            return_status = 0;
        }
    }

template<typename T, typename S>
void     ForkThread<T, S>::ThreadCatcher()
    {
        (*bibOut) << "Thread catcher running. kill_switch = " << GetKillSwitch() << bibOut.endl();
        while (!GetKillSwitch()) CatchLoop();
        EndCatchLoop();
    }

template<typename T, typename S>
void    ForkThread<T, S>::BasicTestForkAction(int time_data)
    {
        std::this_thread::sleep_for(std::chrono::seconds(time_data));
        (*bibOut) << "Child ended after: " << time_data << " seconds." << bibOut.endl();
        //exit(0);
    }

template<typename T, typename S>
bibArtType    &           CreateBibArt(T maxFork, /* std::function<void(const void*)> action */ void (*action) (void *), void * base_data)
    {
        return *(new std::unique_ptr<ForkThread<T, S>>(new ForkThread<T, S>(maxFork, action, base_data)));
    }


template class ForkThread<unsigned char, unsigned char>;
template class ForkThread<unsigned char, unsigned short>;
template class ForkThread<unsigned char, unsigned int>;
template class ForkThread<unsigned char, unsigned long>;
template class ForkThread<unsigned char, unsigned long long>;

template class ForkThread<unsigned short, unsigned short>;
template class ForkThread<unsigned short, unsigned int>;
template class ForkThread<unsigned short, unsigned long>;
template class ForkThread<unsigned short, unsigned long long>;

template class ForkThread<__LIMIT__, __SYSTEM__>;

template class ForkThread<unsigned int, unsigned long>;
template class ForkThread<unsigned int, unsigned long long>;
template class ForkThread<unsigned long, unsigned long>;
template class ForkThread<unsigned long, unsigned long long>;

template class ForkThread<unsigned long long, unsigned long long>;




