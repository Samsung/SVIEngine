/**
 * SGIEffectView.java 
 * @author Joonyong Park
 * @email masterkeaton27@gmail.com
 */

package com.github.svieffect.main;

import java.util.Calendar;
import java.util.Vector;

import android.content.Context;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

import com.github.sviengine.animation.SVIAnimation.InterpolatorType;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.animation.SVIBasicAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVISize;
import com.github.sviengine.basetype.SVIVector4;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlideManager;
import com.github.sviengine.slide.SVISlideManager.SVISlideRequestRender;

public class SVIEffectView extends SVIGLSurface implements SVISlideRequestRender, SVIGLSurface.RenderNotifier{
	
	private final String BUILD_VERSION = "(2013/08/07)";
	private final String BUILD_TIME = "(August 07 TIME:16:30)";

	public final int MSG_SURFACE_RESIZE = 0x9001;
	
	public enum RequestType { 	
		REQUEST_CLICK_RESULT,
		REQUEST_LONGPRESS_RESULT,
		REQUEST_HOVER_RESULT,
		REQUEST_KEY_RESULT,
	};
	
	protected	SVISize					mSize = new SVISize(720.0f,1280.0f);
	protected   SVISize					mPrevSize = new SVISize(720.0f,1280.0f);
	protected 	SVISlide 				mRootSlide = null;
	protected   boolean					mIsHidden = false;
	protected 	SVIEffectNodeCallback 	mCallback = null;
	
	protected 	Vector<SVIEffectNode> 	mNodes = new Vector<SVIEffectNode>();
	protected 	Vector<SVIEffectNode> 	mPannelNodes = new Vector<SVIEffectNode>();
	static protected   SVIEffectListener		mListener = null;
	protected   SVIEffectNode			mLastPressedNode = null;	
	protected   SVIEffectNode			mLastHoveredNode = null;
	protected   View					mTouchDispatcher = null;
	
	private long mStartTime = 0; //System.currentTimeMillis();
	private long mEndTime = 0; 
	private long mInitializationDuration = 0;
	
	//2013-02-23 jdev.nam
	//NOTE GRAPHICS: 
	//Whether Button is still press or not.
	private static int mTypeExtraState;

	//2013-02-23 jdev.nam
	protected int mScrollY = 0;
	protected int mTopOffset = 0;
	
	public SVIEffectView(Context context) {
		super(context);
		
		Log.i("SVIEffect","SVI EFFECT-JAR-RELEASE DATE"+BUILD_VERSION+"");
		Log.i("SVIEffect","SVI EFFECT-JAR-BUILD TIME  "+BUILD_TIME+"");
				
		initializePostEffect();
		
		initialize(true, 8, 8); 		
		SVISlideManager.getInstance().setSVISlideRequestRender(this);
		
		setZOrderOnTop(true);
	}
	
	
	protected void initializePostEffect(){
		
	}
	
	public void initialize(){		
		createSlide();		
		setRenderNotifier(this);
		enableUpdateListener(true);
		mStartTime = System.currentTimeMillis();
	}
	
	public void finalize(){
		clearNodes();
		destroySlide();
		destroyRoot();
		mIsHidden = false;
	}
	
	public void setTouchDispatcher(View view){
		mTouchDispatcher = view;
	}
	
	@Override 
	public boolean onTouchEvent(final MotionEvent event) {		
		if (mTouchDispatcher != null) mTouchDispatcher.onTouchEvent(event);
		return false;
	}
	
	protected boolean createSlide() {
		if( mRootSlide == null ){
			float color[] = {0.0f, 0.0f, 0.0f, 0.0f};
			mRootSlide = new SVISlide(0, 0, 0.0f, 0.0f, mSize.mWidth, mSize.mHeight, color);
			SVISlide.nativeSetRootSlideSVISlide(mRootSlide.getNativeSlideHandle());
		}		
		return true;
	}
	
