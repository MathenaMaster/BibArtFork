#include <string>
#include <iostream>
#include <ostream>
#include <random>
#include <thread>
#include <sys/wait.h>
#include <unistd.h>
#include "ForkThread.hpp"

ForkThread::ForkThread(int maxFork, void (*fork_action_entry)(void *), void * base_data) :
    maxForkNb(maxFork), forkNb(0), killSwitch(false), isTurning(true), forkAction(fork_action_entry),
    fork_catcher(std::thread(&ForkThread::ThreadCatcher, this)), common_data(base_data)
    {
        if (maxForkNb < 0)
        {
            std::string msg = std::string(std::string("You indicated the negative number: ") + std::to_string(maxForkNb) + std::string(" as the maximal number of forks possible.\nIt that has to be null (to be unlimited) or positive (to specify a hard limit)."));
            throw std::out_of_range(msg);
        }
        PrintMsg();
    }

ForkThread::~ForkThread()
    {
        SetKillSwitch(true);
        SetIsTurning(false);
        fork_catcher.join();
    }

int     ForkThread::GetForkNb()
    {
        fork_nb_mutex.lock();
        int fork_nb_got = forkNb;
        fork_nb_mutex.unlock();
        return (fork_nb_got);
    }

void    ForkThread::Fork(void * data)
    {
        pid_t       pid;
        int         random_time;

        if (maxForkNb && GetForkNb() > maxForkNb)
        {
            std::string msg = std::string(std::string("You overpassed the fork number limit of: ") + std::to_string(maxForkNb) + std::string(" that you decided."));
            throw std::out_of_range(msg);
        }
        random_time = (std::rand() % 10) + 1;
        pid = fork();
        if (!pid)
        {
            if (forkAction) forkAction(data);
            else BasicTestForkAction(random_time);
            exit(0);
        } else if (pid > 0)
        {
            SetForkNb(1);
            *this << "Child created with pid: " << pid << " for: " << random_time << " seconds." << std::endl;
        } else *this >> "Fork failed." << std::endl;
    }

void    ForkThread::Fork()
    {
        pid_t       pid;
        int         random_time;

        if (maxForkNb && GetForkNb() > maxForkNb) {
            std::string msg = std::string(std::string("You overpassed the fork number limit of: ") + std::to_string(maxForkNb) + std::string(" that you decided."));
            throw std::out_of_range(msg);
        }
        random_time = (std::rand() % 10) + 1;
        pid = fork();
        if (!pid)
        {
            if (forkAction) forkAction(common_data);
            else BasicTestForkAction(random_time);
            exit(0);
        } else if (pid > 0) {
            SetForkNb(1);
            *this << "Child created with pid: " << pid << " for: " << random_time << " seconds." << std::endl;
        } else *this >> "Fork failed." << std::endl;
    }

void    ForkThread::SetForkAction(void (*action) (void *))
    {
        // maybe to securize
        forkAction = action;
    }

void    ForkThread::SetIsTurning(bool toState)
    {
        is_turning_mutex.lock();
        isTurning = toState;
        is_turning_mutex.unlock();
    }

bool    ForkThread::GetIsTurning()
    {
        is_turning_mutex.lock();
        int is_turning_now = isTurning;
        is_turning_mutex.unlock();
        return (is_turning_now);
    }

void    ForkThread::SetKillSwitch(bool toState)
    {
        kill_switch_mutex.lock();
        killSwitch = toState;
        kill_switch_mutex.unlock();
    }

bool    ForkThread::GetKillSwitch()
    {
        kill_switch_mutex.lock();
        int kill_switch_got = killSwitch;
        kill_switch_mutex.unlock();
        return (kill_switch_got);
    }

void    ForkThread::PrintMsg()
    {
        std::cout << "Press Ctrl+c to test provocated ending." << std::endl;
        std::cout << "Now there is no signal usage to respond to the requested no signal principle." << std::endl;
    }

void    ForkThread::CatchLoop()
    {
        pid_t       catched_pid = 0;
        int         return_status;

        while (GetForkNb() > 0)
        {
            catched_pid = wait(&return_status);
            if (catched_pid > 0)
            {
                SetForkNb(-1);
                *this << "The pid: " << catched_pid << " ended with status: " << return_status << "." << std::endl;
            }
        }
    }

