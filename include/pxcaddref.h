/*
Copyright (c) 2011-2014, Intel Corporation

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
#ifndef __PXCADDREF_H__
#define __PXCADDREF_H__
#include "pxcbase.h"

///////////////////////////////////////////////////////////////////////////////////////

/**
The interface adds a reference count to the supported object.
*/
class PXCAddRef {
public:
    PXC_CUID_OVERWRITE(PXC_UID('B','A','S','S'));

    /** 
        @brief Increase the reference counter of the underlying object.
        @return The increased reference counter value.
    */
    virtual pxcI32 PXCAPI AddRef(void) = 0;

private:
    /* Prohibit using delete operator */
    void operator delete(void* pthis);
};

///////////////////////////////////////////////////////////////////////////////////////

#ifndef _WIN32
#include <atomic>
#endif
/**
This is the base implementation of the PXCAddRef interface.
*/
template <class T>
class PXCAddRefImpl:public T, public PXCAddRef {
public:

    PXCAddRefImpl()
    {
        m_refCount = 1;
    }

    virtual pxcI32 PXCAPI AddRef(void)
    {
#ifdef _WIN32
        return _InterlockedIncrement((volatile long*)&m_refCount);
#else
        return ++m_refCount;
#endif
    }

    virtual void   PXCAPI Release(void)
    {
#ifdef _WIN32
        if (!_InterlockedDecrement((volatile long*)&m_refCount)) ::delete this;
#else
        if (!--m_refCount) ::delete this;
#endif
    }

    virtual void* PXCAPI QueryInstance(pxcUID cuid)
    {
        return (cuid == PXCAddRef::CUID) ? (PXCAddRef*)this : T::QueryInstance(cuid);
    }

    void operator delete(void* pthis) { ((PXCBase*)pthis)->Release(); }

protected:
#ifdef _WIN32
    __declspec(align(32)) pxcI32  m_refCount;
#else
    std::atomic<int>  m_refCount;
#endif
};

#endif
