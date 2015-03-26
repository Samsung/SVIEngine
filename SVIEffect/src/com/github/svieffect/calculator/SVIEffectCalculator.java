/**
 * SGIEffectCalculator.java 
 * @author Joonyong Park
 * @email masterkeaton27@gmail.com
 */

package com.github.svieffect.calculator;

import java.util.Random;
import java.util.Vector;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;
import android.view.MotionEvent;

import com.github.svieffect.main.SVIEffectNode;
import com.github.svieffect.main.SVIEffectNodeCallback;
import com.github.svieffect.main.SVIEffectView;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.basetype.SVIVector4;
import com.github.sviengine.slide.SVISlide;

public class SVIEffectCalculator extends SVIEffectView {
	
	public static final int FOCUS_GUI_01 = 0;
	public static final int FOCUS_GUI_02 = 1;
	public static final int FOCUS_GUI_03 = 2;
	
	public static final int HOVER_GUI_01 = 0;
	public static final int HOVER_GUI_02 = 1;
	public static final int HOVER_GUI_03 = 2;
	
	private static final float 		SENSOR_SCALE = 1.3f;

	private static final float		SHADOW_RADIUS = 25.0f;
	private static final float		DEPTH_SCALE = 1.0f;
	
	private static final int 		BUTTON_ANIMATION_DURATION_UP = 300;
	private static final int 		BUTTON_ANIMATION_DURATION_DOWN = 100;
	

	private static final int 		BUTTON_ANIMATION_DURATION_HOVER_DOWN = 80;
	private static final int 		BUTTON_ANIMATION_DURATION_HOVER_UP = 150;
	
	private float mAnimationSpeed = 1.0f;
	private int mAnimationDuration = BUTTON_ANIMATION_DURATION_DOWN;
	

	public SVIEffectCalculator(Context context) {
		super(context);
		//SVIPostEffectManager.getInstance().enable(false);
		// TODO Auto-generated constructor stub
		this.setAnimationCallback(mAnimationCallback);
	}
			
	public void initialize(){
		setRenderFPS(20);
		super.initialize();
	}
	
	public void setAnimationSpeed(float speed){
		mAnimationSpeed = speed;
	}
	
	public float getAnimationSpeed(){
		return mAnimationSpeed;
	}

	public void setAnimationDuration(int duration){
		mAnimationDuration = duration;
	}
	
	public int getAnimationDuration(){
		return mAnimationDuration;
	}
	
	private Bitmap	mPopupOverlap = null;
	
	private Bitmap	mPopup02Overlap = null;
	private Bitmap	mPopup03Overlap = null;
	
	
	private Bitmap	mFocusOverlap = null;
	//2013-03-13 masterkeaton27@gmail.com
	//2 types of image added. due to deepak's request
	private Bitmap	mFocus02Overlap = null;
	private Bitmap	mFocus03Overlap = null;
	
			
	private Vector<SVIEffectPopupView> mPopupViews = new Vector<SVIEffectPopupView>();
	private Vector<SVIEffectFocusView> mFocusViews = new Vector<SVIEffectFocusView>();

	//HOVER_GUI_01
	private SVIEffectPopupView createPopupView(int index, int imageType){
		SVISlide rootSlide = getRoot();				
		if (rootSlide == null) {
			return null;
		}

		SVIEffectPopupView popupView = new SVIEffectPopupView();		
		float [] colorArray = {1.0f, 1.0f, 1.0f, 1.0f}; 
		SVISlide slide = new SVISlide(0, rootSlide.getNativeSlideHandle(), 
				0.0f, 0.0f, 1.0f, 1.0f, colorArray);
		
		popupView.mOverlapSlide = slide;
				
		
		if (mPopupOverlap != null){
			popupView.mImage = new SVIImage();
			if (imageType == HOVER_GUI_01){
				popupView.mImage.setBitmap(mPopupOverlap);
			}else if (imageType == HOVER_GUI_02){
				popupView.mImage.setBitmap(mPopup02Overlap);
			}else if (imageType == HOVER_GUI_03){
				popupView.mImage.setBitmap(mPopup03Overlap);
			}
			
			slide.setImage(popupView.mImage);
		}else{
			slide.setBackgroundColor(new SVIColor(0.4f,0.4f,1.0f,1.0f));
		}
		
		slide.setOpacity(0.0f);
		popupView.mIndex = index;		
		mPopupViews.add(popupView);
		return popupView;
	}
	
