//-----------------------------------------------------------------------------
// Title: [IoTivity][Blood Pressure Monitor] Server
// Description: Defines "oic.d.bloodpressuremonitor" and its information
//-----------------------------------------------------------------------------

#include "iotivity_config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif
#include <signal.h>
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif
#include "ocstack.h"
#include "logger.h"
#include "server.h"

#define TAG "SERVER"

int gQuitFlag = 0;
static char CRED_FILE[] = "server.dat";
static char INTROSPECTION_FILE[] = "server.idd.dat";

/* SIGINT handler: set gQuitFlag to 1 for graceful termination */
void handleSigInt(int signum)
{
    if (signum == SIGINT)
    {
        gQuitFlag = 1;
    }
}

FILE* server_fopen(const char *path, const char *mode)
{
    if (0 == strcmp(path, OC_SECURITY_DB_DAT_FILE_NAME))
    {
        return fopen(CRED_FILE, mode);
    }
    else if (0 == strcmp(path, OC_INTROSPECTION_FILE_NAME))
    { 
        return fopen(INTROSPECTION_FILE, mode);
    }
    else
    {
        return fopen(path, mode);
    }
}

void *iotivityThread(void *data) {
    struct timespec timeout;

    timeout.tv_sec  = 0;
    timeout.tv_nsec = 100000000L;

    // Break from loop with Ctrl-C
    OIC_LOG(INFO, TAG, "Entering ocserver main loop...");
    signal(SIGINT, handleSigInt);
    while (!gQuitFlag)
    {
        if (OCProcess() != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "OCStack process error");
            return 0;
        }
        nanosleep(&timeout, NULL);
    }

    OIC_LOG(INFO, TAG, "Exiting ocserver main loop...");

    if (OCStop() != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack process error");
    }
}

// Platform Info
/**
 * This structure describes the platform properties. All non-Null properties will be
 * included in a platform discovery request.
 * @deprecated: Use OCSetPropertyValue  to set platform value.
 */
const char *gPlatformID = "12341234-1234-1234-1234-123412341234";
const char *gManufacturerName = "Electronics and Telecommunications Research Institute";
const char *gManufacturerLink = "https://www.etri.re.kr";
const char *gModelNumber = "myModelNumber";
const char *gDateOfManufacture = "2017-11-09";
const char *gPlatformVersion = "1.0";
const char *gOperatingSystemVersion = "Ubuntu 16.04 LTS";
const char *gHardwareVersion = "MacBook Pro";
const char *gFirmwareVersion = "1.0";
const char *gSupportLink = "https://www.etri.re.kr";
const char *gSystemTime = "2011-08-30T13:22:53.108Z";

// Device Info
/**
 * This structure is expected as input for device properties.
 * device name is mandatory and expected from the application.
 * device id of type UUID will be generated by the stack.
 * @deprecated: Use OCSetPropertyValue  to set device value.
 */

// Blood Pressure Monitor
const char *gDeviceTypeBloodPressure = "oic.d.bloodpressuremonitor";
const char *gDeviceNameBloodPressure = "Blood Pressure Monitor";

const char *gSpecVersion = "ocf.1.1.0";
const char *gDataModelVersions = "ocf.res.1.1.0,ocf.sh.1.1.0";
const char *gProtocolIndependentID = "12341234-1234-1234-1234-123412341234";

OCPlatformInfo platformInfo;

void DeletePlatformInfo()
{
    free(platformInfo.platformID);
    free(platformInfo.manufacturerName);
    free(platformInfo.manufacturerUrl);
    free(platformInfo.modelNumber);
    free(platformInfo.dateOfManufacture);
    free(platformInfo.platformVersion);
    free(platformInfo.operatingSystemVersion);
    free(platformInfo.hardwareVersion);
    free(platformInfo.firmwareVersion);
    free(platformInfo.supportUrl);
    free(platformInfo.systemTime);
}

bool DuplicateString(char** targetString, const char* sourceString)
{
    if(!sourceString)
    {
        return false;
    }
    else
    {
        *targetString = (char *) malloc(strlen(sourceString) + 1);

        if(*targetString)
        {
            strncpy(*targetString, sourceString, (strlen(sourceString) + 1));
            return true;
        }
    }
    return false;
}

