Gapcli
================================
.. code-block:: text

    Usage: gapcli [options...] <cmd> [args...]
    List of available options:
        -com <path> : serial communication port (examples : /dev/ttyACM0 on Linux, COM3 on Windows)
    List of available commands:
        ping : Test communication, nothing else
        set-log-verbosity debug|info|warning|error : Modify log verbosity level on debug UART
        set-gyroscope on|off: Enable or disable reading the gyroscope
        set-version <version> : Set the version string of the Server, e.g. 1.2.0
        get-version : Get the version string of the Server
        selftest : Ask the Server to perform a hardware selftest, and return the results
        power-save-mode enter|exit : Make Server enter or exit power saving mode
