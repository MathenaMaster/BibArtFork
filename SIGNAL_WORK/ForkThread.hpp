#include <memory>
#include <thread>
#include <mutex>
#include <string>
#include <ostream>
#include <iostream>
#include <functional>

#ifndef __FORKTHREAD__
#define __FORKTHREAD__

typedef unsigned int __LIMIT__;
typedef unsigned int __SYSTEM__;

template<typename T, typename S> // UNSIGNED HAS TO BE SPECIFIED BY USER. TEMPLATING DO NOT ALLOWS TO EXTENDED TYPENAMES TO 'UNSIGNED TYPENAME'
class                       ForkThread {
    private:

    T                       maxForkNb;
    T                       forkNb;
    T                       classLimit;
    S                       systemLimit;
    bool                    killSwitch;
    //std::function<void(void*)>  forkAction;
    void                    (*forkAction)(void *);
    void *                  common_data;
    std::thread             fork_catcher;
    std::mutex              fork_nb_mutex;
    std::mutex              kill_switch_mutex;
    
    public:

                            ForkThread(T maxFork = 0, T classLimit = -3, S systemLimit = -3, /*std::function<void(void*)> fork_action_entry = nullptr*/ void (*fork_action_entry)(void *) = nullptr, void * base_data = nullptr); // A CPPIFIER
                            ~ForkThread();
    T                       GetForkNb();
    void                    Fork(void *);
    void                    Fork();
    void                    SetForkAction(void (*action) (void *) = nullptr);
    void                    SwitchOffKillSwitch();
    bool                    GetKillSwitch();

    static std::unique_ptr<ForkThread<__LIMIT__, __SYSTEM__>>       CreateBibArt(T maxFork = 0, void (*action) (void *) = nullptr, void * base_data = nullptr);


    private:

    void                    PrintMsg();
    void                    CatchLoop();
    void                    EndCatchLoop();
    void                    SetForkNb(char);
    void                    ThreadCatcher();
    void                    BasicTestForkAction(int);
    bool                    TestSystemAvailability();
};

namespace ostream {
    template<std::string const &>
    static std::ostream &          operator<<(std::ostream &, std::string const &);
    template<std::ostream &>
    static std::ostream &          operator<<(std::ostream &, std::ostream &);
    template<std::ostream &>
    static std::ostream &          operator<<(std::ostream &, std::ostream &(*)(std::ostream &));
};
    std::ostream &          operator*(std::ostream &);

    /*#ifdef FT_8
        typedef unsigned char BIBCOMMON;
        typedef std::unique_ptr<ForkThread<BIBCOMMON>>      bibArtType;
        //template<typename T = unsigned char>
        //bibArtType                         CreateBibArt(T maxFork = 0, void (*action) (void *) = nullptr, void * base_data = nullptr);
    #endif
    #ifdef FT_16
        typedef unsigned short BIBCOMMON;
        typedef std::unique_ptr<ForkThread<BIBCOMMON>>     bibArtType;
        //template<typename T = unsigned short>
        //bibArtType                         CreateBibArt(T maxFork = 0, void (*action) (void *) = nullptr, void * base_data = nullptr);
    #endif
    #ifdef FT_32
        typedef unsigned int BIBCOMMON;
        typedef std::unique_ptr<ForkThread<BIBCOMMON>>       bibArtType;
        //template<typename T = unsigned int>
        //bibArtType                         CreateBibArt(T maxFork = 0, void (*action) (void *) = nullptr, void * base_data = nullptr);
    #endif
    #ifdef FT_64
        typedef unsigned long BIBCOMMON;
        typedef std::unique_ptr<ForkThread<BIBCOMMON>>      bibArtType;
        //template<typename T = unsigned long>
        //bibArtType                         CreateBibArt(T maxFork = 0, void (*action) (void *) = nullptr, void * base_data = nullptr);
    #endif*/
    //#else
        //typedef unsigned short BIBCOMMON;
        //typedef std::unique_ptr<ForkThread<unsigned long, unsigned long>>      bibArtType;
        //template<typename T = BIBCOMMON>
        //bibArtType                         CreateBibArt(T maxFork = 0, void (*action) (void *) = nullptr, void * base_data = nullptr);
        //std::unique_ptr<ForkThread<T>>      CreateBibArt(T maxFork = 0, void (*action) (void *) = nullptr, void * base_data = nullptr);
    //#endif

   
    typedef std::unique_ptr<ForkThread<__LIMIT__, __SYSTEM__>>      bibArtType;

    extern bibArtType       bibArtFork;

#endif /* !__FORKTHREAD__ */