	public void addPannel(float left, float top, float width, float height, float red, float green, float blue, float alpha) {
		//2013-03-05 masterkeaton27@gmail.com
		if (mRootSlide == null) return;
		
		SVIEffectNode node = new SVIEffectNode();
		
		node.setColor(red, green, blue, alpha);
		node.setRegion(left, top, width, height);
		
		float [] colorArray = {red , green, blue, alpha}; 
		SVIRect region = node.getRegion();
		
		SVISlide slide = new SVISlide(0, mRootSlide.getNativeSlideHandle(), 
				region.mOrigin.mX, region.mOrigin.mY, 
				region.mSize.mWidth, region.mSize.mHeight, 
				colorArray);
		
		slide.setPivotPoint(0.5f, 0.5f);
		
		slide.setOpacity(1.0f);	
		slide.setOrthogonal(false);
		
		node.setSlide(slide);
		
		mPannelNodes.add(node);
		if (mCallback != null) mCallback.callbackPannelInitialize(node);
	}
	
	public void addPannel(float left, float top, float width, float height, Bitmap diffuse) {
		//2013-03-05 masterkeaton27@gmail.com
		if (mRootSlide == null) return;
				
		SVIEffectNode node = new SVIEffectNode();
		
		node.setRegion(left, top, width, height);
		
		float [] colorArray = {1.0f , 1.0f , 1.0f, 1.0f}; 
		SVIRect region = node.getRegion();
		
		SVISlide slide = new SVISlide(0, mRootSlide.getNativeSlideHandle(), 
				region.mOrigin.mX, region.mOrigin.mY, 
				region.mSize.mWidth, region.mSize.mHeight, 
				colorArray);
		
		slide.setPivotPoint(0.5f, 0.5f);
		slide.setImage(diffuse);
		
		slide.setOpacity(1.0f);	
		slide.setOrthogonal(false);
		
		node.setSlide(slide);
		
		mPannelNodes.add(node);
		if (mCallback != null) mCallback.callbackPannelInitialize(node);
	}
	
	public void addPannel(float left, float top, float width, float height, Bitmap diffuse, Bitmap normal, Bitmap specular, float normalScalar){
		//2013-03-05 masterkeaton27@gmail.com
		if (mRootSlide == null) return;
				
		SVIEffectNode node = new SVIEffectNode();
		
		node.setColorGUI(diffuse);
		node.setNormalMapGUI(normal);
		node.setSpecularMapGUI(specular);
		node.setRegion(left, top, width, height);
		node.setNormalHeight(normalScalar);
		
		float [] colorArray = {1.0f , 1.0f , 1.0f, 1.0f}; 
		SVIRect region = node.getRegion();
		
		SVISlide slide = new SVISlide(0, mRootSlide.getNativeSlideHandle(), 
				region.mOrigin.mX, region.mOrigin.mY, 
				region.mSize.mWidth, region.mSize.mHeight, 
				colorArray);
		
		//slide.setZPosition(-(float)mPannelNodes.size());
		
		slide.setLightColor(new float[]{1.0f, 1.0f, 1.0f, 64.0f});
		
		SVIImage diffuseImage = node.getColorGUIImage();
		
		SVIImage normalImage = node.getNormalMapGUIImage();
		SVIImage specularImage = node.getSpecularMapGUIImage();
		
		slide.setPivotPoint(0.5f, 0.5f);
		slide.setImage(diffuseImage);
		
		if (normalImage != null){
			slide.setNormalImage(normalImage);
			if (specularImage != null) slide.setSpecularImage(specularImage);
			
			slide.setUseNormalMap(true);
								
			slide.setLightPower(normalScalar);
			slide.setViewDirection(new SVIVector4(0.0f,-0.5f, 1.0f, 1.0f));				
			slide.setLightDirection(new SVIVector4(0.0f, 1.0f, 0.0f, 1.0f));
			
		}else slide.setUseNormalMap(false);
	
		slide.setOpacity(1.0f);	
		slide.setOrthogonal(false);			
		
		node.setSlide(slide);
		
		mPannelNodes.add(node);
		if (mCallback != null) mCallback.callbackPannelInitialize(node);
	}
	
	public SVIEffectNode getPannelNode(int index){
		if (index >= mPannelNodes.size()) return null;
		return mPannelNodes.get(index);
	}
	
	public void setBackGround(float left, float top, float width, float height, Bitmap diffuse, Bitmap normal, Bitmap specular, float normalScalar){				
		addPannel(left,top,width,height,diffuse,normal,specular,normalScalar);		
	}
	
	public boolean getHit(float x, float y, SVIRect rect){
		return rect.checkHit(new SVIPoint(x,y));
	}
	