void    ForkThread::SetForkNb(int paritedUnit)
    {
        fork_nb_mutex.lock();
        forkNb += paritedUnit;
        fork_nb_mutex.unlock();
    }

void     ForkThread::ThreadCatcher()
    {
        *this << "Thread catcher running. is_turning = " << GetIsTurning() << "." << std::endl;
        while (!GetKillSwitch() && GetIsTurning()) CatchLoop();
        CatchLoop(); // maybe to retire if no security proof
    }

void    ForkThread::BasicTestForkAction(int time_data)
    {
        std::this_thread::sleep_for(std::chrono::seconds(time_data));
        *this << "Child ended after: " << time_data << " seconds." << std::endl;
    }

template<typename T>
[[optimize_for_synchronized]]
std::ostream &  ostream::operator<<(std::ostream & hereOs, T const varToSecure)
    {
        synchronized { hereOs.operator<<(varToSecure); }
        return (hereOs);
    }

template<typename T>
[[optimize_for_synchronized]]
std::ostream &  ostream::operator<<(std::ostream & hereOs, T const * varToSecure)
    {
       synchronized { hereOs.operator<<(varToSecure); }
        return (hereOs);        
    }

[[optimize_for_synchronized]]
std::ostream & ostream::operator<<(std::ostream & hereOs, std::string const & following)
    {
        synchronized { hereOs << following; }
        return (hereOs);
    }

[[optimize_for_synchronized]]
std::ostream &  ostream::operator<<(std::ostream & hereOs, std::ostream & following)
    {
        synchronized { hereOs << following.rdbuf(); }
        return (hereOs);
    }

[[optimize_for_synchronized]]
std::ostream & ostream::operator<<(std::ostream & hereOs, std::ostream& (*func) (std::ostream&))
    {
       synchronized { hereOs.operator<<(func); }
        return (hereOs);
    }

template<typename T>
[[optimize_for_synchronized]]
std::ostream &   operator<<(__attribute__((unused)) std::unique_ptr<ForkThread> & that, T const following)
    {
        synchronized /* sync_out_token_if_evolved */ {
            std::cout << following;
            //ostream::operator<<(std::cout, following);
            return (std::cout);
        }
    }

[[optimize_for_synchronized]]
std::ostream &   operator<<(__attribute__((unused)) std::unique_ptr<ForkThread> & that, char const * following)
    {
        synchronized /* sync_out_token_if_evolved */ {
            std::cout << following;
            //ostream::operator<<(std::cout, following);
            return (std::cout);
        }
    }

[[optimize_for_synchronized]]
std::ostream &   operator<<(__attribute__((unused)) std::unique_ptr<ForkThread> & that, std::string const & following)
    {
        synchronized /* sync_out_token_if_evolved */ {
            std::cout << following;
            //ostream::operator<<(std::cout, following);
            return (std::cout);
        }
    }

[[optimize_for_synchronized]]
std::ostream &   operator<<(__attribute__((unused)) std::unique_ptr<ForkThread> & that, std::ostream & following)
    {
        synchronized /* sync_out_token_if_evolved */ {
            std::cout << following.rdbuf();
            //ostream::operator<<(std::cout, following);
            return (std::cout);
        }
    }

[[optimize_for_synchronized]]
std::ostream &  operator<<(__attribute__((unused)) std::unique_ptr<ForkThread> & that, std::ostream & (*func) (std::ostream &))
    {
       synchronized /* sync_out_token_if_evolved */ {
            std::cout << func;
            //ostream::operator<<(std::cout, func);
            return (std::cout);
        }
    }

template<typename T>
[[optimize_for_synchronized]]
std::ostream &   operator>>(__attribute__((unused)) std::unique_ptr<ForkThread> & that, T const  following)
    {
        synchronized /* sync_err_token_if_evolved */ {
            std::cerr << following;
            //ostream::operator<<(std::cerr, following);
            return (std::cerr);
        }
    }

[[optimize_for_synchronized]]
std::ostream &   operator>>(__attribute__((unused)) std::unique_ptr<ForkThread> & that, char const * following)
    {
        synchronized /* sync_err_token_if_evolved */ {
            std::cerr << following;
            //ostream::operator<<(std::cerr, following);
            return (std::cerr);
        }
    }

[[optimize_for_synchronized]]
std::ostream &   operator>>(__attribute__((unused)) std::unique_ptr<ForkThread> & that, std::string const & following)
    {
        synchronized /* sync_err_token_if_evolved */ {
            std::cerr << following;
            //ostream::operator<<(std::cerr, following);
            return (std::cerr);
        }
    }

