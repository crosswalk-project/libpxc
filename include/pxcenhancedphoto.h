/*
Copyright (c) 2014-2015, Intel Corporation

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
/** @file PXCEnhancedPhoto.h
    Defines the PXCEnhancedPhoto interface, which programs may use to process snapshots of captured frames
	to measure distance and refocus the image
 */
#pragma once
#include "pxcphoto.h"
#include "pxcsession.h"

/**
This class defines a standard interface for enhanced photography algorithms.
*/
class PXCEnhancedPhoto:public PXCBase {
public:

	PXC_CUID_OVERWRITE(PXC_UID('E','P','I','N'));

	/** This represents a point in 3D world space in millimeter (mm) units. */
	struct WorldPoint {
		PXCPoint3DF32 coord; /**< Coordinates in mm. */
		pxcF32 confidence; /**< Confidence for this point. The confidence ranges from 0.0 (no confidence) to 1.0 (high confidence). This should be set to NaN if confidence is not available. */ 
		pxcF32 precision; /**< Precision for this point. Precision is given in mm and represents the percision of the depth value at this point in 3D space. This should be set to NaN if precision is not available. */
		pxcF32 reserved[6];
	};

	/** This represents the distance between two world points in millimeters (mm). */
	struct MeasureData {
		pxcF32 distance; /**< The distance measured in mm. */
		pxcF32 confidence; /**< Confidence for this point. The confidence ranges from 0.0 (no confidence) to 1.0 (high confidence). This should be set to NaN if confidence is not available. */ 
		pxcF32 precision; /**< Precision for this point. Precision is given in mm and represents the percision of the depth value at this point in 3D space. This should be set to NaN if precision is not available. */
		WorldPoint startPoint; /**< The first of the two points from which the distance is measured. */
		WorldPoint endPoint; /**< The second of the two points from which the distance is measured. */
		pxcF32 reserved[6];
	};

	/** 
	MeasureDistance: measure the distance between 2 points in mm
	photo: is the photo instance
	startPoint, endPoint: are the start pont and end point of the distance that need to be measured.
	Note: depth data must be availible and accurate at the start and end point selected. 
	*/
	virtual pxcStatus PXCAPI MeasureDistance(const PXCPhoto *photo, PXCPointI32 startPoint, PXCPointI32 endPoint, MeasureData *outData) = 0;
	
	/**
	DepthRefocus: refocus the image at input focusPoint by using depth data refocus
	photo: is the color and depth photo instance
	focusPoint: is the selected point foir refocussing.
	aperture: Range of the blur area = focal length/f-number. approximate range [7, 160]  =  [f/22, f/1.1] 
	Note: The application must release the returned refocussed image
	*/
	virtual PXCPhoto* PXCAPI DepthRefocus(const PXCPhoto *photo, PXCPointI32 focusPoint, pxcF32 aperture) = 0;

	/**
	DepthRefocus: refocus the image at input focusPoint by using depth data refocus
	photo: is the color and depth photo
	focusPoint: is the selected point foir refocussing.
	Note: The application must release the returned refocussed image
	*/
	__inline PXCPhoto* DepthRefocus(const PXCPhoto *photo, PXCPointI32 focusPoint) { 
		return DepthRefocus(photo, focusPoint, 50.0);
	}

	struct MaskParams{
		pxcF32 frontObjectDepth;
		pxcF32 backOjectDepth;
		pxcF32 nearFallOffDepth;
		pxcF32 farFallOffDepth;
		pxcF32 reserved[4];

		MaskParams() {
			frontObjectDepth = -1;
			backOjectDepth = -1;
			nearFallOffDepth = -1;
			farFallOffDepth = -1;
		};
	};

