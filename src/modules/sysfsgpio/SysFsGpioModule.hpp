#pragma once

#include <zmqpp/socket.hpp>
#include <boost/property_tree/ptree.hpp>
#include <zmqpp/reactor.hpp>
#include "SysFSGPIOPin.hpp"

/**
* Handle GPIO management over sysfs.
*/
class SysFsGpioModule
{
public:
    SysFsGpioModule(const boost::property_tree::ptree &config,
            zmqpp::socket *module_manager_pipe,
            zmqpp::context &ctx);

    /**
    * Module's main loop.
    */
    void run();

private:
    zmqpp::socket &pipe_;
    boost::property_tree::ptree config_;

    zmqpp::reactor reactor_;

    bool is_running_;

    /**
    * Handle message coming from the pipe.
    * This is basically handle the stop signal from the module manager.
    */
    void handle_pipe();

    //void handle_

    /**
    * Process the configuration, preparing configured GPIO pin.
    */
    void process_config(const boost::property_tree::ptree &cfg);


    /**
    * Write to "gpio_export_path" so the kernel export the socket to sysfs.
    */
    void export_gpio(int gpio_no);

    zmqpp::context &ctx_;

    std::vector<SysFsGpioPin> gpios_;
};