/**
 * SGIEffectRemoteController.java 
 * @author Joonyong Park
 * @email masterkeaton27@gmail.com
 */

package com.github.svieffect.remotecontroller;

import android.content.Context;
import android.graphics.Bitmap;
import android.view.MotionEvent;

import com.github.svieffect.main.SVIEffectNode;
import com.github.svieffect.main.SVIEffectNodeCallback;
import com.github.svieffect.main.SVIEffectView;
import com.github.sviengine.animation.SVISpriteAnimation;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVIVector4;
import com.github.sviengine.slide.SVISlide;

public class SVIEffectRemoteController extends SVIEffectView {
	
	private static final float 		SENSOR_SCALE = 1.1f;
	private static final float 		DEPTH_TEST = 3.0f;
	
	private static final int 		BUTTON_ANIMATION_DURATION_UP = 300;
	private static final int 		BUTTON_ANIMATION_DURATION_DOWN = 50;		

	private static final int		NODE_CH_UP = 0;
	private static final int		NODE_CH_DOWN = 1;
	private static final int		NODE_VOL_UP = 2;
	private static final int		NODE_VOL_DOWN = 3;	
	private int mAnimationDuration = BUTTON_ANIMATION_DURATION_DOWN;
	private float mAnimationSpeed = 1.0f;
	
	
	public static final int BUTTON_UP_PRESSED = -1;
	public static final int BUTTON_DOWN_PRESSED = 1;
	public static final int BUTTON_STAY = 0;
	
	public SVIEffectRemoteController(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		this.setAnimationCallback(mAnimationCallback);
	}
		
		
	public void initialize(){
		setRenderFPS(20);
		super.initialize();
	}
	
	public void setAnimationDuration(int duration){
		mAnimationDuration = duration;
	}
	
	public int getAnimationDuration(){
		return mAnimationDuration;
	}
	
	public void setAnimationSpeed(float speed){
		mAnimationSpeed = speed;
	}
	
	public float getAnimationSpeed(){
		return mAnimationSpeed;
	}
	
	//2013-02-23 jdev.nam
	public void setHideNode(){
		mAnimationCallback.hideNode();
	}
	
	//2013-02-23 jdev.nam
	public void setShowNode(){
		mAnimationCallback.showNode();
	}
		
	class SVIEffectRemoteControllerAnimCallback extends SVIEffectNodeCallback{		
		
		public SVIEffectRemoteControllerAnimCallback(){}
		
		@Override
		public boolean callbackInitialize(SVIEffectNode node){
			SVISlide slide = node.getSlide();
			slide.setLightPower(-DEPTH_TEST);
			slide.setLightColor(new float[]{10.0f, 10.0f, 10.0f, 64.0f});
									
			SVIImage spriteImage = slide.getImage();
			Bitmap image = spriteImage.mBitmap;
			int frameWidth = image.getWidth() / 7;
			int frameHeight = image.getHeight() / 2;
			
			if (node.getIndex() == NODE_CH_UP || node.getIndex() == NODE_VOL_UP){
				slide.setTextureRegion(frameWidth * 6, frameHeight, frameWidth, frameHeight);
			} else if (node.getIndex() == NODE_CH_DOWN || node.getIndex() == NODE_VOL_DOWN){
				slide.setTextureRegion(0, 0, frameWidth, frameHeight);
			}
			return false;
		}
		