	protected SVIEffectNode getIntersectedNode(float x, float y){		
		for (int n = 0; n < mNodes.size(); n++){
			SVIEffectNode node = mNodes.get(n);
			if (node != null){
				SVISlide slide = node.getSlide();
				SVIRect rect = slide.getGlobalRegion();				
				if (rect.checkHit(new SVIPoint(x,y))){
					return node;
				}
			}			
		}
		return null;
	}
	
	protected boolean cookingSlide(SVIEffectNode node){
		if (node == null) {
			Log.e("SVI Effect View", "Node is null, Cooking slide is failed.");
			return false;
		}
		
		if (mRootSlide == null) {
			Log.e("SVI Effect View", "RootSlide is null, Cooking slide is failed.");
			return false;
		}
		
		SVIRect region = node.getRegion();
		SVIColor color = node.getColor();
		float [] colorArray = {color.mR, color.mG, color.mB, color.mA}; 
		
		if (node.hasBase()){
			SVIImage baseMap = node.getBaseGUIImage();
			
			SVIRect regionBase = node.isBaseRegionSetted() ? 
					node.getBaseRegion() : node.getRegion();
			
			SVISlide baseSlide = new SVISlide(0, mRootSlide.getNativeSlideHandle(), 
					regionBase.mOrigin.mX, regionBase.mOrigin.mY, 
					regionBase.mSize.mWidth, regionBase.mSize.mHeight, 
					colorArray);
					
			baseSlide.setPivotPoint(0.5f, 0.5f);			
			baseSlide.setImage(baseMap);
			baseSlide.setUseNormalMap(false);
			baseSlide.setOpacity(1.0f);						
			baseSlide.setOrthogonal(false);			
			
			node.setBaseSlide(baseSlide);
		}
		
		SVIImage diffuseMap = node.getColorGUIImage();
		SVIImage normalMap = node.getNormalMapGUIImage();
		SVIImage specularMap = node.getSpecularMapGUIImage();
				
		
		SVISlide slide = new SVISlide(0, mRootSlide.getNativeSlideHandle(), 
				region.mOrigin.mX, region.mOrigin.mY, 
				region.mSize.mWidth, region.mSize.mHeight, 
				colorArray);
				
		slide.setPivotPoint(0.5f, 0.5f);		
		slide.setImage(diffuseMap);
				
		if (normalMap != null){
			slide.setNormalImage(normalMap);
			if (specularMap != null) slide.setSpecularImage(specularMap);
			
			slide.setUseNormalMap(true);
								
			slide.setLightPower(node.getNormalHeight());
			slide.setViewDirection(new SVIVector4(0.0f,-0.5f, 1.0f, 1.0f));				
			slide.setLightDirection(new SVIVector4(0.0f, 1.0f, 0.0f, 1.0f));
			
		}else slide.setUseNormalMap(false);		
		
		
		slide.setOpacity(1.0f);						
		slide.setOrthogonal(false);			
				
		slide.setViewDirection(new SVIVector4(0.0f,-0.5f, 1.0f, 1.0f));				
		slide.setLightDirection(new SVIVector4(0.0f, 1.0f, 0.0f, 1.0f));
		
		node.setSlide(slide);		
		
		if (node.hasContent()){
			SVIImage contentMap = node.getContentGUIImage();
			
			SVIRect regionContent = node.isContentRegionSetted() ? 
					node.getContentRegion() : node.getRegion();
			
			SVISlide contentslide = new SVISlide(0, mRootSlide.getNativeSlideHandle(), 
					regionContent.mOrigin.mX, regionContent.mOrigin.mY, 
					regionContent.mSize.mWidth, regionContent.mSize.mHeight, 
					colorArray);
					
			contentslide.setPivotPoint(0.5f, 0.5f);			
			contentslide.setImage(contentMap);
			contentslide.setUseNormalMap(false);
			contentslide.setOpacity(1.0f);						
			contentslide.setOrthogonal(false);			
			
			node.setContentSlide(contentslide);
		}
		
		if (node.hasOverlayContent()){
			SVIImage overlayContentMap = node.getOverlayContentGUIImage();
			SVIRect regionOverlayContent = node.isOverlayContentRegionSetted() ? 
					node.getOverlayContentRegion() : node.getRegion();
			
			SVISlide OverlayContentslide = new SVISlide(0, mRootSlide.getNativeSlideHandle(), 
					regionOverlayContent.mOrigin.mX, regionOverlayContent.mOrigin.mY, 
					regionOverlayContent.mSize.mWidth, regionOverlayContent.mSize.mHeight, 
					colorArray);
					
			OverlayContentslide.setPivotPoint(0.5f, 0.5f);			
			OverlayContentslide.setImage(overlayContentMap);
			OverlayContentslide.setUseNormalMap(false);
			OverlayContentslide.setOpacity(1.0f);						
			OverlayContentslide.setOrthogonal(false);			
			
			node.setOverlayContentSlide(OverlayContentslide);
		}
		
		if (mCallback != null) mCallback.callbackInitialize(node);	
		
		return true;
	}
		
