Bugs
================================

During development, we noticed that from time to time we would receive a response from our server and sometimes not  
(so sometimes we would get the "Got ... response" and sometimes nothing, depending on the command and the timing).

The issue occurs specifically when working on **Windows**.  
If you're on **Linux**, this note likely does not apply to you.

Analysis of the `gapcli` source code
------------------------------------

The Linux `gap-cli` waits to receive a full response from the target card:

*File: `gap-agents/src/communicator_impl_uart_client_linux.c`*

.. code-block:: c

    /**
     * @brief Receive gapcom frame through UART (serial port). This function
     * will try again if not enough bytes are met.
     * 
     * @param self Pointer to communicator interface implementation (self)
     * @param buf Data buffer to fill
     * @param len Number of bytes to read. This parameter is actually ignored as
     * the expected number of bytes can be computed ourselves
     */
    ssize_t communicator_impl_uart_recv_client

The Windows `gap-client`, on the other hand, does **not wait** for the full message. It expects an immediate response:

*File: `gap-agents/src/communicator_impl_uart_client_win32.c`*

.. code-block:: c

    /**
     * @brief Receive gapcom frame through COM port. This function will not try
     * again if the expected number of bytes is not met immediately.
     * 
     * @param self Pointer to communicator interface implementation (self)
     * @param buf Data buffer to fill
     * @param len Number of bytes to read. This parameter is actually ignored as
     * the expected number of bytes can be computed ourselves
     */
    ssize_t communicator_impl_uart_recv_client

Conclusion
----------

If your callback takes a little while to execute, then on Windows, no response will be received.  
However, the same code will work on Linux because the Linux `gap-cli` waits for the full response.  
For example, writing to flash memory (e.g., in `set-version`) takes time, and Linux will wait; Windows won't.
