package com.github.sviengine.unittest;

import com.github.sviengine.slide.SVISlide;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class SceneNodeTestWindow extends PlatformWindow{

	public SceneNodeTestWindow(Context context) {
		super(context);
				
		mMsgHandler = new Handler() {

			@Override
			public void handleMessage(Message msg) {
				super.handleMessage(msg);
				
				sendEmptyMessageDelayed(0, 200);
			}
		};
		
		buildSlideTree();
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
	}
	
	
	public void onConfigurationChanged(int rotation) {
	}
	
	
	@Override 
	public void onPause(){
		SVISlide mainSlide = getMainSlide();
		if (mainSlide != null){
			mainSlide.setCapture(false);
			if (mMsgHandler != null){
				mMsgHandler.removeMessages(0);
			}	
		}
	}

	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub	
		if( event.getAction() == MotionEvent.ACTION_DOWN){
			long time = System.currentTimeMillis();
			if( (time - mClickTime) < 1000 ) {
				//mLeftSlide.removeSceneNode();
				//mRightSlide.removeSceneNode();
				
				//mRemove = true;
			}
			mClickTime = time;
			
			if(mFlag == false) {
				/// set model position animation
				//mLeftSlide.setModelPosition(70.0f, 0.0f, 0.0f, 500);
				//mRightSlide.setModelPosition(0.0f, 100.0f, 0.0f, 500);
				// set model rotation animation
				//mLeftSlide.setModelRotation(0.0f, 0.0f, 180.0f, 500);
				//mRightSlide.setModelRotation(0.0f, 0.0f, 180.0f, 500);
				/// set model scale animation
				//mLeftSlide.setModelScale(2.0f, 2.0f, 2.0f, 500);
				//mRightSlide.setModelScale(2.0f, 2.0f, 2.0f, 500);
				
				mFlag = true;
			}
			else {
				/// set model position animation
				//mLeftSlide.setModelPosition(0.0f, 0.0f, 0.0f, 500);
				//mRightSlide.setModelPosition(0.0f, 0.0f, 0.0f, 500);
				/// set model rotation animation
				//mLeftSlide.setModelRotation(0.0f, 0.0f, 0.0f, 500);
				//mRightSlide.setModelRotation(0.0f, 0.0f, 0.0f, 500);
				/// set model scale animation
				//mLeftSlide.setModelScale(1.0f, 1.0f, 1.0f, 1000);
				//mRightSlide.setModelScale(1.0f, 1.0f, 1.0f, 1000);
				
				mFlag = false;
			}
		}
		
		return true;
	}
	
	
	protected void buildSlideTree() {
		super.buildSubSlide();
		
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				
		mMainSlide = getMainSlide();
		//SVISize mainSize = mMainSlide.getRegion().mSize;
		
		mMainSlide.setBackgroundColor(color);
		
		//float width = mainSize.mWidth / 2.0f;
		//float height = mainSize.mHeight;
		
		//mLeftSlide = new SVISlide(0, mMainSlide.getNativeSlideHandle(), 0, 0, width, height, color);
		
		//mRightSlide = new SVISlide(0, mMainSlide.getNativeSlideHandle(), width + width / 4.0f, height / 4.0f, width / 2.0f, height / 2.0f, color);
		
		//SVISceneNode node = SVISceneNode.createSceneNodeFromModelFile("parent.bsa");
		//mRightSlide.setSceneNode(node);
		//mLeftSlide.setSceneNode(node);
	}
	
	private Handler mMsgHandler;
	
	private SVISlide mMainSlide;
	//private SVISlide mLeftSlide;
	//private SVISlide mRightSlide;
	
	private boolean mFlag = false;
	//private boolean mRemove = false;
	private long mClickTime = 0;
}
