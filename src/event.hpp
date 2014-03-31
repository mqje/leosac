/**
 * \file event.hpp
 * \author Thibault Schueller <thibault.schueller@islog.com>
 * \brief event class for intermodule communications
 */

#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <chrono>

class Event
{
public:
    enum LogLevel {
        Emergency   = 0, /* Emergency messages, system is about to crash or is unstable */
        Alert       = 1, /* Something bad happened and action must be taken immediately */
        Critical    = 2, /* A critical condition occurred like a serious hardware/software failure */
        Error       = 3, /* An error condition, often used by drivers to indicate difficulties with the hardware */
        Warning     = 4, /* A warning, meaning nothing serious by itself but might indicate problems */
        Notice      = 5, /* Nothing serious, but notably nevertheless. Often used to report security events. */
        Info        = 6, /* Informational message e.g. startup information at driver initialization */
        Debug       = 7  /* Debug messages */
    };
    typedef std::chrono::system_clock::time_point Date;

public:
    Event(const std::string& message, const std::string& source = std::string(), LogLevel level = Debug);
    ~Event();

public:
    static const std::string&   getLogLevelString(int level);

public:
    std::string     message;
    std::string     source;
    int             logLevel;
    Date            date;
};

#endif // EVENT_HPP