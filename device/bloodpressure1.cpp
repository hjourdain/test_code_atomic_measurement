//-----------------------------------------------------------------------------
// Title: [IoTivity][Blood Pressure Monitor] Linked Resource Type: Blood Pressure
// Description: Defines "oic.r.blood.pressure" and its behaviors
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
#include "ocpayload.h"
#include "bloodpressure1.h"
#include "../common.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define TAG "SERVER-BLOODPRESSURE-1"

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

/* Structure to represent a resource */
typedef struct BLOODPRESSURE1RESOURCE{
    OCResourceHandle handle;
} BloodPressure1Resource;

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

static BloodPressure1Resource BP1;

const char *gBP1ResourceType= "oic.r.blood.pressure";
const char *gBP1ResourceUri= "/myBloodPressureResURI";

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------

OCRepPayload* getBP1Payload(const char* uri);

/* This method converts the payload to JSON format */
OCRepPayload* constructBP1Response (OCEntityHandlerRequest *ehRequest);

/* Following methods process the GET requests */
OCEntityHandlerResult ProcessBP1GetRequest (OCEntityHandlerRequest *ehRequest,
                                         OCRepPayload **payload);

int createBP1ResourceEx (const char *uri, BloodPressure1Resource *BP1Resource);       

//-----------------------------------------------------------------------------
// Callback functions
//-----------------------------------------------------------------------------

/* Entity Handler callback functions */
OCEntityHandlerResult
BP1OCEntityHandlerCb (OCEntityHandlerFlag flag,
        OCEntityHandlerRequest *entityHandlerRequest);

//-----------------------------------------------------------------------------
// Function Implementations
//-----------------------------------------------------------------------------
OCRepPayload* getBP1Payload(const char* uri)
{
    OCRepPayload* payload = OCRepPayloadCreate();
    if(!payload)
    {
        OIC_LOG(ERROR, TAG, PCF("Failed to allocate Payload"));
        return nullptr;
    }
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };

    dimensions[0] = 1;
    const char *rtStr[] = {"oic.r.blood.pressure"};
    OCRepPayloadSetStringArray(payload, "rt", (const char **)rtStr, dimensions);
    OCRepPayloadSetPropString(payload, "id", "user_example_id");
    OCRepPayloadSetPropInt(payload, "systolic", 0);
    OCRepPayloadSetPropInt(payload, "diastolic", 0);
    OCRepPayloadSetPropString(payload, "units", "mmHg");

    return payload;
}

OCRepPayload* constructBP1Response (OCEntityHandlerRequest *ehRequest)
{
    if(ehRequest->payload && ehRequest->payload->type != PAYLOAD_TYPE_REPRESENTATION)
    {
        OIC_LOG(ERROR, TAG, PCF("Incoming payload not a representation"));
        return nullptr;
    }

    return getBP1Payload(gBP1ResourceUri);
}

OCEntityHandlerResult ProcessBP1GetRequest (OCEntityHandlerRequest *ehRequest,
    OCRepPayload **payload)
{
    OCEntityHandlerResult ehResult;

    OCRepPayload *getResp = constructBP1Response(ehRequest);

    if(getResp)
    {
        *payload = getResp;
        ehResult = OC_EH_OK;
    }
    else
    {
        ehResult = OC_EH_ERROR;
    }

    return ehResult;
}


OCEntityHandlerResult
BP1OCEntityHandlerCb (OCEntityHandlerFlag flag,
        OCEntityHandlerRequest *entityHandlerRequest,
        void* /*callbackParam*/)
{
    OIC_LOG_V (INFO, TAG, "Inside entity handler - flags: 0x%x", flag);

    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    OCEntityHandlerResponse response = { 0, 0, OC_EH_ERROR, 0, 0, { },{ 0 }, false };
    // Validate pointer
    if (!entityHandlerRequest)
    {
        OIC_LOG (ERROR, TAG, "Invalid request pointer");
        return OC_EH_ERROR;
    }

    OCRepPayload* payload = nullptr;

    if (flag & OC_REQUEST_FLAG)
    {
        OIC_LOG (INFO, TAG, "Flag includes OC_REQUEST_FLAG");
        if (entityHandlerRequest)
        {
            ehResult = OC_EH_FORBIDDEN;
        }
    }

    OCRepPayloadDestroy(payload);
    return ehResult;
}

int createBP1Resource () {
    createBP1ResourceEx(gBP1ResourceUri, &BP1);
    return 0;
}

int createBP1ResourceEx (const char *uri, BloodPressure1Resource *BP1Resource)
{
    if (!uri)
    {
        OIC_LOG(ERROR, TAG, "Resource URI cannot be NULL");
        return -1;
    }

    OCStackResult res = OCCreateResource(&(BP1Resource->handle),
            gBP1ResourceType,
            OC_RSRVD_INTERFACE_DEFAULT,
            gBP1ResourceUri,
            BP1OCEntityHandlerCb,
            NULL,
            OC_OBSERVABLE 
#if IS_SECURE_MODE
            | OC_SECURE
#endif
        );
    OCBindResourceInterfaceToResource(&(BP1Resource->handle), OC_RSRVD_INTERFACE_SENSOR);
    OIC_LOG_V(INFO, TAG, "Created BP1 resource with result: %s", getResult(res));

    return 0;
}
