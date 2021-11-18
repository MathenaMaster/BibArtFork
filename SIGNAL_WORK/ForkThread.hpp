
#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <string>

#ifndef _FORKTHREAD_
#define _FORKTHREAD_

class                   ForkThread {
    private:

    int                             fork_nb;
    bool                            is_turning;
    bool                            kill_switch;
    void                            (*fork_action)(void *);
    std::thread                     fork_catcher;
    std::mutex                      fork_nb_mutex;

    public:

                        ForkThread(void (*)(void *), bool);
                        ForkThread(bool);
                        ForkThread();
                        ~ForkThread();
    int                 GetForkNb();
    void                Fork(void *);
    void                Fork();

    void                Set_Fork_Action(void (*action) (void *));
    void                Set_Is_Turning(bool);
    bool                Get_Is_Turning();
    void                Set_Kill_Switch(bool);
    bool                Get_Kill_Switch();
    void                CatchLoop();
    void                StopCatcherThread();
    
    private:

    std::thread         SetThread();
    void                InitSigAndPrintMsg(bool orig);
    void                SetForkNb(int);
    int                 ThreadCatcher();
    void                BasicForkAction(int);
};

void                SignalSigInt(int);
void                SignalSigKill(int);

void                operator<<(std::shared_ptr<ForkThread> &, std::string);
void                operator>>(std::shared_ptr<ForkThread> &, std::string);

extern std::shared_ptr<ForkThread>      bibArtFork;

#endif /*   _FORKTHREAD_    */