//-----------------------------------------------------------------------------
// Title: [IoTivity][Blood Pressure Monitor] Atomic Measurement
// Description: Defines "oic.r.bloodpressuremonitor-am" and its behaviors
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
#include "bloodpressure0.h"
#include "../common.h"

#include <time.h>   

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define TAG "SERVER-BLOODPRESSURE-0"

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

/* Structure to represent a resource */
typedef struct BLOODPRESSURE0RESOURCE{
    OCResourceHandle handle;
} BloodPressure0Resource;

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

static BloodPressure0Resource BP0;

const char *gBP0ResourceType = "oic.wk.atomicmeasurement";
const char *gBP0ResourceUri = "/BloodPressureMonitorAMResURI";

int diastolicBP = 120;
int systolicBP = 80;
int pulse_rate = 58;

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------

OCRepPayload* getBP0Payload(const char* uri, const char * query, OCEntityHandlerResult * ehResult);

/* This method converts the payload to JSON format */
OCRepPayload* constructBP0Response (OCEntityHandlerRequest *ehRequest, OCEntityHandlerResult * ehResult);

/* Following methods process the GET */
OCEntityHandlerResult ProcessBP0GetRequest (OCEntityHandlerRequest *ehRequest,
                                         OCRepPayload **payload);

int createBP0ResourceEx (const char *uri, BloodPressure0Resource *BP0Resource);       

//-----------------------------------------------------------------------------
// Callback functions
//-----------------------------------------------------------------------------

/* Entity Handler callback functions */
OCEntityHandlerResult
BP0OCEntityHandlerCb (OCEntityHandlerFlag flag,
        OCEntityHandlerRequest *entityHandlerRequest);

//-----------------------------------------------------------------------------
// Function Implementations
//-----------------------------------------------------------------------------

void generateRandomValue() {
    srand(time(NULL));
    int r = rand() % 20;
    diastolicBP = 110 + r;  // 110~120 ranged value generate

    r = rand() % 20;
    systolicBP = 70 + r;    // 70~90 ranged value generate

    r = rand() % 20;
    pulse_rate = 50 + r;    // 50~70 ranged value generate

    OIC_LOG_V(INFO, TAG, "generated random value diastolic[%d] systolic[%d] pulserate[%d]", diastolicBP, systolicBP, pulse_rate);
}