OCStackResult SetPlatformInfo(const char* platformID, const char *manufacturerName,
    const char *manufacturerUrl, const char *modelNumber, const char *dateOfManufacture,
    const char *platformVersion, const char* operatingSystemVersion, const char* hardwareVersion,
    const char *firmwareVersion, const char* supportUrl, const char* systemTime)
{

    bool success = true;

    if(manufacturerName != NULL && (strlen(manufacturerName) > MAX_PLATFORM_NAME_LENGTH))
    {
        return OC_STACK_INVALID_PARAM;
    }

    if(manufacturerUrl != NULL && (strlen(manufacturerUrl) > MAX_PLATFORM_URL_LENGTH))
    {
        return OC_STACK_INVALID_PARAM;
    }

    if(!DuplicateString(&platformInfo.platformID, platformID))
    {
        success = false;
    }

    if(!DuplicateString(&platformInfo.manufacturerName, manufacturerName))
    {
        success = false;
    }

    if(!DuplicateString(&platformInfo.manufacturerUrl, manufacturerUrl))
    {
        success = false;
    }

    if(!DuplicateString(&platformInfo.modelNumber, modelNumber))
    {
        success = false;
    }

    if(!DuplicateString(&platformInfo.dateOfManufacture, dateOfManufacture))
    {
        success = false;
    }

    if(!DuplicateString(&platformInfo.platformVersion, platformVersion))
    {
        success = false;
    }

    if(!DuplicateString(&platformInfo.operatingSystemVersion, operatingSystemVersion))
    {
        success = false;
    }

    if(!DuplicateString(&platformInfo.hardwareVersion, hardwareVersion))
    {
        success = false;
    }

    if(!DuplicateString(&platformInfo.firmwareVersion, firmwareVersion))
    {
        success = false;
    }

    if(!DuplicateString(&platformInfo.supportUrl, supportUrl))
    {
        success = false;
    }

    if(!DuplicateString(&platformInfo.systemTime, systemTime))
    {
        success = false;
    }

    if(success)
    {
        return OC_STACK_OK;
    }

    DeletePlatformInfo();
    return OC_STACK_ERROR;
}

#define VERIFY_SUCCESS(op)                          \
{                                                   \
    if (op !=  OC_STACK_OK)                         \
    {                                               \
        OIC_LOG_V(FATAL, TAG, "%s failed!!", #op);  \
        goto exit;                                  \
    }                                               \
}

OCStackResult SetDeviceInfo(char c)
{
    OCResourceHandle resourceHandle = OCGetResourceHandleAtUri(OC_RSRVD_DEVICE_URI);
    if (resourceHandle == NULL)
    {
        OIC_LOG(ERROR, TAG, "Device Resource does not exist.");
        goto exit;
    }

    VERIFY_SUCCESS(OCBindResourceTypeToResource(resourceHandle, gDeviceTypeBloodPressure));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME, gDeviceNameBloodPressure));
    
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION, gSpecVersion));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DATA_MODEL_VERSION,
                                      gDataModelVersions));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_PROTOCOL_INDEPENDENT_ID,
                                      gProtocolIndependentID));

    OIC_LOG(INFO, TAG, "Device information(Blood Pressure Monitor) initialized successfully.");
    
    return OC_STACK_OK;

exit:
    return OC_STACK_ERROR;
}

int main()
{
    OIC_LOG(DEBUG, TAG, "OCServer is starting...");

    char command = 'P';

    
#if IS_SECURE_MODE
    // Initialize Persistent Storage for SVR database
    OCPersistentStorage ps = { server_fopen, fread, fwrite, fclose, unlink };
    OCRegisterPersistentStorageHandler(&ps);
#endif

    if (OCInit(NULL, 0, OC_SERVER) != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack init error");
        return 0;
    }

    OCStackResult registrationResult =
    SetPlatformInfo(gPlatformID, gManufacturerName, gManufacturerLink, gModelNumber,
                    gDateOfManufacture, gPlatformVersion, gOperatingSystemVersion,
                    gHardwareVersion, gFirmwareVersion, gSupportLink, gSystemTime);
    if (registrationResult != OC_STACK_OK)
    {
        OIC_LOG(INFO, TAG, "Platform info setting failed locally!");
        exit (EXIT_FAILURE);
    }

    registrationResult = OCSetPlatformInfo(platformInfo);    
    if (registrationResult != OC_STACK_OK)
    {
        OIC_LOG(INFO, TAG, "Platform Registration failed!");
        exit (EXIT_FAILURE);
    } else {
        OIC_LOG(INFO, TAG, "Platform information initialized successfully.");
    }

    registrationResult = SetDeviceInfo(command);
    if (registrationResult != OC_STACK_OK)
    {
        OIC_LOG(INFO, TAG, "Device Registration failed!");
        exit (EXIT_FAILURE);
    }

    //Declare and create the example resource: BP
    createBP0Resource();
    createBP1Resource();
    createBP2Resource();

    int status;
    pthread_t p_thread[3];
    int thread_id;

    char p2[] = "iotivity_thread";
    thread_id = pthread_create(&p_thread[1], NULL, iotivityThread, (void *)p2);
    if (thread_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }


    pthread_join(p_thread[1], (void **)&status);

    return 0;
}
