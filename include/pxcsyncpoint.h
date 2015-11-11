/*
Copyright (c) 2011-2013, Intel Corporation

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/** @file pxcscheduler.h
    Defines the PXCScheduler interface, which is used for controlling
    asynchronous pipeline execution.
  */
#pragma once
#include "pxcbase.h"

/**
   This interface manages asynchronous pipeline execution. The application calls an 
   asynchronous function, which will return immediately with an SP. The application then
   synchronize the SP to retrieve the function results.
 */
class PXCSyncPoint:public PXCBase {
public:

    PXC_CUID_OVERWRITE(PXC_UID('S','H','S','P'));
    PXC_DEFINE_CONST(TIMEOUT_INFINITE,-1);
    PXC_DEFINE_CONST(SYNCEX_LIMIT,64);

    /**
        @brief    The function synchronizes a single SP with timeout.
        @param[in] timeout        The timeout value in ms.
        @return PXC_STATUS_NO_ERROR        Successful execution.
        @return PXC_STATUS_EXEC_TIMEOUT    The timeout value is reached.
    */
    virtual pxcStatus PXCAPI Synchronize(pxcI32 timeout)=0;

    /**
        @brief    The function synchronizes a single SP infinitely.
        @return PXC_STATUS_NO_ERROR        Successful execution.
    */
    pxcStatus __inline Synchronize(void) { return Synchronize(TIMEOUT_INFINITE); }

protected:

    /**
        @reserved
        Internal function. Do not use.
    */
    virtual pxcStatus PXCAPI SynchronizeExINT(pxcI32 n1, PXCSyncPoint **sps, pxcI32 n2, void **events, pxcI32 *idx, pxcI32 timeout)=0;

public:

    /**
        @brief    The function synchronizes multiple SPs as well as OS events. Zero SPs or OS events are skipped automatically.
        If the idx argument is NULL, the function waits until all events are signaled.
        If the idx argument is not NULL, the function waits until any of the events is signaled and returns the index of the signalled events.
        @param[in] n                The number of SPs to be synchronized.
        @param[in] sps              The SP array.
        @param[in] n2               The number of OS events to be synchronized.
        @param[in] events           The OS event array.
        @param[out] idx             The event index, to be returned. 
        @param[in] timeout          The timeout value in ms.
        @return PXC_STATUS_NO_ERROR        Successful execution.
        @return PXC_STATUS_EXEC_TIMEOUT    The timeout value is reached.
    */
    __inline static pxcStatus SynchronizeEx(pxcI32 n, PXCSyncPoint **sps, pxcI32 n2, void **events, pxcI32 *idx, pxcI32 timeout) {
        for (int i=0;i<n;i++)
            if (sps[i]) return sps[i]->SynchronizeExINT(n,sps,n2,events,idx,timeout);
        return PXC_STATUS_HANDLE_INVALID;
    }

    /**
        @brief    The function synchronizes multiple SPs. Zero SPs are skipped automatically.
        If the idx argument is NULL, the function waits until all events are signaled.
        If the idx argument is not NULL, the function waits until any of the events is signaled and returns the index of the signalled events.
        @param[in] n                The number of SPs to be synchronized.
        @param[in] sps              The SP array.
        @param[out] idx             The event index, to be returned. 
        @param[in] timeout          The timeout value in ms.
        @return PXC_STATUS_NO_ERROR        Successful execution.
        @return PXC_STATUS_EXEC_TIMEOUT    The timeout value is reached.
    */
    __inline static pxcStatus SynchronizeEx(pxcI32 n, PXCSyncPoint **sps, pxcI32 *idx, pxcI32 timeout) {
        return SynchronizeEx(n, sps, 0, 0, idx, timeout);
    }

    /**
        @brief    The function synchronizes multiple SPs infinitely. Zero SPs are skipped automatically.
        If the idx argument is NULL, the function waits until all events are signaled.
        If the idx argument is not NULL, the function waits until any of the events is signaled and returns the index of the signalled events.
        @param[in] n                The number of SPs to be synchronized.
        @param[in] sps              The SP array.
        @param[out] idx             The event index, to be returned. 
        @return PXC_STATUS_NO_ERROR        Successful execution.
    */
    __inline static pxcStatus SynchronizeEx(pxcI32 n, PXCSyncPoint **sps, pxcI32 *idx) { 
        return SynchronizeEx(n,sps,idx,(pxcI32)TIMEOUT_INFINITE); 
    }

    /**
        @brief    The function synchronizes multiple SPs infinitely and returns until all events are signalled.
        Zero SPs are skipped automatically.
        @param[in] n                The number of SPs to be synchronized.
        @param[in] sps              The SP array.
        @return PXC_STATUS_NO_ERROR        Successful execution.
    */
    __inline static pxcStatus SynchronizeEx(pxcI32 n, PXCSyncPoint **sps) {
        return SynchronizeEx(n,sps,0); 
    }

    /**
        @brief    The function releases the elements of an SP array, if the elements are not NULL.
        @param[in] objects          The SP array.
        @param[in] startIndex       The start index.
        @param[in] nitems           The number of items.
    */
    __inline static void ReleaseSP(PXCSyncPoint **objects, int startIndex, pxcI32 nitems) {
        for (int i=startIndex;i<startIndex+nitems;i++) {
            if (objects[i]) objects[i]->Release();
            objects[i]=0;
        }
    }

};