	public SVIEffectNode findNode(int index){
		if (mNodes.size() == 0) return null;
		
		for (int n = 0; n < mNodes.size(); n++){
			SVIEffectNode node = mNodes.get(n);
			if (node != null && node.getIndex() == index){
				return node;
			}				
		}
		return null;	
	}
	
	public boolean addNode(SVIEffectNode node){
		if (mNodes.contains(node)) return false;
		mNodes.add(node);
		return cookingSlide(node);
	}
	
	public SVIEffectNodeCallback getAnimationCallback(){
		return mCallback;
	}
	
	public void setAnimationCallback(SVIEffectNodeCallback callback){
		mCallback = callback;
	}
	
	public void setListener(SVIEffectListener listener){
		mListener = listener;
	}
	
	protected boolean reLayout(){
		//if( mRootSlide != null ){
		//	mRootSlide.setRegion(0.0f,0.0f,mSize.mWidth, mSize.mHeight);
		//}
		return false;
	}
	
	public SVIEffectNode getIntersectedObject(MotionEvent inputEvent){
		if (inputEvent == null) return null;		
		return getIntersectedNode(inputEvent.getX(), inputEvent.getY());
	}
	
	public boolean validateOnSensorChange(float [] value, int type){
		for (int n = 0; n < mNodes.size(); n++){
			SVIEffectNode node = mNodes.get(n);
			if (node != null){
				mCallback.callbackSensor(node, value, type);
			}
		}				
		
		for (int n = 0; n < mPannelNodes.size(); n++){
			SVIEffectNode node = mPannelNodes.get(n);
			if (node != null){
				mCallback.callbackSensor(node, value, type);
			}
		}		
		return true;
	}
	
	public boolean validateOnLongClick(){	
		if (mLastPressedNode == null) return false;
		mLastPressedNode.onLongPress(mLastPressedNode.getIndex(), 0, null);
		if (mCallback != null) mCallback.callbackLongPressAnimation(mLastPressedNode, null);
		mLastPressedNode = null;
		return true;
	}
	
	private void processOnHover(final MotionEvent inputEvent, float x, float y){
		if (inputEvent == null) {
			Log.e("SVI Effect View","Motion event is null, please check touch event handler");
			return;
		}
		
		//2012-12-04 masterkeaton27@gmail.com - need to change to using regional intersection check
		SVIEffectNode node = getIntersectedNode(x, y);
		
		if (node != null){						
			if (inputEvent.getAction() == MotionEvent.ACTION_HOVER_EXIT){
				if (mLastHoveredNode != null){
					mLastHoveredNode.onHovering(mLastHoveredNode.getIndex(), MotionEvent.ACTION_HOVER_EXIT, inputEvent);
					mLastHoveredNode = null;
				}
				node.onHovering(node.getIndex(), MotionEvent.ACTION_HOVER_EXIT, inputEvent);
			}else{
				boolean hoverChanged = mLastHoveredNode != node;
				if (mLastHoveredNode != null && hoverChanged){
					mLastHoveredNode.onHovering(mLastHoveredNode.getIndex(), MotionEvent.ACTION_HOVER_EXIT, inputEvent);
				}
				mLastHoveredNode = node;
				node.onHovering(node.getIndex(), hoverChanged ? MotionEvent.ACTION_HOVER_ENTER : MotionEvent.ACTION_HOVER_MOVE, inputEvent);
			}
		}else{
			if (mLastHoveredNode != null){
				mLastHoveredNode.onHovering(mLastHoveredNode.getIndex(), MotionEvent.ACTION_HOVER_EXIT, inputEvent);
				mLastHoveredNode = null;
			}
		}
	}
	