	/** 
	ComputeMaskFromThreshold: calculates a mask from the threshold computed 
	photo: input color and depth photo, only used the depth map.
	depthThreshold: depth threshold. 
	pxcF32 frontObjectDepth: foreground depth
	pxcF32 backOjectDepth: background depth
	pxcF32 nearFallOffDepth: ??
	pxcF32 farFallOffDepth ::?
	Returns a mask in the form of PXCImage for blending with the current photo frame.
	
	notes:
	For every pixel, if the mask is between the range of [POIdepth - frontObjectDepth, POIdepth + backObjectDepth], mask[p] -1.
	For every pixel p with depth in the range [POI - frontObjectDepth - nearFalloffDepth, POI - frontObjectDepth], mask[p] equals the "smoothstep" function value.
	For every pixel p with depth in the range [POI  + backObjectDepth , POI + backOjectDepth + farFallOffDepth], mask[p] equals the "smoothstep" function value.
	For every pixel p with other depth value, mask[p] = 1.
	*/
	virtual PXCImage* PXCAPI ComputeMaskFromThreshold(const PXCPhoto *photo, pxcF32 depthThreshold, MaskParams *maskParams) = 0;
	__inline PXCImage* ComputeMaskFromThreshold(const PXCPhoto *photo, pxcF32 depthThreshold){
		MaskParams maskParams;
		return ComputeMaskFromThreshold(photo, depthThreshold, &maskParams);
	};

	/** 
	ComputeMaskFromCoordinate: convenience function that creates a mask directly from a depth coordinate.
	photo: input color and depth photo, only used the depth map.
	coord: input (x,y) coordinates on the depth map.  
	Returns a mask in the form of PXCImage for blending with the current photo frame.
	Note: this function simply calls ComputeMaskFromThreshold underneath.
	*/
	virtual PXCImage* PXCAPI ComputeMaskFromCoordinate(const PXCPhoto *photo, PXCPointI32 coord, MaskParams *maskParams) = 0;
	__inline PXCImage* ComputeMaskFromCoordinate(const PXCPhoto *photo, PXCPointI32 coord){
		MaskParams maskParams;
		return ComputeMaskFromCoordinate(photo, coord, &maskParams);
	};
	
	/**
	InitMotionEffect: the function initializes the 6DoF parallax function with the photo that needs processing. 
	photo: 2D+depth input image.
	returns PXCStatus.
	*/
	virtual pxcStatus PXCAPI InitMotionEffect(const PXCPhoto *photo) = 0;

	/**
	ApplyMotionEffect: the function applies a 6DoF parallax effect which is the difference in the apparent position of an object
	when it is viewed from two different positions or viewpoints. 

	motion[3]: is the right, up, and forward motion when (+ve), and Left, down and backward motion when (-ve)
	motion[0]: + right   / - left 
	motion[1]: + up      / - down
	motion[2]: + forward / - backward
	rotation[3]: is the Pitch, Yaw, Roll rotations in degrees in the Range: 0-360. 
	rotaion[0]: pitch 
	rotaion[1]: yaw
	rotaion[2]: roll
	zoomFactor: + zoom in / - zoom out
	PXCImage: the returned parallaxed image.
	*/
	virtual PXCImage* PXCAPI ApplyMotionEffect(pxcF32 motion[3], pxcF32 rotation[3], pxcF32 zoomFactor) = 0;

	class PhotoUtils:public PXCBase {
	public:

		PXC_CUID_OVERWRITE(PXC_UID('E','P','U','T'));

		_inline static PhotoUtils* CreateInstance(PXCSession* session){
			PhotoUtils *me=0;
			session->CreateImpl<PhotoUtils>(&me);
			return me;
		}

		/** 
		Input param for Depth fill Quality: 
		High: better quality, slow execution for post processing (image)
		Low : lower quality, fast execution for realtime processing (live video sequence)
		*/
		enum DepthFillQuality {
			HIGH = 0, 
			LOW, 
		};
		
		/**
		EnhanceDepth: enhance the depth image quality by filling holes and denoising
		outputs the enhanced depth image
		photo: input color, depth photo, and calibration data
		depthQuality: Depth fill Quality: HIGH or LOW for post or realtime processing respectively
		Note: The application must release the returned enhanced depth image
		*/
		virtual PXCPhoto* PXCAPI EnhanceDepth(const PXCPhoto *photo, DepthFillQuality depthQuality)= 0;

		/** 
		DepthMapQuality: output param for Depth Map Quality: 
		BAD : ??,
		FAIR: ??,
		GOOD: ??
		*/
		enum DepthMapQuality
		{
			BAD = 0,
			FAIR = 1,
			GOOD = 2
		};

		/**
		DepthQuality: retruns the quality of the the depth map
		depthIm: input raw depth map
		depthQuality: BAD, FAIR, GOOD
		*/
		virtual DepthMapQuality PXCAPI GetDepthQuality(const PXCImage *depthIm)= 0;

