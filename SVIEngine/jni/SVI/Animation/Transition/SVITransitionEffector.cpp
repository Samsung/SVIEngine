#include "SVITransitionEffector.h"

#include "./../Interpolator/SVIInterpolator.h"
#include "../../Slide/SVISlide.h"
#include "../../Slide/SVIBeamedSlide.h"
#include "../../Slide/SVIBaseSlide.h"
#include "../../Slide/SVIRenderPartManager.h"

#include "../../SVICores.h"


#include "SVIFlipEffector.h"
#include "SVIRotateEffector.h"
#include "SVIRotateCubeEffector.h"
#include "SVIScaleEffector.h"
#include "SVISlideEffector.h"
#include "SVISmoothEffector.h"
#include "SVITumbleEffector.h"
#include "SVIBreakEffector.h"
#include "SVIThrowEffector.h"
#include "SVICurtainEffector.h"
#include "SVIDominoFlipEffector.h"
#include "SVISideWindowEffector.h"
#include "SVIExploidEffector.h"
#include "SVIWaveEffector.h"
#include "SVIWizzleEffector.h"
#include "SVIZoomINEffector.h"
#include "SVIBlindEffector.h"

#include "SVITwistEffector.h"

#include "SVIFlashEffector.h"
#include "SVIFlyThroughEffector.h"
#include "SVITwirlEffector.h"
#include "SVISwapEffector.h"
#include "SVISwooshEffector.h"
#include "SVIHoneyCombEffector.h"
#include "SVIShredEffector.h"
#include "SVICheckerBoardEffector.h"
#include "SVICenterBlindEffector.h"
#include "SVICube2PiecesEffector.h"
#include "SVIBrickCubeEffector.h"

#include "SVIBoxEffector.h"
#include "SVICoverEffector.h"
#include "SVIUncoverEffector.h"
#include "SVIFallEffector.h"
#include "SVIFadeThroughColorEffector.h"
#include "SVIRevolvingDoorEffector.h"
#include "SVIWipeEffector.h"
#include "SVIRevealEffector.h"
#include "SVISplitEffector.h"
#include "SVICube4PiecesEffector.h"

#include "SVISwitchEffector.h"
#include "SVIGalleryEffector.h"
#include "SVIFlip2Effector.h"
#include "SVIMoveInEffector.h"
#include "SVIShapeEffector.h"
#include "SVIFoldingScreenEffector.h"
#include "SVIGallery2Effector.h"


#include "../../Render/SVIRenderer.h"
#include "../../Render/SVIRenderScene.h"
#include "../../Render/SVIFrameRenderer.h"
#include "../../Render/SVITexture.h"

#include "../SVIAnimationSet.h"

#include "../SVIBasicAnimation.h"
#include "../SVIKeyFrameAnimation.h"

namespace SVI {

	static const SVIBool DEBUG = SVIFALSE;

	SVITransitionEffector::SVITransitionEffector(SVIGLSurface *surface) :
		mFullTimeDuration(0),
		mOffsetDuration(0),
		mSlideDuration(0),
		mGlovalOffsetDuration(0),
		mRepeatCount(1),
		mAutoReverse(SVIFALSE),
		mRowCount(1),
		mColumnCount(1),
		mTextureWidth(0),
		mTextureHeight(0),
		mFromSlide(NULL),
		mToSlide(NULL),
		mIsUseParent(SVIFALSE),
		mIsUseClone(SVIFALSE),
		mIsUseFakeParent(SVIFALSE),
		mFromNParentSlide(NULL),
		mToNParentSlide(NULL),
		mFakeParentSlide(NULL),
		mFromNslide(NULL),
		mToNslide(NULL),
		mFromCloneSlide(NULL),
		mToCloneSlide(NULL),
		mParentSlide(NULL),
		mTransitaionAni(NULL),
		mDirectionType(SVITransitionAnimation::LEFT),
		mRandomValue(0.5f),
		mSVIGLSurface(surface){
	}


	SVITransitionEffector::~SVITransitionEffector() {
		mSVIGLSurface = NULL;
		DEBUG_CHECK_LOGI("%s(%d) : into destroy", "SVITransitionEffector", (SVIInt)this);
	}