	public boolean validateOnHover(final MotionEvent inputEvent){
		int touchCount = inputEvent.getPointerCount();
		if (touchCount == 0) return false;
		float x = inputEvent.getX(0);
		float y = inputEvent.getY(0);
		processOnHover(inputEvent, x, y);		
		return true;
	}

	//2013-03-01 si.bang  
	public int getNodeSize() {
		return mNodes.size();
	}	
	
	private boolean processOnTouch(RequestType type, final MotionEvent inputEvent, float x, float y, int index){
		if (inputEvent == null) {
			Log.e("SVI Effect View","Motion event is null, please check touch event handler");
			return false;
		}		
			
		//2012-12-04 masterkeaton27@gmail.com - need to change to using regional intersection check
		//2013-02-23 jdev.nam  
		//NOTE GRAPHICS: 
		//++ (mScrollY+mTopOffset)
		//Figure out intersect position for the offset from top.
		SVIEffectNode node = getIntersectedNode(x, y-(mScrollY+mTopOffset));
		
		
		mTypeExtraState = 0;
		if (mCallback != null) {
			mTypeExtraState = mCallback.callbackExtraTask(node, inputEvent, index);
		}
		if (node == null){
			return false;
		}
		
		SVISlide intersectedSlide = node.getSlide();
				
		if (intersectedSlide == null) {
			mLastPressedNode = null;
			return false;
		}else{			
			if (inputEvent.getActionMasked()==MotionEvent.ACTION_DOWN
				    || inputEvent.getActionMasked()==MotionEvent.ACTION_POINTER_DOWN){				
				if (inputEvent.getPointerId((inputEvent.getActionIndex()& MotionEvent.ACTION_MASK)) == index){
					mLastPressedNode = node;
				}
			}
		}		
				
		if (type == RequestType.REQUEST_CLICK_RESULT){
			if (mCallback != null) mCallback.callbackClickAnimation(node, inputEvent, index);
			node.onClick(index, mTypeExtraState, inputEvent);					 
		}
		return true;
	}
	
	//2013-02-23 jdev.nam  
	public void setScrollPositionY(int aValue) {
		mScrollY = aValue;
	}
	
	//2013-02-23 jdev.nam  
	public void setTopOffset(int aValue) {
		mTopOffset = aValue;
	}
	
	public boolean validateOnTouch(RequestType type, final MotionEvent inputEvent){
		int touchCount = inputEvent.getPointerCount();
		//2013-02-23 jdev.nam  
		//NOTE GRAPHICS: 
		//If all of button is not press. return false.
		int resultCount =  0;
		for (int n = 0; n < touchCount; n++){
			float x = inputEvent.getX(n);
			float y = inputEvent.getY(n)-mScrollY;
			if(processOnTouch(type, inputEvent, x, y, n)==true){
				resultCount++;
			}
		}
		//2013-02-23 jdev.nam  
		if(resultCount == 0){
			return false;
		}
		return true;	
	}
	
		
	@Override
	public void onUpdateFinished() {
		
		// TODO Auto-generated method stub
		if (mListener != null) {
			mListener.onFirstRenderFinished();
		}
		
		mEndTime = System.currentTimeMillis();
		mInitializationDuration = mEndTime - mStartTime;
		Log.i("SVIEffect", "Engine Initialize Duration(First Update Finished):" + mInitializationDuration + " msec");
		
		enableUpdateListener(false);
	}

	@Override
	public void animationSlideNotify() {
		// TODO Auto-generated method stub
	}

	
	private Handler mSVITimeHandler = new Handler() {
		@Override
		public void handleMessage(Message message) {
			swapAnimationSVIGLSurface();
			requestRender();
		}
	};
	
	private Calendar mSVIPrevTime = null;
	private long mSVIKeepTime = 0;
	private int SVI_DRAW_TIME = 1000 / 60;
	
