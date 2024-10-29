# HC-05 library

## Purpose:

The library is designed to enable communication between multiple wireless (via bluetooth 2.0) modules as part of an interactive game. 
If one of the modules loses connection, the master module will automatically reconnect with a new module.

### Master Module Tasks:

The master module's task is to find nearby devices within its range. It should then query each device for information such as name and MAC address, allowing the user to control which slave module the master should connect to. If the connection is lost, the master will continue searching for modules.

### Slave Module Tasks:

Enables pairing mode independently, allowing connection even if a specific master does not initiate the connection.

