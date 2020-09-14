
Getting Started
===============

Installation
------------

#. To get started, download and install `VSCode <https://code.visualstudio.com>`_. 
#. Next, install `PlatformIO <https://platformio.org>`_ extension for VSCode. 
#. Create a new project by selecting "New Project" under quick access menu in PlatformIO Home. 
#. Enter "MyApp" as the project name and "Adafruit Feather M0" as the board type. 
#. Lastly, select "Arduino" as Framework then click "Done".

PlatformIO now proceeds to install required libraries, wait for it to finish. Then, after opening the newly created project. Type the following commands: 

.. code-block:: bash
    :linenos:

    gitsubmodule add https://github.com/OPEnSLab-OSU/OPEnSamplerFramework lib/Framework
    gitsubmodule init
    gitsubmodule update

Now enter the following:

.. code-block:: bash
    :linenos:

    git config push.recurseSubmodules check
    git config submodule.recurse true 

The above code prevents the main project from being pushed if there is any unpushed modifcations to the submodule. Next, modify `platformio.ini` to include required external libraries:

.. code-block:: ini
    :linenos:
    
    [env]
    platform = atmelsam
    board = adafruit_feather_m0
    framework = arduino
    monitor_speed = 115200
    monitor_flags =
        --raw
        --echo
    lib_deps = 
        ArduinoJson
        StreamUtils
        WiFi101
        Low-Power
        868
        DS3232RTC


Configuring .ini file
---------------------

- :code:`monitor_flags: --raw` Enables ANSI encoding so colored output can be interpreted by the built-in serial monitor
- :code:`monitor_flags: --echo` Makes the built-in serial monitor mirrors back what's being typed
- :code:`ArduinoJson` An excellent third-party library for manipulating json data 
- :code:`StreamUtils` Library that accelerate streaming serial data over multiple channels (WiFi, serial bus)
- :code:`WiFi101` Library for interfacing with the onboard WiFi controller
- :code:`Low-Power` Library for putting the cpu into low-power mode
- :code:`Arduino SD` Simple library for reading/writing to SD card
- :code:`DS3232RTC` Library for controlling a much more accurate onboard DS3231 real-time clock

Please refer to `offical PlatformIO.ini guide <https://docs.platformio.org/en/latest/projectconf/index.html>`_ for more information.


Creating your first app
-----------------------

.. role:: cpp(code)
    :language: c++

The framework encourages you to put your entire logic in an container called "App". First, create a new folder call "App", then inside the folder create two files: `app.hpp` and `app.cpp` respectively. This way, the application logic can be splited into multiple files, simply by :cpp:`#include <App/app.hpp>`

Open `app.hpp` and include the following header: :cpp:`#include <KPController>`. We will next create a subclass of `KPController`---which itself is nothing more than a data container with some lifecycle mehods added.  asdf 

.. code-block:: c++
    :linenos:

    #pragma once
    #include <KPController>

    class App : public KPController {
    public:

        void setup() override {
            Serial.begin(115200)
        }

        void update() override {

        }
    };


Using built-in classes: adding serial input
-------------------------------------------

The framework provides a number of default classes for coding the sampler. Feel free to look around in the :doc:`Library API <../api/library_root>`. In this section, we will add serial input capability to the application. Modify the application class to match the following:

.. code-block:: c++
    :linenos:

    #pragma once
    #include <KPController>
    #include <KPSerialInputObserver>

    class App : public KPController, public KPSerialInputObserver {
    public:

        void setup() override {
            Serial.begin(115200)
        }

        void update() override {

        }

    protected:

        void commandReceived(const char * line, size_t size) override {
            println("Input: ", line);
        }
    };


Upload your application
-----------------------

Next click on the upload button |upload| located in the bottom task bar of VSCode then click the built-in serial monitor button |monitor|. You should be able be able to see "Input: <your input>" echo back to the monitor on enter.

.. |upload| image:: ../_static/upload_btn.png
    :width: 120px

.. |monitor| image:: ../_static/monitor_btn.png
    :width: 120px