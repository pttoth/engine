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

    int    _argc;
    char** _argv;

    inline int    getArgc() const{ return _argc; }
    inline char** getArgv() const{ return _argv; }
    void setErrorMessage(char* const msg);
    void setErrorMessage(const std::string& msg);
    void signalShutdownReady();
private:
    void setExecuting(bool val);

    bool    _is_executing;
    std::string _error;
    std::mutex  _mut_exec;
    std::mutex  _mut_query_error;
    std::mutex  _mut_query_exec;
};

