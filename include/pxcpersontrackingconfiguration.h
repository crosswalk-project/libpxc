/*
Copyright (c) 2013-2015, Intel Corporation

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

/** @file PXCPersonTrackingConfiguration.h
 */
#pragma once
#include "pxcpersontrackingdata.h"
#include "pxcpersontrackingmodule.h"

/**
	@class PXCPersonTrackingConfiguration
*/
class PXCPersonTrackingConfiguration: public PXCBase
{
public:
	/* Constants */
	PXC_CUID_OVERWRITE(PXC_UID('P','O','T','C'));

	enum TrackingStrategyType
	{
		STRATEGY_APPEARANCE_TIME,
		STRATEGY_CLOSEST_TO_FARTHEST,
		STRATEGY_FARTHEST_TO_CLOSEST,
		STRATEGY_LEFT_TO_RIGHT,
		STRATEGY_RIGHT_TO_LEFT
	};

	class TrackingConfiguration
	{
	public:
		enum TrackingMode
		{
			TRACKING_MODE_FOLLOWING,
			TRACKING_MODE_INTERACTIVE
		};

		virtual ~TrackingConfiguration() {}

		virtual void PXCAPI Enable() = 0;

		virtual void PXCAPI Disable() = 0;

		virtual pxcBool PXCAPI IsEnabled() = 0;

		virtual void PXCAPI EnableSegmentation() = 0;

		virtual void PXCAPI DisableSegmentation() = 0;

		virtual pxcBool PXCAPI IsSegmentationEnabled() = 0;

		virtual void PXCAPI EnableHeadPose() = 0;

		virtual void PXCAPI DisableHeadPose() = 0;

		virtual pxcBool PXCAPI IsHeadPoseEnabled() = 0;

		virtual void PXCAPI EnableBlob() = 0;

		virtual void PXCAPI DisableBlob() = 0;

		virtual pxcBool PXCAPI IsBlobEnabled() = 0;

		virtual void PXCAPI EnablePersonOrientation() = 0;

		virtual void PXCAPI DisablePersonOrientation() = 0;

		virtual pxcBool PXCAPI IsPersonOrientationEnabled() = 0;

		virtual void PXCAPI EnableHeadBoundingBox() = 0;
		
		virtual void PXCAPI DisableHeadBoundingBox() = 0;

		virtual pxcBool PXCAPI IsHeadBoundingBoxEnabled() = 0;

		virtual void PXCAPI SetMaxTrackedPersons(pxcI32 maxTrackedPersons) = 0;

		virtual pxcI32 PXCAPI GetMaxTrackedPersons() const = 0;

		virtual void PXCAPI SetTrackingMode(TrackingMode mode) = 0;

		virtual TrackingMode PXCAPI GetTrackingMode() = 0;
	};
	
	class SkeletonJointsConfiguration
	{
	public:
		enum SkeletonMode
		{
			AREA_UPPER_BODY,	//all joints in upper body
			AREA_UPPER_BODY_ROUGH,	//only 4 points - head, hands, chest
			AREA_FULL_BODY_ROUGH,
			AREA_FULL_BODY
		};
		virtual ~SkeletonJointsConfiguration() {}

		virtual void PXCAPI Enable() = 0;

		virtual void PXCAPI Disable() = 0;

		virtual pxcBool PXCAPI IsEnabled() = 0;

		virtual void PXCAPI SetMaxTrackedPersons(pxcI32 maxTrackedPersons) = 0;

		virtual void PXCAPI  SetTrackingArea(SkeletonMode area) = 0;
	};

	class PoseConfiguration
	{
	public:
		virtual ~PoseConfiguration() {}

		virtual void PXCAPI Enable() = 0;

		virtual void PXCAPI Disable() = 0;

		virtual pxcBool PXCAPI IsEnabled() = 0;

		virtual void PXCAPI SetMaxTrackedPersons(pxcI32 maxTrackedPersons) = 0;
	};

	class RecognitionConfiguration
	{
	public:
		virtual ~RecognitionConfiguration() {}

		virtual void PXCAPI Enable() = 0;

		virtual void PXCAPI Disable() = 0;

		virtual pxcBool PXCAPI IsEnabled() = 0;

		virtual void PXCAPI SetDatabaseBuffer(pxcBYTE* buffer, pxcI32 size) = 0;
	};
	
	class GesturesConfiguration
	{
	public:
		virtual ~GesturesConfiguration() {}

		virtual void PXCAPI Enable() = 0;

		virtual void PXCAPI Disable() = 0;

		virtual pxcBool PXCAPI IsEnabled() = 0;

		virtual void PXCAPI SetMaxTrackedPersons(pxcI32 maxTrackedPersons) = 0;

