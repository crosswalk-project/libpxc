/*
Copyright (c) 2014, Intel Corporation

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
/** @file PXCEmotion.h
    Defines the PXCEmotion interface, which programs may use to detect emotions
    based on real-time facial expression analysis.
 */
#pragma once
#include "pxcsession.h"
#include "pxccapture.h"
#pragma warning(push)
#pragma warning(disable:4201) /* nameless structs/unions */

/**
This class defines a standard interface for emotion detection algorithms.
*/
class PXCEmotion:public PXCBase {
public:
	PXC_CUID_OVERWRITE(PXC_UID('E','M','T','N'));

	enum Emotion {
		EMOTION_PRIMARY_ANGER		=0x00000001,	// primary emotion ANGER
		EMOTION_PRIMARY_CONTEMPT	=0x00000002,	// primary emotion CONTEMPT
		EMOTION_PRIMARY_DISGUST		=0x00000004,	// primary emotion DISGUST
		EMOTION_PRIMARY_FEAR		=0x00000008,	// primary emotion FEAR
		EMOTION_PRIMARY_JOY			=0x00000010,	// primary emotion JOY
		EMOTION_PRIMARY_SADNESS		=0x00000020,	// primary emotion SADNESS
		EMOTION_PRIMARY_SURPRISE	=0x00000040,	// primary emotion SURPRISE

		EMOTION_SENTIMENT_POSITIVE  =0x00010000,	// Overall sentiment: POSITIVE
		EMOTION_SENTIMENT_NEGATIVE  =0x00020000,	// Overall sentiment: NEGATIVE
		EMOTION_SENTIMENT_NEUTRAL   =0x00040000,	// Overall sentiment: NEUTRAL
	};

	/// The Emotion Data Structure  
	struct EmotionData {
		pxcI64          timeStamp;                // Time stamp in 100ns when the emotion data is detected
		Emotion         emotion;
		pxcI32          fid;                      // Face ID
		Emotion			eid;					  // Emotion identifier
		pxcF32			intensity;				  // In range [0,1]. The intensity value is the detection output
		// to indicate the presence likelihood of an emotion:
		//     [0.0, 0.2): expression is likely absent
		//     [0.2, 0.4): expression is of low intensity
		//     [0.4, 0.6): expression is of medium intensity
		//     [0.6, 0.8): expression is of high intensity
		//     [0.8, 1.0]: expression is of very high intensity
		pxcI32			evidence;                 // The evidence value, between -5 and  5, represents the odds in 
		// 10-based logaritmic scale of a target expression being present.
		// For instance,
		//   Value 2 indicates that an emotion is 100 (10^2) times more likely
		//   to be categorized as its presence than not presence; while
		//   Value -2 indicates that an emotion is 100 times more likely
		//   to be categorized as its not presence than presence. 
		PXCRectI32		rectangle;				  // Detected face rectangle
		pxcI32          reserved[8];
	};

	/// Query the total number of detected faces for a given frame.
	virtual pxcI32 PXCAPI QueryNumFaces()=0;

	/// Query the total number of detected emotions for a given frame.
	virtual pxcI32 PXCAPI QueryEmotionSize()=0; 

	/// Get Emotion data of the specified face and emotion.
	/// fid     The face ID, zero-based
	/// eid     The emotion identifier
	/// data    The EmotionData data structure, to be returned
	virtual pxcStatus PXCAPI QueryEmotionData(pxcI32 /*fid*/, Emotion /*eid*/, EmotionData * /*data*/) =0;

	/// Get all Emotion data of a specified face.
	/// fid      The face ID, zero-based
	/// data     The array of EmotionData data structures, to be returned
	///			 Application should allocate the EmotionData array of size 10 for all 10 emotions  	
	virtual pxcStatus QueryAllEmotionData(pxcI32 /*fid*/, EmotionData * /*data*/) =0;  
};
#pragma warning(pop)