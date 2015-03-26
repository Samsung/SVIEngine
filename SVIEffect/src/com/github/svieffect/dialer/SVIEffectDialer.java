/**
 * SGIEffectDialer.java 
 * @author Joonyong Park
 * @email masterkeaton27@gmail.com
 */

package com.github.svieffect.dialer;

import java.util.Random;
import java.util.Vector;

import android.content.Context;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Message;
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
import com.github.sviengine.slide.SVISlide;

public class SVIEffectDialer extends SVIEffectView {

	public static final int FOCUS_GUI_01 = 0;
	public static final int FOCUS_GUI_02 = 1;
	public static final int FOCUS_GUI_03 = 2;
	public static final int FOCUS_GUI_04 = 3;
	public static final int FOCUS_GUI_05 = 4;
	public static final int FOCUS_GUI_06 = 5;
	
	private static final float 		SENSOR_SCALE = 1.0f;
	

	private static final int 		BUTTON_ANIMATION_DURATION_UP = 200;
	private static final int 		BUTTON_ANIMATION_DURATION_DOWN = 100;
	
	private static final int 		BUTTON_ANIMATION_DURATION_HOVER_DOWN = 80;
	private static final int 		BUTTON_ANIMATION_DURATION_HOVER_UP = 150;
	
	
	private static final float		TARGET_SCALE = 0.95f;
	private static final float		DEPTH_SCALE = 1.0f;
	
	private int mAnimationDuration = BUTTON_ANIMATION_DURATION_DOWN;
	private int mAnimationDurationUp = BUTTON_ANIMATION_DURATION_UP;
	private float mAnimationSpeed = 1.0f;
	
	private Bitmap	mPopupOverlap = null;
	private Bitmap	mFocusOverlap = null;
	
	//2013-02-27 masterkeaton27@gmail.com
	//2 types of image added. due to K dialer request.
	private Bitmap	mFocus02Overlap = null;
	private Bitmap	mFocus03Overlap = null;
	private Bitmap	mFocus04Overlap = null;
	private Bitmap	mFocus05Overlap = null;
	private Bitmap	mFocus06Overlap = null;
	
	private Vector<SVIEffectPopupView> mPopupViews = new Vector<SVIEffectPopupView>();
	private Vector<SVIEffectFocusView> mFocusViews = new Vector<SVIEffectFocusView>();
		
	private SVIColor mPressBlue = new SVIColor(73.0f/255.0f,163.0f/255.0f,242.0f/255.0f,1.0f);
	private SVIColor mBlue = new SVIColor(95.0f/255.0f,147.0f/255.0f,189.0f/255.0f,1.0f);
	private SVIColor mGreen = new SVIColor(103.0f/255.0f,178.0f/255.0f,12.0f/255.0f,1.0f);
	private SVIColor mDarkBlue = new SVIColor(90.0f/255.0f,113.0f/255.0f,133.0f/255.0f,1.0f);
	
	//2013-03-14 rareboy0112@facebook.com
	//use ripple value
	private boolean mUseRipple = true;
	
	public SVIColor getDefaultPressBlue(){return  mPressBlue;}
	public SVIColor getDefaultBlue(){return  mBlue;}
	public SVIColor getDefaultGreen(){return  mGreen;}
	public SVIColor getDefaultDarkBlue(){return  mDarkBlue;}
	
	public void setAnimationSpeed(float speed){
		mAnimationSpeed = speed;
	}
	
	public float getAnimationSpeed(){
		return mAnimationSpeed;
	}
	
	@Deprecated
	public void setFocus2X1OverlapGUI(Bitmap bitmap){} 

