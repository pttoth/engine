//#define APPLICATION_H
#ifndef APPLICATION_H
#define APPLICATION_H

// disables assertions, define this for release versions
//#define NDEBUG

#include <string>
#include <mutex>

union SDL_Event;

namespace pttoth{
    class Application{
    public:
        Application();
        Application(int const argc, char* argv[]);
        virtual ~Application();

        bool initialize();
        void execute();
        bool isExecuting();
        std::string getError();
    protected:
        virtual void onStart();
        virtual void onExit();
        virtual void onEvent(SDL_Event* event) = 0;

        int    _argc;
        char** _argv;

        inline int    getArgc() const{ return _argc; }
        inline char** getArgv() const{ return _argv; }
        void setErrorMessage(char* const msg);
        void setErrorMessage(const std::string& msg);
    private:
        void setExecuting(bool val);

        bool    _is_executing;
        std::string _error;
        std::mutex  _mut_exec;
        std::mutex  _mut_query_error;
        std::mutex  _mut_query_exec;
    };
}

#endif // APPLICATION_H
