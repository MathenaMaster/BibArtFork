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

    int                     maxForkNb;
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

                            ForkThread(int maxFork = 0, void (*fork_action_entry)(void *) = nullptr, void * base_data = nullptr);
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

    void                    PrintMsg();
    void                    CatchLoop();
    void                    SetForkNb(int);
    void                    ThreadCatcher();
    void                    BasicTestForkAction(int);
};

namespace ostream {
    template<typename T>
    std::ostream &          operator<<(std::ostream &, T const);
    template<typename T>
    std::ostream &          operator<<(std::ostream &, T const *);    
    std::ostream &          operator<<(std::ostream &, std::string const &);
    std::ostream &          operator<<(std::ostream &, std::ostream &);
    std::ostream &          operator<<(std::ostream &, std::ostream & (*) (std::ostream &));
}

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

namespace std {
    typedef std::unique_ptr<ForkThread>     bibArtType;
    std::bibArtType                         CreateBibArt(int maxFork = 0, void (*action) (void *) = nullptr, void * base_data = nullptr);
}

#ifdef __MAKE_BIBART__
    extern std::bibArtType  bibArtFork;
    std::bibArtType         bibArtFork = std::CreateBibArt();
#endif

#endif /* !__FORKTHREAD__ */