	@Deprecated
	public void setFocus4X1OverlapGUI(Bitmap bitmap){}


	
	public void setHoverOverlapGUI(Bitmap bitmap){
		mPopupOverlap = bitmap;
	}
		
	
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
		}else if (index == FOCUS_GUI_04){
			mFocus04Overlap = bitmap;
		}else if (index == FOCUS_GUI_05){
			mFocus05Overlap = bitmap;
		}else if (index == FOCUS_GUI_06){
			mFocus06Overlap = bitmap;
		}
	}	
	
	private SVIEffectPopupView createPopupView(int index){
		SVISlide rootSlide = getRoot();				
		if (rootSlide == null) {
			return null;
		}
		
		SVIEffectPopupView popupView = new SVIEffectPopupView();		
		float [] colorArray = {1.0f, 1.0f, 1.0f, 1.0f}; 
		SVISlide slide = new SVISlide(0, rootSlide.getNativeSlideHandle(), 
				0.0f, 0.0f, 1.0f, 1.0f, colorArray);
		popupView.mOverlapSlide = slide;
		popupView.mImage = new SVIImage();
		popupView.mImage.setBitmap(mPopupOverlap);
		slide.setImage(popupView.mImage);
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
		}else if (imageType == FOCUS_GUI_04){
			focusView.mImage.setBitmap(mFocus04Overlap);
		}else if (imageType == FOCUS_GUI_05){
			focusView.mImage.setBitmap(mFocus05Overlap);
		}else if (imageType == FOCUS_GUI_06){
			focusView.mImage.setBitmap(mFocus06Overlap);
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
			
			Log.e("SVIEffect-Dialer", "Hover Shows Up (btnIndex: " + mIndex + ")");
			
			return true;
		}
		
		public void hide(){
			if (!mIsHovered) return;
			runHideAnimation(mOverlapSlide);
			mIsHovered = false;
			
			Log.e("SVIEffect-Dialer", "Hover Hide Down (btnIndex: " + mIndex + ")");
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
			
			Log.e("SVIEffect-Dialer", "Focus Shows Up (btnIndex: " + mIndex + ")");
			
			return true;
		}
		
		public void hide(){
			
			runHideAnimation(mOverlapSlide);
			
			Log.e("SVIEffect-Dialer", "Focus Hides Down (btnIndex: " + mIndex + ")");
		}
		
		private void runShowAnimation(SVISlide slide){
			if (slide == null) return;
			int duration = BUTTON_ANIMATION_DURATION_HOVER_DOWN;
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
	
	public SVIEffectDialer(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		//enableShowFPS(true);
		//2013-02-08 masterkeaton27@gmail.com
		setZOrderOnTop(true);
		setAnimationCallback(mAnimationCallback);
		setPreserveEGLContextOnPause(true);
		setSVIPreserveEGLContext(true);
	}
	
	public SVIEffectDialer(Context context, boolean useRipple, boolean preserveEGLContext) {
		super(context);
		// TODO Auto-generated constructor stub
		//enableShowFPS(true);
		//2013-02-08 masterkeaton27@gmail.com
		setZOrderOnTop(true);
		setAnimationCallback(mAnimationCallback);
		
		mPreserveEGLContext = preserveEGLContext;
		mUseRipple = useRipple;
		
		setPreserveEGLContextOnPause(mPreserveEGLContext);
		setSVIPreserveEGLContext(mPreserveEGLContext);
		
		//2013-03-14 rareboy0112@facebook.com
		//because mUseRipple not setting(first called initializePostEffect from super(context)
		initializePostEffect();
	}
	
	
	public void setNormalFPS(){
		setRenderFPS(20);
	}
	
	public void setRippleFPS(){
		setRenderFPS(30);
	}
	
	@Override
	protected void initializePostEffect(){
		setRenderFPS(20);
		if(mUseRipple) {
			//SVIPostEffectManager.getInstance().setType(SVIPostEffectManager.POST_EFFECT_RIPPLE);
			//SVIPostEffectManager.getInstance().enable(true);
			//SVIPostEffectManager.getInstance().setRippleInfo(0.825f, 0.01f);
		}		
		super.initializePostEffect();
	}
	
	public void initialize(){
		super.initialize();
	}
	
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
	
	public void setAnimationDurationDown(int duration){
		mAnimationDuration = duration;
	}
	
	public void setAnimationDurationUp(int duration){
		mAnimationDurationUp = duration;
	}
	
	public int getAnimationDurationDown(){
		return mAnimationDuration;
	}
	
	public int getAnimationDurationUp(){
		return mAnimationDurationUp;
	}
	
	@Deprecated
	public void showPopupView(Bitmap thumnail, Bitmap nameImage, Bitmap numberImage, int btnIndex){		
		SVIEffectPopupView popupView = findPopupView(btnIndex);		
		if (popupView == null){
			popupView = createPopupView(btnIndex); 
		}		
		SVIEffectNode btnNode = findNode(btnIndex);
		SVISlide slide = btnNode.getSlide();
		SVIRect region = slide.getRegion();
		popupView.mOverlapSlide.setRegion(
				region.mOrigin.mX, 
				region.mOrigin.mY, 
				region.mSize.mWidth, 
				region.mSize.mHeight);
		popupView.show();
	}
	
	@Deprecated
	public void hidePopupView(int btnIndex){		
		SVIEffectPopupView popupView = findPopupView(btnIndex);
		if (popupView == null){
			return;
		}		
		popupView.hide();
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
				
	
	private Random mRandom = new Random();
	private Handler mDropHandler = new Handler(){
		@Override
		public void handleMessage(Message message) {
			if(mUseRipple) {
				SVIPoint pt = (SVIPoint)message.obj;			
				//SVIPostEffectManager.getInstance().setRipple(pt.mX, pt.mY, 0.4f + mRandom.nextFloat() * 0.4f);
			}
		}
	};
	
	private SVIImage mRippleEnvImage = new SVIImage();
	
	public void setRippleEnvImage(Bitmap bitmap){
		if(mUseRipple) {
			mRippleEnvImage.setBitmap(bitmap);
			//SVIPostEffectManager.getInstance().setRippleEnvironmentImage(mRippleEnvImage);
		}
	}
		
	public void resetRipple(){
		if(mUseRipple) {
			//SVIPostEffectManager.getInstance().resetRipple();
		}
		requestRender();
	}
	
	public void downRipple(float x, float y){
		if(mUseRipple) {
			//SVIPostEffectManager.getInstance().setRipple(x, y, 0.4f + mRandom.nextFloat() * 0.4f);
		}
		//requestRender();
	}
	
	public void downRipple(int btnIndex, float x, float y){
		if(mUseRipple) {
			SVIEffectNode btnNode = findNode(btnIndex);		
			if (btnNode == null) {
				Log.e("SVIEffect-Dialer", "Can't find matching node (btnIndex: " + btnIndex + ", nodeSize: " + mNodes.size() + ")");
				return;
			}		
			
			SVIRect region = btnNode.getRegion();
			x = region.mOrigin.mX + x;
			y = region.mOrigin.mY + y;
					
			//SVIPostEffectManager.getInstance().setRipple(x, y, 0.4f + mRandom.nextFloat() * 0.4f);
						
			//Message msg = new Message();
			//msg.obj = new SVIPoint(x,y);
			//mDropHandler.sendMessage(msg);
		}		
		//requestRender();
	}
	
	public void onHoverEnter(int btnIndex){
		boolean isUsingColor = false;
				
		SVIEffectPopupView popupView = findPopupView(btnIndex);				
		if (popupView == null && !isUsingColor ){
			popupView = createPopupView(btnIndex); 
		}
				
		SVIEffectNode btnNode = findNode(btnIndex);		
		if (btnNode == null) {
			Log.e("SVIEffect-Dialer", "Can't find matching node (btnIndex: " + btnIndex + ", nodeSize: " + mNodes.size() + ")");
			return;
		}		
				
		if (!isUsingColor){
			SVISlide slide = btnNode.getSlide();
			SVIRect region = slide.getRegion();			
			popupView.mOverlapSlide.setRegion(
					region.mOrigin.mX, 
					region.mOrigin.mY, 
					region.mSize.mWidth, 
					region.mSize.mHeight);
			
			popupView.show();		
		}else{
			SVISlide contentSlide = btnNode.getContentSlide();		
			if (contentSlide != null) {
				contentSlide.setBackgroundColor(mPressBlue);
			}
		}
	}
	
	public void onHoverExit(int btnIndex){
		boolean isUsingColor = false;
				
		if (!isUsingColor){
			SVIEffectPopupView popupView = findPopupView(btnIndex);
			if (popupView == null){
				Log.e("SVIEffect-Dialer", "Can't find matching hover node (btnIndex: " + btnIndex + ", nodeSize: " + mNodes.size() + ")");
				return;
			}		
			popupView.hide();
		}
		
		if (isUsingColor){
			SVIEffectNode btnNode = findNode(btnIndex);				
			if (btnNode == null) {
				Log.e("SVIEffect-Dialer", "Can't find matching hover node (btnIndex: " + btnIndex + ", nodeSize: " + mNodes.size() + ")");
				return;
			}		
			
			SVISlide contentSlide = btnNode.getContentSlide();
			if (contentSlide != null) {
				//btnNode.getColor()
				contentSlide.setBackgroundColor(btnNode.getColor());
			}
		}
	}
		
	public void onTouchDown(int btnIndex){
		SVIEffectNode node = findNode(btnIndex);
		
		if (node == null) return;
	
		SVISlide slide = node.getSlide();
		SVISlide contentSlide = node.getContentSlide();
		SVISlide overlayContentSlide = node.getOverlayContentSlide();
		
		if (mAnimationCallback != null){
			mAnimationCallback.runDownAnimation(slide);
			mAnimationCallback.runDownContentsAnimation(contentSlide, node);
			if (overlayContentSlide != null) 
				mAnimationCallback.runDownContentsAnimation(overlayContentSlide, node);
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
			mAnimationCallback.runUpContentsAnimation(contentSlide, node);
			if (overlayContentSlide != null) 
				mAnimationCallback.runUpContentsAnimation(overlayContentSlide, node);
		}
	}
		
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
			Log.e("SVIEffect-Dialer", "Can't find matching node (btnIndex: " + btnIndex + ", nodeSize: " + mNodes.size() + ")");
			return;
		}		
		
		if (!isUsingColor){
			SVISlide slide = btnNode.getSlide();
			SVIRect region = slide.getRegion();
			focusView.mOverlapSlide.setRegion(
					region.mOrigin.mX, 
					region.mOrigin.mY, 
					region.mSize.mWidth, 
					region.mSize.mHeight);
			
			focusView.show();
		}else{		
			SVISlide contentSlide = btnNode.getContentSlide();
			if (contentSlide != null) {
				contentSlide.setBackgroundColor(mPressBlue);			
			}
		}
	}
	
	public void onFocusedEnter(int btnIndex){
		onFocusedEnter(btnIndex, FOCUS_GUI_01);		
	}
	
	public void onFocusedExit(int btnIndex){
		boolean isUsingColor = false;
		
		if (!isUsingColor){
			SVIEffectFocusView focusView = findFocusView(btnIndex);
			if (focusView == null){
				Log.e("SVIEffect-Dialer", "Can't find matching focus node (btnIndex: " + btnIndex + ", nodeSize: " + mNodes.size() + ")");
				return;
			}		
			focusView.hide();
		}else{		
			SVIEffectNode btnNode = findNode(btnIndex);				
			if (btnNode == null) {
				Log.e("SVIEffect-Dialer", "Can't find matching focus node (btnIndex: " + btnIndex + ", nodeSize: " + mNodes.size() + ")");
				return;
			}		
			
			SVISlide contentSlide = btnNode.getContentSlide();
			if (contentSlide != null) {
				contentSlide.setBackgroundColor(btnNode.getColor());			
			}
		}
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
	
	
	public void reset(){
		if (mNodes.size() > 0){
			for (int n =0; n < mNodes.size(); n++){
				SVIEffectNode button = mNodes.get(n);
				SVISlide slide = button.getSlide();
				SVISlide contentSlide = button.getContentSlide();
				SVISlide overlayContentSlide = button.getOverlayContentSlide();
				
				if (slide != null){
					slide.setBackgroundColor(new SVIColor(1.0f,1.0f,1.0f,0.0f), 10);
					slide.setScale(1.0f, 1.0f, 1.0f, 10);
				}				
				
				if (contentSlide != null){
					contentSlide.setBackgroundColor(button.getColor(), 10);
					contentSlide.setScale(1.0f, 1.0f, 1.0f, 10);
				}
				
				if (overlayContentSlide != null){
					overlayContentSlide.setBackgroundColor(button.getColor(), 10);
					overlayContentSlide.setScale(1.0f, 1.0f, 1.0f, 10);
				}
			}	
		}		
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
	
	class SVIEffectDialerAnimCallback extends SVIEffectNodeCallback{		
		
		private boolean mChangeRotation = false;
		private Random mRandom = new Random();
		
		public SVIEffectDialerAnimCallback(){}	
		
		public void runUpFocusedAnimation(SVISlide slide){
			int duration = (int)(mAnimationDurationUp * mAnimationSpeed);
			int duration_min = (int)(mAnimationDurationUp * mAnimationSpeed);
			
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
			keyColorAni.addKeyProperty(0.0f, new SVIColor(1.0f,0.0f,0.0f,1.0f)); //x ratio, radius, range
			keyColorAni.addKeyProperty(1.0f, new SVIColor(1.0f,1.0f,1.0f,1.0f)); //x ratio, radius, range
			aniSet.addAnimation(keyColorAni);		
												
			slide.setOpacity(1.0f);
			slide.startAnimation(aniSet);
		}
		
		public void runDownFocusedAnimation(SVISlide slide){
			int duration = (int)(mAnimationDurationUp * mAnimationSpeed);
			int duration_min = (int)(mAnimationDurationUp * mAnimationSpeed);
			
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
			keyColorAni.addKeyProperty(1.0f, new SVIColor(1.0f,0.0f,0.0f,1.0f)); //x ratio, radius, range
			aniSet.addAnimation(keyColorAni);		
												
			slide.setOpacity(1.0f);
			slide.startAnimation(aniSet);
		}
		
		
		public void runUpContentsAnimation(SVISlide slide, SVIEffectNode node){
			int duration = (int)(mAnimationDurationUp * mAnimationSpeed);
			int duration_min = (int)(mAnimationDurationUp * mAnimationSpeed);
			
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.DECELERATE);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
			aniSet.setOffset(0);			
						
			/*
			SVIKeyFrameAnimation keyColorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.BG_COLOR);
			keyColorAni.setDuration(duration_min);
			keyColorAni.setRepeatCount(0);
			keyColorAni.setAutoReverse(false);
			keyColorAni.setOffset(0);
			keyColorAni.addKeyProperty(0.0f, mPressBlue); //x ratio, radius, range
			keyColorAni.addKeyProperty(1.0f, node.getColor()); //x ratio, radius, range
			aniSet.addAnimation(keyColorAni);	
			 */	
			
			SVIKeyFrameAnimation keyScaleAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SCALE);
			keyScaleAni.setDuration(duration_min);
			keyScaleAni.setRepeatCount(0);
			keyScaleAni.setAutoReverse(false);
			keyScaleAni.setOffset(0);
			keyScaleAni.addKeyProperty(0.0f, new SVIVector3(TARGET_SCALE,TARGET_SCALE,1.0f)); //x ratio, radius, range
			keyScaleAni.addKeyProperty(1.0f, new SVIVector3(1.0f,1.0f,1.0f)); //x ratio, radius, range
						
			aniSet.addAnimation(keyScaleAni);			
						
			slide.setScale(TARGET_SCALE, TARGET_SCALE, 1.0f);
			slide.setOpacity(1.0f);
			slide.startAnimation(aniSet);
		}
		
		public void runDownContentsAnimation(SVISlide slide, SVIEffectNode node){
			int duration = (int)(BUTTON_ANIMATION_DURATION_DOWN * mAnimationSpeed);
			int duration_min = (int)(BUTTON_ANIMATION_DURATION_DOWN * mAnimationSpeed);
			
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.DECELERATE);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
			aniSet.setOffset(0);			
						
			/*
			SVIKeyFrameAnimation keyColorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.BG_COLOR);
			keyColorAni.setDuration(duration_min);
			keyColorAni.setRepeatCount(0);
			keyColorAni.setAutoReverse(false);
			keyColorAni.setOffset(0);
			keyColorAni.addKeyProperty(0.0f, node.getColor()); //x ratio, radius, range
			keyColorAni.addKeyProperty(1.0f, mPressBlue); //x ratio, radius, range
			aniSet.addAnimation(keyColorAni);		
			*/
								
			SVIKeyFrameAnimation keyScaleAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SCALE);
			keyScaleAni.setDuration(duration_min);
			keyScaleAni.setRepeatCount(0);
			keyScaleAni.setAutoReverse(false);
			keyScaleAni.setOffset(0);
			keyScaleAni.addKeyProperty(0.0f, new SVIVector3(1.0f,1.0f,1.0f)); //x ratio, radius, range
			keyScaleAni.addKeyProperty(1.0f, new SVIVector3(TARGET_SCALE, TARGET_SCALE, 1.0f)); //x ratio, radius, range
						
			aniSet.addAnimation(keyScaleAni);			
						
			slide.setScale(1.0f, 1.0f, 1.0f);			
			slide.setOpacity(1.0f);
			slide.startAnimation(aniSet);
		}
				
		
		public void runUpContentsAnimationHover(SVISlide slide, SVIEffectNode node){
			
			int duration = (int)(mAnimationDurationUp * mAnimationSpeed);
			int duration_min = (int)(mAnimationDurationUp * mAnimationSpeed);
			
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.DECELERATE);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
			aniSet.setOffset(0);			
						
			/*
			SVIKeyFrameAnimation keyColorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.BG_COLOR);
			keyColorAni.setDuration(duration_min);
			keyColorAni.setRepeatCount(0);
			keyColorAni.setAutoReverse(false);
			keyColorAni.setOffset(0);
			keyColorAni.addKeyProperty(0.0f, mPressBlue); //x ratio, radius, range
			keyColorAni.addKeyProperty(1.0f, node.getColor()); //x ratio, radius, range
			aniSet.addAnimation(keyColorAni);	
			*/								
			
			if (slide.getScale().mX > 1.0f){
				SVIKeyFrameAnimation keyScaleAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SCALE);
				keyScaleAni.setDuration(duration_min);
				keyScaleAni.setRepeatCount(0);
				keyScaleAni.setAutoReverse(false);
				keyScaleAni.setOffset(0);
				keyScaleAni.addKeyProperty(0.0f, new SVIVector3(TARGET_SCALE,TARGET_SCALE,1.0f)); //x ratio, radius, range
				keyScaleAni.addKeyProperty(1.0f, new SVIVector3(1.0f,1.0f,1.0f)); //x ratio, radius, range
				aniSet.addAnimation(keyScaleAni);			
			}			
			
			slide.setOpacity(1.0f);
			slide.startAnimation(aniSet);
		}
		
		public void runDownContentsAnimationHover(SVISlide slide){
			int duration = (int)(BUTTON_ANIMATION_DURATION_DOWN * mAnimationSpeed);
			int duration_min = (int)(BUTTON_ANIMATION_DURATION_DOWN * mAnimationSpeed);
			
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.DECELERATE);
			aniSet.setDuration(duration);
			aniSet.setRepeatCount(0);
			aniSet.setAutoReverse(false);
			aniSet.setOffset(0);									
			
			/*
			SVIKeyFrameAnimation keyColorAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.BG_COLOR);
			keyColorAni.setDuration(duration_min);
			keyColorAni.setRepeatCount(0);
			keyColorAni.setAutoReverse(false);
			keyColorAni.setOffset(0);
			keyColorAni.addKeyProperty(0.0f, slide.getBackbgoundColor()); //x ratio, radius, range
			keyColorAni.addKeyProperty(1.0f, mPressBlue); //x ratio, radius, range
			aniSet.addAnimation(keyColorAni);		
			*/
			
			slide.setOpacity(1.0f);
			slide.startAnimation(aniSet);
		}
		
		public void runUpAnimation(SVISlide slide){
			
			int duration = (int)(mAnimationDurationUp * mAnimationSpeed);
			int duration_min = (int)(mAnimationDurationUp * mAnimationSpeed);
			
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
			keyColorAni.addKeyProperty(1.0f, new SVIColor(1.0f,1.0f,1.0f,0.0f)); //x ratio, radius, range
			aniSet.addAnimation(keyColorAni);		
														
			slide.setOpacity(1.0f);
			slide.startAnimation(aniSet);
			
		}
		
		public void runDownAnimation(SVISlide slide){
			
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
			keyColorAni.addKeyProperty(0.0f, new SVIColor(1.0f,1.0f,1.0f,0.0f)); //x ratio, radius, range
			keyColorAni.addKeyProperty(1.0f, new SVIColor(1.0f,1.0f,1.0f,1.0f)); //x ratio, radius, range
			aniSet.addAnimation(keyColorAni);		
												
			slide.setOpacity(1.0f);
			slide.startAnimation(aniSet);			
			
		}
		
		
		
		@Override
		public boolean callbackInitialize(SVIEffectNode node){
			SVISlide slide = node.getSlide();
			//slide.setOpacity(0.0f);
			slide.setBackgroundColor(new SVIColor(1.0f,1.0f,1.0f,0.0f));
			slide.setLightColor(new float[]{1.0f, 1.0f, 1.0f, 64.0f});
						
			SVISlide contentSlide = node.getContentSlide();
			if (contentSlide != null){
				contentSlide.setBackgroundColor(node.getColor());
			}	
			
			SVISlide overlayContentSlide = node.getOverlayContentSlide();
			if (overlayContentSlide != null){
				overlayContentSlide.setBackgroundColor(node.getColor());
			}
			
			return false;
		}
		
		@Override
		public int callbackExtraTask(SVIEffectNode node, MotionEvent event, int index) {
			
			boolean isUp = event.getActionMasked()==MotionEvent.ACTION_UP
				    || event.getActionMasked()==MotionEvent.ACTION_POINTER_UP;
			boolean isDown = event.getActionMasked()==MotionEvent.ACTION_DOWN
				    || event.getActionMasked()==MotionEvent.ACTION_POINTER_DOWN;
			boolean isPointer = event.getPointerId((event.getActionIndex()& MotionEvent.ACTION_MASK)) == index;
			
			int size = mInsectedNodes.size();
			
			if (isUp && isPointer && size > 0){				
				SVIEffectNode insectedNode = mInsectedNodes.get(size-1);
				if (insectedNode != null){
					SVISlide slide = insectedNode.getSlide();
					SVISlide contentslide = insectedNode.getContentSlide();
					runUpAnimation(slide);
					runUpContentsAnimation(contentslide, insectedNode);	
					SVISlide overlayContentSlide = insectedNode.getOverlayContentSlide();
					if (overlayContentSlide != null) runUpContentsAnimation(overlayContentSlide, insectedNode);
				}										
				mInsectedNodes.remove(insectedNode);				
			}
			if (isDown && isPointer){
				mInsectedNodes.add(node);
			}
			// TODO Auto-generated method stub
			return 0;
		}
		
		
		
		@Override
		public boolean callbackClickAnimation(SVIEffectNode node, MotionEvent event, int index){ 
			
			boolean isUp = event.getActionMasked()==MotionEvent.ACTION_UP
				    || event.getActionMasked()==MotionEvent.ACTION_POINTER_UP;
			boolean isDown = event.getActionMasked()==MotionEvent.ACTION_DOWN
				    || event.getActionMasked()==MotionEvent.ACTION_POINTER_DOWN;
			boolean isPointer = event.getPointerId((event.getActionIndex() & MotionEvent.ACTION_MASK)) == index;
			
			SVISlide slide = node.getSlide();
			SVISlide contentSlide = node.getContentSlide();
			SVISlide overlayContentSlide = node.getOverlayContentSlide();
								
			if (isUp && isPointer) {
				runUpAnimation(slide);
				runUpContentsAnimation(contentSlide, node);
				if (overlayContentSlide != null) runUpContentsAnimation(overlayContentSlide, node);					
			}else if (isDown && isPointer) {
				runDownAnimation(slide);
				runDownContentsAnimation(contentSlide, node);
				if (overlayContentSlide != null) runDownContentsAnimation(overlayContentSlide, node);					
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
		
		float mSensorX = 10.0f;
		float mSensorY = 10.0f;
		@Override
		public boolean callbackSensor(SVISlide slide, float [] value , int type){ 
			/*
			float x = value[0];
			float y = value[1];
			float z = value[2];
						
			if (y < 7.0f*SENSOR_SCALE) y = 7.0f*SENSOR_SCALE;
			if (z < 7.0f*SENSOR_SCALE) z = 7.0f*SENSOR_SCALE;			
			
			slide.setLightDirection(new SVIVector4(x, y, z, 1.0f));
			//slide.setRotation(value[3] , value[4], 0.0f , 0.0f);
			*/
			return false;
		}

		@Override
		public boolean callbackSensor(SVIEffectNode node, float [] value , int type){
			/*
			SVISlide slide = node.getSlide();
			float x = value[0];
			float y = value[1];
			float z = value[2];
			
			mSensorX = x + 2.0f;
			mSensorY = y + 2.0f;
						
			if (y < 7.0f*SENSOR_SCALE) y = 7.0f*SENSOR_SCALE;
			if (z < 7.0f*SENSOR_SCALE) z = 7.0f*SENSOR_SCALE;			
			
			slide.setLightDirection(new SVIVector4(x, y, z, 1.0f));
			//slide.setViewDirection(new SVIVector4(x, y, z, 1.0f));	
			//slide.setRotation(value[3] , value[4], 0.0f , 0.0f);
			  
			*/
			return false;
		}


		@Override
		public boolean callbackPannelInitialize(SVIEffectNode node) {
			//SVISlide slide = node.getSlide();
			//g : 0.55 b : 0.6
			//slide.setLightColor(new float[]{1.0f, 0.55f, 0.6f, 64.0f}, 0);
			// TODO Auto-generated method stub
			return false;
		}
	}
	
	
	private Vector<SVIEffectNode> mInsectedNodes = new Vector<SVIEffectNode>();
	private SVIEffectDialerAnimCallback mAnimationCallback = new SVIEffectDialerAnimCallback();

}
