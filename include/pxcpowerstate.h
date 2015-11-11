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
/** @file pxcpowerstate.h
    Defines the PXCPowerState interface,  which exposes controls for
    detecting and changing the power state of the runtimes. 
  */
#pragma once
#include "pxcsession.h"

/**
   This interface manages the SDK implementation power state.  Any SDK I/O
   or algorithm module implementation that are power aware exposes this
   interface.  Programs may use the QueryInstance function to bind to this
   interface from any module instance.
 */
class PXCPowerState: public PXCBase {
public:
    PXC_CUID_OVERWRITE(PXC_UID('P','W','M','G'));

    enum State 
    {
        STATE_PERFORMANCE,         /* full feature set/best algorithm */
        STATE_BATTERY,   
    };

    /* Query current power state of the device, returns maximal used state */
    virtual PXCPowerState::State PXCAPI QueryState()=0;

    /* Try to set power state of all used devices, all streams, application should call 
       QueryStream to check if the desired state was set */
    virtual pxcStatus PXCAPI SetState(State state)=0;

	/* Sets inactivity interval */ 
	virtual pxcStatus PXCAPI SetInactivityInterval(pxcI32 timeInSeconds)=0;

	/* Returns inactivity interval */
	virtual pxcI32    PXCAPI QueryInactivityInterval()=0;
};










