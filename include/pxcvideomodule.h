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
#include "pxccapture.h"
#include "pxcsession.h"

class PXCVideoModule : public PXCBase {
public:

    PXC_CUID_OVERWRITE(0x69D5B036);
    PXC_DEFINE_CONST(DEVCAP_LIMIT,120);

    /** 
        @structure DeviceCap
        Describes a pair value of device property and its value.
        Use the inline functions to access specific device properties.
    */
    struct DeviceCap {
        PXCCapture::Device::Property   label;       /* Property type */
        pxcF32                         value;       /* Property value */
    };

    /** 
        @structure StreamDesc
        Describes the streams requested by a module implementation.
    */
    struct StreamDesc {
        PXCSizeI32    sizeMin;          /* minimum size */
        PXCSizeI32    sizeMax;          /* maximum size */
        PXCRangeF32   frameRate;        /* frame rate   */
        PXCCapture::Device::StreamOption options; /* stream options */
        pxcI32        propertySet;
        pxcI32        reserved[4];
    };

    /** 
        @structure StreamDescSet
        A set of stream descriptors accessed by StreamType.
    */
    struct StreamDescSet {
        StreamDesc color;
        StreamDesc depth;
        StreamDesc ir;
		StreamDesc left;
        StreamDesc right;
        StreamDesc reserved[PXCCapture::STREAM_LIMIT-5];

        /**
            @brief Access the stream descriptor by the stream type.
            @param[in] type        The stream type.
            @return The stream descriptor instance.
        */
        __inline StreamDesc& operator[](PXCCapture::StreamType type) {
            switch (type) {
            case PXCCapture::STREAM_TYPE_COLOR: return color;
            case PXCCapture::STREAM_TYPE_DEPTH: return depth;
            case PXCCapture::STREAM_TYPE_IR:    return ir;
		    case PXCCapture::STREAM_TYPE_LEFT:  return left;
            case PXCCapture::STREAM_TYPE_RIGHT: return right;
            default:
                for (int i=sizeof(reserved)/sizeof(reserved[0])-1,j=(1<<(PXCCapture::STREAM_LIMIT-1));i>=0;i--,j>>=1)
                    if (type&j) return reserved[i];    
                return reserved[PXCCapture::STREAM_LIMIT-6];
            }
        }
    };

    /** 
        @structure DataDesc
        Data descriptor to describe the module input needs.
    */
    struct DataDesc {
        StreamDescSet       streams;                /** requested stream characters */
        DeviceCap           devCaps[DEVCAP_LIMIT];  /** requested device properties */
        PXCCapture::DeviceInfo deviceInfo;          /** requested device info */
        pxcI32              reserved[8];
    };

    /** 
        @brief Return the available module input descriptors.
        @param[in]  pidx        The zero-based index used to retrieve all configurations.
        @param[out] inputs      The module input descriptor, to be returned.
        @return PXC_STATUS_NO_ERROR            Successful execution.
        @return PXC_STATUS_ITEM_UNAVAILABLE    No specified input descriptor is not available.
    */
    virtual pxcStatus PXCAPI QueryCaptureProfile(pxcI32 pidx, DataDesc *inputs) = 0;

    /** 
        @brief Return the active input descriptor that the module works on.
        @param[out] inputs      The module input descriptor, to be returned.
        @return PXC_STATUS_NO_ERROR            Successful execution.
    */
    __inline pxcStatus QueryCaptureProfile(DataDesc *inputs) {
        return QueryCaptureProfile(WORKING_PROFILE, inputs);
    }

    /** 
        @brief Set the active input descriptor with the device information from the capture device.
        @param[in] inputs       The input descriptor with the device information.
        @return PXC_STATUS_NO_ERROR    Successful execution.
    */
    virtual pxcStatus PXCAPI SetCaptureProfile(DataDesc *inputs) = 0;

    /** 
        @brief Feed captured samples to module for processing. If the samples are not available 
        immediately, the function will register to run the module processing when the samples 
        are ready. This is an asynchronous function. The application must synchronize the 
        returned SP before retrieving any module data, which is not available during processing.
        @param[in]  images      The samples from the capture device.
        @param[out] sp          The SP, to be returned.
        @return PXC_STATUS_NO_ERROR    Successful execution.
    */
    virtual pxcStatus PXCAPI ProcessImageAsync(PXCCapture::Sample *sample, PXCSyncPoint **sp) = 0;
};
