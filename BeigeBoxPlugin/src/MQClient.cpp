//
// Created by Zero on 9/27/2024.
//

#include "include/MQClient.h"

#define BUFFER_LENGTH 256

#ifdef IBM

HRESULT createWinMQ(LPWSTR pathName, PSECURITY_DESCRIPTOR psecurityDescriptor, LPWSTR outputFormatName, DWORD *outputFormatNameLength)
{
    const int NUM_PROPERTIES = 2;

    //queue property structure
    MQQUEUEPROPS queueProperties;
    MQPROPVARIANT queuePropertyVariants[NUM_PROPERTIES];
    QUEUEPROPID queuepropid[NUM_PROPERTIES];
    HRESULT queueStatus[NUM_PROPERTIES];
    HRESULT hresult = MQ_OK;

    //validation
    if(pathName == NULL || outputFormatName == NULL || outputFormatNameLength == NULL)
    {
        hresult = MQ_ERROR_INVALID_PARAMETER;
    }
    else
    {
        //set queue properties
        DWORD propId = 0;
        queuepropid[propId] = PROPID_Q_PATHNAME;
        queuePropertyVariants[propId].vt = VT_LPWSTR;
        queuePropertyVariants[propId].pwszVal = pathName;
        propId++;

        WCHAR label[MQ_MAX_MSG_LABEL_LEN] = L"BeigeBox Queue";
        queuepropid[propId] = PROPID_Q_LABEL;
        queuePropertyVariants[propId].vt = VT_LPWSTR;
        queuePropertyVariants[propId].pwszVal = label;

        //init queue property structure
        queueProperties.cProp = propId;
        queueProperties.aPropID = queuepropid;
        queueProperties.aPropVar = queuePropertyVariants;
        queueProperties.aStatus = queueStatus;

        //create queue
        WCHAR  formatNameBuffer[BUFFER_LENGTH];
        DWORD formatBufferLength = BUFFER_LENGTH;
        hresult = MQCreateQueue(psecurityDescriptor, &queueProperties, formatNameBuffer, &formatBufferLength);

        if(hresult == MQ_OK || hresult == MQ_INFORMATION_PROPERTY)
        {
            if(*outputFormatNameLength >= formatBufferLength)
            {
                wcsncpy_s(outputFormatName, *outputFormatNameLength - 1, formatNameBuffer,_TRUNCATE);
                outputFormatName[*outputFormatNameLength - 1] = L'\0';
                *outputFormatNameLength = formatBufferLength;
            }
        }


    }

    return hresult;
}
#endif