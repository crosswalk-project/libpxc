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
#include "pxcbase.h"
#include "pxcaudio.h"

/**
	The PXCAudioSourceService extends PXCAudioSource and operates on audio device selected
    in PXCAudioSource::SetDevice function.
*/
class PXCAudioSourceService : public PXCBase {
public:
    PXC_CUID_OVERWRITE(0x2048D7A3);

    /** @structure AudioProfile
	    Describes the audio stream configuration parameters
    */
    struct AudioProfile {
        PXCAudio::AudioInfo audioInfo;
        pxcI32              reserved[8];
    };

    /** @brief Return the valid stream configuration parameters
	    @param[in]	pidx			Zero-based index to retrieve all valid profiles.
	    @param[out] profile			The AudioProfile structure for the audio configuration parameters, to be returned. 
	    @return PXC_STATUS_NO_ERROR successful execution.
        @return PXC_STATUS_ITEM_UNAVAILABLE index out of range.
    */
    virtual pxcStatus PXCAPI QueryProfile(pxcI32 pidx, AudioProfile *profile)=0;

    /** @brief Return the active stream configuration parameters
	    @param[out] profile			The AudioProfile structure for the audio configuration parameters, to be returned. 
	    @return PXC_STATUS_NO_ERROR successful execution.
    */
    pxcStatus __inline QueryProfile(AudioProfile *profile) { 
        return QueryProfile(WORKING_PROFILE,profile); 
    }

    /** @brief Opens audio stream, call once before ReadSample
	    @param[in] profile			The audio configuration parameters.
	    @return PXC_STATUS_NO_ERROR             successful execution.
        @return PXC_STATUS_DEVICE_LOST          the device is disconnected.
        @return PXC_STATUS_PARAM_UNSUPPORTED    the streams are of different frame rates.
    */
    virtual pxcStatus PXCAPI Open(AudioProfile *profile)=0;

    /** @brief Read the audio streams synchronously.
	    @param[out] sample				The audio sample, to be returned.
	    @return PXC_STATUS_NO_ERROR		    Successful execution.
	    @return PXC_STATUS_DEVICE_LOST	    The device is disconnected.
		@return PXC_STATUS_ITEM_UNAVAILABLE The audio stream is ended.
		@return PXC_STATUS_TIME_GAP			There is a jump in time stamp.
    */
    virtual pxcStatus PXCAPI ReadSample(PXCAudio **sample)=0;

    /** @brief Close the audio stream.
	    @return PXC_STATUS_NO_ERROR successful execution.
    */
    virtual void PXCAPI Close(void)=0;
};
