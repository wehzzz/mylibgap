Architecture
================================
.. code-block:: text

    gap-groupe10/
    ├── README
    ├── developer-guide/                  # Documentation
    │   ├── README
    │   ├── Dockerfile                    # Docker to generate documentation
    │   ├── Doxyfile                      # Doxygen config file
    │   ├── build.sh                      # Script shell (ex: génération automatique)
    │   └── source/                       # Sources Sphinx
    │       ├── architecture.rst
    │       ├── callbacks.rst
    │       ├── conf.py
    │       ├── headers.rst
    │       ├── index.rst
    │       ├── logger.rst
    │       ├── mpu6050.rst
    │       ├── utils.rst
    │       ├── version.rst
    │       └── ...
    └── stm32cubeide/                     # STM32CubeIDE project
        └── armproject/
           ├── armproject.ioc
           ├── Core/
           │   ├── Inc/                  # Headers
           │   │   ├── callbacks.h
           │   │   ├── logger.h
           │   │   ├── main.h
           │   │   ├── mpu6050.h
           │   │   ├── stm32f4xx_hal_conf.h
           │   │   ├── stm32f4xx_it.h
           │   │   ├── utils.h
           │   │   └── version.h
           │   ├── Src/                  # C source files
           │   │   ├── callbacks.c
           │   │   ├── communicator_impl_server.c
           │   │   ├── logger.c
           │   │   ├── main.c
           │   │   ├── mpu6050.c
           │   │   ├── stm32f4xx_hal_msp.c
           │   │   ├── stm32f4xx_it.c
           │   │   ├── syscalls.c
           │   │   ├── sysmem.c
           │   │   ├── system_stm32f4xx.c
           │   │   ├── utils.c
           │   │   └── version.c
           │   └── Startup/
           │       └── ...
           └── Drivers/                 # STM32 HAL drivers
               └── ...