	SVITransitionEffector* SVITransitionEffector::createEffector(SVIGLSurface *surface, SVITransitionAnimation::TransitionType type) {
		
		SVITransitionEffector *effector =NULL;

		switch(type) {
		case SVITransitionAnimation::FLIP:
			DEBUG_CHECK_LOGI("%s : into FLIP", "SVITransitionEffector:createEffector");
			effector = new SVIFlipEffector(surface);
			break;
		
		case SVITransitionAnimation::ROTATE:
			DEBUG_CHECK_LOGI("%s : into ROTATE", "SVITransitionEffector:createEffector");
			effector = new SVIRotateEffector(surface);
			break;
		
		case SVITransitionAnimation::ROTATE_CUBE:
			DEBUG_CHECK_LOGI("%s : into ROTATE_CUBE", "SVITransitionEffector:createEffector");
			effector = new SVIRotateCubeEffector(surface);
			break;

		case SVITransitionAnimation::SCALE:
			DEBUG_CHECK_LOGI("%s : into SCALE", "SVITransitionEffector:createEffector");
			effector = new SVIScaleEffector(surface);
			break;

		case SVITransitionAnimation::SLIDE:
			DEBUG_CHECK_LOGI("%s : into SLIDE", "SVITransitionEffector:createEffector");
			effector = new SVISlideEffector(surface);
			break;

		case SVITransitionAnimation::SMOOTH:
			DEBUG_CHECK_LOGI("%s : into SMOOTH", "SVITransitionEffector:createEffector");
			effector = new SVISmoothEffector(surface);
			break;

		case SVITransitionAnimation::TUMBLE:
			DEBUG_CHECK_LOGE("%s : into TUMBLE", "SVITransitionEffector:createEffector");
			effector = new SVITumbleEffector(surface);
			break;
		case SVITransitionAnimation::CURTAIN:
			DEBUG_CHECK_LOGE("%s : into CURTAIN", "SVITransitionEffector:createEffector");
			effector = new SVICurtainEffector(surface);
			break;
		case SVITransitionAnimation::DOMINOFLIP:
			DEBUG_CHECK_LOGE("%s : into DOMINOFLIP", "SVITransitionEffector:createEffector");
			effector = new SVIDominoFlipEffector(surface);
			break;
		case SVITransitionAnimation::SIDEWINDOW:
			DEBUG_CHECK_LOGE("%s : into SIDEWINDOW", "SVITransitionEffector:createEffector");
			effector = new SVISideWindowEffector(surface);
			break;
		case SVITransitionAnimation::EXPLOID:
			DEBUG_CHECK_LOGE("%s : into EXPLOID", "SVITransitionEffector:createEffector");
			effector = new SVIExploidEffector(surface);
			break;
		case SVITransitionAnimation::WAVE:
			DEBUG_CHECK_LOGE("%s : into EXPLOID", "SVITransitionEffector:createEffector");
			effector = new SVIWaveEffector(surface);
			break;
		case SVITransitionAnimation::WIZZLE:
			DEBUG_CHECK_LOGE("%s : into EXPLOID", "SVITransitionEffector:createEffector");
			effector = new SVIWizzleEffector(surface);
			break;

		case SVITransitionAnimation::ZOOMIN:
			DEBUG_CHECK_LOGE("%s : into ZOOM IN", "SVITransitionEffector:createEffector");
			effector = new SVIZoomINEffector(surface);
			break;
		case SVITransitionAnimation::BLIND:
			DEBUG_CHECK_LOGE("%s : into BLIND", "SVITransitionEffector:createEffector");
			effector = new SVIBlindEffector(surface);
			break;

		case SVITransitionAnimation::TWIST:
			DEBUG_CHECK_LOGE("%s : into TWIST", "SVITransitionEffector:createEffector");
			effector = new SVITwistEffector(surface);
			break;

		case SVITransitionAnimation::BREAK:
			DEBUG_CHECK_LOGE("%s : into BREAK", "SVITransitionEffector:createEffector");
			effector = new SVIBreakEffector(surface);
			break;
			
		case SVITransitionAnimation::FLASH:
			DEBUG_CHECK_LOGE("%s : into FLASH", "SVITransitionEffector:createEffector");
			effector = new SVIFlashEffector(surface);
			break;

		case SVITransitionAnimation::FLY_THROUGH:
			DEBUG_CHECK_LOGE("%s : into FLY_THROUGH", "SVITransitionEffector:createEffector");
			effector = new SVIFlyThroughEffector(surface);
			break;

		case SVITransitionAnimation::TWIRL:
			DEBUG_CHECK_LOGE("%s : into TWIRL", "SVITransitionEffector:createEffector");
			effector = new SVITwirlEffector(surface);
			break;

		case SVITransitionAnimation::SWAP:
			DEBUG_CHECK_LOGE("%s : into SWAP", "SVITransitionEffector:createEffector");
			effector = new SVISwapEffector(surface);
			break;

		case SVITransitionAnimation::SWOOSH:
			DEBUG_CHECK_LOGE("%s : into SWOOSH", "SVITransitionEffector:createEffector");
			effector = new SVISwooshEffector(surface);
			break;

		case SVITransitionAnimation::HONEYCOMB:
			DEBUG_CHECK_LOGE("%s : into HONEYCOMB", "SVITransitionEffector:createEffector");
			effector = new SVIHoneyCombEffector(surface);
			break;

		case SVITransitionAnimation::SHRED:
			effector = new SVIShredEffector(surface);
			break;

		case SVITransitionAnimation::CHECKER_BOARD:
			effector = new SVICheckerBoardEffector(surface);
			break;

		case SVITransitionAnimation::CENTER_BLIND:
			effector = new SVICenterBlindEffector(surface);
			break;

			
        case SVITransitionAnimation::CUBE_2PIECES:
			effector = new SVICube2PiecesEffector(surface);
			break;

		case SVITransitionAnimation::BRICK_CUBE:
			effector = new SVIBrickCubeEffector(surface);
			break;

			


			//Dohyun's Effect
		case SVITransitionAnimation::BOX:
			DEBUG_CHECK_LOGI("%s : into BOX", "SVITransitionEffector:createEffector");
			effector = new SVIBoxEffector(surface);
			break;

		case SVITransitionAnimation::COVER:
			DEBUG_CHECK_LOGI("%s : into COVER", "SVITransitionEffector:createEffector");
			effector = new SVICoverEffector(surface);
			break;

		case SVITransitionAnimation::UNCOVER:
			DEBUG_CHECK_LOGI("%s : into UNCOVER", "SVITransitionEffector:createEffector");
			effector = new SVIUncoverEffector(surface);
			break;

		case SVITransitionAnimation::FALL:
			DEBUG_CHECK_LOGI("%s : into FALL", "SVITransitionEffector:createEffector");
			effector = new SVIFallEffector(surface);
			break;

		case SVITransitionAnimation::FADE_THROUGH_COLOR:
			DEBUG_CHECK_LOGI("%s : into FADE_THROUGH_COLOR", "SVITransitionEffector:createEffector");
			effector = new SVIFadeThroughColorEffector(surface);
			break;

		case SVITransitionAnimation::REVOLVING_DOOR:
			DEBUG_CHECK_LOGI("%s : into REVOLVING_DOOR", "SVITransitionEffector:createEffector");
			effector = new SVIRevolvingDoorEffector(surface);
			break;

		case SVITransitionAnimation::WIPE:
			DEBUG_CHECK_LOGI("%s : into WIPE", "SVITransitionEffector:createEffector");
			effector = new SVIWipeEffector(surface);
			break;

		case SVITransitionAnimation::REVEAL:
			DEBUG_CHECK_LOGI("%s : into REVEAL", "SVITransitionEffector:createEffector");
			effector = new SVIRevealEffector(surface);
			break;
			
		case SVITransitionAnimation::SPLIT:
			DEBUG_CHECK_LOGI("%s : into SPLIT", "SVITransitionEffector:createEffector");
			effector = new SVISplitEffector(surface);
			break;

		case SVITransitionAnimation::CUBE_4PIECES:
			DEBUG_CHECK_LOGI("%s : CUBE_4PIECES ", "SVITransitionEffector:createEffector");
			effector = new SVICube4PiecesEffector(surface);
			break;
			



			//Jungwook's Effect
		case SVITransitionAnimation::SWITCH:
			DEBUG_CHECK_LOGI("%s : into SWITCH", "SVITransitionEffector:createEffector");
			effector = new SVISwitchEffector(surface);
			break;

		case SVITransitionAnimation::GALLERY:
			DEBUG_CHECK_LOGI("%s : into GALLERY", "SVITransitionEffector:createEffector");
			effector = new SVIGalleryEffector(surface);
			break;

		case SVITransitionAnimation::FLIP_2:
			DEBUG_CHECK_LOGI("%s : into FLIP2", "SVITransitionEffector:createEffector");
			effector = new SVIFlip2Effector(surface);
			break;

		case SVITransitionAnimation::MOVE_IN:
			DEBUG_CHECK_LOGI("%s : into MOVE_IN", "SVITransitionEffector:createEffector");
			effector = new SVIMoveInEffector(surface);
			break;

        case SVITransitionAnimation::SHAPE:
			DEBUG_CHECK_LOGI("%s : into SHAPE", "SVITransitionEffector:createEffector");
			effector = new SVIShapeEffector(surface);
			break;

        case SVITransitionAnimation::FOLDING_SCREEN:
			DEBUG_CHECK_LOGI("%s : into FOLDING_SCREEN", "SVITransitionEffector:createEffector");
			effector = new SVIFoldingScreenEffector(surface);
			break;

        case SVITransitionAnimation::GALLERY_2:
			DEBUG_CHECK_LOGI("%s : into GALLERY_2", "SVITransitionEffector:createEffector");
			effector = new SVIGallery2Effector(surface);
			break;

		}

		return effector;
	}


