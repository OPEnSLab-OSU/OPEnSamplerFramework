.. _pg_getting-started:

Getting Started
===============

#. To get started, download and install `VSCode <https://code.visualstudio.com>`_. 
#. Next, install PlatformIO extension for VSCode. 
#. Create a new project by select "New Project" under quick access menu in PlatformIO Home. 
#. Enter "MyApp" as the project name and "Adafruit Feather M0" as the board type. 
#. Lastly, select "Arduino" as Framework then click "Done".

PlatformIO now proceeds to install required libraries, wait for it to finish. Then, after opening  the newly created project. Type the following commands: 

.. code-block:: bash
   :linenos:

   gitsubmodule add https://github.com/OPEnSLab-OSU/OPEnSamplerFramework lib/Framework
   gitsubmodule init
   gitsubmodule update