/*
Copyright (c) 2012-2014, Intel Corporation

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
/** @file pxccalibration.h
Defines the PXCCalibration interface, which is used to retrieve the calibration data.
*/
#pragma once
#include "pxccapture.h"

class PXCCalibration : public PXCBase {

public:
	PXC_CUID_OVERWRITE(0x494A8538);

	struct StreamTransform
	{
		pxcF32 translation[3];   /* The translation in mm of the camera coordinate system origin to the world coordinate system origin. The world coordinate system coincides with the depth camera coordinate system. */
		pxcF32 rotation[3][3];   /* The rotation of the camera coordinate system with respect to the world coordinate system. The world coordinate system coincides with the depth camera coordinate system. */
	};

	struct StreamCalibration
	{
		PXCPointF32 focalLength;    /* The sensor focal length in pixels along the x and y axes. The parameters vary with the stream resolution setting. */
		PXCPointF32 principalPoint; /*  The sensor principal point in pixels along the x and y axes. The parameters vary with the stream resolution setting. */
		pxcF32 radialDistortion[3];     /*  The radial distortion coefficients, as described by camera model equations. */
		pxcF32 tangentialDistortion[2]; /* The tangential distortion coefficients, as described by camera model equations. */
		PXCCapture::DeviceModel model; /* Defines the distortion model of the device - different device models may use different distortion models */
	};

	/**
	@brief Query camera calibration and transformation data for a sensor.
	@param[in]  streamType      The stream type which is produced by the sensor.
	@param[out] calibration     The intrinsics calibration parameters of the sensor.
	@param[out] transformation  The extrinsics transformation parameters from the sensor to the camera coordinate system origin.
	@return PXC_STATUS_NO_ERROR Successful execution.
	*/
	virtual pxcStatus PXCAPI QueryStreamProjectionParameters(PXCCapture::StreamType streamType, StreamCalibration *calibration, StreamTransform *transformation) = 0;

    class CalibrationEx : public PXCBase {
    public:
        PXC_CUID_OVERWRITE(0x708D3F6A);
        virtual pxcStatus PXCAPI QueryStreamProjectionParametersEx(PXCCapture::StreamType streamType, PXCCapture::Device::StreamOption options, StreamCalibration *calibration, StreamTransform *transformation) = 0;
    };

	/**
	@brief Query camera calibration and transformation data for a sensor according to user defined options.
	@param[in]  streamType      The stream type which is produced by the sensor.
	@param[in]  options         The options that selects specific calibration and transformation data which is produced by the sensor.
	@param[out] calibration     The intrinsics calibration parameters of the sensor.
	@param[out] transformation  The extrinsics transformation parameters from the sensor to the camera coordinate system origin.
	@return PXC_STATUS_NO_ERROR Successful execution.
	*/
    __inline pxcStatus PXCAPI QueryStreamProjectionParametersEx(PXCCapture::StreamType streamType, PXCCapture::Device::StreamOption options, StreamCalibration *calibration, StreamTransform *transformation)
    {
        CalibrationEx *calibrationEx = QueryInstance<CalibrationEx>();
		if (calibrationEx) return calibrationEx->QueryStreamProjectionParametersEx(streamType, options, calibration, transformation);
		if (options == PXCCapture::Device::STREAM_OPTION_ANY) return QueryStreamProjectionParameters(streamType, calibration, transformation);
		return PXC_STATUS_FEATURE_UNSUPPORTED;
    }
};
