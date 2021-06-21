

#include <thread>
#include <mutex>
#include <memory>

#include <iostream>
#include <random>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include "ForkThread.hpp"

std::shared_ptr<ForkThread>      bibArtFork(new ForkThread(true));

ForkThread::ForkThread(void (*fork_action_entry)(void *), bool orig = true):
    fork_nb(0), is_turning(true), kill_switch(false), fork_action(fork_action_entry),
    fork_catcher(this->SetThread())
    {
        InitSigAndPrintMsg(orig);
    }

ForkThread::ForkThread(bool orig):
    fork_nb(0), is_turning(true), kill_switch(false), fork_action(NULL),
    fork_catcher(this->SetThread())
    {
       InitSigAndPrintMsg(orig);
    }

ForkThread::ForkThread():
    fork_nb(0), is_turning(true), kill_switch(false), fork_action(NULL),
    fork_catcher(this->SetThread())
    {
        InitSigAndPrintMsg(false);
    }

std::thread    ForkThread::SetThread()
    {
        return (std::thread(&ForkThread::ThreadCatcher, this));
    }

void    ForkThread::InitSigAndPrintMsg(bool orig)
    {
        if (orig) bibArtFork = std::shared_ptr<ForkThread>(this);
        signal(SIGINT, SignalSigInt);
        cout_mutex.lock();
        std::cout << "Press Ctrl+c to correctly end." << std::endl
        << "Repress Ctrl+c to kill all children and process." << std::endl;
        cout_mutex.unlock();
    }

ForkThread::~ForkThread()
    {
        Set_Is_Turning(false);
        fork_catcher.join();
    }

int     ForkThread::GetForkNb()
    {
        fork_nb_mutex.lock();
        int fork_nb_got = fork_nb;
        fork_nb_mutex.unlock();
        return (fork_nb_got);
    }

void    ForkThread::SetForkNb(int with)
    {
        fork_nb_mutex.lock();
        fork_nb += with;
        fork_nb_mutex.unlock();
    }

int     ForkThread::ThreadCatcher()
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cout_mutex.lock();
        std::cout << "Thread catcher running. is_turning = " << Get_Is_Turning() << ", kill_switch = " << Get_Kill_Switch() << std::endl;
        cout_mutex.unlock();
        while (Get_Is_Turning() && !Get_Kill_Switch())
            CatchLoop();
        return (0);
    }

void    ForkThread::CatchLoop()
    {
        pid_t       catched_pid;
        int         return_status;

        while (GetForkNb())
        {
            catched_pid = wait(&return_status);
            if (catched_pid > 0) {
                SetForkNb(-1);
                cout_mutex.lock();
                std::cout << "the pid: " << catched_pid << " ended with status: " << return_status << std::endl;
                cout_mutex.unlock();
            }
        }
    }

void    ForkThread::Fork(void *data)
    {
        pid_t       pid;
        int         random_time;

        random_time = (std::rand() % 10) + 1;
        pid = fork();
        if (!pid) {
            if (fork_action)
                fork_action(data);
            else
                BasicForkAction(random_time);
            exit(0);
        } else if (pid > 0) {
            SetForkNb(1);
            cout_mutex.lock();
            std::cout << "Child created with pid: " << pid << " for: " << random_time << " seconds." << std::endl;
            cout_mutex.unlock();
        } else {
            cout_mutex.lock();
            std::cerr << "Fork failed." << std::endl;
            cout_mutex.unlock();
        }
    }

void    ForkThread::Fork()
    {
        pid_t       pid;
        int         random_time;

        random_time = (std::rand() % 10) + 1;
        pid = fork();
        if (!pid) {
            if (fork_action)
                fork_action(&random_time);
            else
                BasicForkAction(random_time);
            exit(0);
        } else if (pid > 0) {
            SetForkNb(1);
            cout_mutex.lock();
            std::cout << "Child created with pid: " << pid << " for: " << random_time << " seconds." << std::endl;
            cout_mutex.unlock();
        } else {
            cout_mutex.lock();
            std::cerr << "Fork failed." << std::endl;
            cout_mutex.unlock();
        }
    }

void    ForkThread::BasicForkAction(int time_data)
    {
        std::this_thread::sleep_for(std::chrono::seconds(time_data));
        std::cout << "Child ended after: " << time_data << " seconds." << std::endl;
    }

void    SignalSigInt(int __attribute__((unused)) sig)
    {
        bibArtFork.get()->Set_Is_Turning(false);
        signal(SIGINT, SignalSigKill);
        exit(0);
    }

void    SignalSigKill(int __attribute__((unused)) sig)
    {
        bibArtFork.get()->Set_Kill_Switch(true);
        kill(-1, SIGKILL);
        exit(0);
    }

void    ForkThread::Set_Fork_Action(void (*action) (void *))
    {
        fork_action = action;
    }


void    ForkThread::Set_Is_Turning(bool is)
    {
        static std::mutex mutex;
        mutex.lock();
        is_turning = is;
        mutex.unlock();
    }

bool    ForkThread::Get_Is_Turning()
    {
        static std::mutex mutex;
        mutex.lock();
        int is_turning_now = is_turning;
        mutex.unlock();
        return (is_turning_now);
    }

void    ForkThread::Set_Kill_Switch(bool set)
    {
        static std::mutex mutex;
        mutex.lock();
        kill_switch = set;
        mutex.unlock();
    }

bool    ForkThread::Get_Kill_Switch()
    {
        static std::mutex mutex;
        mutex.lock();
        int kill_switch_got = kill_switch;
        mutex.unlock();
        return (kill_switch);
    }


void    ForkThread::StopCatcherThread()
    {
        delete bibArtFork.get();
    }

