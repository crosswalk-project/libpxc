/*
Copyright (c) 2011, Intel Corporation

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

class PXCSchedulerService;
class PXCAccelerator;

class PXCSessionService:public PXCBase {
public:
    PXC_CUID_OVERWRITE(PXC_UID('S','E','S','2'));

    /* algorithms in this group are core services */
    PXC_DEFINE_CONST(IMPL_SUBGROUP_ACCELERATOR,0x80000000);
    PXC_DEFINE_CONST(IMPL_SUBGROUP_SCHEDULER,0x40000000);
    PXC_DEFINE_CONST(IMPL_SUBGROUP_POWER_MANAGEMENT,0x20000000);

    PXC_DEFINE_UID(SUID_DLL_EXPORT_TABLE,'D','L','E',2);
    struct DLLExportTable {
        DLLExportTable  *next;
        pxcStatus       (PXCAPI *createInstance)(PXCSession *session, PXCSchedulerService *scheduler, PXCAccelerator *accel, DLLExportTable *table, pxcUID cuid, PXCBase **instance);
        pxcUID          suid;
        PXCSession::ImplDesc  desc;
    };

    virtual pxcStatus PXCAPI QueryImplEx(PXCSession::ImplDesc *templat, pxcI32 idx, DLLExportTable **table, void ***instance)=0;

    virtual pxcStatus PXCAPI LoadImpl(DLLExportTable *table)=0;
    virtual pxcStatus PXCAPI UnloadImpl(DLLExportTable *table)=0;

    virtual void   PXCAPI TraceEvent(const pxcCHAR* /*event_name*/) {}
    virtual void   PXCAPI TraceBegin(const pxcCHAR* /*task_name*/) {}
    virtual void   PXCAPI TraceEnd(void) {}
    virtual void   PXCAPI TraceParam(const pxcCHAR* /*param_name*/, const pxcCHAR* /*param_value*/) {}
};