[[optimize_for_synchronized]]
std::ostream &   operator>>(__attribute__((unused)) std::unique_ptr<ForkThread> & that, std::ostream & following)
    {
        synchronized /* sync_err_token_if_evolved */ {
            std::cerr << following.rdbuf();
            //ostream::operator<<(std::cerr, following);
            return (std::cerr);
        }
    }

[[optimize_for_synchronized]]
std::ostream &  operator>>(__attribute__((unused)) std::unique_ptr<ForkThread> & that, std::ostream & (*func) (std::ostream &))
    {
       synchronized /* sync_err_token_if_evolved */ {
            std::cerr << func;
            //ostream::operator<<(std::cerr, func);
            return (std::cerr);
        }
    }

template<typename T>
[[optimize_for_synchronized]]
std::ostream &   operator<<(__attribute__((unused)) ForkThread & that, T const following)
    {
        synchronized /* sync_out_token_if_evolved */ {
            std::cout << following;
            //ostream::operator<<(std::cout, following);
            return (std::cout);
        }
    }

template<typename T>
[[optimize_for_synchronized]]
std::ostream &   operator<<(__attribute__((unused)) ForkThread & that, T const * following)
    {
        synchronized /* sync_out_token_if_evolved */ {
            std::cout << following;
            //ostream::operator<<(std::cout, following);
            return (std::cout);
        }
    }

[[optimize_for_synchronized]]
std::ostream &   operator<<(__attribute__((unused)) ForkThread & that, std::string const & following)
    {
        synchronized /* sync_out_token_if_evolved */ {
            std::cout << following;
            //ostream::operator<<(std::cout, following);
            return (std::cout);
        }
    }

[[optimize_for_synchronized]]
std::ostream &   operator<<(__attribute__((unused)) ForkThread & that, std::ostream & following)
    {
        synchronized /* sync_out_token_if_evolved */ {
            std::cout << following.rdbuf();
            //ostream::operator<<(std::cout, following);
            return (std::cout);
        }
    }

[[optimize_for_synchronized]]
std::ostream &  operator<<(__attribute__((unused)) ForkThread & that, std::ostream & (*func) (std::ostream &))
    {
       synchronized /* sync_out_token_if_evolved */ {
            std::cout << func;
            //ostream::operator<<(std::cout, func);
            return (std::cout);
        }
    }

template<typename T>
[[optimize_for_synchronized]]
std::ostream &   operator>>(__attribute__((unused)) ForkThread & that, T const following)
    {
        synchronized /* sync_err_token_if_evolved */ {
            std::cerr << following;
            //ostream::operator<<(std::cerr, following);
            return (std::cerr);
        }
    }

template<typename T>
[[optimize_for_synchronized]]
std::ostream &   operator>>(__attribute__((unused)) ForkThread & that, T const * following)
    {
        synchronized /* sync_err_token_if_evolved */ {
            std::cerr << following;
            //ostream::operator<<(std::cerr, following);
            return (std::cerr);
        }
    }

[[optimize_for_synchronized]]
std::ostream &   operator>>(__attribute__((unused)) ForkThread & that, std::string const & following)
    {
        synchronized /* sync_err_token_if_evolved */ {
            std::cerr << following;
            //ostream::operator<<(std::cerr, following);
            return (std::cerr);
        }
    }

[[optimize_for_synchronized]]
std::ostream &   operator>>(__attribute__((unused)) ForkThread & that, std::ostream & following)
    {
        synchronized /* sync_err_token_if_evolved */ {
            std::cerr << following.rdbuf();
            //ostream::operator<<(std::cerr, following);
            return (std::cerr);
        }
    }

[[optimize_for_synchronized]]
std::ostream &  operator>>(__attribute__((unused)) ForkThread & that, std::ostream & (*func) (std::ostream &))
    {
       synchronized /* sync_err_token_if_evolved */ {
            std::cerr << func;
            //ostream::operator<<(std::cerr, func);
            return (std::cerr);
        }
    }

namespace std {
    std::bibArtType   CreateBibArt(int maxFork, void (*action) (void *), void * base_data)
        {
            return (std::unique_ptr<ForkThread>(new ForkThread(maxFork, action, base_data)));
        }
}
