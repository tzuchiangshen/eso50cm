#ifndef CONSOLE_H
#define CONSOLE_H

typedef enum _ProcessStatus { ProcessUnknown=-1, ProcessConnected, ProcessDisconnected, ProcessError} ProcessStatus;
typedef enum _TelescopeStatus { TelescopeUnknown=-1, TelescopeStop, TelescopeMoving, TelescopeError} TelescopeStatus;
typedef enum _TrackingStatus { TrackingUnknown=-1, TrackingOn, TrackingOff, TrackingError} TrackingStatus;
#endif // CONSOLE_H
