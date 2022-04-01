#include <iostream>
#include <ostream>
#include <thread>
#include <mutex>
#include <memory>
#include <string>

#ifndef __FORKTHREAD__
#define __FORKTHREAD__

class                       ForkThread {
    private:

    int                     forkNb;
    int                     killSwitch;
    bool                    isTurning;
    void                    (*forkAction)(void *);
    std::thread             fork_catcher;
    std::mutex              fork_nb_mutex;
    std::mutex              is_turning_mutex;
    std::mutex              kill_switch_mutex;
    void *                  common_data;
    
    public:

                            ForkThread(void (*fork_action_entry)(void *) = nullptr, void * base_data = nullptr);
                            ~ForkThread();
    int                     GetForkNb();
    void                    Fork(void *);
    void                    Fork();
    void                    SetForkAction(void (*action) (void *) = nullptr);
    void                    SetIsTurning(bool);
    bool                    GetIsTurning();
    void                    SetKillSwitch(bool);
    bool                    GetKillSwitch();

    private:

    void                    InitSigAndPrintMsg();
    void                    CatchLoop();
    void                    SetForkNb(int);
    void                    ThreadCatcher();
    void                    BasicForkAction(int);
};

namespace ostream {
    template<typename T>
    std::ostream &          operator<<(std::ostream &, T const);
    template<typename T>
    std::ostream &          operator<<(std::ostream &, T const *);    
    std::ostream &          operator<<(std::ostream &, std::string const &);
    std::ostream &          operator<<(std::ostream &, std::ostream &);
    std::ostream &          operator<<(std::ostream &, std::ostream & (*) (std::ostream &));
};

    template<typename T>
    std::ostream &          operator<<(ForkThread &, T const);
    template<typename T>
    std::ostream &          operator<<(ForkThread &, T const *);
    std::ostream &          operator<<(ForkThread &, std::string const &);
    std::ostream &          operator<<(ForkThread &, std::ostream &);
    std::ostream &          operator<<(ForkThread &, std::ostream & (*) (std::ostream &));

    template<typename T>
    std::ostream &          operator>>(ForkThread &, T const);
    template<typename T>
    std::ostream &          operator>>(ForkThread &, T const *);
    std::ostream &          operator>>(ForkThread &, std::string const &);
    std::ostream &          operator>>(ForkThread &, std::ostream &);
    std::ostream &          operator>>(ForkThread &, std::ostream & (*) (std::ostream &));


    template<typename T>
    std::ostream &          operator<<(std::unique_ptr<ForkThread> &, T const);
    std::ostream &          operator<<(std::unique_ptr<ForkThread> &, char const *);
    std::ostream &          operator<<(std::unique_ptr<ForkThread> &, std::string const &);
    std::ostream &          operator<<(std::unique_ptr<ForkThread> &, std::ostream &);
    std::ostream &          operator<<(std::unique_ptr<ForkThread> &, std::ostream & (*) (std::ostream &));

    template<typename T>
    std::ostream &          operator>>(std::unique_ptr<ForkThread> &, T const);
    std::ostream &          operator>>(std::unique_ptr<ForkThread> &, char const *);
    std::ostream &          operator>>(std::unique_ptr<ForkThread> &, std::string const &);
    std::ostream &          operator>>(std::unique_ptr<ForkThread> &, std::ostream &);
    std::ostream &          operator>>(std::unique_ptr<ForkThread> &, std::ostream & (*) (std::ostream &));


extern std::unique_ptr<ForkThread>      bibArtFork;

#endif /* !__FORKTHREAD__ */
