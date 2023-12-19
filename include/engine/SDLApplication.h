#pragma once

#include <string>
#include <mutex>

union SDL_Event;

// TODO: move this whole class into some other project and remove dependencies

class SDLApplication{
public:
    SDLApplication();
    SDLApplication(int const argc, char* argv[]);
    virtual ~SDLApplication();

    bool Initialize();
    virtual void Execute();
    bool IsExecuting();
    std::string GetError();
    void Quit();
protected:
    virtual void OnStart();
    virtual void OnExit();
    virtual void OnEvent(SDL_Event* event) = 0;

    /**
     * @brief onShutdownSignal():
     *   Called some time after calling Application::quit()
     *     May be initiated from outside the Application,
     *     if so, the Application can perform emergency cleanup
     *     here for a clean exit.
     *   signalShutdownReady() has to be called
     *     at the end of the procedure to finish shutdown.
     */
    virtual void OnShutdownSignal();

    int    mArgc    = 0;
    char** mArgv    = nullptr;

    inline int    GetArgc() const{ return mArgc; }
    inline char** GetArgv() const{ return mArgv; }
    void SetErrorMessage(char* const msg);
    void SetErrorMessage(const std::string& msg);
    void SignalShutdownReady();
private:
    void SetExecuting(bool val);

    bool        mIsExecuting    = false;
    std::string mError;
    std::mutex  mMutExec;
    std::mutex  mMutQueryError;
    std::mutex  mMutQueryExec;
};