	SVIBool SVITransitionEffector::initialize(SVITransitionAnimation* transAni, SVIBeamedSlide *sizeSlide, SVIBeamedSlide *parentSlide, SVIBeamedSlide *fromSlide, SVIBeamedSlide *toSlide, SVIInt duration, SVIInt offset, SVIInt repeatCount, SVIInt transitionType) {
		
		if(transAni == NULL) {
			return SVIFALSE;
		}

		mSizeSlide = sizeSlide;
		mParentSlide = parentSlide;
		mFromSlide = fromSlide;
		mToSlide = toSlide;

		mParentSlide->setOrthogonal(SVIFALSE);

		mTextureWidth = mToSlide->getRegion().getSize().mWidth;
		mTextureHeight = mToSlide->getRegion().getSize().mHeight;

		mFullTimeDuration = duration;
		mGlovalOffsetDuration = offset;
		mRepeatCount = repeatCount;

		mTransitaionAni = transAni;
		mDirectionType = mTransitaionAni->getDirectionType();

		mPrevWindowSize = transAni->mPrevWindowSize;
		mCurWindowSize = transAni->mCurWindowSize;

		mTransitionType = transitionType;

		setRowColumnCount();

		if(mTransitionType == SVITransitionAnimation::BRICK_CUBE){
			makeBrickSlide();
		}else{
			makeNslide();
		}
		setAnimation();

		return SVITRUE;
	}

