//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: CNTS_MSG_SERVICE_STARTED
//
// MessageText:
//
// "%1" started successfully.
//
#define CNTS_MSG_SERVICE_STARTED         0x40000001L

//
// MessageId: CNTS_MSG_SERVICE_INSTALLED
//
// MessageText:
//
// "%1" was successfully installed.
//
#define CNTS_MSG_SERVICE_INSTALLED       0x40000002L

//
// MessageId: CNTS_MSG_SERVICE_STOPPED
//
// MessageText:
//
// "%1" stopped successfully.
//
#define CNTS_MSG_SERVICE_STOPPED         0x40000003L

//
// MessageId: CNTS_MSG_SERVICE_PAUSED
//
// MessageText:
//
// "%1" paused successfully.
//
#define CNTS_MSG_SERVICE_PAUSED          0x40000004L

//
// MessageId: CNTS_MSG_SERVICE_CONTINUED
//
// MessageText:
//
// "%1" continued successfully.
//
#define CNTS_MSG_SERVICE_CONTINUED       0x40000005L

//
// MessageId: CNTS_MSG_SERVICE_FAIL_CONNECT_SCM
//
// MessageText:
//
// Failed to connect to the Service Control Manager, Error: %1.
//
#define CNTS_MSG_SERVICE_FAIL_CONNECT_SCM 0xC0000006L

//
// MessageId: CNTS_MSG_SERVICE_FAIL_OPEN_SERVICE
//
// MessageText:
//
// Failed to open the service, Error: %1.
//
#define CNTS_MSG_SERVICE_FAIL_OPEN_SERVICE 0xC0000007L

//
// MessageId: CNTS_MSG_SERVICE_FAIL_DELETE_SERVICE
//
// MessageText:
//
// Failed to delete the service, Error: %1.
//
#define CNTS_MSG_SERVICE_FAIL_DELETE_SERVICE 0xC0000008L

//
// MessageId: CNTS_MSG_SERVICE_SHUTDOWN
//
// MessageText:
//
// "%1" was successfully shutdown.
//
#define CNTS_MSG_SERVICE_SHUTDOWN        0x40000009L

//
// MessageId: CNTS_MSG_SERVICE_FAIL_START
//
// MessageText:
//
// Failed to start the service, Error: %1.
//
#define CNTS_MSG_SERVICE_FAIL_START      0xC000000AL

//
// MessageId: CNTS_MSG_SERVICE_FAIL_STOP
//
// MessageText:
//
// Failed to stop the service, Error: %1.
//
#define CNTS_MSG_SERVICE_FAIL_STOP       0xC000000BL

//
// MessageId: CNTS_MSG_SERVICE_FAIL_CONTINUE
//
// MessageText:
//
// Failed to continue the service, Error: %1.
//
#define CNTS_MSG_SERVICE_FAIL_CONTINUE   0xC000000CL

//
// MessageId: CNTS_MSG_SERVICE_FAIL_PAUSE
//
// MessageText:
//
// Failed to pause the service, Error: %1.
//
#define CNTS_MSG_SERVICE_FAIL_PAUSE      0xC000000DL

//
// MessageId: CNTS_MSG_SERVICE_FAIL_CHANGE_STATE
//
// MessageText:
//
// Failed to wait for service to change to desired state, Desired State: %1, Error: %2.
//
#define CNTS_MSG_SERVICE_FAIL_CHANGE_STATE 0xC000000EL

//
// MessageId: CNTS_MSG_SERVICE_FAIL_QUERY_STATUS
//
// MessageText:
//
// Failed to query the status of the service, Error: %1.
//
#define CNTS_MSG_SERVICE_FAIL_QUERY_STATUS 0xC000000FL

//
// MessageId: CNTS_MSG_SERVICE_FULLSTRING
//
// MessageText:
//
// %1.
//
#define CNTS_MSG_SERVICE_FULLSTRING      0x40000010L

//
// MessageId: MSG_SERVICE_SET_FREQUENCY
//
// MessageText:
//
// Beep Interval has been set to %1 milliseconds.
//
#define MSG_SERVICE_SET_FREQUENCY        0x40000011L

