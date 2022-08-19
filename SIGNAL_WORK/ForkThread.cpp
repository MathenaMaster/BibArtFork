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
std::ostream &    ForkThread<T, S>::CommonPrinter(char const * format, std::ostream & os, va_list & suit)
{
    union Printable_t Printable;

    for (int i = 0; format[i]; ++i)
    {
        switch (format[i])
        {
            case 'l':
                Printable.l = va_arg(suit, unsigned long);
                os << Printable.l;
                break;
            case 'i':
                Printable.i = va_arg(suit, unsigned int);
                os << Printable.i;
                break;
            case 's':
                Printable.s = va_arg(suit, unsigned int);
                os << Printable.s;
                break;
            case 'c':
                Printable.c = va_arg(suit, unsigned int);
                os << Printable.c;
                break;
            case 'd':
                Printable.d = va_arg(suit, double);
                os << Printable.d;
                break;
            case 'f':
                Printable.f = va_arg(suit, double);
                os << Printable.f;
                break;
            case 'p':
                Printable.p = va_arg(suit, char *);
                os << Printable.p;
                break;
            }
        }
    return (os);
}

template<typename T, typename S>
std::ostream &    ForkThread<T, S>::ToCout(char const * format, ...)
{
    va_list     suit;

    va_start(suit, format);
    std::ostream & outOs = Grab(CommonPrinter(format, std::cout, suit));
    va_end(suit);
    return (outOs);
}

template<typename T, typename S>
std::ostream &    ForkThread<T, S>::ToCerr(char const * format, ...)
{
    va_list     suit;

    va_start(suit, format);
    std::ostream & outOs = Grab(CommonPrinter(format, std::cerr, suit));
    va_end(suit);
    return (outOs);
}

template<typename T, typename S>
bool    ForkThread<T, S>::TestSystemAvailability()
    {
        S   activeNb = 0;
        T   actualUsage = GetForkNb();

        if ((T) maxForkNb == (T) -1) {
            try {
                activeNb = system("ps -aux --no-header | wc -l");
                ToCout("plp", "Real system usage is: ", activeNb, "\n");
                ToCout("plplplp", "Actual employment is: ", actualUsage, " on: ", maxForkNb, " or: ", classLimit, "\n");
            } catch (std::exception e) {
                ToCerr("ppp", "Problem to get real system usage info! Error info is: ", e.what(), "\n");
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
ForkThread<T, S>::ForkThread(T maxFork, T classLimit, S systemLimit, void (*fork_action_entry)(void *), void * base_data) :
    maxForkNb(maxFork), forkNb(0), classLimit(classLimit), systemLimit(systemLimit), killSwitch(false), forkAction(fork_action_entry),
    common_data(base_data), fork_catcher(std::thread(&ForkThread<T, S>::ThreadCatcher, this))
    {
        if (sizeof(T) > sizeof(S)) {
            ToCerr("p", "You cannot define the system binary size to an inferior value than the class is supposed to handle.\n");
            throw std::invalid_argument("System size inferior to usage size");
        } if ((T) maxFork == (T) -1) {
            ToCerr("p", "You used an option (-1) that means: 'manage -automaticaly/during the runtime- the max number and the limitation of available processes in the limit of the system and the instance definition.\n");
        }
        PrintMsg();
        ToCerr("p", "AN INSTANCE HAS BEEN CREATED\n");
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
        ToCout("p", "Press Ctrl+c to test provocated ending.\n");
        ToCout("p", "Now there is no signal handling to respond to the requested no signal principle.\n");
    }

template<typename T, typename S>
void    ForkThread<T, S>::SetForkNb(char paritedUnit)
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
        killSwitch = false;
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
                exit(0);
            } else if (pid > 0) {
                SetForkNb(1);
                ToCout("plpip", "Child created with pid: ", pid, " for: ", random_time, " seconds.\n");
            } else ToCerr("p", "Fork failed.\n");
        } else ToCerr("p", "You have reached the max number of available processes.\n");
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
                ToCout("plpip", "Child created with pid: ", pid, " for: ", random_time, " seconds.\n");
            } else ToCerr("p", "Fork failed.\n");
        } else ToCerr("p", "You have reached the max number of available processes.\n");
    }

/*
template<typename T, typename S>
void    ForkThread<T, S>::EndCatchLoop()
    {
        pid_t       catched_pid;
        int         return_status;

        while (GetForkNb() > (T) 0)
        {
            catched_pid = 0;
            catched_pid = waitpid(-1, &return_status, WNOHANG);
            if (catched_pid > 0) {
                SetForkNb(-1);
                ToCout("pipip", "The END CATCHED pid: ", catched_pid, " ended with status: ", return_status, ".\n");
            }
        }
    }
*/

template<typename T, typename S>
void    ForkThread<T, S>::CatchLoop()
    {
        pid_t       catched_pid = 0;
        int         return_status;

        while (GetForkNb() > (T) 0)
        {
            catched_pid = wait(&return_status);
            if (catched_pid > 0) {
                SetForkNb(-1);
                ToCout("pipip", "The pid: ", catched_pid, " ended with status: ", return_status, ".\n");
            }
            catched_pid = 0;
        }
    }

template<typename T, typename S>
void     ForkThread<T, S>::ThreadCatcher()
    {
        ToCout("pip", "Thread catcher running. kill_switch = ", GetKillSwitch(), ".\n");
        while (!GetKillSwitch()) CatchLoop();
        CatchLoop(); // maybe to retire if no security proof
    }

template<typename T, typename S>
void    ForkThread<T, S>::BasicTestForkAction(int time_data)
    {
        std::this_thread::sleep_for(std::chrono::seconds(time_data));
        ToCout("pip", "Child ended after: ", time_data, " seconds.\n");
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

template<typename T, typename S>
bibArtType    ForkThread<T, S>::CreateBibArt(T maxFork, void (*action) (void *), void * base_data)
    {
        return (std::unique_ptr<ForkThread<T, S>>(new ForkThread<T, S>(maxFork, -3, -4, action, base_data)));
    }

template <typename T, typename S>
std::ostream &  ForkThread<T, S>::Grab(std::ostream & os)
    {
        return (os.flush());
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