		/** 
		CommonFOV: Matches the Feild Of View (FOV) of color and depth in the photo. Useful for still images.
		photo: input image color+depth
		Returns a photo with primary,unedited color images, and depthmaps cropped to the 
		common FOV and the camera meatadata recalculated accordingly.
		Note: Returns a nullptr if function fails
		*/
		virtual PXCPhoto* PXCAPI CommonFOV(const PXCPhoto *photo) = 0;

		/** 
		Crop: The primary image, the camera[0] RGB and depth images are cropped 
		and the intrinsic / extrinsic info is updated.
		photo: input image color+depth
		rect : top left corner (x,y) plus width and height of the window to keep 
		and crop all the rest
		Returns a photo that has all its images cropped and metadata fixed accordingly.
		Note: Returns a nullptr if function fails
		*/
		virtual PXCPhoto* PXCAPI PhotoCrop(const PXCPhoto *photo, PXCRectI32 rect) = 0;

		/** 
		UpScaleDepth: Change the size of the enhanced depth map. 
		This function preserves aspect ratio, so only new width is required.
		photo: input image color+depth
		width: the new width.
		enhancementType: if the inPhoto has no enhanced depth, then do this type of depth enhancement before resizing.
		Returns a Depth map that has the same aspect ratio as the color image resolution.
		Note: Returns a nullptr if the aspect ratio btw color and depth is not preserved
		*/
		virtual PXCPhoto* PXCAPI DepthResize(const PXCPhoto *photo, pxcI32 width, DepthFillQuality enhancementType) = 0;
		__inline PXCPhoto* DepthResize(const PXCPhoto *photo, pxcI32 width) { 
			return DepthResize(photo, width, DepthFillQuality::HIGH);
		}

		/** 
		PhotoResize: Change the size of the reference (primary) image. 
		This function preserves aspect ratio, so only new width is required.
		Only the primary image is resized.
		photo - input photo.
		width - the new width.
		Returns a photo with the reference (primary) color image resized while maintaining the aspect ratio.
		Note: Returns a nullptr when the fcn fails
		*/
		virtual PXCPhoto* PXCAPI ColorResize(const PXCPhoto *photo, pxcI32 width) = 0;

		/**
		PhotoRotate: rotates a Photo (color, depth and metadata).
		this function rotates the primary image, the RGB and depth images in camera 0, and updates 
		the corresponding intrinsic/extrinsic info.
		photo: input photo.
		degrees: the angle of rotation around the center of the color image in degrees +/- sign for clockwise and counterclockwise.
		Returns a rotated photo.
		Note: Returns a nullptr when the function fails
		*/	
		virtual PXCPhoto* PXCAPI PhotoRotate(const PXCPhoto *photo, pxcF32 degrees) = 0;
	};

	/**
	This class defines a standard interface for enhanced photography segmentation algorithms.
	*/
	class Segmentation:public PXCBase {
	public:

		PXC_CUID_OVERWRITE(PXC_UID('E','P','S','G'));

		_inline static Segmentation* CreateInstance(PXCSession* session){
			Segmentation *me=0;
			session->CreateImpl<Segmentation>(&me);
			return me;
		}

		/** 
		ObjectSegment: generates an initial mask for any object selected by the bounding mask. 
		The mask can then be refined by hints supplied by the user in RefineMask() function. 
		photo: input color and depth photo.
		inMask : a mask signaling the foreground or the object to be segmented. the pixels of the object should be set to 255.  
		Returns a mask in the form of PXCImage with detected pixels set to 255 and undetected pixels set to 0.
		*/
		virtual PXCImage* PXCAPI ObjectSegment(const PXCPhoto *sample, const PXCImage *inMask) = 0;
			
		/** 
		RefineMask: refines the mask generated by the ObjectSegment() function by using hints.
		points: input arrays with hints' coordinates.
		length: length of the array
		isForeground: bool set to true if input hint locations are foreground and false if background
		Returns a mask in the form of PXCImage with detected pixels set to 255 and undetected pixels set to 0.
		*/
		virtual PXCImage* PXCAPI RefineMask(const PXCPointI32* points, pxcI32 length, bool isForeground) = 0;
		
