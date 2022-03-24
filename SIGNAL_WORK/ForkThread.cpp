#include <string>
#include <iostream>
#include <ostream>
#include <random>
#include <thread>
#include <sys/wait.h>
#include <unistd.h>
#include "ForkThread.hpp"

std::unique_ptr<ForkThread>      bibArtFork(new ForkThread());

ForkThread::ForkThread(void (*fork_action_entry)(void *)) :
    forkNb(0), killSwitch(false), isTurning(true), forkAction(fork_action_entry),
    fork_catcher(std::thread(&ForkThread::ThreadCatcher, this))
    {
        InitSigAndPrintMsg();
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

        random_time = (std::rand() % 10) + 1;
        pid = fork();
        if (!pid) {
            if (forkAction)
                forkAction(data);
            else
                BasicForkAction(random_time);
            exit(0);
        } else if (pid > 0) {
            SetForkNb(1);
            *this << "Child created with pid: " << pid << " for: " << random_time << " seconds." << std::endl;
        } else {
            *this >> "Fork failed." << std::endl;
        }
    }

void    ForkThread::Fork()
    {
        pid_t       pid;
        int         random_time;

        random_time = (std::rand() % 10) + 1;
        pid = fork();
        if (!pid) {
            if (forkAction)
                forkAction(&random_time);
            else
                BasicForkAction(random_time);
            exit(0);
        } else if (pid > 0) {
            SetForkNb(1);
            *this << "Child created with pid: " << pid << " for: " << random_time << " seconds." << std::endl;
        } else {
            *this >> "Fork failed." << std::endl;
        }
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

void    ForkThread::InitSigAndPrintMsg()
    {
        std::cout << "This basic testing purposes program can fail if the number of fork to do is to high, so be aware of." << std::endl;
        std::cout << "Press Ctrl+c to test normal ending." << std::endl;
        std::cout << "Now there is no signal usage to respond to the requested empty-signal principle." << std::endl;
    }

void    ForkThread::CatchLoop()
    {
        pid_t       catched_pid;
        int         return_status;

        while (GetForkNb() > 0)
        {
            catched_pid = wait(&return_status);
            if (catched_pid > 0) {
                SetForkNb(-1);
                *this << "The pid: " << catched_pid << " ended with status: " << return_status << "." << std::endl;
                catched_pid = 0;
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
        while (!GetKillSwitch() && GetIsTurning())
            CatchLoop();
        CatchLoop();
    }

void    ForkThread::BasicForkAction(int time_data)
    {
        std::this_thread::sleep_for(std::chrono::seconds(time_data));
        *this << "Child ended after: " << time_data << " seconds." << std::endl;
    }

template<typename T>
[[optimize_for_synchronized]]
std::ostream &  ostream::operator<<(std::ostream & hereOs, const T varToSecure)
    {
        synchronized {
            hereOs.operator<<(varToSecure);
        }
        return (hereOs);
    }

template<typename T>
[[optimize_for_synchronized]]
std::ostream &  ostream::operator<<(std::ostream & hereOs, const T * varToSecure)
    {
       synchronized {
            hereOs.operator<<(varToSecure);
        }
        return (hereOs);        
    }

[[optimize_for_synchronized]]
std::ostream & ostream::operator<<(std::ostream & hereOs, std::string const & following)
    {
        synchronized {
            hereOs << following;
        }
        return (hereOs);
    }

[[optimize_for_synchronized]]
std::ostream &  ostream::operator<<(std::ostream & hereOs, std::ostream & following)
    {
        synchronized {
            hereOs << following.rdbuf();
        }
        return (hereOs);
    }

[[optimize_for_synchronized]]
std::ostream & ostream::operator<<(std::ostream & hereOs, std::ostream& (*func) (std::ostream&))
    {
       synchronized {
           hereOs.operator<<(func);
        }
        return (hereOs);
    }

template<typename T>
[[optimize_for_synchronized]]
std::ostream &   operator<<(__attribute__((unused)) ForkThread const & that, const T following)
    {
        synchronized /* sync_out_token_if_evolved */ {
            std::cout << following;
            return (std::cout);
        }
    }

template<typename T>
[[optimize_for_synchronized]]
std::ostream &   operator<<(__attribute__((unused)) ForkThread const & that, const T * following)
    {
        synchronized /* sync_out_token_if_evolved */ {
            std::cout << following;
            return (std::cout);
        }
    }

[[optimize_for_synchronized]]
std::ostream &   operator<<(__attribute__((unused)) ForkThread const & that, std::string const & following)
    {
        synchronized /* sync_out_token_if_evolved */ {
            std::cout << following;
            return (std::cout);
        }
    }

[[optimize_for_synchronized]]
std::ostream &   operator<<(__attribute__((unused)) ForkThread const & that, std::ostream & following)
    {
        synchronized /* sync_out_token_if_evolved */ {
            std::cout << following.rdbuf();
            return (std::cout);
        }
    }

[[optimize_for_synchronized]]
std::ostream &  operator<<(__attribute__((unused)) ForkThread const & that, std::ostream & (*func) (std::ostream &))
    {
       synchronized /* sync_out_token_if_evolved */ {
            std::cout << func;
            return (std::cout);
        }
    }

template<typename T>
[[optimize_for_synchronized]]
std::ostream &   operator>>(__attribute__((unused)) ForkThread const & that, const T following)
    {
        synchronized /* sync_err_token_if_evolved */ {
            std::cerr << following;
            return (std::cerr);
        }
    }

template<typename T>
[[optimize_for_synchronized]]
std::ostream &   operator>>(__attribute__((unused)) ForkThread const & that, const T * following)
    {
        synchronized /* sync_err_token_if_evolved */ {
            std::cerr << following;
            return (std::cerr);
        }
    }

[[optimize_for_synchronized]]
std::ostream &   operator>>(__attribute__((unused)) ForkThread const & that, std::string const & following)
    {
        synchronized /* sync_err_token_if_evolved */ {
            std::cerr << following;
            return (std::cerr);
        }
    }

[[optimize_for_synchronized]]
std::ostream &   operator>>(__attribute__((unused)) ForkThread const & that, std::ostream & following)
    {
        synchronized /* sync_err_token_if_evolved */ {
            std::cerr << following.rdbuf();
            return (std::cerr);
        }
    }

[[optimize_for_synchronized]]
std::ostream &  operator>>(__attribute__((unused)) ForkThread const & that, std::ostream & (*func) (std::ostream &))
    {
       synchronized /* sync_err_token_if_evolved */ {
            std::cerr << func;
            return (std::cerr);
        }
    }
