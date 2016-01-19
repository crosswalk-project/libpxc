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
/** @file pxcbase.h
    Defines PXCBase, the base interface for all SDK interfaces
 */
#pragma once
#include "pxcdefs.h"
#include "pxcstatus.h"

#if defined(_WIN32) || defined(_WIN64)
#define PXCAPI __stdcall
#else
#define PXCAPI
#endif

///////////////////////////////////////////////////////////////////////////////////////
/**
   This interface forms the base of all SDK interface definitions.
   The interface overrides the class delete operator to work with the SDK
   dispatching mechanism; and exposes a custom (RTTI-free) dynaimc casting
   mechanism with the QueryInstance method.  The application that implements
   any PXCBase derived interface must derive from one of the PXCBaseImpl
   class templates.
 */
class PXCBase {
public:
    enum { CUID=0 };
    enum { WORKING_PROFILE=-1 };

    /**
        @brief The function checks if the object instance supports a specific interface.
        If so, return the instance of the interface. Otherwise, returns NULL.
        @param[in] cuid        The interface identifier.
        @return The interface instance, or NULL.
    */
    virtual void* PXCAPI QueryInstance(pxcUID cuid) = 0;

    /**
        @brief The function checks if the object instance supports a specific interface.
        If so, return the instance of the interface. Otherwise, returns NULL.
        @return The interface instance, or NULL.
    */
    template <class T> T* QueryInstance(void) { return (T*)this->QueryInstance(T::CUID); }

    /**
        @brief The function releases the object instance. Certain objects are reference
        counted, in which case the function reduces the reference count by 1.
    */
    virtual void  PXCAPI Release(void) = 0;

    /**
        @brief The base constructor.
    */
    PXCBase(void) {}

private:
    /* Call Release() to delele object */
    void operator delete(void* pthis);
    /* Prohibit using copy & assignment constructors */
    PXCBase(PXCBase const &);
    PXCBase& operator = (PXCBase const &);
};

#define PXC_CUID_OVERWRITE(X) enum { CUID=X }

///////////////////////////////////////////////////////////////////////////////////////
/**
   This template class contains a default implementation of the PXCBase
   interface.  The application that implements any PXCBase derived interface,
   such as a callback handler, should derive the implementation from this
   template class.  See the SDK Interface section (in the sdkcore manual)
   for details.  The PXCBaseImpl template assumes single inheritance.
   The variations, PXCBaseImpl2 and PXCBaseImpl3, implement two or three
   inheritances, respectively.
*/
template <class T>
class PXCBaseImpl:public T {
public:

    virtual ~PXCBaseImpl(void) {}
    enum { CUID=T::CUID+1 };
    virtual void* PXCAPI QueryInstance(pxcUID cuid) {
        if (cuid==CUID) return this;
        if (cuid==T::CUID) return (T*)this;
        if (cuid==PXCBase::CUID) return (PXCBase*)this;
        return 0; 
    }
    virtual void  PXCAPI Release(void) { ::delete this; }
    void operator delete(void* pthis) { ::delete(pthis); }
};

/** See PXCBaseImpl */
template <class T1, class T2>
class PXCBaseImpl2:public T1, public T2 {
public:

    virtual ~PXCBaseImpl2(void) {}
    enum { CUID = T1::CUID ^ T2::CUID };
    virtual void* PXCAPI QueryInstance(pxcUID cuid) {
        if (cuid==CUID) return this;
        if (cuid==T1::CUID) return (T1*)this;
        if (cuid==T2::CUID) return (T2*)this;
        if (cuid==PXCBase::CUID) return (PXCBase*)(T1*)this;
        return 0;
    }
    virtual void  PXCAPI Release(void) { ::delete this; }
    void operator delete(void* pthis) { ::delete(pthis); }
};

/** See PXCBaseImpl */
template <class T1, class T2, class T3>
class PXCBaseImpl3:public T1, public T2, public T3 {
public:

    virtual ~PXCBaseImpl3(void) {}
    enum { CUID = T1::CUID ^ T2::CUID ^ T3::CUID };
    virtual void* PXCAPI QueryInstance(pxcUID cuid) {
        if (cuid==CUID) return this;
        if (cuid==T1::CUID) return (T1*)this;
        if (cuid==T2::CUID) return (T2*)this;
        if (cuid==T3::CUID) return (T3*)this;
        if (cuid==PXCBase::CUID) return (PXCBase*)(T1*)this;
        return 0;
    }
    virtual void  PXCAPI Release(void) { ::delete this; }
    void operator delete(void* pthis) { ::delete(pthis); }
};

/** See PXCBaseImpl */
template <class T1, class T2, class T3, class T4>
class PXCBaseImpl4:public T1, public T2, public T3, public T4 {
public:

    virtual ~PXCBaseImpl4(void) {}
    enum { CUID = T1::CUID ^ T2::CUID ^ T3::CUID ^ T4::CUID };
    virtual void* PXCAPI QueryInstance(pxcUID cuid) {
        if (cuid==CUID) return this;
        if (cuid==T1::CUID) return (T1*)this;
        if (cuid==T2::CUID) return (T2*)this;
        if (cuid==T3::CUID) return (T3*)this;
        if (cuid==T4::CUID) return (T4*)this;
        if (cuid==PXCBase::CUID) return (PXCBase*)(T1*)this;
        return 0;
    }
    virtual void  PXCAPI Release(void) { ::delete this; }
    void operator delete(void* pthis) { ::delete(pthis); }
};

/** See PXCBaseImpl */
template <class T1, class T2, class T3, class T4, class T5>
class PXCBaseImpl5:public T1, public T2, public T3, public T4, public T5 {
public:

    virtual ~PXCBaseImpl5(void) {}
    enum { CUID = T1::CUID ^ T2::CUID ^ T3::CUID ^ T4::CUID ^ T5::CUID };
    virtual void* PXCAPI QueryInstance(pxcUID cuid) {
        if (cuid==CUID) return this;
        if (cuid==T1::CUID) return (T1*)this;
        if (cuid==T2::CUID) return (T2*)this;
        if (cuid==T3::CUID) return (T3*)this;
        if (cuid==T4::CUID) return (T4*)this;
        if (cuid==T5::CUID) return (T5*)this;
        if (cuid==PXCBase::CUID) return (PXCBase*)(T1*)this;
        return 0;
    }
    virtual void  PXCAPI Release(void) { ::delete this; }
    void operator delete(void* pthis) { ::delete(pthis); }
};

template <class T1, class T2, class T3, class T4, class T5, class T6>
class PXCBaseImpl6:public T1, public T2, public T3, public T4, public T5, public T6 {
public:

    virtual ~PXCBaseImpl6(void) {}
    enum { CUID = T1::CUID ^ T2::CUID ^ T3::CUID ^ T4::CUID ^ T5::CUID ^ T6::CUID };
    virtual void* PXCAPI QueryInstance(pxcUID cuid) {
        if (cuid==CUID) return this;
        if (cuid==T1::CUID) return (T1*)this;
        if (cuid==T2::CUID) return (T2*)this;
        if (cuid==T3::CUID) return (T3*)this;
        if (cuid==T4::CUID) return (T4*)this;
        if (cuid==T5::CUID) return (T5*)this;
        if (cuid==T6::CUID) return (T6*)this;
        if (cuid==PXCBase::CUID) return (PXCBase*)(T1*)this;
        return 0;
    }
    virtual void  PXCAPI Release(void) { ::delete this; }
    void operator delete(void* pthis) { ::delete(pthis); }
};