	@Override
	public boolean requestSlideRender() {
		
		SVI_DRAW_TIME = 1000 / mFPS;
		
		if(mSVIPrevTime == null ) {
			mSVIPrevTime = Calendar.getInstance();
		}
						
		Calendar curTime = Calendar.getInstance();
		long deltaTime = curTime.getTimeInMillis() - mSVIPrevTime.getTimeInMillis();
		
		//2013-03-07 masterkeaton27@gmail.com
		//due to date problem, exception case added.
		if (curTime.getTimeInMillis() < mSVIPrevTime.getTimeInMillis()){
			mSVIPrevTime = curTime;
			mSVIKeepTime = 0;
			deltaTime = 0;
		}		
				
		
		mSVIKeepTime += deltaTime;
		
		if(mSVIKeepTime >= SVI_DRAW_TIME) {
			swapAnimationSVIGLSurface();
			requestRender();			
			mSVIKeepTime = 0;
		} else {
			long gap = SVI_DRAW_TIME - mSVIKeepTime;				
			//2013-03-07 masterkeaton27@gmail.com
			//changing delay method with thread using.
			mSVITimeHandler.sendEmptyMessageDelayed(0, gap);
		}		
		mSVIPrevTime = curTime;		
		
		// TODO Auto-generated method stub
		//swapAnimationSVIGLSurface();
		//requestRender();
		return false;
	}
	
	
	static protected SVIAnimationListener mHideAnimationListener = new SVIAnimationListener() {
		  
		  @Override
		  public void onAnimationStart(String arg0) {
		  }
		  
		  @Override
		  public void onAnimationRepeat(String arg0) {   
		  }
		  
		  @Override
		  public void onAnimationEnd(String arg0) {
		   if (mListener != null)
				//2013-02-23 jdev.nam
				//NOTE GRAPHICS: 
				//Pass animation end event.
			    //If TypeExtraState is 0, that means Animation end.
			   if(SVIEffectView.mTypeExtraState == 0)
				   mListener.onAnimateHideFinished();
		   		//mIsHidden = true;
		  }
		 };

	
	static private SVIAnimationListener mShowAnimationListener = new SVIAnimationListener(){
		@Override
		public void onAnimationEnd(String arg0) {
			// TODO Auto-generated method stub
			if (mListener != null) mListener.onAnimateShowFinished();
		}

		@Override
		public void onAnimationRepeat(String arg0) {}

		@Override
		public void onAnimationStart(String arg0) {
			
		}
		
	};
	
	private SVIPoint mCurrentRootPosition = new SVIPoint(0.0f,0.0f);
	
	public void animateShow(int duration){
		if (mRootSlide != null) {
			mRootSlide.setHidden(false);
			mIsHidden = false;
		   
			SVIPoint fromPoint = new SVIPoint(mRootSlide.getPosition().mX, mRootSlide.getPosition().mY);
			SVIPoint toPoint = new SVIPoint(mCurrentRootPosition.mX, mCurrentRootPosition.mY);
			SVIBasicAnimation positionAnimation = new SVIBasicAnimation(
					SVIPropertyAnimation.PropertyAnimationType.POSITION,fromPoint, toPoint);
			positionAnimation.setInterpolator(InterpolatorType.ACCELERATE_DECELERATE);
			positionAnimation.setListener(mShowAnimationListener);
			positionAnimation.setDuration(duration);
			positionAnimation.setRepeatCount(0);
			positionAnimation.setAutoReverse(false);
			positionAnimation.setOffset(0);
		   
			mRootSlide.setOpacity(1.0f);
			mRootSlide.startAnimation(positionAnimation);
		   
			mRootSlide.setPosition(mCurrentRootPosition.mX, mCurrentRootPosition.mY);
		}
	}
	
	public void animateShow(int duration, float startX, float startY, float endX, float endY){
		if (mRootSlide != null) {
			mRootSlide.setHidden(false);
			mIsHidden = false;
			
			mCurrentRootPosition.mX = (float)startX;
			mCurrentRootPosition.mY = (float)startY;
			
			SVIPoint fromPoint = new SVIPoint(startX, startY);
			SVIPoint toPoint = new SVIPoint(endX, endY);
			SVIBasicAnimation positionAnimation = new SVIBasicAnimation(
					SVIPropertyAnimation.PropertyAnimationType.POSITION,fromPoint, toPoint);
			positionAnimation.setInterpolator(InterpolatorType.ACCELERATE_DECELERATE);
			positionAnimation.setListener(mShowAnimationListener);
			positionAnimation.setDuration(duration);
			positionAnimation.setRepeatCount(0);
			positionAnimation.setAutoReverse(false);
			positionAnimation.setOffset(0);
		   
			mRootSlide.setOpacity(1.0f);
			mRootSlide.startAnimation(positionAnimation);
		   
			mRootSlide.setPosition(mCurrentRootPosition.mX, mCurrentRootPosition.mY);
		}
	}
	
	
	public void setPannelPosition(int x, int y){
		if (mRootSlide != null) {
			mCurrentRootPosition.mX = (float)x;
			mCurrentRootPosition.mY = (float)y;
			mRootSlide.setPosition(x, y);						
		}
	}
	