		/** 
		Undo: undo last hints.
		Returns a mask in the form of PXCImage with detected pixels set to 255 and undetected pixels set to 0.
		*/
		virtual PXCImage* PXCAPI Undo() = 0;
		
		/** 
		Redo: Redo the previously undone hint.
		Returns a mask in the form of PXCImage with detected pixels set to 255 and undetected pixels set to 0.
		*/
		virtual PXCImage* PXCAPI Redo() = 0;

		/** 
		ObjectSegment: generates an initial mask for any object selected by the bounding box. 
		The mask can then be refined by hints supplied by the user in RefineMask() function. 
		photo: input color and depth photo.
		topLeftCoord    : top left corner of the object to segment.  
		bottomRightCoord: Bottom right corner of the object to segment.
		Returns a mask in the form of PXCImage with detected pixels set to 255 and undetected pixels set to 0.
		*/
		virtual __declspec(deprecated("Use ObjectSegment instead")) PXCImage* PXCAPI ObjectSegmentDeprecated(const PXCPhoto *photo, PXCPointI32 topLeftCoord, PXCPointI32 bottomRightCoord) = 0;
		_inline PXCImage* ObjectSegment(const PXCPhoto *photo, PXCPointI32 topLeftCoord, PXCPointI32 bottomRightCoord){
			return ObjectSegmentDeprecated(photo, topLeftCoord, bottomRightCoord);
		}

		/** 
		RefineMask: refines the mask generated by the ObjectSegment() function by using hints.
		hints: input mask with hints. hint values.
		0 = no hint
		1 = foreground
		2 = background
		Returns a mask in the form of PXCImage with detected pixels set to 255 and undetected pixels set to 0.
		*/
		virtual __declspec(deprecated("Use RefineMask instead")) PXCImage* PXCAPI RefineMaskDeprecated(const PXCImage *hints) = 0;
		_inline PXCImage* RefineMask(const PXCImage *hints){
			return RefineMaskDeprecated(hints);
		}
	};

	class Paster:public PXCBase {
	public:

		PXC_CUID_OVERWRITE(PXC_UID('E','P','P','P'));

		_inline static Paster* CreateInstance(PXCSession* session){
			Paster *me=0;
			session->CreateImpl<Paster>(&me);
			return me;
		}

		/** 
		PasteEffects:  
		matchIllumination: flag to match Illumination, default value is true		
		transparency: (default) 0.0f = opaque, 1.0f = transparent sticker
		embossHighFreqPass: High Frequency Pass during emboss, default 0.0f no emboss, 1.0f max	
		byPixelCorrection: default false, flag to use by pixel illumination correction, takes shadows in account
		colorCorrection: default false, flag to add color correction		
		*/
		struct PasteEffects {
			pxcBool matchIllumination; // flag to match Illumination, default value is true		
			pxcF32 transparency;       // (default) 0.0f = opaque, 1.0f = transparent sticker
			pxcF32 embossHighFreqPass; // High Frequency Pass during emboss, default 0.0f no emboss, 1.0f max	
			pxcBool shadingCorrection;   // default false, flag to use pixel illumination correction, takes shadows in account
			pxcBool colorCorrection;     // default false, flag to add color correction		
			pxcF32 reserved[6];

			PasteEffects(){
				matchIllumination = true;
				transparency = 0.0;
				embossHighFreqPass = 0.0;	
				shadingCorrection = false;
				colorCorrection = false;
			};
		};

		/** 
		SetPhoto: sets the photo that needs to be processed.
		photo: photo to be processed [color + depth] 
		Returns PXC_STATUS_NO_ERROR if success. PXC_STATUS_PROCESS_FAILED if process failed
		*/
		virtual pxcStatus PXCAPI SetPhoto(const PXCPhoto *photo) = 0;
		
		/**
		GetPlanesMap: return plane indices map for current SetPhoto
		Returns a PXCImage of the plane indices in a form of a mask.
		*/
		virtual PXCImage* PXCAPI GetPlanesMap() = 0;

		/** 
		StickerData:  
		coord : insertion coordinates
		height: sticker height in mm, default -1 auto-scale		
		rotation: in-plane rotation in degree, default 0	
		isCenter: Anchor point. False means coordinate is top left, true means coordinate is center.
		*/
		struct StickerData {
			pxcF32  height; 
			pxcF32  rotation;
			pxcBool isCenter;
			pxcF32 reserved[6];

