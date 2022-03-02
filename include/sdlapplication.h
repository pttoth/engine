#pragma once

#include <string>
#include <mutex>

union SDL_Event;

class SDLApplication{
public:
    SDLApplication();
    SDLApplication(int const argc, char* argv[]);
    virtual ~SDLApplication();

    bool initialize();
    void execute();
    bool isExecuting();
    std::string getError();
    void quit();
protected:
    virtual void onStart();
    virtual void onExit();
    virtual void onEvent(SDL_Event* event) = 0;

    /**
     * @brief onShutdownSignal():
     *   Called some time after calling Application::quit()
     *     May be initiated from outside the Application,
     *     if so, the Application can perform emergency cleanup
     *     here for a clean exit.
     *   signalShutdownReady() has to be called
     *     at the end of the procedure to finish shutdown.
     */
    virtual void onShutdownSignal();

    int    mArgc;
    char** mArgv;

    inline int    getArgc() const{ return mArgc; }
    inline char** getArgv() const{ return mArgv; }
    void setErrorMessage(char* const msg);
    void setErrorMessage(const std::string& msg);
    void signalShutdownReady();
private:
    void setExecuting(bool val);

    bool        mIsExecuting;
    std::string mError;
    std::mutex  mMutExec;
    std::mutex  mMutQueryError;
    std::mutex  mMutQueryExec;
};