	public void animateHide(int duration, float hideDistance){
		if (mRootSlide != null) {
			mCurrentRootPosition.mX = mRootSlide.getPosition().mX;
			mCurrentRootPosition.mY = mRootSlide.getPosition().mY;
			SVIPoint fromPoint = new SVIPoint(mCurrentRootPosition.mX, mCurrentRootPosition.mY);
			SVIPoint toPoint = new SVIPoint(mCurrentRootPosition.mX, mCurrentRootPosition.mY + hideDistance);
			SVIBasicAnimation positionAnimation = new SVIBasicAnimation(
					SVIPropertyAnimation.PropertyAnimationType.POSITION,fromPoint, toPoint);
			positionAnimation.setInterpolator(InterpolatorType.ACCELERATE_DECELERATE);
			positionAnimation.setListener(mHideAnimationListener);
			positionAnimation.setDuration(duration);   
			positionAnimation.setRepeatCount(0);
			positionAnimation.setAutoReverse(false);
			positionAnimation.setOffset(0);
			   
			mRootSlide.setOpacity(1.0f);
			mRootSlide.startAnimation(positionAnimation);
			   
			mRootSlide.setPosition(mCurrentRootPosition.mX, mCurrentRootPosition.mY + hideDistance);
		}
	}
	
	public void animateHide(int duration, float startX, float startY, float endX, float endY){
		if (mRootSlide != null) {
			mCurrentRootPosition.mX = startX;
			mCurrentRootPosition.mY = startX;
			SVIPoint fromPoint = new SVIPoint(startX, startY);
			SVIPoint toPoint = new SVIPoint(endX, endY);
			SVIBasicAnimation positionAnimation = new SVIBasicAnimation(
					SVIPropertyAnimation.PropertyAnimationType.POSITION,fromPoint, toPoint);
			positionAnimation.setInterpolator(InterpolatorType.ACCELERATE_DECELERATE);
			positionAnimation.setListener(mHideAnimationListener);
			positionAnimation.setDuration(duration);   
			positionAnimation.setRepeatCount(0);
			positionAnimation.setAutoReverse(false);
			positionAnimation.setOffset(0);
			   
			mRootSlide.setOpacity(1.0f);
			mRootSlide.startAnimation(positionAnimation);
			   
			mRootSlide.setPosition(endX, endY);
		}
	}
	
	public void surfaceShow(boolean show){
		if (mRootSlide != null) {
			mRootSlide.setHidden(!show);
		}
	}
	
	public void destroy(){
		clearNodes();
		destroySlide();
		destroyRoot();		
		mIsHidden = false;
	}
	
	public SVISlide getRoot(){
		return mRootSlide;
	}
	