OCRepPayload *getBP0Payload(const char *uri, const char *query, OCEntityHandlerResult *ehResult)
{
    
    OIC_LOG_V(INFO, TAG, "query[%s]", query);
    *ehResult = OC_EH_OK;

    generateRandomValue();

    size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };
    if (strcmp(query, "if=oic.if.baseline") == 0) {
        // IUT responds to /BloodPressureMonitorAMResURI?if=oic.if.baseline

        OCRepPayload* payload = OCRepPayloadCreate();
        if(!payload)
        {
            OIC_LOG(ERROR, TAG, PCF("Failed to allocate Payload"));
            return nullptr;
        }

        dimensions[0] = 2;
        const char *rtStr[] = {"oic.r.bloodpressuremonitor-am", "oic.wk.atomicmeasurement"};
        OCRepPayloadSetStringArray(payload, "rt", (const char **)rtStr, dimensions);

        dimensions[0] = 3;
        const char *ifStr[] = {"oic.if.b", "oic.if.ll", "oic.if.baseline"};
        OCRepPayloadSetStringArray(payload, "if", (const char **)ifStr, dimensions);

        dimensions[0] = 1;
        const char *rtsmStr[] = {"oic.r.blood.pressure"};
        OCRepPayloadSetStringArray(payload, "rts-m", (const char **)rtsmStr, dimensions);

        dimensions[0] = 2;
        const char *rtsStr[] = {"oic.r.blood.pressure", "oic.r.pulserate"};
        OCRepPayloadSetStringArray(payload, "rts", (const char **)rtsStr, dimensions);

        OCRepPayloadSetPropString(payload, "id", "user_example_id");

        OCRepPayload* href1 = OCRepPayloadCreate();
        OCRepPayloadSetPropString(href1, "href", "/myBloodPressureResURI");
        dimensions[0] = 1;
        const char *href1RtStr[] = {"oic.r.blood.pressure"};
        OCRepPayloadSetStringArray(href1, "rt", (const char **)href1RtStr, dimensions);
        dimensions[0] = 2;
        const char *href1IfStr[] = {"oic.if.s", "oic.if.baseline"};
        OCRepPayloadSetStringArray(href1, "if", (const char **)href1IfStr, dimensions);

        OCRepPayload* bp_p = OCRepPayloadCreate();
        OCRepPayloadSetPropInt(bp_p, "bm", 2);
        OCRepPayloadSetPropObject(href1, "p", bp_p);

        OCRepPayload* href2 = OCRepPayloadCreate();
        OCRepPayloadSetPropString(href2, "href", "/myPulseRateResURI");
        dimensions[0] = 1;
        const char *href2RtStr[] = {"oic.r.pulserate"};
        OCRepPayloadSetStringArray(href2, "rt", (const char **)href2RtStr, dimensions);
        dimensions[0] = 2;
        const char *href2IfStr[] = {"oic.if.s", "oic.if.baseline"};
        OCRepPayloadSetStringArray(href2, "if", (const char **)href2IfStr, dimensions);

        OCRepPayload* pr_p = OCRepPayloadCreate();
        OCRepPayloadSetPropInt(pr_p, "bm", 2);
        OCRepPayloadSetPropObject(href2, "p", pr_p);

        OCRepPayload * hrefs[] = { href1, href2};
        dimensions[0] = 2;
        OCRepPayloadSetPropObjectArray(payload, "links", (const OCRepPayload **)hrefs, dimensions);
        return payload;
    }
    else if (strcmp(query, "if=oic.if.b") == 0) {
        // IUT responds to /BloodPressureMonitorAMResURI?if=oic.if.b
        OCRepPayload* payload = OCRepPayloadCreate();
        if(!payload)
        {
            OIC_LOG(ERROR, TAG, PCF("Failed to allocate Payload"));
            return nullptr;
        }

        OCRepPayload* child1Rep = OCRepPayloadCreate();
        OCRepPayloadSetPropInt(child1Rep, "systolic", systolicBP);
        OCRepPayloadSetPropInt(child1Rep, "diastolic", diastolicBP);
        OCRepPayloadSetPropString(child1Rep, "units", "mmHg");
        OCRepPayloadSetPropObject(payload, "rep", child1Rep);
        OCRepPayloadSetPropString(payload, "href", "/myBloodPressureResURI");

        OCRepPayload* child2 = OCRepPayloadCreate();

        OCRepPayload* child2Rep = OCRepPayloadCreate();
        OCRepPayloadSetPropInt(child2Rep, "pulserate", pulse_rate);
        OCRepPayloadSetPropObject(child2, "rep", child2Rep);
        OCRepPayloadSetPropString(child2, "href", "/myPulseRateResURI");

        OCRepPayloadAppend(payload, child2);
        return payload;
    }
    else if (strcmp(query, "if=oic.if.ll") == 0) {
        // IUT responds to /BloodPressureMonitorAMResURI?if=oic.if.ll
        OCRepPayload* payload = OCRepPayloadCreate();
        if(!payload)
        {
            OIC_LOG(ERROR, TAG, PCF("Failed to allocate Payload"));
            return nullptr;
        }

        OCRepPayloadSetPropString(payload, "href", "/myBloodPressureResURI");
        dimensions[0] = 1;
        const char *chile1rtStr[] = {"oic.r.blood.pressure"};
        OCRepPayloadSetStringArray(payload, "rt", (const char **)chile1rtStr, dimensions);
        dimensions[0] = 2;
        const char *child1ifStr[] = {"oic.if.s", "oic.if.baseline"};
        OCRepPayloadSetStringArray(payload, "if", (const char **)child1ifStr, dimensions);

        OCRepPayload* bp_p = OCRepPayloadCreate();
        OCRepPayloadSetPropInt(bp_p, "bm", 2);
        OCRepPayloadSetPropObject(payload, "p", bp_p);

        OCRepPayload* child2 = OCRepPayloadCreate();
        OCRepPayloadSetPropString(child2, "href", "/myPulseRateResURI");
        dimensions[0] = 1;
        const char *chile2rtStr[] = {"oic.r.pulserate"};
        OCRepPayloadSetStringArray(child2, "rt", (const char **)chile2rtStr, dimensions);
        dimensions[0] = 2;
        const char *child2ifStr[] = {"oic.if.s", "oic.if.baseline"};
        OCRepPayloadSetStringArray(child2, "if", (const char **)child2ifStr, dimensions);

        OCRepPayload* pr_p = OCRepPayloadCreate();
        OCRepPayloadSetPropInt(pr_p, "bm", 2);
        OCRepPayloadSetPropObject(child2, "p", pr_p);

        OCRepPayloadAppend(payload, child2);
        return payload;
    }
    else if (strncmp(query, "if=", 3) == 0) {
        *ehResult = OC_EH_FORBIDDEN;
        OIC_LOG(ERROR, TAG, PCF("Interface not supported!"));
        return nullptr;
    }
    else if (strncmp(query, "rt=", 3) == 0) {
        // IUT responds to Batch RETRIEVE using an 'rt' query
        *ehResult = OC_EH_FORBIDDEN;
        OIC_LOG(ERROR, TAG, PCF("rt query not supported!"));
        return nullptr;
    }
    else {
        // IUT responds to RETRIEVE without an Interface query
        // Default Interface of an Atomic Measurement Resourt Type is oic.if.b
        OCRepPayload* payload = OCRepPayloadCreate();
        if(!payload)
        {
            OIC_LOG(ERROR, TAG, PCF("Failed to allocate Payload"));
            return nullptr;
        }

        OCRepPayload* child1Rep = OCRepPayloadCreate();
        OCRepPayloadSetPropInt(child1Rep, "systolic", systolicBP);
        OCRepPayloadSetPropInt(child1Rep, "diastolic", diastolicBP);
        OCRepPayloadSetPropString(child1Rep, "units", "mmHg");
        OCRepPayloadSetPropObject(payload, "rep", child1Rep);
        OCRepPayloadSetPropString(payload, "href", "/myBloodPressureResURI");

        OCRepPayload* child2 = OCRepPayloadCreate();

        OCRepPayload* child2Rep = OCRepPayloadCreate();
        OCRepPayloadSetPropInt(child2Rep, "pulserate", pulse_rate);
        OCRepPayloadSetPropObject(child2, "rep", child2Rep);
        OCRepPayloadSetPropString(child2, "href", "/myPulseRateResURI");

        OCRepPayloadAppend(payload, child2);
        return payload;
    }
}

