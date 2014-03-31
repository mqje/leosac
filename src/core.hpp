/**
 * \file core.hpp
 * \author Thibault Schueller <thibault.schueller@islog.com>
 * \brief module managing class
 */

#ifndef CORE_HPP
#define CORE_HPP

#include <mutex>
#include <list>

#include "event.hpp"
#include "modules/iloggermodule.hpp"
#include "hardware/ihwmanager.hpp"

class Core
{
public:
    Core();
    ~Core();

private:
    Core(const Core& other);
    Core& operator=(const Core& other);

public:
    void    run();

private:
    void    load();
    void    unload();
    void    dispatchEvent(const Event& event);

private:
    std::mutex                  _runMutex;
    bool                        _isRunning;
    IHWManager*                  _hwManager;
    std::list<ILoggerModule*>   _loggerModules;
};

#endif // CORE_HPP