	public void clearNodes(){
		
		if (mPannelNodes.size() > 0){
			for (int n = 0; n < mPannelNodes.size(); n++){
				SVIEffectNode node = mPannelNodes.get(n);
				SVISlide contentSlide = node.getContentSlide();
				SVISlide slide = node.getSlide();
				
				SVISlide baseSlide = node.getBaseSlide();
				SVISlide overlaySlide = node.getOverlayContentSlide();
				
				if (overlaySlide != null){
					overlaySlide.setImage((SVIImage)null);
					overlaySlide.setNormalImage((SVIImage)null);
					overlaySlide.setSpecularImage((SVIImage)null);
				}
				
				if (baseSlide != null){
					baseSlide.setImage((SVIImage)null);
					baseSlide.setNormalImage((SVIImage)null);
					baseSlide.setSpecularImage((SVIImage)null);
				}
				
				if (contentSlide != null){
					contentSlide.setImage((SVIImage)null);
					contentSlide.setNormalImage((SVIImage)null);
					contentSlide.setSpecularImage((SVIImage)null);
				}
				
				if (slide != null){
					slide.setImage((SVIImage)null);
					slide.setNormalImage((SVIImage)null);
					slide.setSpecularImage((SVIImage)null);
				}				
				
				
				node.setSlide(null);
				node.setContentSlide(null);
				node = null;
			}		
			mPannelNodes.clear();
		}
		
				
		if (mNodes.size() > 0){
			for (int n = 0; n < mNodes.size(); n++){
				SVIEffectNode node = mNodes.get(n);
				SVISlide contentSlide = node.getContentSlide();
				SVISlide slide = node.getSlide();
				SVISlide baseSlide = node.getBaseSlide();
				SVISlide overlaySlide = node.getOverlayContentSlide();
				
				if (overlaySlide != null){
					overlaySlide.setImage((SVIImage)null);
					overlaySlide.setNormalImage((SVIImage)null);
					overlaySlide.setSpecularImage((SVIImage)null);
				}
				
				if (baseSlide != null){
					baseSlide.setImage((SVIImage)null);
					baseSlide.setNormalImage((SVIImage)null);
					baseSlide.setSpecularImage((SVIImage)null);
				}
				
				if (contentSlide != null){
					contentSlide.setImage((SVIImage)null);
					contentSlide.setNormalImage((SVIImage)null);
					contentSlide.setSpecularImage((SVIImage)null);
				}
				
				if (slide != null){
					slide.setImage((SVIImage)null);
					slide.setNormalImage((SVIImage)null);
					slide.setSpecularImage((SVIImage)null);
				}
				
				node.setSlide(null);
				node.setContentSlide(null);
				node = null;
			}		
			mNodes.clear();
		}
		mLastPressedNode = null;
		
		if( mRootSlide != null ) {
			for(int i=mRootSlide.getSubSlideCount()-1; i>=0; i--) {
				SVISlide removeSlide = mRootSlide.getSubSlide(i);
				if( removeSlide != null ){
					//2013-02-15 rareboy0112@facebook.com
					//child sub slide reset
					for(int j = 0; j < removeSlide.getSubSlideCount(); ++j) {
						SVISlide childSlide = removeSlide.getSubSlide(j);
						if (childSlide.getImage() != null) childSlide.setImage((SVIImage)null);
					}
					if (removeSlide.getImage() != null) removeSlide.setImage((SVIImage)null);
					mRootSlide.removeSlide(removeSlide);
					removeSlide = null;
				}
			}
		}
	}
	
	protected boolean destroySlide() {
		if( mRootSlide != null ) {
			for(int i=mRootSlide.getSubSlideCount()-1; i>=0; i--) {
				SVISlide removeSlide = mRootSlide.getSubSlide(i);
				if( removeSlide != null )
					mRootSlide.removeSlide(removeSlide);
			}
		}
		return true;
	}
	
	protected void destroyRoot(){
		if (mRootSlide != null){
			SVISlide.nativeSetRootSlideSVISlide(0);
			mRootSlide = null;
		}
	}
	
	@Override
	public void onResize(int width, int height) {
		// TODO Auto-generated method stub
		super.onResize(width, height);
		Message msg = new Message();
		msg.what = MSG_SURFACE_RESIZE;
		msg.arg1 = width;
		msg.arg2 = height;
		mViewHandler.dispatchMessage(msg);
	}
	
	//2013-01-04 masterkeaton27@gmail.com
	/*
	@Override
	public void surfaceDestroyed(SurfaceHolder holder){
		super.surfaceDestroyed(holder);		
		Surface surface = holder.getSurface();
		if (surface != null) surface.release();
		
		this.destroyDrawingCache();
	}
		
	public void releaseSurface(){
		SurfaceHolder holder = getHolder();
		
		if (holder == null) return;
		
		Surface surface = holder.getSurface();
		if (surface != null) surface.release();
	}
	*/
	private Handler mViewHandler = new Handler() {
		public void handleMessage(Message msg) {
			switch(msg.what) {
			case MSG_SURFACE_RESIZE:
				mPrevSize.mWidth = mSize.mWidth;
				mPrevSize.mHeight = mSize.mHeight;
				mSize.mWidth = msg.arg1;
				mSize.mHeight = msg.arg2;
				reLayout();				
				break;
		}
		}
	};	
	
}