OCRepPayload* constructBP0Response (OCEntityHandlerRequest *ehRequest, OCEntityHandlerResult * ehResult)
{
    if(ehRequest->payload && ehRequest->payload->type != PAYLOAD_TYPE_REPRESENTATION)
    {
        OIC_LOG(ERROR, TAG, PCF("Incoming payload not a representation"));
        return nullptr;
    }

    return getBP0Payload(gBP0ResourceUri, ehRequest->query, ehResult);
}

OCEntityHandlerResult ProcessBP0GetRequest (OCEntityHandlerRequest *ehRequest,
    OCRepPayload **payload)
{
    OCEntityHandlerResult ehResult;

    OCRepPayload *getResp = constructBP0Response(ehRequest, &ehResult);

    if(getResp)
    {
        *payload = getResp;
    }
    else
    {
        ehResult = OC_EH_ERROR;
    }

    return ehResult;
}
 
int threadQuitFlag = 0;
void handleSigIntForTheread(int signum)
{
    if (signum == SIGINT)
    {
        threadQuitFlag = 1;
    }
}

void *valueGenerateForObserveThread(void *data) {
    struct timespec timeout;
    timeout.tv_sec  = 2; 
    timeout.tv_nsec = 0; 

    signal(SIGINT, handleSigIntForTheread);
    while(!threadQuitFlag) {
        generateRandomValue(); 
        OCStackResult result = OCNotifyAllObservers(BP0.handle, OC_NA_QOS);
        if(OC_STACK_NO_OBSERVERS == result) {
            threadQuitFlag = 1;
        }
        nanosleep(&timeout, NULL);
    }
    return NULL;
}