		virtual void PXCAPI EnableGesture(PXCPersonTrackingData::PersonGestures::GestureType gestureType) = 0;

		virtual void PXCAPI EnableAllGestures() = 0;

		virtual void PXCAPI DisableGesture(PXCPersonTrackingData::PersonGestures::GestureType gestureType) = 0;

		virtual void PXCAPI DisableAllGestures() = 0;
	};

	class ExpressionsConfiguration
	{
	public:
		virtual ~ExpressionsConfiguration()
		{}

		virtual void PXCAPI Enable() = 0;

		virtual void PXCAPI Disable() = 0;

		virtual pxcBool PXCAPI IsEnabled() = 0;

		virtual void PXCAPI SetMaxTrackedPeople(pxcI32 maxTrackedPeople) = 0;

		virtual void PXCAPI EnableAllExpressions() = 0;

		virtual void PXCAPI DisableAllExpressions() = 0;

		virtual pxcStatus PXCAPI EnableExpression(PXCPersonTrackingData::PersonExpressions::PersonExpressionsEnum expression) = 0;

		virtual void PXCAPI DisableExpression(PXCPersonTrackingData::PersonExpressions::PersonExpressionsEnum expression) = 0;

		virtual pxcBool PXCAPI IsExpressionEnabled(PXCPersonTrackingData::PersonExpressions::PersonExpressionsEnum expression) = 0;
	};

	/**
	 	@brief Returns the Person Tracking Detection Configuration interface
	 */
	virtual TrackingConfiguration* PXCAPI QueryTracking() = 0;
	/**
	 	@brief Returns the Person Tracking Skeleton Joints Configuration interface
	 */
	virtual SkeletonJointsConfiguration* PXCAPI QuerySkeletonJoints() = 0;
	/**
	 	@brief Returns the Person Tracking Pose Configuration interface
	 */
	virtual PoseConfiguration* PXCAPI QueryPose() = 0;

	/**
		@brief Returns the Person Tracking Recognition Configuration interface
	*/
	virtual RecognitionConfiguration* PXCAPI QueryRecognition() = 0;

	/**
	@brief Returns the Person Tracking Gestures Configuration interface
	*/
	virtual GesturesConfiguration* PXCAPI QueryGestures() = 0;

	/**
	@brief Returns the Person Tracking Expressions Configuration interface
	*/
	virtual ExpressionsConfiguration* PXCAPI QueryExpressions() = 0;

	//Profile includes frontal
	enum TrackingAngles
	{
		TRACKING_ANGLES_FRONTAL = 0,
		TRACKING_ANGLES_PROFILE,
		TRACKING_ANGLES_ALL
	};

	/**
		@brief Sets the range of user angles to be tracked
	*/
	virtual void PXCAPI SetTrackedAngles(TrackingAngles angles) = 0;
	
#ifdef PT_MW_DEV
	/* Event Handlers */
	
	/**	
		@class AlertHandler	
	*/
	class AlertHandler {
	public:
		/**
		 @brief The OnFiredAlert method is called when a registered alert event is fired.		
		*/
		virtual void PXCAPI OnFiredAlert(const PXCPersonTrackingData::AlertData & alertData) = 0;
	};

	/* Tracking Configuration */
	
    /** 
        @brief Restart the tracking process and reset all the output data information. 		
     */
    virtual pxcStatus PXCAPI ResetTracking() = 0;
	
	/* Alerts Configuration */
		
	/** 
		@brief Enable alert messaging for a specific event.            	
	*/
	virtual pxcStatus PXCAPI EnableAlert(PXCPersonTrackingData::AlertType alertEvent) = 0;
	
	/** 
		@brief Enable all alert messaging events.            		
	*/
	virtual pxcStatus PXCAPI EnableAllAlerts(void) = 0;
	
	/** 
		@brief Test the activation status of the given alert.		
	*/
	virtual pxcBool PXCAPI IsAlertEnabled(PXCPersonTrackingData::AlertType alertEvent) const = 0;
	
	/** 
		@brief Disable alert messaging for a specific event.            		
	*/
	virtual pxcStatus PXCAPI DisableAlert(PXCPersonTrackingData::AlertType alertEvent) = 0;

	/** 
		@brief Disable messaging for all alerts.                        		
	*/
	virtual pxcStatus PXCAPI DisableAllAlerts(void) = 0;
	
	/** 
		@brief Register an event handler object for the alerts. 		
	*/
	virtual pxcStatus PXCAPI SubscribeAlert(AlertHandler *alertHandler) = 0;

	/** 
		@brief Unsubscribe an alert handler object.		
	*/
	virtual pxcStatus PXCAPI UnsubscribeAlert(AlertHandler *alertHandler) = 0;
#endif
};
