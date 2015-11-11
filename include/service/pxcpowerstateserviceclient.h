/*
Copyright (c) 2013-2014, Intel Corporation

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
#pragma once
#include "pxcsession.h"
#include "pxcpowerstate.h"

class PXCPowerStateServiceClient :public PXCBase {
public:
    PXC_CUID_OVERWRITE(PXC_UID('P','W','M','C'));

	/* Queries unique id for desired device & stream and client id */
	virtual pxcUID QueryUniqueId(pxcI32 deviceId, pxcI32 streamId, pxcI32 mId)=0;

    /* Register module with the Power Manager */
    virtual pxcStatus RegisterModule(pxcUID uId, PXCSession::ImplGroup group, PXCSession::ImplSubgroup subGroup) = 0;

    /* Unregister module from certain device & stream. All further requests for this device from this module will be ignored */
    virtual pxcStatus UnregisterModule(pxcUID uId) = 0;

    /* Request state for stream on device, module may call QueryState to test if the state was actually set */
    virtual pxcStatus SetState(pxcUID uId, PXCPowerState::State state) = 0;

    /* Query power state on stream on device */
    virtual pxcStatus QueryState(pxcUID uId, PXCPowerState::State* state) = 0;
};