			StickerData(){
				height = -1.0f;
				rotation = 0.0f;	
				isCenter = false;
			};
		};

		/** 
		SetSticker: sets the sticker that will be pasted with all configurations needed and paste effects.
		sticker: the image to paste onto the photo (foreground image)
		coord : insertion coordinates
		stickerData: the sticker size, paste location and anchor point.
		pasteEffects: the pasting effects.
		Returns PXC_STATUS_NO_ERROR if success. PXC_STATUS_PROCESS_FAILED if process failed
		*/
		virtual pxcStatus PXCAPI SetSticker(PXCImage* sticker, PXCPointI32 coord, StickerData *stickerData, PasteEffects* pasteEffects) = 0;
		_inline pxcStatus PXCAPI SetSticker(PXCImage* sticker, PXCPointI32 coord, StickerData *stickerData){
			PasteEffects pasteEffects;
			return SetSticker(sticker, coord, stickerData, &pasteEffects);
		}
		_inline pxcStatus PXCAPI SetSticker(PXCImage* sticker, PXCPointI32 coord){
			PasteEffects pasteEffects;
			StickerData  stickerData;
			return SetSticker(sticker, coord, &stickerData, &pasteEffects);
		}

		/** 
		PreviewSticker: returns a sticker mask showing the location of the pasted sticker.
		Returns a PXCImage of the previewed sticker in a form of a mask.
		*/
		virtual PXCImage* PXCAPI PreviewSticker() = 0;

		/** 
		Paste: pastes a smaller 2D image (sticker) onto a bigger color + depth image (background).
		The smaller foreground image, is rendered according to a
		user-specified position and an auto-detected plane orientation onto the background image.
		The auto-oriented foreground image and the color data of the background image are composited together
		according to the alpha channal of the foreground image.

		Returns the embeded foreground with background image.
		*/
		virtual PXCPhoto* PXCAPI Paste() = 0;
		
		/** 
		PasteOnPlane: This function is provided for texturing a smaller 2D image (foreground)
		onto a bigger color + depth image (background). The smaller foreground image, is rendered according to a
		user-specified position and an auto-detected plane orientation onto the background image.
		The auto-oriented foreground image and the color data of the background image are composited together
		according to the alpha channal of the foreground image.

		embedIm: the image to embed in the photo (foreground image)
		topLeftCoord, bottomLeftCoord: are the top left corner and the bottom left corner of where the user wants to embed the image.
		Returns the embeded foreground with background image.
		*/
		virtual __declspec(deprecated("Use PasteOnPlane instead")) PXCPhoto* PXCAPI PasteOnPlaneDeprecated(const PXCPhoto *photo, PXCImage* embedIm, PXCPointI32 topLeftCoord, PXCPointI32 bottomLeftCoord, PasteEffects* pasteEffects) = 0;
		_inline PXCPhoto* PXCAPI PasteOnPlane(const PXCPhoto *photo, PXCImage* embedIm, PXCPointI32 topLeftCoord, PXCPointI32 bottomLeftCoord, PasteEffects* pasteEffects){
			return PasteOnPlaneDeprecated(photo, embedIm, topLeftCoord, bottomLeftCoord, pasteEffects);
		}

		/** 
		PasteOnPlane: This function is provided for texturing a smaller 2D image (foreground)
		onto a bigger color + depth image (background). The smaller foreground image, is rendered according to a
		user-specified position and an auto-detected plane orientation onto the background image.
		The auto-oriented foreground image and the color data of the background image are composited together
		according to the alpha channal of the foreground image.

		embedIm: the image to embed in the photo (foreground image)
		topLeftCoord, bottomLeftCoord: are the top left corner and the bottom left corner of where the user wants to embed the image.
		Returns the embeded foreground with background image.
		*/
		_inline PXCPhoto* PXCAPI PasteOnPlane(const PXCPhoto *photo, PXCImage* embedIm, PXCPointI32 topLeftCoord, PXCPointI32 bottomLeftCoord){
			PasteEffects pasteEffects;
			return PasteOnPlaneDeprecated(photo, embedIm, topLeftCoord, bottomLeftCoord, &pasteEffects);
		}

	};
	
};