	void SVITransitionEffector::makeBrickSlide(){

		mFromNslide = new SVIBeamedSlide **[mRowCount];
		mToNslide = new SVIBeamedSlide **[mRowCount];

		
		mFromNslide[0] = new SVIBeamedSlide *[mRowCount];
		mToNslide[0] = new SVIBeamedSlide *[mRowCount];

		for(int j=0; j<mRowCount; j++){
			mFromNslide[0][j] = new SVIBeamedSlide(mSVIGLSurface);
			mToNslide[0][j] = new SVIBeamedSlide(mSVIGLSurface);
		}

		mToSlide->getProjectionSlide()->invalidate(SVITRUE);
		mSVIGLSurface->getRenderer()->getScene()->renderSpecifyTraversal(mToSlide->getProjectionSlide());

		SVIBeamedSlide* parent = (SVIBeamedSlide *)(mParentSlide);
		SVIProjectionSlide* projectionSlideParent = parent->getProjectionSlide();

		SVIRect *slideRect = new SVIRect[mRowCount];
		SVIRect *textureRect = new SVIRect[mRowCount];

		SVIFloat width = mTextureWidth / 8.0f;
		SVIFloat height = mTextureHeight / 5.0f;

		SVIFloat mX = mSizeSlide->getRegion().mOrigin.mX;
		SVIFloat mY = mSizeSlide->getRegion().mOrigin.mY;

		
		for(int i = 0 ; i< mRowCount; i++){
			mFromNslide[0][i]->setDeformType(DEFORM_BRICK);
		}
		
		//make Texture Rect
		//1 row
		textureRect[0] = SVIRect(width * 0.0f / mTextureWidth, 0.0f / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[1] = SVIRect(width * 1.0f / mTextureWidth, 0.0f / mTextureHeight, (width * 2)/mTextureWidth, height/mTextureHeight);
		textureRect[2] = SVIRect(width * 3.0f / mTextureWidth, 0.0f / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[3] = SVIRect(width * 4.0f / mTextureWidth, 0.0f / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[4] = SVIRect(width * 5.0f / mTextureWidth, 0.0f / mTextureHeight, (width * 2)/mTextureWidth, height/mTextureHeight);
		textureRect[5] = SVIRect(width * 7.0f / mTextureWidth, 0.0f / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		
		//2row
		textureRect[6] = SVIRect(width * 0.0f / mTextureWidth, height / mTextureHeight, (width * 2.0f)/mTextureWidth, height/mTextureHeight);
		textureRect[7] = SVIRect(width * 2.0f / mTextureWidth, height / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[8] = SVIRect(width * 3.0f / mTextureWidth, height / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[9] = SVIRect(width * 4.0f / mTextureWidth, height / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[10] = SVIRect(width * 5.0f / mTextureWidth, height / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[11] = SVIRect(width * 6.0f / mTextureWidth, height / mTextureHeight, (width * 2.0f) /mTextureWidth, height/mTextureHeight);

		//3row
		textureRect[12] = SVIRect(width * 0.0f / mTextureWidth, height * 2.0f / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[13] = SVIRect(width * 1.0f / mTextureWidth, height * 2.0f / mTextureHeight, (width * 3.0f)/mTextureWidth, height/mTextureHeight);
		textureRect[14] = SVIRect(width * 4.0f / mTextureWidth, height * 2.0f / mTextureHeight, (width * 3.0f)/mTextureWidth, height/mTextureHeight);
		textureRect[15] = SVIRect(width * 7.0f / mTextureWidth, height * 2.0f / mTextureHeight, width / mTextureWidth, height/mTextureHeight);
		
		//4row
		textureRect[16] = SVIRect(width * 0.0f / mTextureWidth, height * 3.0f / mTextureHeight, (width * 2.0f)/mTextureWidth, height/mTextureHeight);
		textureRect[17] = SVIRect(width * 2.0f / mTextureWidth, height * 3.0f / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[18] = SVIRect(width * 3.0f / mTextureWidth, height * 3.0f / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[19] = SVIRect(width * 4.0f / mTextureWidth, height * 3.0f / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[20] = SVIRect(width * 5.0f / mTextureWidth, height * 3.0f / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[21] = SVIRect(width * 6.0f / mTextureWidth, height * 3.0f / mTextureHeight, (width * 2.0f) /mTextureWidth, height/mTextureHeight);

		//5row
		textureRect[22] = SVIRect(width * 0.0f / mTextureWidth, height * 4.0f / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[23] = SVIRect(width * 1.0f / mTextureWidth, height * 4.0f / mTextureHeight, (width * 2)/mTextureWidth, height/mTextureHeight);
		textureRect[24] = SVIRect(width * 3.0f / mTextureWidth, height * 4.0f / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[25] = SVIRect(width * 4.0f / mTextureWidth, height * 4.0f / mTextureHeight, width/mTextureWidth, height/mTextureHeight);
		textureRect[26] = SVIRect(width * 5.0f / mTextureWidth, height * 4.0f / mTextureHeight, (width * 2)/mTextureWidth, height/mTextureHeight);
		textureRect[27] = SVIRect(width * 7.0f / mTextureWidth, height * 4.0f / mTextureHeight, width/mTextureWidth, height/mTextureHeight);

		for(int i =0; i< mRowCount; i++){
			slideRect[i] = SVIRect(textureRect[i].getOrigin().mX * mTextureWidth + mX, textureRect[i].getOrigin().mY * mTextureHeight + mY,
								textureRect[i].getSize().mWidth * mTextureWidth, textureRect[i].getSize().mHeight * mTextureHeight);
		}

		for(int i=0; i<mRowCount; i++){
			mFromNslide[0][i]->setRegion(slideRect[i]);
			mToNslide[0][i]->setRegion(slideRect[i]);

			mFromNslide[0][i]->setTextureRegion(textureRect[i]);
			mToNslide[0][i]->setTextureRegion(textureRect[i]);
		}

		////initialize default value
		for(int i = 0; i<mRowCount; i++){

			mFromNslide[0][i]->setDeformRatio(SVIVector3(mFromNslide[0][i]->getRegion().getSize().mWidth, 0.5f, 0.0f));
			
			mFromNslide[0][i]->setOrthogonal(SVITRUE);
			mToNslide[0][i]->setOrthogonal(SVITRUE);

			mFromNslide[0][i]->setHoldOpacity(SVITRUE);
			mToNslide[0][i]->setHoldOpacity(SVITRUE);
			mFromNslide[0][i]->setOpacity(1.0f);
			mToNslide[0][i]->setOpacity(1.0f);


			SVIProjectionSlide* parentFromProjectionSlide;
			SVIProjectionSlide* parentToProjectionSlide;

			mParentSlide->addSubSlide(mFromNslide[0][i]);
			mParentSlide->addSubSlide(mToNslide[0][i]);

			parentFromProjectionSlide = mParentSlide->getProjectionSlide();
			parentToProjectionSlide = mParentSlide->getProjectionSlide();

			mFromNslide[0][i]->mProjectionSlide = new SVIProjectionSlide(mSVIGLSurface);
			mToNslide[0][i]->mProjectionSlide = new SVIProjectionSlide(mSVIGLSurface);

			mToNslide[0][i]->mProjectionSlide->initialize(parentToProjectionSlide, mToNslide[0][i], NULL, mToNslide[0][i]->mSlideType);
			mFromNslide[0][i]->mProjectionSlide->initialize(parentFromProjectionSlide, mFromNslide[0][i], NULL, mFromNslide[0][i]->mSlideType);

			mSVIGLSurface->getRenderer()->getScene()->renderSpecifyTraversal(mToNslide[0][i]->getProjectionSlide());
			mSVIGLSurface->getRenderer()->getScene()->renderSpecifyTraversal(mFromNslide[0][i]->getProjectionSlide());

			mSVIGLSurface->getSlideCapture()->setCaptureSlideToSlide(mToSlide->getProjectionSlide(), mToNslide[0][i]->getProjectionSlide());
			mSVIGLSurface->getSlideCapture()->setCaptureSlideToSlide(mFromSlide->getProjectionSlide(), mFromNslide[0][i]->getProjectionSlide());

		}

		for(int i = 0; i< mRowCount; i++){
			mSVIGLSurface->getSlideCapture()->setBackFaceCaptureSlideToSlide(mToSlide->getProjectionSlide(),mFromNslide[0][i]->getProjectionSlide());

			SVISlideTextureContainer * pContainer = mFromNslide[0][i]->getProjectionSlide()->getTextureContainer();
			SVICaptureData * pCaptureData = pContainer != NULL ? pContainer->getBackFaceCaptureTexture() : NULL;

			if (pCaptureData != NULL){
				mFromNslide[0][i]->setBackFaceTextureRegion(SVIRect(textureRect[i].getOrigin().mX * mTextureWidth, textureRect[i].getOrigin().mY * mTextureHeight,
					textureRect[i].getSize().mWidth * mTextureWidth, textureRect[i].getSize().mHeight * mTextureHeight));
			}
		}

		SVI_SVIFE_DELETE_ARRAY(slideRect);
		SVI_SVIFE_DELETE_ARRAY(textureRect);

	}


	void SVITransitionEffector::setRowColumnCount(){
		if(mTransitionType == SVITransitionAnimation::SIDEWINDOW
			|| mTransitionType == SVITransitionAnimation::CUBE_4PIECES
			|| mTransitionType == SVITransitionAnimation::BRICK_CUBE
			|| mTransitionType == SVITransitionAnimation::FOLDING_SCREEN
			|| mTransitionType == SVITransitionAnimation::CUBE_2PIECES){

			DEBUG_CHECK_LOGI("default Row Column Count");

		}else{
			if(mTextureWidth < mTextureHeight){
				SVIInt temp = mRowCount;
				mRowCount = mColumnCount;
				mColumnCount = temp;
			}
			if(mTransitionType == SVITransitionAnimation::BLIND
				|| mTransitionType == SVITransitionAnimation::CURTAIN
				|| mTransitionType == SVITransitionAnimation::CENTER_BLIND){
				if(mDirectionType == SVITransitionAnimation::RIGHT || mDirectionType == SVITransitionAnimation::LEFT){
					mColumnCount = 1;
				}else if(mDirectionType == SVITransitionAnimation::UP || mDirectionType == SVITransitionAnimation::DOWN){
					mRowCount = 1;
				}
			}
		}
	}

	void SVITransitionEffector::updateToSlideInfo(SVIBeamedSlide* toSlide) {

		mSVIGLSurface->getSlideCapture()->stop(mFromSlide->getProjectionSlide());//Multi-Instance-Support
		mSVIGLSurface->getSlideCapture()->stop(mToSlide->getProjectionSlide());//Multi-Instance-Support
		if(toSlide != NULL) /* P140523-04859  P140523-01376 fix */ 
			toSlide->setHidden(SVIFALSE);
	}

	void SVITransitionEffector::makeNslide(){

		SVIFloat width = mTextureWidth/mRowCount;
		SVIFloat height = mTextureHeight/mColumnCount;

		SVIFloat mX = mSizeSlide->getRegion().mOrigin.mX;
		SVIFloat mY = mSizeSlide->getRegion().mOrigin.mY;

		mFromNslide = new SVIBeamedSlide **[mRowCount*mColumnCount];
		mToNslide = new SVIBeamedSlide **[mRowCount*mColumnCount];

		for(int i=0; i<mColumnCount; i++){
			mFromNslide[i] = new SVIBeamedSlide *[mRowCount];
			mToNslide[i] = new SVIBeamedSlide *[mRowCount];

			for(int j=0; j<mRowCount; j++){
				mFromNslide[i][j] = new SVIBeamedSlide(mSVIGLSurface);
				mToNslide[i][j] = new SVIBeamedSlide(mSVIGLSurface);
			}

		}

		if(mIsUseParent){
			mFromNParentSlide = new SVIBeamedSlide(mSVIGLSurface);
			mToNParentSlide = new SVIBeamedSlide(mSVIGLSurface);

			mFromNParentSlide->setRegion(mParentSlide->getRegion());
			mToNParentSlide->setRegion(mParentSlide->getRegion());
			mFromNParentSlide->setPosition(mParentSlide->getPosition());
			mToNParentSlide->setPosition(mParentSlide->getPosition());

			mFromNParentSlide->setOrthogonal(SVIFALSE);
			mToNParentSlide->setOrthogonal(SVIFALSE);

			mParentSlide->addSubSlide(mFromNParentSlide);
			mParentSlide->addSubSlide(mToNParentSlide);

			mFromNParentSlide->mProjectionSlide = new SVIProjectionSlide(mSVIGLSurface);
			mToNParentSlide->mProjectionSlide = new SVIProjectionSlide(mSVIGLSurface);

			mToNParentSlide->mProjectionSlide->initialize(mParentSlide->getProjectionSlide(), mToNParentSlide, NULL, mToNParentSlide->mSlideType);
			mFromNParentSlide->mProjectionSlide->initialize(mParentSlide->getProjectionSlide(), mFromNParentSlide, NULL, mFromNParentSlide->mSlideType);
		}

		if(mIsUseFakeParent){
			mFakeParentSlide = new SVIBeamedSlide(mSVIGLSurface);
			mFakeParentSlide->setRegion(mParentSlide->getRegion());
			mFakeParentSlide->setPosition(mParentSlide->getPosition());

			mFakeParentSlide->setOrthogonal(SVIFALSE);

			mParentSlide->addSubSlide(mFakeParentSlide);

			mFakeParentSlide->mProjectionSlide = new SVIProjectionSlide(mSVIGLSurface);

			mFakeParentSlide->mProjectionSlide->initialize(mParentSlide->getProjectionSlide(), mFakeParentSlide, NULL, mFakeParentSlide->mSlideType);
		}
		mToSlide->getProjectionSlide()->invalidate(SVITRUE);
		mSVIGLSurface->getRenderer()->getScene()->renderSpecifyTraversal(mToSlide->getProjectionSlide());

		prepareEffect(mTransitionType);


		SVIBeamedSlide* parent = (SVIBeamedSlide *)(mParentSlide);
		SVIProjectionSlide* projectionSlideParent = parent->getProjectionSlide();
		SVIRect slideRect, textureRect;

		for(int i=0; i<mColumnCount; i++){
			for(int j=0; j<mRowCount; j++){

				slideRect = SVIRect(j*width + mX, i*height + mY, width, height);
				
				textureRect = SVIRect(j*width/mTextureWidth, i*height/mTextureHeight, width/mTextureWidth, height/mTextureHeight);

				mFromNslide[i][j]->setRegion(slideRect);
				mToNslide[i][j]->setRegion(slideRect);

				mFromNslide[i][j]->setTextureRegion(textureRect);
				mToNslide[i][j]->setTextureRegion(textureRect);

				mFromNslide[i][j]->setOrthogonal(SVIFALSE);
				mToNslide[i][j]->setOrthogonal(SVIFALSE);

				mFromNslide[i][j]->setHoldOpacity(SVITRUE);
				mToNslide[i][j]->setHoldOpacity(SVITRUE);
				mFromNslide[i][j]->setOpacity(1.0f);
				mToNslide[i][j]->setOpacity(1.0f);

				
				SVIProjectionSlide* parentFromProjectionSlide;
				SVIProjectionSlide* parentToProjectionSlide;

				if(mIsUseParent){
					mFromNParentSlide->addSubSlide(mFromNslide[i][j]);
					mToNParentSlide->addSubSlide(mToNslide[i][j]);

					parentFromProjectionSlide = mFromNParentSlide->getProjectionSlide();
					parentToProjectionSlide = mToNParentSlide->getProjectionSlide();

				}else if(mIsUseFakeParent){
					mFakeParentSlide->addSubSlide(mFromNslide[i][j]);
					mFakeParentSlide->addSubSlide(mToNslide[i][j]);

					parentFromProjectionSlide = mFakeParentSlide->getProjectionSlide();
					parentToProjectionSlide = mFakeParentSlide->getProjectionSlide();
				}else{
					
					mParentSlide->addSubSlide(mFromNslide[i][j]);
					mParentSlide->addSubSlide(mToNslide[i][j]);

					parentFromProjectionSlide = mParentSlide->getProjectionSlide();
					parentToProjectionSlide = mParentSlide->getProjectionSlide();

				}				

				mFromNslide[i][j]->mProjectionSlide = new SVIProjectionSlide(mSVIGLSurface);
				mToNslide[i][j]->mProjectionSlide = new SVIProjectionSlide(mSVIGLSurface);

				mToNslide[i][j]->mProjectionSlide->initialize(parentToProjectionSlide, mToNslide[i][j], NULL, mToNslide[i][j]->mSlideType);
				mFromNslide[i][j]->mProjectionSlide->initialize(parentFromProjectionSlide, mFromNslide[i][j], NULL, mFromNslide[i][j]->mSlideType);

				mSVIGLSurface->getRenderer()->getScene()->renderSpecifyTraversal(mToNslide[i][j]->getProjectionSlide());
				mSVIGLSurface->getRenderer()->getScene()->renderSpecifyTraversal(mFromNslide[i][j]->getProjectionSlide());

				mSVIGLSurface->getSlideCapture()->setCaptureSlideToSlide(mToSlide->getProjectionSlide(), mToNslide[i][j]->getProjectionSlide());//Multi-Instance-Support
				mSVIGLSurface->getSlideCapture()->setCaptureSlideToSlide(mFromSlide->getProjectionSlide(), mFromNslide[i][j]->getProjectionSlide());//Multi-Instance-Support

			}
		}

        // 2012-07-04 jwww.jang
        // init clone slides
        if(mIsUseClone) {
            mFromCloneSlide = new SVIBeamedSlide(mSVIGLSurface);
		    mToCloneSlide = new SVIBeamedSlide(mSVIGLSurface);

            mFromCloneSlide->setRegion(slideRect);
		    mToCloneSlide->setRegion(slideRect);

		    mFromCloneSlide->setTextureRegion(textureRect);
		    mToCloneSlide->setTextureRegion(textureRect);

		    mFromCloneSlide->setOrthogonal(SVIFALSE);
		    mToCloneSlide->setOrthogonal(SVIFALSE);

		    mFromCloneSlide->setHoldOpacity(SVITRUE);
		    mToCloneSlide->setHoldOpacity(SVITRUE);

		    mFromCloneSlide->setOpacity(1.0f);
		    mToCloneSlide->setOpacity(1.0f);

            SVIProjectionSlide* parentFromProjectionSlide;
		    SVIProjectionSlide* parentToProjectionSlide;

		    if(mIsUseParent){
			    mFromNParentSlide->addSubSlide(mFromCloneSlide);
			    mToNParentSlide->addSubSlide(mToCloneSlide);

			    parentFromProjectionSlide = mFromNParentSlide->getProjectionSlide();
			    parentToProjectionSlide = mToNParentSlide->getProjectionSlide();
			}else if(mIsUseFakeParent){
				mFakeParentSlide->addSubSlide(mFromCloneSlide);
				mFakeParentSlide->addSubSlide(mToCloneSlide);

				parentFromProjectionSlide = mFakeParentSlide->getProjectionSlide();
				parentToProjectionSlide = mFakeParentSlide->getProjectionSlide();
		    } else {
			    mParentSlide->addSubSlide(mFromCloneSlide);
			    mParentSlide->addSubSlide(mToCloneSlide);

			    parentFromProjectionSlide = mParentSlide->getProjectionSlide();
			    parentToProjectionSlide = mParentSlide->getProjectionSlide();
		    }
                
            mFromCloneSlide->mProjectionSlide = new SVIProjectionSlide(mSVIGLSurface);
            mToCloneSlide->mProjectionSlide = new SVIProjectionSlide(mSVIGLSurface);
        		
            mFromCloneSlide->mProjectionSlide->initialize(parentFromProjectionSlide, mFromCloneSlide, NULL, mFromCloneSlide->mSlideType);
            mToCloneSlide->mProjectionSlide->initialize(parentToProjectionSlide, mToCloneSlide, NULL, mToCloneSlide->mSlideType);

			mSVIGLSurface->getRenderer()->getScene()->renderSpecifyTraversal(mFromCloneSlide->getProjectionSlide());
		    mSVIGLSurface->getRenderer()->getScene()->renderSpecifyTraversal(mToCloneSlide->getProjectionSlide());		    

			
            mSVIGLSurface->getSlideCapture()->setCaptureSlideToSlide(mFromSlide->getProjectionSlide(), mFromCloneSlide->getProjectionSlide());
		    mSVIGLSurface->getSlideCapture()->setCaptureSlideToSlide(mToSlide->getProjectionSlide(), mToCloneSlide->getProjectionSlide());		    
        }
        
		if(mIsUseParent){
			mFromNParentSlide->setOpacity(0.0f);
			mToNParentSlide->setOpacity(0.0f);
		}

		if(mIsUseFakeParent){
			mFakeParentSlide->setHoldOpacity(SVIFALSE);
			mFakeParentSlide->setOpacity(0.0f);
		}
	}

	

	void SVITransitionEffector::prepareEffect(SVIInt transitionType){

		srand(time(NULL));
		mRandomValue = (SVIFloat) rand() / RAND_MAX;

		switch(transitionType){
		case SVITransitionAnimation::EXPLOID :
			mFromNslide[0][0]->setOrthogonal(true);
			mFromNslide[0][0]->setDeformType(DEFORM_EXPLODE);
			mFromNslide[0][0]->setDeformRatio(SVIVector3(0.0f, 0.0f, 0.0f));
			break;
		case SVITransitionAnimation::TWIST :
			mFromNslide[0][0]->setLightPower(0.0f);
			mFromNslide[0][0]->setDeformType(DEFORM_TWIST);
			mFromNslide[0][0]->setDeformRatio(SVIVector3(-2.0f, 0.5f, 1.0f));
			break;
		case SVITransitionAnimation::WAVE :
			mFromNslide[0][0]->setCustomRTE(SVITRUE);
			mToNslide[0][0]->setCustomRTE(SVITRUE);
			mFromNslide[0][0]->setRTEffectType(RT_MAGNIFY);
			mToNslide[0][0]->setRTEffectType(RT_MAGNIFY);
			mFromNslide[0][0]->setRTERatio(SVIVector3(0.0f, 0.01f, 1.0f));
			mToNslide[0][0]->setRTERatio(SVIVector3(0.0f, 0.01f, 1.0f));
			break;
		case SVITransitionAnimation::WIZZLE :
			mFromNslide[0][0]->setCustomRTE(SVITRUE);
			mToNslide[0][0]->setCustomRTE(SVITRUE);
			mFromNslide[0][0]->setRTEffectType(RT_WIZZLE);
			mToNslide[0][0]->setRTEffectType(RT_WIZZLE);
			mFromNslide[0][0]->setRTERatio(SVIVector3(0.0f, 0.0f, 0.0f));
			mToNslide[0][0]->setRTERatio(SVIVector3(0.0f, 0.0f, 0.0f));
			break;

		case SVITransitionAnimation::FLASH:
			mFromNslide[0][0]->setLightType(1);
			mFromNslide[0][0]->setLightRadius(0.3f);
			break;

		case SVITransitionAnimation::WIPE:
			mToNslide[0][0]->setSwipeType(SWIPE_LINEAR);
			break;
		case SVITransitionAnimation::REVEAL:
			mFromNslide[0][0]->setSwipeType(SWIPE_LINEAR);
			mToNslide[0][0]->setSwipeType(SWIPE_LINEAR);
			break;
        case SVITransitionAnimation::SHAPE:
            mToNslide[0][0]->setSwipeType(SWIPE_RADIAL);
            break;
			
		case SVITransitionAnimation::SPLIT:
			for(SVIInt i=0; i<mColumnCount; i++){
				for(SVIInt j=0; j<mRowCount; j++){
					mFromNslide[i][j]->setSwipeType(SWIPE_LINEAR);
				}
			}
			break;

		case SVITransitionAnimation::CUBE_4PIECES:
			for(SVIInt i =0; i<mColumnCount; i++){
				mFromNslide[i][0]->setDeformType(DEFORM_PANNEL);
				mFromNslide[i][0]->setDeformRatio(SVIVector3(20.0f, 0.5f, 0.0f));
			}
			break;


		}
	}

	void SVITransitionEffector::cancelAnimation(){
		if(mFromNslide != NULL && mToNslide != NULL){
			for(int i=0; i<mColumnCount; i++){
				for(int j=0; j<mRowCount; j++){
					//2012-01-09 masterkeaton27@gmail.com
					//release back-faced capture texture container method added.
					SVIProjectionSlide * pFromSlide = mFromNslide[i][j]->getProjectionSlide();
					SVIProjectionSlide * pToSlide = mToNslide[i][j]->getProjectionSlide();
					SVISlideTextureContainer * pFromContainer = pFromSlide->getTextureContainer();
					SVISlideTextureContainer * pToContainer = pToSlide->getTextureContainer();

					if (pFromContainer != NULL && pFromContainer->hasCaptureData()){
						mSVIGLSurface->getSlideCapture()->releaseCaptureFromSlide(pFromSlide);//Multi-Instance-Support
					}
					if (pToContainer != NULL && pToContainer->hasCaptureData()){
						mSVIGLSurface->getSlideCapture()->releaseCaptureFromSlide(pToSlide);//Multi-Instance-Support
					}								
					if (pFromContainer != NULL && pFromContainer->hasBackFaceCaptureData()){
						mSVIGLSurface->getSlideCapture()->releaseBackFaceCaptureFromSlide(pFromSlide);//Multi-Instance-Support
					}
					if (pToContainer != NULL && pToContainer->hasBackFaceCaptureData()){
						mSVIGLSurface->getSlideCapture()->releaseBackFaceCaptureFromSlide(pToSlide);//Multi-Instance-Support
					}						

					if( mFromNslide[i][j]->getAnimation() != NULL ){
						mFromNslide[i][j]->getAnimation()->cancel();
					}
					if( mToNslide[i][j]->getAnimation() != NULL ){
						mToNslide[i][j]->getAnimation()->cancel();
					}
				}
			}
		}

        if(mFromCloneSlide != NULL && mToCloneSlide != NULL){
			//2012-01-09 masterkeaton27@gmail.com
			//release back-faced capture texture container method added.
			SVIProjectionSlide * pFromSlide = mFromCloneSlide->getProjectionSlide();
			SVIProjectionSlide * pToSlide = mToCloneSlide->getProjectionSlide();
			SVISlideTextureContainer * pFromContainer = pFromSlide->getTextureContainer();
			SVISlideTextureContainer * pToContainer = pToSlide->getTextureContainer();

			if (pFromContainer != NULL && pFromContainer->hasCaptureData()){
				mSVIGLSurface->getSlideCapture()->releaseCaptureFromSlide(pFromSlide);
			}
			if (pToContainer != NULL && pToContainer->hasCaptureData()){
				mSVIGLSurface->getSlideCapture()->releaseCaptureFromSlide(pToSlide);
			}								
			if (pFromContainer != NULL && pFromContainer->hasBackFaceCaptureData()){
				mSVIGLSurface->getSlideCapture()->releaseBackFaceCaptureFromSlide(pFromSlide);	
			}
			if (pToContainer != NULL && pToContainer->hasBackFaceCaptureData()){
				mSVIGLSurface->getSlideCapture()->releaseBackFaceCaptureFromSlide(pToSlide);	
			}						

			if( mFromCloneSlide->getAnimation() != NULL ){
				mFromCloneSlide->getAnimation()->cancel();
			}
			if( mToCloneSlide->getAnimation() != NULL ){
				mToCloneSlide->getAnimation()->cancel();
			}
		}
	}

	void SVITransitionEffector::removeNslide(){
		if(mFromNslide != NULL && mToNslide != NULL){

			for(int i=0; i<mColumnCount; i++){
				for(int j=0; j<mRowCount; j++){
					SVI_SVIFE_DELETE(mFromNslide[i][j]);
					SVI_SVIFE_DELETE(mToNslide[i][j]);
				}
				SVI_SVIFE_DELETE_ARRAY(mFromNslide[i]);
				SVI_SVIFE_DELETE_ARRAY(mToNslide[i]);
			}

			SVI_SVIFE_DELETE_ARRAY(mFromNslide);
			SVI_SVIFE_DELETE_ARRAY(mToNslide);

			mFromNslide = NULL;
			mToNslide = NULL;
		}
        
        if(mFromCloneSlide != NULL){
			SVI_SVIFE_DELETE(mFromCloneSlide);
		}
        if(mToCloneSlide != NULL){
			SVI_SVIFE_DELETE(mToCloneSlide);
		}

		if(mFromNParentSlide != NULL){
			SVI_SVIFE_DELETE(mFromNParentSlide);
		}
		if(mToNParentSlide != NULL){
			SVI_SVIFE_DELETE(mToNParentSlide);
		}
		if(mFakeParentSlide != NULL){
			SVI_SVIFE_DELETE(mFakeParentSlide);
		}
	}

	void SVITransitionEffector::addRotationAnimation(SVIAnimationSet* aniSet, SVIVector3 preAngle, SVIVector3 nextAngle, SVIInt duration, SVIInt offset) {
		
		SVIKeyFrameAnimation* rotateAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ROTATION, mSVIGLSurface);
		rotateAni->mDuration = duration;
		rotateAni->mOffset = offset;

		SVIFloat x, y, z;

		// from angle
		x = SVI_DEGTORAD(preAngle.x);
		y = SVI_DEGTORAD(preAngle.y);
		z = SVI_DEGTORAD(preAngle.z);
		SVIQuaternion q;
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(0.0f, SVIVector4(q.x, q.y, q.z, q.w)));
		
		// to angle
		x = SVI_DEGTORAD(nextAngle.x);
		y = SVI_DEGTORAD(nextAngle.y);
		z = SVI_DEGTORAD(nextAngle.z);
		q.setIdentity();
		q.setEuler(SVIVector3(x, y, z));
		rotateAni->addKeyProperty(SVIKeyFrameProperty(1.0f, SVIVector4(q.x, q.y, q.z, q.w)));
		
		aniSet->addAnimation(rotateAni);
	}


	void SVITransitionEffector::addScaleAnimation(SVIAnimationSet* aniSet, SVIVector3 preScale, SVIVector3 nextScale, SVIInt duration, SVIInt offset) {
		
		SVIKeyFrameAnimation* scaleAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::SCALE, mSVIGLSurface);
		scaleAni->mDuration = duration;
		scaleAni->mOffset = offset;

		// from scale
		scaleAni->addKeyProperty(SVIKeyFrameProperty(0.0f, preScale));
		// to scale
		scaleAni->addKeyProperty(SVIKeyFrameProperty(1.0f, nextScale));
		
		aniSet->addAnimation(scaleAni);
	}


	void SVITransitionEffector::addRegionAnimation(SVIAnimationSet* aniSet, SVIRect preRect, SVIRect nextRect, SVIInt duration, SVIInt offset) {
		
		SVIKeyFrameAnimation* regionAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::REGION, mSVIGLSurface);
		regionAni->mDuration = duration;
		regionAni->mOffset = offset;

		// from region
		regionAni->addKeyProperty(SVIKeyFrameProperty(0.0f, preRect));
		// to region
		regionAni->addKeyProperty(SVIKeyFrameProperty(1.0f, nextRect));
		
		aniSet->addAnimation(regionAni);
	}

	void SVITransitionEffector::addTextureRegionAnimation(SVIAnimationSet* aniSet, SVIRect preRect, SVIRect nextRect, SVIInt duration, SVIInt offset) {

		SVIKeyFrameAnimation* texRegionAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::TEXTURE_REGION, mSVIGLSurface);
		texRegionAni->mDuration = duration;
		texRegionAni->mOffset = offset;

		// from region
		texRegionAni->addKeyProperty(SVIKeyFrameProperty(0.0f, preRect));
		// to region
		texRegionAni->addKeyProperty(SVIKeyFrameProperty(1.0f, nextRect));

		aniSet->addAnimation(texRegionAni);
	}
	void SVITransitionEffector::addZPositionAnimation(SVIAnimationSet* aniSet, SVIFloat preZPosition, SVIFloat nextZPosition, SVIInt duration, SVIInt offset) {
		
		SVIKeyFrameAnimation* zPosAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::ZPOSITION, mSVIGLSurface);
		zPosAni->mDuration = duration;
		zPosAni->mOffset = offset;

		// from Z-Position
		zPosAni->addKeyProperty(SVIKeyFrameProperty(0.0f, preZPosition));
		// to Z-Position
		zPosAni->addKeyProperty(SVIKeyFrameProperty(1.0f, nextZPosition));
		
		aniSet->addAnimation(zPosAni);
	}


	void SVITransitionEffector::addPositionAnimation(SVIAnimationSet* aniSet, SVIPoint prePosition, SVIPoint nextPosition, SVIInt duration, SVIInt offset) {
		
		SVIKeyFrameAnimation* posAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::POSITION, mSVIGLSurface);
		posAni->mDuration = duration;
		posAni->mOffset = offset;

		// from point
		posAni->addKeyProperty(SVIKeyFrameProperty(0.0f, prePosition));
		// to point
		posAni->addKeyProperty(SVIKeyFrameProperty(1.0f, nextPosition));
		
		aniSet->addAnimation(posAni);
	}


	void SVITransitionEffector::addOpacityAnimation(SVIAnimationSet* aniSet, SVIFloat preOpacity, SVIFloat nextOpacity, SVIInt duration, SVIInt offset) {
	
		SVIKeyFrameAnimation* posAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::OPACITY, mSVIGLSurface);
		posAni->mDuration = duration;
		posAni->mOffset = offset;

		// from opacity
		posAni->addKeyProperty(SVIKeyFrameProperty(0.0f, preOpacity));
		// to opacity
		posAni->addKeyProperty(SVIKeyFrameProperty(1.0f, nextOpacity));
		
		aniSet->addAnimation(posAni);
	}

	void SVITransitionEffector::addScaleToFitRegionAnimation(SVIAnimationSet* aniSet, SVIRect preRect, SVIRect nextRect, SVIInt duration, SVIInt offset) {

		SVIKeyFrameAnimation* scaleToFitRegionAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::TEXTURE_SCALEFIT_REGION, mSVIGLSurface);
		scaleToFitRegionAni->mDuration = duration;
		scaleToFitRegionAni->mOffset = offset;

		// from region
		scaleToFitRegionAni->addKeyProperty(SVIKeyFrameProperty(0.0f, preRect));
		// to region
		scaleToFitRegionAni->addKeyProperty(SVIKeyFrameProperty(1.0f, nextRect));

		aniSet->addAnimation(scaleToFitRegionAni);
	}

	//2012-06-21 masterkeaton27@gmail.com
	//new shader effect property for mobile office
	void SVITransitionEffector::addSwipeAnimation(SVIAnimationSet* aniSet, SVIVector3 preSwipe, SVIVector3 nextSwipe, SVIInt duration, SVIInt offset) {

		SVIKeyFrameAnimation* swipeAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::SWIPE_RATIO, mSVIGLSurface);
		swipeAni->mDuration = duration;
		swipeAni->mOffset = offset;

		// from Swipe
		swipeAni->addKeyProperty(SVIKeyFrameProperty(0.0f, preSwipe));
		// to Swipe
		swipeAni->addKeyProperty(SVIKeyFrameProperty(1.0f, nextSwipe));

		aniSet->addAnimation(swipeAni);
	}


	void SVITransitionEffector::addDeformAnimation(SVIAnimationSet* aniSet, SVIVector3 preDeform, SVIVector3 nextDeform, SVIInt duration, SVIInt offset) {

		SVIKeyFrameAnimation* deformAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::DEFORM_RATIO, mSVIGLSurface);
		deformAni->mDuration = duration;
		deformAni->mOffset = offset;

		// from Deform
		deformAni->addKeyProperty(SVIKeyFrameProperty(0.0f, preDeform));
		// to Deform
		deformAni->addKeyProperty(SVIKeyFrameProperty(1.0f, nextDeform));

		aniSet->addAnimation(deformAni);
	}


	void SVITransitionEffector::addColorAnimation(SVIAnimationSet* aniSet, SVIColor preColor, SVIColor nextColor, SVIInt duration, SVIInt offset) {

		SVIKeyFrameAnimation* ColorAni = SVIKeyFrameAnimation::create(SVIPropertyAnimation::BG_COLOR,mSVIGLSurface);
		ColorAni->mDuration = duration;
		ColorAni->mOffset = offset;

		// from Color
		ColorAni->addKeyProperty(SVIKeyFrameProperty(0.0f, preColor));
		// to Color
		ColorAni->addKeyProperty(SVIKeyFrameProperty(1.0f, nextColor));

		aniSet->addAnimation(ColorAni);
	}

}