	private SVIEffectPopupView findPopupView(int index){
		if (mPopupViews.size() == 0) return null;		
		for (int n =0; n < mPopupViews.size(); n++){
			SVIEffectPopupView popupView = mPopupViews.get(n);
			if (popupView != null && popupView.mIndex == index){
				return popupView; 
			}
		}
		return null;
	}
	
	private SVIEffectFocusView createFocusView(int index, int imageType){
		SVISlide rootSlide = getRoot();				
		if (rootSlide == null) {
			return null;
		}

		SVIEffectFocusView focusView = new SVIEffectFocusView();		
		float [] colorArray = {1.0f, 1.0f, 1.0f, 1.0f};
		SVISlide slide = new SVISlide(0, rootSlide.getNativeSlideHandle(), 
				0.0f, 0.0f, 1.0f, 1.0f, colorArray);

		focusView.mOverlapSlide = slide;
		focusView.mImage = new SVIImage();
				
		if (imageType == FOCUS_GUI_01){
			focusView.mImage.setBitmap(mFocusOverlap);
		}else if (imageType == FOCUS_GUI_02){
			focusView.mImage.setBitmap(mFocus02Overlap);
		}else if (imageType == FOCUS_GUI_03){
			focusView.mImage.setBitmap(mFocus03Overlap);
		}
		
		slide.setImage(focusView.mImage);
		slide.setOpacity(0.0f);
		focusView.mIndex = index;		
		mFocusViews.add(focusView);
		return focusView;
	}
			
	
	private SVIEffectFocusView findFocusView(int index){
		if (mFocusViews.size() == 0) return null;		
		for (int n =0; n < mFocusViews.size(); n++){
			SVIEffectFocusView focusView = mFocusViews.get(n);
			if (focusView != null && focusView.mIndex == index){
				return focusView; 
			}
		}
		return null;
	}
	
	
	protected class SVIEffectPopupView{				
		public boolean mIsHovered = false;
		
		public SVIEffectPopupView(){
			
		}
				
		public boolean show(){
			if (mIsHovered) return false;
			runShowAnimation(mOverlapSlide);
			mIsHovered = true;
			return true;
		}
		
		public void hide(){
			if (!mIsHovered) return;
			runHideAnimation(mOverlapSlide);
			mIsHovered = false;
		}
		
		public void runShowAnimation(SVISlide slide){
			if (slide == null) return;
			int duration = (int)(BUTTON_ANIMATION_DURATION_HOVER_DOWN * mAnimationSpeed);
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
			aniSet.setOffset(0);									
			
			SVIKeyFrameAnimation keyColorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.OPACITY);
			keyColorAni.setDuration(duration);
			keyColorAni.setRepeatCount(0);
			keyColorAni.setAutoReverse(false);
			keyColorAni.setOffset(0);
			keyColorAni.addKeyProperty(0.0f, 0.0f); 
			keyColorAni.addKeyProperty(1.0f, 1.0f); 
			aniSet.addAnimation(keyColorAni);		
											
			slide.setOpacity(1.0f);
			slide.startAnimation(aniSet);
		}
		
		public void runHideAnimation(SVISlide slide){
			if (slide == null) return;
			int duration = (int)(BUTTON_ANIMATION_DURATION_HOVER_UP * mAnimationSpeed);
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
			aniSet.setOffset(0);									
			
			SVIKeyFrameAnimation keyColorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.OPACITY);
			keyColorAni.setDuration(duration);
			keyColorAni.setRepeatCount(0);
			keyColorAni.setAutoReverse(false);
			keyColorAni.setOffset(0);
			keyColorAni.addKeyProperty(0.0f, 1.0f); 
			keyColorAni.addKeyProperty(1.0f, 0.0f); 
			aniSet.addAnimation(keyColorAni);		
											
