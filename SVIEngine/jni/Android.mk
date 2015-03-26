# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := SVIEngine
LOCAL_CFLAGS := -w

PARTICLE = \
	SVI/Particle/Updater/SVIParticleUpdater.cpp \
	SVI/Particle/Updater/SVIPhysicParticleUpdater.cpp \
	SVI/Particle/Updater/SVILinearParticleUpdater.cpp \
	SVI/Particle/SVIParticles.cpp \
	SVI/Particle/SVIParticleEffect.cpp \
	SVI/Particle/SVIParticleEffectManager.cpp \
	SVI/Particle/SVIKeyFrameParticles.cpp \
	SVI/Particle/SVIKeyFrameParticleEffect.cpp \
	
ANIMATION = \
	SVI/Animation/SVIAnimation.cpp \
    SVI/Animation/SVIAnimationSet.cpp \
    SVI/Animation/SVIBasicAnimation.cpp \
    SVI/Animation/SVIKeyFrameAnimation.cpp \
    SVI/Animation/SVIKeyFrameProperty.cpp \
    SVI/Animation/SVIPropertyAnimation.cpp \
    SVI/Animation/SVISpriteAnimation.cpp \
    SVI/Animation/SVIUnionProperty.cpp \
    SVI/Animation/SVIAnimationListenerProxy.cpp \
	SVI/Animation/SVITransitionAnimation.cpp \
	SVI/Animation/SVIParticleAnimation.cpp \
	SVI/Animation/SVIParticleKeyFrameAnimation.cpp \
	SVI/Animation/SVIParticleKeyFrameProperty.cpp \
	SVI/Animation/SVIParticlePropertyAnimation.cpp \
	
	
TRANSITION = \
	SVI/Animation/Transition/SVITransitionEffector.cpp \
	SVI/Animation/Transition/SVIFlipEffector.cpp \
	SVI/Animation/Transition/SVIRotateEffector.cpp \
	SVI/Animation/Transition/SVIRotateCubeEffector.cpp \
	SVI/Animation/Transition/SVIScaleEffector.cpp \
	SVI/Animation/Transition/SVISlideEffector.cpp \
	SVI/Animation/Transition/SVISmoothEffector.cpp \
	SVI/Animation/Transition/SVITumbleEffector.cpp \
	SVI/Animation/Transition/SVICurtainEffector.cpp \
	SVI/Animation/Transition/SVIDominoFlipEffector.cpp \
	SVI/Animation/Transition/SVISideWindowEffector.cpp \
	SVI/Animation/Transition/SVIExploidEffector.cpp \
	SVI/Animation/Transition/SVIWaveEffector.cpp \
	SVI/Animation/Transition/SVIWizzleEffector.cpp \
	SVI/Animation/Transition/SVIZoomINEffector.cpp \
	SVI/Animation/Transition/SVIBlindEffector.cpp \
	SVI/Animation/Transition/SVITwistEffector.cpp \
	SVI/Animation/Transition/SVIBreakEffector.cpp \
	SVI/Animation/Transition/SVIThrowEffector.cpp \
	SVI/Animation/Transition/SVIFlashEffector.cpp \
	SVI/Animation/Transition/SVIFlyThroughEffector.cpp \
	SVI/Animation/Transition/SVITwirlEffector.cpp \
	SVI/Animation/Transition/SVISwapEffector.cpp \
	SVI/Animation/Transition/SVISwooshEffector.cpp \
	SVI/Animation/Transition/SVIHoneyCombEffector.cpp \
	SVI/Animation/Transition/SVIBoxEffector.cpp \
	SVI/Animation/Transition/SVICoverEffector.cpp \
	SVI/Animation/Transition/SVIUncoverEffector.cpp \
	SVI/Animation/Transition/SVIFallEffector.cpp \
	SVI/Animation/Transition/SVIFadeThroughColorEffector.cpp \
	SVI/Animation/Transition/SVIRevolvingDoorEffector.cpp \
	SVI/Animation/Transition/SVISwitchEffector.cpp \
	SVI/Animation/Transition/SVIGalleryEffector.cpp \
	SVI/Animation/Transition/SVIFlip2Effector.cpp \
	SVI/Animation/Transition/SVIMoveInEffector.cpp \
	SVI/Animation/Transition/SVIWipeEffector.cpp \
	SVI/Animation/Transition/SVIRevealEffector.cpp \
	SVI/Animation/Transition/SVIShapeEffector.cpp \
	SVI/Animation/Transition/SVISplitEffector.cpp \
	SVI/Animation/Transition/SVICube4PiecesEffector.cpp \
	SVI/Animation/Transition/SVIShredEffector.cpp \
	SVI/Animation/Transition/SVICheckerBoardEffector.cpp \
	SVI/Animation/Transition/SVICenterBlindEffector.cpp \
	SVI/Animation/Transition/SVICube2PiecesEffector.cpp \
	SVI/Animation/Transition/SVIBrickCubeEffector.cpp \
	SVI/Animation/Transition/SVIFoldingScreenEffector.cpp \
	SVI/Animation/Transition/SVIGallery2Effector.cpp


MATHEMATICS = \
	SVI/Mathematics/SVIMatrix.cpp \
    SVI/Mathematics/SVIPlane.cpp \
    SVI/Mathematics/SVIQuaternion.cpp \
    SVI/Mathematics/SVIVector2.cpp \
    SVI/Mathematics/SVIVector3.cpp \
    SVI/Mathematics/SVIVector4.cpp \
	SVI/Mathematics/SVIBound.cpp \
	SVI/Mathematics/SVIMathUtil.cpp
	