void startObserve() {
    char p[] = "valueGenerateForObserveThread";
    pthread_t p_thread;
    threadQuitFlag = 0;
    int thread_id = pthread_create(&p_thread, NULL, valueGenerateForObserveThread, (void*)p);
    if (thread_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
}

void stopObserve() {
    threadQuitFlag = 1;
}


OCEntityHandlerResult
BP0OCEntityHandlerCb (OCEntityHandlerFlag flag,
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
            if (OC_REST_GET == entityHandlerRequest->method)
            {
                OIC_LOG (INFO, TAG, "Received OC_REST_GET from client");

                ehResult = ProcessBP0GetRequest (entityHandlerRequest, &payload);
            }

            else
            {
                OIC_LOG_V (INFO, TAG, "Received unsupported method %d from client",
                        entityHandlerRequest->method);
                ehResult = OC_EH_METHOD_NOT_ALLOWED;
            }

            if (ehResult == OC_EH_OK && ehResult != OC_EH_FORBIDDEN)
            {
                // Format the response.  Note this requires some info about the request
                response.requestHandle = entityHandlerRequest->requestHandle;
                response.ehResult = ehResult;
                response.payload = reinterpret_cast<OCPayload*>(payload);
                response.numSendVendorSpecificHeaderOptions = 0;
                memset(response.sendVendorSpecificHeaderOptions, 0, sizeof response.sendVendorSpecificHeaderOptions);
                memset(response.resourceUri, 0, sizeof(response.resourceUri));
                // Indicate that response is NOT in a persistent buffer
                response.persistentBufferFlag = 0;

                // Send the response
                if (OCDoResponse(&response) != OC_STACK_OK)
                {
                    OIC_LOG(ERROR, TAG, "Error sending response");
                    ehResult = OC_EH_ERROR;
                }
            } else if(ehResult == OC_EH_FORBIDDEN) {
                OIC_LOG(INFO, TAG, "FORBIDDEN RESULT"); // FORBIDDEN RESULT
                
                response.requestHandle = entityHandlerRequest->requestHandle;
                response.ehResult = ehResult;
                response.payload = nullptr;//reinterpret_cast<OCPayload*>(payload);
                response.numSendVendorSpecificHeaderOptions = 0;
                memset(response.sendVendorSpecificHeaderOptions, 0, sizeof response.sendVendorSpecificHeaderOptions);
                memset(response.resourceUri, 0, sizeof(response.resourceUri));
                // Indicate that response is NOT in a persistent buffer
                response.persistentBufferFlag = 0;

                // Send the response
                if (OCDoResponse(&response) != OC_STACK_OK)
                {
                    OIC_LOG(ERROR, TAG, "Error sending response");
                    ehResult = OC_EH_ERROR;
                }
            }
        }
    }
    
    else {
        OIC_LOG(ERROR, TAG, "Flag Error!");
    }

    if(flag & OC_OBSERVE_FLAG) {
        if(OC_OBSERVE_REGISTER == entityHandlerRequest->obsInfo.action) {
            ehResult = OC_EH_OK;
            OIC_LOG(DEBUG, TAG, "OBSERVER REGISTER RECEIVED.");
            startObserve();
        }
        else if(OC_OBSERVE_DEREGISTER == entityHandlerRequest->obsInfo.action) {
            ehResult = OC_EH_OK;
            OIC_LOG(ERROR, TAG, "OBSERVER DEREGISTER RECEIVED.");
            stopObserve();
        }

        response.requestHandle = entityHandlerRequest->requestHandle;
        response.ehResult = ehResult;
        response.payload = nullptr;//reinterpret_cast<OCPayload*>(payload);
        response.numSendVendorSpecificHeaderOptions = 0;
        memset(response.sendVendorSpecificHeaderOptions, 0, sizeof response.sendVendorSpecificHeaderOptions);
        memset(response.resourceUri, 0, sizeof(response.resourceUri));
        // Indicate that response is NOT in a persistent buffer
        response.persistentBufferFlag = 0;
    }

    return ehResult;
}

int createBP0Resource () {
    createBP0ResourceEx(gBP0ResourceUri, &BP0);
    return 0;
}

int createBP0ResourceEx (const char *uri, BloodPressure0Resource *BP0Resource)
{
    if (!uri)
    {
        OIC_LOG(ERROR, TAG, "Resource URI cannot be NULL");
        return -1;
    }

    OCStackResult res = OCCreateResource(&(BP0Resource->handle),
            gBP0ResourceType,
            OC_RSRVD_INTERFACE_BATCH,       
            gBP0ResourceUri,
            BP0OCEntityHandlerCb,
            NULL,
            OC_DISCOVERABLE | OC_OBSERVABLE 
#if IS_SECURE_MODE
            | OC_SECURE
#endif
        );
    
    OCResourceHandle rHandle = OCGetResourceHandleAtUri(gBP0ResourceUri);
    OCBindResourceInterfaceToResource(rHandle, OC_RSRVD_INTERFACE_LL);   
    OCBindResourceTypeToResource(rHandle, "oic.r.bloodpressuremonitor-am");
    OIC_LOG_V(INFO, TAG, "Created BP0 resource with result: %s", getResult(res));

    return 0;
}