			slide.setOpacity(0.0f);
			slide.startAnimation(aniSet);
		}
		
		public int	  mIndex = -1;	
		
		public SVIImage mImage = null;
		public SVISlide mOverlapSlide = null;		
	};
	
	
	protected class SVIEffectFocusView{
		
		
		public SVIEffectFocusView(){
			
		}
				
		public boolean show(){
			runShowAnimation(mOverlapSlide);
			return true;
		}
		
		public void hide(){
			runHideAnimation(mOverlapSlide);
		}
		
		private void runShowAnimation(SVISlide slide){
			if (slide == null) return;
			int duration = (int)(BUTTON_ANIMATION_DURATION_HOVER_DOWN * mAnimationSpeed);
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
			aniSet.setOffset(0);									
			
			SVIKeyFrameAnimation keyColorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.OPACITY);
			keyColorAni.setDuration(duration);
			keyColorAni.setRepeatCount(0);
			keyColorAni.setAutoReverse(false);
			keyColorAni.setOffset(0);
			keyColorAni.addKeyProperty(0.0f, 0.0f); 
			keyColorAni.addKeyProperty(1.0f, 1.0f); 
			aniSet.addAnimation(keyColorAni);		
											
			slide.setOpacity(1.0f);
			slide.startAnimation(aniSet);
		}
		
		private void runHideAnimation(SVISlide slide){
			if (slide == null) return;
			int duration = (int)(BUTTON_ANIMATION_DURATION_HOVER_UP * mAnimationSpeed);
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
			aniSet.setOffset(0);									
			
			SVIKeyFrameAnimation keyColorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.OPACITY);
			keyColorAni.setDuration(duration);
			keyColorAni.setRepeatCount(0);
			keyColorAni.setAutoReverse(false);
			keyColorAni.setOffset(0);
			keyColorAni.addKeyProperty(0.0f, 1.0f); 
			keyColorAni.addKeyProperty(1.0f, 0.0f); 
			aniSet.addAnimation(keyColorAni);		
											
			slide.setOpacity(0.0f);
			slide.startAnimation(aniSet);
		}
		
		public int	  mIndex = -1;
		
		public SVIImage mImage = null;
		public SVISlide mOverlapSlide = null;		
	};
	
	public void clearNodes(){
		super.clearNodes();
		
		if (mPopupViews.size() > 0){
			for (int n =0; n < mPopupViews.size(); n++){
				SVIEffectPopupView popupView = mPopupViews.get(n);
				if (popupView.mOverlapSlide != null){
					popupView.mImage = null;
					popupView.mOverlapSlide.setImage((SVIImage)null);
				}
			}	
			mPopupViews.clear();
		}
		
		if (mFocusViews.size() > 0){
			for (int n =0; n < mFocusViews.size(); n++){
				SVIEffectFocusView focusView = mFocusViews.get(n);
				if (focusView.mOverlapSlide != null){
					focusView.mImage = null;
					focusView.mOverlapSlide.setImage((SVIImage)null);
				}
			}
			mFocusViews.clear();		
		}
	}
	
	@Deprecated
	public void setHoverOverlapGUI(Bitmap bitmap){
		mPopupOverlap = bitmap;
	}
			
	public void setHoverOverlapGUI(Bitmap bitmap, int index){
		if (index == HOVER_GUI_01){
			mPopupOverlap = bitmap;
		}else if (index == HOVER_GUI_02){
			mPopup02Overlap = bitmap;
		}else if (index == HOVER_GUI_03){
			mPopup03Overlap = bitmap;
		}
	}	
	
	@Deprecated
	public void setFocusOverlapGUI(Bitmap bitmap){
		mFocusOverlap = bitmap;
	}	
		
	public void setFocusOverlapGUI(Bitmap bitmap, int index){
		if (index == FOCUS_GUI_01){
			mFocusOverlap = bitmap;
		}else if (index == FOCUS_GUI_02){
			mFocus02Overlap = bitmap;
		}else if (index == FOCUS_GUI_03){
			mFocus03Overlap = bitmap;
		}
	}	
	
	@Deprecated
	public boolean validateOnLongClick(){
		super.validateOnLongClick();
		return false;
	}
	
	@Deprecated
	public boolean validateOnHover(final MotionEvent inputEvent){
		super.validateOnHover(inputEvent);
		return false;
	}
	
	@Deprecated
	public boolean validateOnTouch(RequestType type, final MotionEvent inputEvent){
		super.validateOnTouch(type, inputEvent);
		return false;
	}
				
	
	public void onHoverEnter(int btnIndex){
		SVIEffectPopupView popupView = findPopupView(btnIndex);		
		if (popupView == null){
			popupView = createPopupView(btnIndex, HOVER_GUI_01); 
		}		
		SVIEffectNode btnNode = findNode(btnIndex);
		
		if (btnNode == null) {
			Log.e("SGIEffect-Calculator", "Can't find matching node");
			return;
		}

		SVISlide slide = btnNode.getSlide();
		SVIRect region = slide.getRegion();
		
		popupView.mOverlapSlide.setRegion(
				region.mOrigin.mX, 
				region.mOrigin.mY, 
				region.mSize.mWidth, 
				region.mSize.mHeight);
		
		popupView.show();
	}
	
	//2013-03-18 masterkeaton27@gmail.com
	//for deepak's request
	public void onHoverEnter(int btnIndex, int imageType){
		SVIEffectPopupView popupView = findPopupView(btnIndex);		
		if (popupView == null){
			popupView = createPopupView(btnIndex, imageType); 
		}		
		SVIEffectNode btnNode = findNode(btnIndex);
		
		if (btnNode == null) {
			Log.e("SGIEffect-Calculator", "Can't find matching node");
			return;
		}

		SVISlide slide = btnNode.getSlide();
		SVIRect region = slide.getRegion();
		
		popupView.mOverlapSlide.setRegion(
				region.mOrigin.mX, 
				region.mOrigin.mY, 
				region.mSize.mWidth, 
				region.mSize.mHeight);
		
		popupView.show();
	}
	
	public void onHoverExit(int btnIndex){

		SVIEffectPopupView popupView = findPopupView(btnIndex);
		if (popupView == null){
			return;
		}		
		popupView.hide();
	}
		
	public void onTouchDown(int btnIndex){
		SVIEffectNode node = findNode(btnIndex);
		
		if (node == null) return;
	
		SVISlide slide = node.getSlide();
		SVISlide contentSlide = node.getContentSlide();
		SVISlide overlayContentSlide = node.getOverlayContentSlide();
		
		if (mAnimationCallback != null){
			mAnimationCallback.runDownAnimation(slide);
			mAnimationCallback.runDownContentsAnimation(contentSlide);
			if (overlayContentSlide != null) 
				mAnimationCallback.runDownContentsAnimation(overlayContentSlide);
		}
	}
	
	public void onTouchUp(int btnIndex){
		SVIEffectNode node = findNode(btnIndex);
		
		if (node == null) return;
	
		SVISlide slide = node.getSlide();
		SVISlide contentSlide = node.getContentSlide();
		SVISlide overlayContentSlide = node.getOverlayContentSlide();
		
		if (mAnimationCallback != null){
			mAnimationCallback.runUpAnimation(slide);
			mAnimationCallback.runUpContentsAnimation(contentSlide);
			if (overlayContentSlide != null) 
				mAnimationCallback.runUpContentsAnimation(overlayContentSlide);
		}
	}	
	
	//2013-03-18 masterkeaton27@gmail.com
	//for deepak's request
	public void onFocusedEnter(int btnIndex, int imageType){
		boolean isUsingColor = false;		
		SVIEffectFocusView focusView = findFocusView(btnIndex);		
		if (focusView == null && !isUsingColor){
			focusView = createFocusView(btnIndex, imageType); 
			if (focusView == null)
				return;
		}
		
		SVIEffectNode btnNode = findNode(btnIndex);				
		if (btnNode == null) {
			Log.e("SVIEffect-Calculator", "Can't find matching node (btnIndex: " + btnIndex + ", nodeSize: " + mNodes.size() + ")");
			return;
		}				
		
		SVISlide slide = btnNode.getSlide();
		SVIRect region = slide.getRegion();
		focusView.mOverlapSlide.setRegion(
				region.mOrigin.mX, 
				region.mOrigin.mY, 
				region.mSize.mWidth, 
				region.mSize.mHeight);
		
		focusView.show();		
	}
	
	public void onFocusedEnter(int btnIndex){
		onFocusedEnter(btnIndex, FOCUS_GUI_01);		
	}		
	
	public void onFocusedExit(int btnIndex){
		SVIEffectFocusView focusView = findFocusView(btnIndex);
		if (focusView == null){
			return;
		}		
		focusView.hide();		
	}		
	
	@Override
	public void destroy(){
		if (mPopupViews.size() > 0){
			for (int n =0; n < mPopupViews.size(); n++){
				SVIEffectPopupView popupView = mPopupViews.get(n);
				if (popupView.mOverlapSlide != null){
					popupView.mImage = null;
					popupView.mOverlapSlide.setImage((SVIImage)null);
				}
			}	
			mPopupViews.clear();
		}
		
		if (mFocusViews.size() > 0){
			for (int n =0; n < mFocusViews.size(); n++){
				SVIEffectFocusView focusView = mFocusViews.get(n);
				if (focusView.mOverlapSlide != null){
					focusView.mImage = null;
					focusView.mOverlapSlide.setImage((SVIImage)null);
				}
			}
			mFocusViews.clear();		
		}
		super.destroy();
	}
	
	@Override
	public void finalize(){
		if (mPopupViews.size() > 0){
			for (int n =0; n < mPopupViews.size(); n++){
				SVIEffectPopupView popupView = mPopupViews.get(n);
				if (popupView.mOverlapSlide != null){
					popupView.mImage = null;
					popupView.mOverlapSlide.setImage((SVIImage)null);
				}
			}	
			mPopupViews.clear();
		}
		
		if (mFocusViews.size() > 0){
			for (int n =0; n < mFocusViews.size(); n++){
				SVIEffectFocusView focusView = mFocusViews.get(n);
				if (focusView.mOverlapSlide != null){
					focusView.mImage = null;
					focusView.mOverlapSlide.setImage((SVIImage)null);
				}
			}
			mFocusViews.clear();		
		}
		super.finalize();		
	}
	
	class SVIEffectCalculatorAnimCallback extends SVIEffectNodeCallback{		
		
		private boolean mChangeRotation = false;
		private Random mRandom = new Random();
		
		public SVIEffectCalculatorAnimCallback(){}	
		
		public void runUpContentsAnimation(SVISlide slide){
			if (slide == null) return;
			
			int duration = (int)(BUTTON_ANIMATION_DURATION_UP * mAnimationSpeed);
			int duration_min = (int)(BUTTON_ANIMATION_DURATION_UP * mAnimationSpeed);
			
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
			aniSet.setOffset(0);			
						
			
			SVIKeyFrameAnimation keyColorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.BG_COLOR);
			keyColorAni.setDuration(duration_min);
			keyColorAni.setRepeatCount(0);
			keyColorAni.setAutoReverse(false);
			keyColorAni.setOffset(0);
			keyColorAni.addKeyProperty(0.0f, new SVIColor(0.8f,0.8f,0.8f,1.0f)); //x ratio, radius, range
			keyColorAni.addKeyProperty(1.0f, new SVIColor(1.0f,1.0f,1.0f,1.0f)); //x ratio, radius, range
			aniSet.addAnimation(keyColorAni);		
			
			SVIKeyFrameAnimation keyShadowOpacityAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_OPACITY);
			keyShadowOpacityAni.setDuration(duration_min);
			keyShadowOpacityAni.setRepeatCount(0);
			keyShadowOpacityAni.setAutoReverse(false);
			keyShadowOpacityAni.setOffset(0);
			keyShadowOpacityAni.addKeyProperty(0.0f, 0.08f); //x ratio, radius, range
			keyShadowOpacityAni.addKeyProperty(1.0f, 0.0f); //x ratio, radius, range
			aniSet.addAnimation(keyShadowOpacityAni);		
			
			SVIKeyFrameAnimation keyShadowRadiusAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_RADIUS);
			keyShadowRadiusAni.setDuration(duration_min);
			keyShadowRadiusAni.setRepeatCount(0);
			keyShadowRadiusAni.setAutoReverse(false);
			keyShadowRadiusAni.setOffset(0);
			keyShadowRadiusAni.addKeyProperty(0.0f, SHADOW_RADIUS); //x ratio, radius, range
			keyShadowRadiusAni.addKeyProperty(1.0f, 0.0f); //x ratio, radius, range
			aniSet.addAnimation(keyShadowRadiusAni);		
			
			SVIPoint offset = slide.getShadowOffect();
			
			SVIKeyFrameAnimation keyShadowOffsetAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_OFFSET);
			keyShadowOffsetAni.setDuration(duration_min);
			keyShadowOffsetAni.setRepeatCount(0);
			keyShadowOffsetAni.setAutoReverse(false);
			keyShadowOffsetAni.setOffset(0);
			keyShadowOffsetAni.addKeyProperty(0.0f, offset); //x ratio, radius, range
			keyShadowOffsetAni.addKeyProperty(1.0f, new SVIPoint(0.0f,0.0f)); //x ratio, radius, range
			aniSet.addAnimation(keyShadowOffsetAni);				
			
			
			SVIKeyFrameAnimation keyScaleAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SCALE);
			keyScaleAni.setDuration(duration_min);
			keyScaleAni.setRepeatCount(0);
			keyScaleAni.setAutoReverse(false);
			keyScaleAni.setOffset(0);
			keyScaleAni.addKeyProperty(0.0f, new SVIVector3(0.85f,0.85f,1.0f)); //x ratio, radius, range
			keyScaleAni.addKeyProperty(1.0f, new SVIVector3(1.0f,1.0f,1.0f)); //x ratio, radius, range
						
			aniSet.addAnimation(keyScaleAni);			
									
			slide.setOpacity(1.0f);
			slide.startAnimation(aniSet);
		}
		
		public void runDownContentsAnimation(SVISlide slide){
			if (slide == null) return;
			
			int duration = (int)(BUTTON_ANIMATION_DURATION_DOWN * mAnimationSpeed);
			int duration_min = (int)(BUTTON_ANIMATION_DURATION_DOWN * mAnimationSpeed);
			
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
			aniSet.setOffset(0);			
						
			
			SVIKeyFrameAnimation keyColorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.BG_COLOR);
			keyColorAni.setDuration(duration_min);
			keyColorAni.setRepeatCount(0);
			keyColorAni.setAutoReverse(false);
			keyColorAni.setOffset(0);
			keyColorAni.addKeyProperty(0.0f, new SVIColor(1.0f,1.0f,1.0f,1.0f)); //x ratio, radius, range
			keyColorAni.addKeyProperty(1.0f, new SVIColor(0.8f,0.8f,0.8f,1.0f)); //x ratio, radius, range
			aniSet.addAnimation(keyColorAni);		
			
			
			SVIKeyFrameAnimation keyShadowOpacityAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_OPACITY);
			keyShadowOpacityAni.setDuration(duration_min);
			keyShadowOpacityAni.setRepeatCount(0);
			keyShadowOpacityAni.setAutoReverse(false);
			keyShadowOpacityAni.setOffset(0);
			keyShadowOpacityAni.addKeyProperty(0.0f, 0.0f); //x ratio, radius, range
			keyShadowOpacityAni.addKeyProperty(1.0f, 0.08f); //x ratio, radius, range
			aniSet.addAnimation(keyShadowOpacityAni);		
			
			SVIKeyFrameAnimation keyShadowRadiusAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_RADIUS);
			keyShadowRadiusAni.setDuration(duration_min);
			keyShadowRadiusAni.setRepeatCount(0);
			keyShadowRadiusAni.setAutoReverse(false);
			keyShadowRadiusAni.setOffset(0);
			keyShadowRadiusAni.addKeyProperty(0.0f, 0.0f); //x ratio, radius, range
			keyShadowRadiusAni.addKeyProperty(1.0f, SHADOW_RADIUS); //x ratio, radius, range
			aniSet.addAnimation(keyShadowRadiusAni);		
						
			SVIPoint offset = slide.getShadowOffect();
			
			SVIKeyFrameAnimation keyShadowOffsetAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_OFFSET);
			keyShadowOffsetAni.setDuration(duration_min);
			keyShadowOffsetAni.setRepeatCount(0);
			keyShadowOffsetAni.setAutoReverse(false);
			keyShadowOffsetAni.setOffset(0);
			keyShadowOffsetAni.addKeyProperty(0.0f, new SVIPoint(0.0f,0.0f)); //x ratio, radius, range
			keyShadowOffsetAni.addKeyProperty(1.0f, offset); //x ratio, radius, range
			aniSet.addAnimation(keyShadowOffsetAni);		
					
					
			SVIKeyFrameAnimation keyScaleAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SCALE);
			keyScaleAni.setDuration(duration_min);
			keyScaleAni.setRepeatCount(0);
			keyScaleAni.setAutoReverse(false);
			keyScaleAni.setOffset(0);
			keyScaleAni.addKeyProperty(0.0f, new SVIVector3(1.0f,1.0f,1.0f)); //x ratio, radius, range
			keyScaleAni.addKeyProperty(1.0f, new SVIVector3(0.85f,0.85f,1.0f)); //x ratio, radius, range
						
			aniSet.addAnimation(keyScaleAni);					
			

			
			slide.setOpacity(1.0f);
			slide.startAnimation(aniSet);
		}
		
		public void runUpAnimation(SVISlide slide){
			if (slide == null) return;
			
			int duration = (int)(BUTTON_ANIMATION_DURATION_UP * mAnimationSpeed);
			int duration_min = (int)(BUTTON_ANIMATION_DURATION_UP * mAnimationSpeed);
			
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
			aniSet.setOffset(0);			
			
			if (mChangeRotation){
				SVIKeyFrameAnimation keyRotationAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.ROTATION);
				keyRotationAni.setDuration(duration_min);
				keyRotationAni.setRepeatCount(0);
				keyRotationAni.setAutoReverse(false);
				keyRotationAni.setOffset(0);
				keyRotationAni.addKeyProperty(0.0f, new SVIVector3(0.0f,0.0f,0.0f)); //x ratio, radius, range			 
				keyRotationAni.addKeyProperty(0.5f, new SVIVector3((mRandom.nextFloat() - 0.5f) * 80.0f,(mRandom.nextFloat() - 0.5f) * 1.0f, 0.0f)); //x ratio, radius, range
				keyRotationAni.addKeyProperty(1.0f, new SVIVector3(0.0f,0.0f,0.0f)); //x ratio, radius, range
				aniSet.addAnimation(keyRotationAni);					
			}
						
			SVIKeyFrameAnimation keyColorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.BG_COLOR);
			keyColorAni.setDuration(duration_min);
			keyColorAni.setRepeatCount(0);
			keyColorAni.setAutoReverse(false);
			keyColorAni.setOffset(0);
			keyColorAni.addKeyProperty(0.0f, new SVIColor(1.0f,1.0f,1.0f,1.0f)); //x ratio, radius, range
			keyColorAni.addKeyProperty(1.0f, new SVIColor(1.0f,1.0f,1.0f,0.0f)); //x ratio, radius, range
			aniSet.addAnimation(keyColorAni);		
					
			SVIKeyFrameAnimation keyLightPowerAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_POWER);
			keyLightPowerAni.setDuration(duration_min);
			keyLightPowerAni.setRepeatCount(0);
			keyLightPowerAni.setAutoReverse(false);
			keyLightPowerAni.setOffset(0);
			keyLightPowerAni.addKeyProperty(0.0f, -DEPTH_SCALE); //x ratio, radius, range
			keyLightPowerAni.addKeyProperty(1.0f,  DEPTH_SCALE); //x ratio, radius, range
						
			aniSet.addAnimation(keyLightPowerAni);			
									
			slide.setOpacity(1.0f);
			slide.startAnimation(aniSet);
		}
		
		public void runDownAnimation(SVISlide slide){
			if (slide == null) return;
			
			int duration = (int)(BUTTON_ANIMATION_DURATION_DOWN * mAnimationSpeed);
			int duration_min = (int)(BUTTON_ANIMATION_DURATION_DOWN * mAnimationSpeed);
			
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
			aniSet.setOffset(0);			
			
			if (mChangeRotation){
				SVIKeyFrameAnimation keyRotationAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.ROTATION);
				keyRotationAni.setDuration(duration_min);
				keyRotationAni.setRepeatCount(0);
				keyRotationAni.setAutoReverse(false);
				keyRotationAni.setOffset(0);
				keyRotationAni.addKeyProperty(0.0f, new SVIVector3(0.0f,0.0f,0.0f)); //x ratio, radius, range			 
				keyRotationAni.addKeyProperty(0.5f, new SVIVector3((mRandom.nextFloat() - 0.5f) * 80.0f,(mRandom.nextFloat() - 0.5f) * 1.0f, 0.0f)); //x ratio, radius, range
				keyRotationAni.addKeyProperty(1.0f, new SVIVector3(0.0f,0.0f,0.0f)); //x ratio, radius, range
				aniSet.addAnimation(keyRotationAni);					
			}
						
			
			SVIKeyFrameAnimation keyColorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.BG_COLOR);
			keyColorAni.setDuration(duration_min);
			keyColorAni.setRepeatCount(0);
			keyColorAni.setAutoReverse(false);
			keyColorAni.setOffset(0);
			keyColorAni.addKeyProperty(0.0f, new SVIColor(1.0f,1.0f,1.0f,0.0f)); //x ratio, radius, range
			keyColorAni.addKeyProperty(1.0f, new SVIColor(1.0f,1.0f,1.0f,1.0f)); //x ratio, radius, range
			aniSet.addAnimation(keyColorAni);		
			
					
			SVIKeyFrameAnimation keyLightPowerAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_POWER);
			keyLightPowerAni.setDuration(duration_min);
			keyLightPowerAni.setRepeatCount(0);
			keyLightPowerAni.setAutoReverse(false);
			keyLightPowerAni.setOffset(0);
			keyLightPowerAni.addKeyProperty(0.0f,  DEPTH_SCALE); //x ratio, radius, range
			keyLightPowerAni.addKeyProperty(1.0f, -DEPTH_SCALE); //x ratio, radius, range
						
			aniSet.addAnimation(keyLightPowerAni);			
									
			slide.setOpacity(1.0f);
			slide.startAnimation(aniSet);
		}	
		
		
		@Override
		public int callbackExtraTask(SVIEffectNode node, MotionEvent event, int index) {
			// TODO Auto-generated method stub
			if (event.getAction() == MotionEvent.ACTION_UP){
				if (mInsectedNode != null){
					SVISlide slide = mInsectedNode.getSlide();
					SVISlide contentSlide = mInsectedNode.getContentSlide();
					runUpContentsAnimation(contentSlide);
					runUpAnimation(slide);
					mInsectedNode = null;
				}
			}
			if (event.getAction() == MotionEvent.ACTION_DOWN){
				mInsectedNode = node;
			}
			return 0;
		}
		
		@Override
		public boolean callbackInitialize(SVIEffectNode node){
			SVISlide slide = node.getSlide();
			slide.setOpacity(0.0f);
			slide.setLightPower(-DEPTH_SCALE);
			slide.setBackgroundColor(new SVIColor(1.0f,1.0f,1.0f,0.0f));
			slide.setLightColor(new float[]{1.0f, 1.0f, 1.0f, 64.0f});
			return false;
		}
		
		@Override
		public boolean callbackPannelInitialize(SVIEffectNode node) {
			SVISlide slide = node.getSlide();
						
			//g : 0.55 b : 0.6
			//slide.setLightColor(new float[]{1.0f, 0.55f, 0.6f, 8.0f}, 1);
			
			slide.setLightColor(new float[]{0.01f, 0.0f, 0.9f, 40.0f});
			//slide.setViewDirection(new SVIVector4(0.0f, 0.55f, 1.0f, 1.0f));		
			
			// TODO Auto-generated method stub
			return false;
		}
		
		@Override
		public boolean callbackClickAnimation(SVIEffectNode node, MotionEvent event, int index){ 
			SVISlide slide = node.getSlide();
			SVISlide contentSlide = node.getContentSlide();
			if (event.getAction() == MotionEvent.ACTION_UP) {
				runUpContentsAnimation(contentSlide);
				runUpAnimation(slide);
			}else if (event.getAction() == MotionEvent.ACTION_DOWN) {
				runDownContentsAnimation(contentSlide);
				runDownAnimation(slide);
			}				
			return false; 
		}
		
		@Override
		public boolean callbackLongPressAnimation(SVIEffectNode node, MotionEvent event){ 

			
			return false; 
		}
		
		@Override
		public boolean callbackHoveringAnimation(SVIEffectNode node, MotionEvent event){ 

			
			return false; 
		}
		
		@Override
		public boolean callbackSensor(SVISlide slide, float [] value, int type){ 
			float x = value[0];
			float y = value[1];
			float z = value[2];
			
			if (type == 0){				
				if (y < 7.0f*SENSOR_SCALE) y = 7.0f*SENSOR_SCALE;
				if (z < 7.0f*SENSOR_SCALE) z = 7.0f*SENSOR_SCALE;							
				slide.setLightDirection(new SVIVector4(x, y, z, 1.0f));				
			}else{
				slide.setViewDirection(new SVIVector4(x, y, z, 1.0f));
			}
			
			return false;
		}

		@Override
		public boolean callbackSensor(SVIEffectNode node, float [] value, int type){
			SVISlide slide = node.getSlide();
			float x = value[0];
			float y = value[1];
			float z = value[2];
			
			if (type == 0){
				
				if (y < 7.0f*SENSOR_SCALE) y = 7.0f*SENSOR_SCALE;
				if (z < 7.0f*SENSOR_SCALE) z = 7.0f*SENSOR_SCALE;			
				
				slide.setLightDirection(new SVIVector4(x, y, z, 1.0f));
				
			}else{
				slide.setViewDirection(new SVIVector4(x, y, z, 1.0f));
			}
			return false;
		}
	}
	
	private SVIEffectNode mInsectedNode = null;
	private SVIEffectCalculatorAnimCallback mAnimationCallback = new SVIEffectCalculatorAnimCallback();

}