GEOMETRY = \
	SVI/Render/Geometry/SVIPoly.cpp \
	SVI/Render/Geometry/SVIExplodePoly.cpp \
	SVI/Render/Geometry/SVIQuadPoly.cpp \
	SVI/Render/Geometry/SVIRoundPoly.cpp \
	SVI/Render/Geometry/SVITwistPoly.cpp \
	SVI/Render/Geometry/SVIBrickPoly.cpp \
	SVI/Render/Geometry/SVIPannelPoly.cpp \
	SVI/Render/Geometry/SVIDomePoly.cpp \
	SVI/Render/Geometry/SVIDrumPoly.cpp \
	SVI/Render/Geometry/SVIConeCurvePoly.cpp \
	SVI/Render/Geometry/SVIPathPoly.cpp \
	SVI/Render/Geometry/SVITesselationPoly.cpp \
	

RENDER = \
	SVI/Render/SVIDebugRenderer.cpp \
    SVI/Render/SVIFrameRenderer.cpp \
    SVI/Render/SVIProgramManager.cpp \
	SVI/Render/SVIBinaryProgram.cpp \
    SVI/Render/SVIRenderer.cpp \
    SVI/Render/SVIRenderPatch.cpp \
    SVI/Render/SVIRenderScene.cpp \
    SVI/Render/SVITexture.cpp \
    SVI/Render/SVITextureManager.cpp \
    SVI/Render/SVIViewport.cpp \
	SVI/Render/SVI3DCamera.cpp \
	SVI/Render/SVI3DScene.cpp \
	SVI/Render/SVIFaceBuffer.cpp \
	SVI/Render/SVIModelManager.cpp \
	SVI/Render/SVIRigidMesh.cpp \
	SVI/Render/SVISceneNode.cpp \
	SVI/Render/SVIModelNode.cpp \
	SVI/Render/SVIStaticModelNode.cpp \
	SVI/Render/SVIMaterial.cpp \
	SVI/Render/SVIParticleRenderer.cpp \
	SVI/Render/SVIPostEffectRenderer.cpp \

PHYSICS = \
	SVI/Physics/SVISpring.cpp \
	SVI/Physics/SVIPivot.cpp \
	SVI/Physics/SVIClothSimulator.cpp \
	SVI/Physics/SVICurveSimulator.cpp \
	SVI/Physics/SVITwistSimulator.cpp \
	SVI/Physics/SVIExplodeSimulator.cpp \
	SVI/Physics/SVIDomeSimulator.cpp \
	SVI/Physics/SVIDrumSimulator.cpp \
	SVI/Physics/SVIConeCurveSimulator.cpp \
	
UTILS = \
	SVI/Utils/SVIMutex.cpp \
    SVI/Utils/SVITime.cpp \
	SVI/Utils/SVIFTrace.cpp
	
TEST_BED_DELEGATOR = \
	SVI/TestBedDelegator/TestBedDelegator.cpp \
    SVI/TestBedDelegator/TestBedForAnimation.cpp \
    SVI/TestBedDelegator/TestBedForPerformance.cpp
	
SLIDE = \
	SVI/Slide/SVIBaseSlide.cpp \
    SVI/Slide/SVIBeamedSlide.cpp \
    SVI/Slide/SVIProjectionSlide.cpp \
    SVI/Slide/SVIRenderPartManager.cpp \
    SVI/Slide/SVISlide.cpp
	

NATIVE_INTERFACE = \
	SVI/NativeInterface/nativeGLSurface.cpp \
	SVI/NativeInterface/nativeGLExternalSurface.cpp \
	SVI/NativeInterface/nativeGLWallpaper.cpp \
    SVI/NativeInterface/nativeSlide.cpp \
    SVI/NativeInterface/nativeAnimation.cpp \
    SVI/NativeInterface/nativeAnimationSet.cpp \
    SVI/NativeInterface/nativeBasicAnimation.cpp \
    SVI/NativeInterface/nativeKeyFrameAnimation.cpp \
    SVI/NativeInterface/nativeSpriteAnimation.cpp \
	SVI/NativeInterface/nativeTransitionAnimation.cpp \
    SVI/NativeInterface/nativeSVIImage.cpp \
    SVI/NativeInterface/nativeSlideManager.cpp \
	SVI/NativeInterface/nativeFTrace.cpp \
	SVI/NativeInterface/nativeSceneNode.cpp \
	SVI/NativeInterface/nativeParticleAnimation.cpp \
	SVI/NativeInterface/nativeParticleEffect.cpp \
	SVI/NativeInterface/nativeParticles.cpp \
	SVI/NativeInterface/nativePostEffectManager.cpp \

GLSURFACE = \
    SVI/GLSurface/SVIGLSurface.cpp \
	

LOCAL_SRC_FILES += \
	SVI/Animation/Interpolator/SVIInterpolator.cpp \
	$(PARTICLE)\
    $(ANIMATION)\
	$(TRANSITION)\
	$(MATHEMATICS)\
	$(GEOMETRY)\
	$(RENDER)\
    $(PHYSICS)\
    $(UTILS)\
	$(TEST_BED_DELEGATOR)\
    $(SLIDE)\
    $(NATIVE_INTERFACE)\
	$(GLSURFACE)

LOCAL_C_INCLUDES += \
	SVI \
	SVI/Particle \
	SVI/Animation \
	SVI/Animation/Transition \
	SVI/Animation/Interpolator \
	SVI/BaseType \
	SVI/Mathematics \
	SVI/Render/DefaultProgram \
	SVI/Render \
	SVI/Slide \
	SVI/NativeInterface \
	SVI/Utils \
	SVI/GLSurface \

LOCAL_LDLIBS := -llog -lGLESv2 -ljnigraphics -landroid
	
include $(BUILD_SHARED_LIBRARY)


