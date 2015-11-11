/*
Copyright (c) 2015, Intel Corporation

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
/// @file pxc3dscan.h
/// 3D Scan video module interface
#pragma once
#include "pxccapture.h"

class PXC3DScan : public PXCBase
{
public:
    PXC_CUID_OVERWRITE(PXC_UID('S', 'C', 'I', '1'));

    /// Scanning modes
    enum ScanningMode {
        VARIABLE = 0,                        // Customizable scanning area, defaults to largest area
        OBJECT_ON_PLANAR_SURFACE_DETECTION,  // Auto-fit scanning area to detected object & remove surface
        FACE,                                // Fixed scanning size for scanning human faces
        HEAD,                                // Fixed scanning size for scanning human head (and shoulders, or hat)
        BODY                                 // Fixed scanning size for scanning human body (with arms at sides)
    };

    /// Reconstruction options
    enum ReconstructionOption {
        NONE           = 0,
        SOLIDIFICATION = (1<<0),             // Fill holes and close mesh (manifold)
        TEXTURE        = (1<<1)              // Generate texture map (instead of vertex color)
    };

    /// Configuration structure
    struct Configuration {
        pxcBool              startScan;
        ScanningMode         mode;
        ReconstructionOption options;
        pxcI32               maxTriangles;
        pxcI32               reserved[64];
    };

    /// Get a copy of the current configuration
    virtual Configuration PXCAPI QueryConfiguration(void) = 0;

    /// Reconfigure the scanning configuration according to the provided values 
    /// and, if successful, restart the scanning process
    virtual pxcStatus PXCAPI SetConfiguration(Configuration config) = 0;

    /// Allocate (from a pool) and return a rendered image of the scanned data 
    /// from the perspective of the last frame processed.
    /// Call Release to deallocate each returned image object.
    virtual PXCImage* PXCAPI AcquirePreviewImage(void) = 0;

    /// Determine if the scan has started.
    virtual pxcBool PXCAPI IsScanning(void) = 0;

    /// Supported output file formats.
    enum FileFormat { OBJ, PLY, STL };

    /// Generate a mesh on disk from the scanned data.
    virtual pxcStatus PXCAPI Reconstruct(FileFormat format, const pxcCHAR* fileName) = 0;

    /// PXC3DScan reconstruction file format extension helper
    __inline static const pxcCHAR * FileFormatToString(FileFormat format) {
        switch (format) {
        case OBJ: return (const pxcCHAR*)L"obj";
        case PLY: return (const pxcCHAR*)L"ply";
        case STL: return (const pxcCHAR*)L"stl";
        }
        return (const pxcCHAR*)L"Unknown";
    }

	/// Scanning area
	struct Area {
		PXCSize3DF32 shape;      // Scanning volume (width, height, depth) in camera space (m). Set to zero (0) to auto select smallest area.
		pxcI32       resolution; // Voxel resolution (along longest shape axis). Set to zero (0) to auto select according to shape.
		pxcI32       reserved[64];
	};

	/// Get a copy of the current scanning area values
	virtual Area PXCAPI QueryArea(void) = 0;

	/// Reconfigure the scanning area are according to the provided values 
	/// and, if successful, restart the scanning process
	virtual pxcStatus PXCAPI SetArea(Area area) = 0;

	// Usage alerts
    enum AlertEvent
    {
        ALERT_IN_RANGE = 0,
        ALERT_TOO_CLOSE,
        ALERT_TOO_FAR,
        ALERT_TRACKING,
        ALERT_LOST_TRACKING
    };

    struct AlertData
    {
        pxcI64     timeStamp;
        AlertEvent label;
        pxcI32     reserved[5];
    };

    class AlertHandler
    {
    public:
        virtual void PXCAPI OnAlert(const AlertData& data) = 0;
    };

    /// Optionally register to receive event notifications.
    /// A subsequent call will replace the previously registered handler object.
    /// Subscribe(NULL) to unsubscribe.
    virtual void PXCAPI Subscribe(PXC3DScan::AlertHandler* handler) = 0;
};

/// ReconstructionOption helper function
__inline static PXC3DScan::ReconstructionOption operator|(PXC3DScan::ReconstructionOption a, PXC3DScan::ReconstructionOption b)
{
    return (PXC3DScan::ReconstructionOption)((int)a | (int)b);
}

/// ReconstructionOption helper function
__inline static PXC3DScan::ReconstructionOption operator^(PXC3DScan::ReconstructionOption a, PXC3DScan::ReconstructionOption b)
{
    return (PXC3DScan::ReconstructionOption)((int)a ^ (int)b);
}
