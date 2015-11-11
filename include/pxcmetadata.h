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
/** @file pxcmetadata.h
    Defines the PXCMetadata interface, which is used for managing
    metadata storage.
 */
#pragma once
#include "pxcbase.h"

/**
    This interface manages metadata storage.  The PXCSession, PXCImage
    and PXCAudio implementations expose the PXCMetadata interface.
 */
class PXCMetadata:public PXCBase {
public:
    PXC_CUID_OVERWRITE(0x62398423);

    /**
        @brief The function returns a unique identifier for the meta data storage.
        @return the unique identifier.
    */
    virtual pxcUID PXCAPI QueryUID(void)=0;

    /**
        @brief The function retrieves the identifiers of all available meta data.
        @param[in] idx          The zero-based index to retrieve all identifiers.
        @return the metadata identifier, or zero if not available.
    */
    virtual pxcUID PXCAPI QueryMetadata(pxcI32 idx)=0;

    /**
        @brief The function detaches the specified metadata.
        @param[in] id           The metadata identifier.
        @return PXC_STATUS_NO_ERROR                Successful execution.
        @return PXC_STATUS_ITEM_UNAVAILABLE        The metadata is not found.
    */
    virtual pxcStatus PXCAPI DetachMetadata(pxcUID id)=0;

    /**
        @brief The function attaches the specified metadata.
        @param[in] id           The metadata identifier.
        @param[in] buffer       The metadata buffer.
        @param[in] size         The metadata buffer size, in bytes.
        @return PXC_STATUS_NO_ERROR                Successful execution.
    */
    virtual pxcStatus PXCAPI AttachBuffer(pxcUID id, pxcBYTE *buffer, pxcI32 size)=0;

    /**
        @brief The function returns the specified metadata buffer size.
        @param[in] id           The metadata identifier.
        @return the metadata buffer size, or zero if the metadata is not available.
    */
    virtual pxcI32 PXCAPI QueryBufferSize(pxcUID id)=0;

    /**
        @brief The function retrieves the specified metadata.
        @param[in] id           The metadata identifier.
        @param[in] buffer       The buffer pointer to retrieve the metadata.
        @param[in] size         The buffer size in bytes.
        @return PXC_STATUS_NO_ERROR         Successful execution.
    */
    virtual pxcStatus PXCAPI QueryBuffer(pxcUID id, pxcBYTE *buffer, pxcI32 size)=0;

    /**
        @brief The function attaches an instance of a serializeable interface to be metadata storage.
        @param[in] id           The metadata identifier.
        @param[in] instance     The serializable instance.
        @return PXC_STATUS_NO_ERROR         Successful execution.
    */
    virtual pxcStatus PXCAPI AttachSerializable(pxcUID id, PXCBase *instance)=0;

    /**
        @brief The function creates an instance of a serializeable interface from the metadata storage.
        @param[in] id           The metadata identifier.
        @param[in] cuid         The interface identifier.
        @param[out] instance    The serializable instance, to be returned.
        @return PXC_STATUS_NO_ERROR         Successful execution.
    */
    virtual pxcStatus PXCAPI CreateSerializable(pxcUID id, pxcUID cuid, void **instance)=0;

    /**
        @brief The function creates an instance of a serializeable interface from the metadata storage.
        @param[in] id           The metadata identifier.
        @param[out] instance    The serializable instance, to be returned.
        @return PXC_STATUS_NO_ERROR         Successful execution.
    */
    template <class T> pxcStatus __inline CreateSerializable(pxcUID id, T **instance) {
        return CreateSerializable(id, T::CUID, (void**)instance); 
    }
};
