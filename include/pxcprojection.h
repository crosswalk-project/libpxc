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
/** @file pxcprojection.h
    Defines the PXCProjection interface, which defines mappings between
    pixel, depth, and real world coordinates.
  */
#pragma once
#include "pxcimage.h"
#include "pxccalibration.h"

/**
    This interface defines mappings between various coordinate systems
    used by modules of the SDK. Call the PXCCapture::Device::CreateProjection 
    to create an instance of this interface.

    The class extends PXCSerializeableService. Use QueryInstance<PXCSerializeableService> 
    to access the PXCSerializableService interface.
 */
class PXCProjection: public PXCBase {
public:
    PXC_CUID_OVERWRITE(0x494A8537);
    enum { CUID_PROJECTION_CLIPPING_NONE = 0x11a4c912 };

    enum ProjectionOption {
        PROJECTION_OPTION_DEFAULT = 0,
        PROJECTION_OPTION_CLIPPING_NONE = 1,
    };

    __inline PXCProjection* SelectOption(ProjectionOption option) {
        if (option == PROJECTION_OPTION_CLIPPING_NONE) return (PXCProjection*)QueryInstance(CUID_PROJECTION_CLIPPING_NONE);
        return (PXCProjection*)QueryInstance(PXCProjection::CUID);
    }

    /** 
        @brief Map depth coordinates to color coordinates for a few pixels.
        @param[in]  npoints         The number of pixels to be mapped.
        @param[in]  pos_uvz         The array of depth coordinates + depth value in the PXCPoint3DF32 structure.
        @param[out] pos_ij          The array of color coordinates, to be returned.
        @return PXC_STATUS_NO_ERROR Successful execution.
    */ 
    virtual pxcStatus PXCAPI MapDepthToColor(pxcI32 npoints, PXCPoint3DF32 *pos_uvz, PXCPointF32  *pos_ij)=0;

    /** 
        @brief Map color coordinates to depth coordiantes for a few pixels.
        @param[in]  depth           The depthmap image.
        @param[in]  npoints         The number of pixels to be mapped.
        @param[in]  pos_ij          The array of color coordinates.
        @param[out] pos_uv          The array of depth coordinates, to be returned.
        @return PXC_STATUS_NO_ERROR     Successful execution.
    */ 
    virtual pxcStatus PXCAPI MapColorToDepth(PXCImage *depth, pxcI32 npoints, PXCPointF32 *pos_ij, PXCPointF32 *pos_uv)=0;

    /** 
        @brief Map depth coordinates to world coordinates for a few pixels.
        @param[in]   npoints        The number of pixels to be mapped.
        @param[in]   pos_uvz        The array of depth coordinates + depth value in the PXCPoint3DF32 structure.
        @param[out]  pos3d          The array of world coordinates, in mm, to be returned.
        @return PXC_STATUS_NO_ERROR     Successful execution.
    */ 
    virtual pxcStatus PXCAPI ProjectDepthToCamera(pxcI32 npoints, PXCPoint3DF32 *pos_uvz, PXCPoint3DF32 *pos3d)=0;

    /** 
        @brief Map color pixel coordinates to camera coordinates for a few pixels.
        @param[in]   npoints        The number of pixels to be mapped.
        @param[in]   pos_ijz        The array of color coordinates + depth value in the PXCPoint3DF32 structure.
        @param[out]  pos3d          The array of camera coordinates, in mm, to be returned.
        @return PXC_STATUS_NO_ERROR     Successful execution.
    */ 
    virtual pxcStatus PXCAPI ProjectColorToCamera(pxcI32 npoints, PXCPoint3DF32 *pos_ijz, PXCPoint3DF32 *pos3d)=0;

    /** 
        @brief Map camera coordinates to depth coordinates for a few pixels.
        @param[in]    npoints       The number of pixels to be mapped.
        @param[in]    pos3d         The array of world coordinates, in mm.
        @param[out]   pos_uv        The array of depth coordinates, to be returned.
        @return PXC_STATUS_NO_ERROR     Successful execution.
    */ 
    virtual pxcStatus PXCAPI ProjectCameraToDepth(pxcI32 npoints, PXCPoint3DF32 *pos3d, PXCPointF32 *pos_uv)=0;

    /** 
        @brief Map camera coordinates to color coordinates for a few pixels.
        @param[in]    npoints       The number of pixels to be mapped.
        @param[in]    pos3d         The array of world coordinates, in mm.
        @param[out]   pos_ij        The array of color coordinates, to be returned.
        @return PXC_STATUS_NO_ERROR     Successful execution.
    */ 
    virtual pxcStatus PXCAPI ProjectCameraToColor(pxcI32 npoints, PXCPoint3DF32 *pos3d, PXCPointF32 *pos_ij)=0;

    /** 
        @brief Retrieve the UV map for the specific depth image. The UVMap is a PXCPointF32 array of depth size width*height.
        @param[in]  depth        The depth image instance.
        @param[out] uvmap        The UV map, to be returned.
        @return PXC_STATUS_NO_ERROR     Successful execution.
    */ 
    virtual pxcStatus PXCAPI QueryUVMap(PXCImage *depth, PXCPointF32 *uvmap)=0;

    /** 
        @brief Retrieve the inverse UV map for the specific depth image. The inverse UV map maps color coordinates
        back to the depth coordinates. The inverse UVMap is a PXCPointF32 array of color size width*height.
        @param[in]  depth        The depth image instance.
        @param[out] inv_uvmap    The inverse UV map, to be returned.
        @return PXC_STATUS_NO_ERROR     Successful execution.
    */ 
    virtual pxcStatus PXCAPI QueryInvUVMap(PXCImage *depth, PXCPointF32 *inv_uvmap)=0;

    /** 
        @brief Retrieve the vertices for the specific depth image. The vertices is a PXCPoint3DF32 array of depth 
        size width*height. The world coordiantes units are in mm.
        @param[in]  depth        The depth image instance.
        @param[out] vertices     The 3D vertices in World coordinates, to be returned.
        @return PXC_STATUS_NO_ERROR Successful execution.
    */ 
    virtual pxcStatus PXCAPI QueryVertices(PXCImage *depth, PXCPoint3DF32 *vertices)=0;

    /** 
        @brief Get the color pixel for every depth pixel using the UV map, and output a color image, aligned in space 
        and resolution to the depth image.
        @param[in] depth        The depth image instance.
        @param[in] color        The color image instance.
        @return The output image in the depth image resolution.
    */ 
    virtual PXCImage* PXCAPI CreateColorImageMappedToDepth(PXCImage *depth, PXCImage *color)=0;

    /** 
        @brief Map every depth pixel to the color image resolution, and output a depth image, aligned in space
        and resolution to the color image. The color image size may be different from original.
        @param[in] depth        The depth image instance.
        @param[in] color        The color image instance.
        @return The output image in the color image resolution.
    */ 
    virtual PXCImage* PXCAPI CreateDepthImageMappedToColor(PXCImage *depth, PXCImage *color)=0;

    /**
    @brief    A helper function to access PXCCalibration instance
    */
    __inline PXCCalibration* QueryCalibration(void) {
        return QueryInstance<PXCCalibration>();
    }

    /**
    @brief Increase a reference count.
    */
    __inline void AddRef(void) {
        QueryInstance<PXCAddRef>()->AddRef();
    }
};