		@Override
		public int callbackExtraTask(SVIEffectNode node, MotionEvent event, int index) {
			
			if (event.getAction() == MotionEvent.ACTION_UP){			
				
				if (mInsectedNode != null ){					
										
					int nodeIdx = mInsectedNode.getIndex();
					if (nodeIdx == NODE_VOL_UP || nodeIdx == NODE_CH_UP){
						runAnimation(mInsectedNode.getSlide(), (int)(BUTTON_ANIMATION_DURATION_UP * mAnimationSpeed), 0, 13);	
					}else if (nodeIdx == NODE_VOL_DOWN || nodeIdx == NODE_CH_DOWN){
						runAnimation(mInsectedNode.getSlide(), (int)(BUTTON_ANIMATION_DURATION_UP * mAnimationSpeed), 13, 0);
					}		
					
					mInsectedNode = null;
					return BUTTON_STAY;
				}
			}			
			return getUpOrDown(node,event.getX(),event.getY()-(mScrollY+mTopOffset));
		}				
		
				
		private int getUpOrDown(SVIEffectNode node,float x, float y){
			if (node == null) return BUTTON_STAY;
			
			SVISlide slide = node.getSlide();
			SVIRect region = slide.getGlobalRegion();			
			float halfOffset = region.mOrigin.mY + region.mSize.mHeight * 0.5f;
			
			boolean isHit = region.checkHit(new SVIPoint(x,y));
			
			if (halfOffset > y && isHit) {
				return BUTTON_UP_PRESSED;
			}else if (halfOffset <= y && isHit){
				return BUTTON_DOWN_PRESSED;
			}
			
			return BUTTON_STAY;
		}
		
		private SVIEffectNode processChannel(float x, float y){
			if (mNodes.size() < 4) return null;
			
			SVIEffectNode nodeUp = mNodes.get(NODE_CH_UP);	
			SVIEffectNode nodeDown = mNodes.get(NODE_CH_DOWN);
			
			SVISlide upSlide = nodeUp.getSlide();
			SVISlide downSlide = nodeDown.getSlide();
			SVIRect region = upSlide.getGlobalRegion();			
			float halfOffset = region.mOrigin.mY + region.mSize.mHeight * 0.5f;
			
			boolean isHit = region.checkHit(new SVIPoint(x,y));
			
			if (!isHit) return null ;
			
			if (halfOffset > y) {
				upSlide.setOpacity(1.0f);
				downSlide.setOpacity(0.0f);
				return nodeUp;
			}else{
				upSlide.setOpacity(0.0f);
				downSlide.setOpacity(1.0f);
				return nodeDown;
			}
			
		}
		
		private SVIEffectNode processVolume(float x, float y){
			if (mNodes.size() < 4) return null;
			
			SVIEffectNode nodeUp = mNodes.get(NODE_VOL_UP);			
			SVIEffectNode nodeDown = mNodes.get(NODE_VOL_DOWN);
			
			SVISlide upSlide = nodeUp.getSlide();
			SVISlide downSlide = nodeDown.getSlide();
			SVIRect region = upSlide.getGlobalRegion();			
			float halfOffset = region.mOrigin.mY + region.mSize.mHeight * 0.5f;
			
			boolean isHit = region.checkHit(new SVIPoint(x,y));
			
			if (!isHit) return null ;
			
			if (halfOffset > y) {
				upSlide.setOpacity(1.0f);
				downSlide.setOpacity(0.0f);
				return nodeUp;
			}else{
				upSlide.setOpacity(0.0f);
				downSlide.setOpacity(1.0f);
				return nodeDown;
			}
		}
		
		public void showNode(SVIEffectNode node){
			SVISlide slide = node.getSlide();
			slide.setOpacity(1.0f);
		}
		
		//2013-02-23 jdev.nam
		public void showNode(){
			showNode(mNodes.get(NODE_VOL_UP));
			showNode(mNodes.get(NODE_VOL_DOWN));
			showNode(mNodes.get(NODE_CH_UP));
			showNode(mNodes.get(NODE_CH_DOWN));
		}
		
		public void hideNode(SVIEffectNode node){
			SVISlide slide = node.getSlide();
			slide.setOpacity(0.0f);
		}
		
		//2013-02-23 jdev.nam
		public void hideNode()
		{
			hideNode(mNodes.get(NODE_VOL_UP));
			hideNode(mNodes.get(NODE_VOL_DOWN));
			hideNode(mNodes.get(NODE_CH_UP));
			hideNode(mNodes.get(NODE_CH_DOWN));
		}
		
		public void runAnimation(SVISlide slide, int duration ,int start, int end){
			SVIImage spriteImage = slide.getImage();
			Bitmap image = spriteImage.mBitmap;
			
			int frameWidth = image.getWidth() / 7;
			int frameHeight = image.getHeight() / 2;
			
			SVISpriteAnimation spriteAnimation = new SVISpriteAnimation(SVISpriteAnimation.PlayType.PLAY_PARTIAL, 
					spriteImage, frameWidth, frameHeight);
			
			//2013-02-23 jdev.nam
			//NOTE GRAPHICS: 
			//Create ScrollView for the test cause by Top menu bar.
			spriteAnimation.setListener(mHideAnimationListener);
			
			spriteAnimation.setAutoReverse(false);
			spriteAnimation.setDuration(duration);
					
			spriteAnimation.setInterval(start, end);
									
			slide.startAnimation(spriteAnimation);
		}
		
		@Override
		public boolean callbackPannelInitialize(SVIEffectNode node) {
			
			// TODO Auto-generated method stub
			return false;
		}
		
		
		@Override
		public boolean callbackClickAnimation(SVIEffectNode node, MotionEvent event, int index){ 
						
			if (event.getAction() == MotionEvent.ACTION_MOVE || event.getAction() == MotionEvent.ACTION_UP) return false;		
			
			
			int nodeIdx = 0;
			float x = event.getX();
			
			//2013-02-23 jdev.nam
			//NOTE GRAPHICS: 
			//Figure out intersect position for the offset from top.
			float y = event.getY()-(mScrollY+mTopOffset);
			
			SVIEffectNode animChannelNode = processChannel(x,y);
			SVIEffectNode animVolumeNode = processVolume(x,y);
			
			if (animChannelNode != null){
				mInsectedNode = animChannelNode;
				
				nodeIdx = animChannelNode.getIndex();
				if (nodeIdx == NODE_CH_DOWN){
					runAnimation(animChannelNode.getSlide(), (int)(BUTTON_ANIMATION_DURATION_DOWN * mAnimationSpeed), 0, 13);
					
				}else{
					runAnimation(animChannelNode.getSlide(), (int)(BUTTON_ANIMATION_DURATION_DOWN * mAnimationSpeed), 13, 0);
		
				}								
			}
			
			if (animVolumeNode != null){
				mInsectedNode = animVolumeNode;
				
				nodeIdx = animVolumeNode.getIndex();
				if (nodeIdx == NODE_VOL_DOWN){
					runAnimation(animVolumeNode.getSlide(), (int)(BUTTON_ANIMATION_DURATION_DOWN * mAnimationSpeed), 0, 13);
					
				}else{
					runAnimation(animVolumeNode.getSlide(), (int)(BUTTON_ANIMATION_DURATION_DOWN * mAnimationSpeed), 13, 0);
				
				}		
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
			
			if (y < 7.0f*SENSOR_SCALE) y = 7.0f*SENSOR_SCALE;
			if (z < 7.0f*SENSOR_SCALE) z = 7.0f*SENSOR_SCALE;			
			
			slide.setLightDirection(new SVIVector4(x, y, z, 1.0f));
			return false;
		}

		@Override
		public boolean callbackSensor(SVIEffectNode node, float [] value, int type){
			SVISlide slide = node.getSlide();
			float x = value[0];
			float y = value[1];
			float z = value[2];
			
			if (y < 7.0f*SENSOR_SCALE) y = 7.0f*SENSOR_SCALE;
			if (z < 7.0f*SENSOR_SCALE) z = 7.0f*SENSOR_SCALE;			
			
			slide.setLightDirection(new SVIVector4(x, y, z, 1.0f));
			return false;
		}

		
	}
	
	private SVIEffectNode mInsectedNode = null;
	private SVIEffectRemoteControllerAnimCallback mAnimationCallback = new SVIEffectRemoteControllerAnimCallback();

}
