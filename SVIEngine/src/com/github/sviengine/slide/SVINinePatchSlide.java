package com.github.sviengine.slide;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIBasicAnimation;
import com.github.sviengine.animation._PackageAccessAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.animation.SVIPropertyAnimation.PropertyAnimationType;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.basetype._PackageAccessBasetype;
import com.github.sviengine.glsurface.SVIGLSurface;

import android.content.res.Resources.NotFoundException;
import android.graphics.Bitmap;
import android.graphics.NinePatch;
import android.graphics.Rect;
import android.util.Log;

public class SVINinePatchSlide extends SVISlide {
	public static final String 		EXTENSION_9PATCH = ".9.png";
	private final static String		TAG = "SGINinePatchSlide";
	private final static float		DEFAULT_SCALE = 1.0f;

	private SVIImage mImage = null;
	
	// position of child slides
	// 0 1 2
	// 3 4 5
	// 6 7 8
	private SVISlide[] mChildSlide = new SVISlide[9];
	
	private float mOriginalImg_width = 0.0f;
	private float mOriginalImg_height = 0.0f;

	private int mStretchableArea_x = 0;
	private int mStretchableArea_y = 0;
	private int mStretchableArea_width = 0;
	private int mStretchableArea_height = 0;

	private int mPadding_left = 0;
	private int mPadding_right = 0;
	private int mPadding_top = 0;	
	private int mPadding_bottom = 0;
	
	private float mScaledWidth = 0f;
	private float mScaledHeight = 0f;

	// get adapted image
	public SVIImage getImage() {
		return mImage;
	}
	
	// get width and height scaled image
	public float getWidth(){
		return mScaledWidth;
	}
	public float getHeight(){
		return mScaledHeight;
	}
	
	// get padding
	public int getPaddingLeft()		{ return mPadding_left; }
	public int getPaddingRight()	{ return mPadding_right; }
	public int getPaddingTop()		{ return mPadding_top; }
	public int getPaddingBottom()	{ return mPadding_bottom; }
	
	public SVINinePatchSlide(Bitmap bitmap) {
		this(null, bitmap);
	}

	SVINinePatchSlide(SVIGLSurface surface, Bitmap bitmap) {
		super(surface);
				
		// check the chunk of nine patch image
		byte[] chunk = bitmap.getNinePatchChunk();
		if (!NinePatch.isNinePatchChunk(chunk)) {
			String errorMessage = "cannot get NinePatchChunk. Please check NinePatchImage file.";
			throw new NotFoundException(errorMessage);
		}
		final NinePatchChunk ninePatchChunk = NinePatchChunk.deserialize(chunk);
		if (ninePatchChunk.equals(null)) {
			String errorMessage = "NinePatchChunk was not serialized.";
			throw new NotFoundException(errorMessage);
		}
		
		mStretchableArea_x = ninePatchChunk.mDivX[0];
		mStretchableArea_width = ninePatchChunk.mDivX[1] - mStretchableArea_x;
		mStretchableArea_y = ninePatchChunk.mDivY[0];
		mStretchableArea_height = ninePatchChunk.mDivY[1] - mStretchableArea_y;
		
		mImage = _PackageAccessBasetype.createSVIImage(mAttachedSurface);
		mImage.setBitmap(bitmap);
		
		mOriginalImg_width = mImage.mBitmap.getWidth();
		mOriginalImg_height = mImage.mBitmap.getHeight();

		// init slides
		this.setPivotPoint(0.5f, 0.5f);
		
		for (int i = 0; i < mChildSlide.length; i++) {
			mChildSlide[i] = new SVISlide(mAttachedSurface, 0);
			SVISlide childSlide = mChildSlide[i];
			childSlide.setImage(mImage);
			childSlide.setHoldScale(true);
			this.addSlide(childSlide);
		}
		// top
		mChildSlide[0].setPivotPoint(1f, 1f);
		mChildSlide[1].setPivotPoint(0.5f, 1f);
		mChildSlide[2].setPivotPoint(0f, 1f);
		// middle
		mChildSlide[3].setPivotPoint(1f, 0.5f);
		mChildSlide[4].setPivotPoint(0.5f, 0.5f);
		mChildSlide[5].setPivotPoint(0f, 0.5f);
		// bottom
		mChildSlide[6].setPivotPoint(1f, 0f);
		mChildSlide[7].setPivotPoint(0.5f, 0f);
		mChildSlide[8].setPivotPoint(0f, 0f);
		
		mScaledWidth = mOriginalImg_width;
		mScaledHeight = mOriginalImg_height;
		
		setChildSlideRegion();
	}
	
	@Override
	public void setScale(float x, float y, float z) {
        setScale(x, y, z, IMPLICIT_ANIMATION_OFF_DURATION);
    }
	
	@Override
	public void setScale(float x, float y, float z, int duration) {
		if(x < 1.0f) x = 1.0f;
		if(y < 1.0f) y = 1.0f;
		
		// handling null exception
		for (int i = 0; i < mChildSlide.length; i++) {
			SVISlide childSlide = mChildSlide[i];
			if(childSlide == null){
				Log.d(TAG, "child slide is null");
				return;
			}
		}
		
		float scaledImgWidth = mOriginalImg_width * x;
		float scaledImgHeight = mOriginalImg_height * y;
		
		float leftRectWidth = mStretchableArea_x;
		float rightRectWidth = mOriginalImg_width - (mStretchableArea_x + mStretchableArea_width);
		float topRectHeight = mStretchableArea_y;
		float bottomRectHeight = mOriginalImg_height - (mStretchableArea_y + mStretchableArea_height);
		
		float centerRect_scaledWidth = scaledImgWidth - (leftRectWidth + rightRectWidth);
		float centerRect_scaledHeight = scaledImgHeight - (topRectHeight + bottomRectHeight);
		
		float scaleValue_x = centerRect_scaledWidth / (float) mStretchableArea_width;
		float scaleValue_y = centerRect_scaledHeight / (float) mStretchableArea_height;
		
		mScaledWidth = scaledImgWidth;
		mScaledHeight = scaledImgHeight;
		
		// set position
		SVISlide child = null;
		SVIPoint fromPosition = null;
		SVIPoint toPosition = null;
		SVIVector3 fromScale = null;
		SVIVector3 toScale = null;

		// top left
		child = mChildSlide[0];
		fromPosition = child.getPosition();
		toPosition = new SVIPoint(-centerRect_scaledWidth*0.5f, -centerRect_scaledHeight*0.5f);
		fromScale = toScale = child.getScale();
		startChildAnimation(child, fromPosition, toPosition, fromScale, toScale, duration);
		
		// top middle
		child = mChildSlide[1];
		fromPosition = child.getPosition();
		toPosition = new SVIPoint(0.0f, -centerRect_scaledHeight*0.5f);
		fromScale = child.getScale();
		toScale = new SVIVector3(scaleValue_x, DEFAULT_SCALE, z);
		startChildAnimation(child, fromPosition, toPosition, fromScale, toScale, duration);
		
		// top right
		child = mChildSlide[2];
		fromPosition = child.getPosition();
		toPosition = new SVIPoint(centerRect_scaledWidth*0.5f, -centerRect_scaledHeight*0.5f);
		fromScale = toScale = child.getScale();
		startChildAnimation(child, fromPosition, toPosition, fromScale, toScale, duration);
		
		// middle left
		child = mChildSlide[3];
		fromPosition = child.getPosition();
		toPosition = new SVIPoint(-centerRect_scaledWidth*0.5f, 0.0f);
		fromScale = child.getScale();
		toScale = new SVIVector3(DEFAULT_SCALE, scaleValue_y, z);
		startChildAnimation(child, fromPosition, toPosition, fromScale, toScale, duration);
		
		// center
		child = mChildSlide[4];
		fromPosition = toPosition = child.getPosition();
		fromScale = child.getScale();
		toScale = new SVIVector3(scaleValue_x, scaleValue_y, z);
		startChildAnimation(child, fromPosition, toPosition, fromScale, toScale, duration);
		
		// middle right
		child = mChildSlide[5];
		fromPosition = child.getPosition();
		toPosition = new SVIPoint(centerRect_scaledWidth*0.5f, 0.0f);
		fromScale = child.getScale();
		toScale = new SVIVector3(DEFAULT_SCALE, scaleValue_y, z);
		startChildAnimation(child, fromPosition, toPosition, fromScale, toScale, duration);
		
		// bottom left
		child = mChildSlide[6];
		fromPosition = child.getPosition();
		toPosition = new SVIPoint(-centerRect_scaledWidth*0.5f, centerRect_scaledHeight*0.5f);
		fromScale = toScale = child.getScale();
		startChildAnimation(child, fromPosition, toPosition, fromScale, toScale, duration);
		
		// bottom middle
		child = mChildSlide[7];
		fromPosition = child.getPosition();
		toPosition = new SVIPoint(0.0f, centerRect_scaledHeight*0.5f);
		fromScale = child.getScale();
		toScale = new SVIVector3(scaleValue_x, DEFAULT_SCALE, z);
		startChildAnimation(child, fromPosition, toPosition, fromScale, toScale, duration);
		
		// bottom right
		child = mChildSlide[8];
		fromPosition = child.getPosition();
		toPosition = new SVIPoint(centerRect_scaledWidth*0.5f, centerRect_scaledHeight*0.5f);
		fromScale = toScale = child.getScale();
		startChildAnimation(child, fromPosition, toPosition, fromScale, toScale, duration);
	}

	private void startChildAnimation(final SVISlide child, final SVIPoint fromPoint, final SVIPoint toPoint, 
			final SVIVector3 fromScale, final SVIVector3 toScale, final int duration) {

		SVIAnimationSet animationSet = _PackageAccessAnimation.createSVIAnimationSet(mAttachedSurface);		
		animationSet.setDuration(duration);
		animationSet.setRepeatCount(0);
		animationSet.setOffset(0);
		animationSet.setAutoReverse(false);
		animationSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.LINEAR);
		
		SVIBasicAnimation positionAni = _PackageAccessAnimation.createSVIBasicAnimation(mAttachedSurface, PropertyAnimationType.POSITION, fromPoint, toPoint);
		positionAni.setAnimationProperty(duration, 0, false, 0);
		SVIBasicAnimation scaleAni = _PackageAccessAnimation.createSVIBasicAnimation(mAttachedSurface, PropertyAnimationType.SCALE, fromScale, toScale);
		scaleAni.setAnimationProperty(duration, 0, false, 0);
		
		animationSet.addAnimation(positionAni);
		animationSet.addAnimation(scaleAni);
		animationSet.setListener(new SVIAnimationListener(){
			@Override
			public void onAnimationEnd(String arg0) {
				child.setPosition(toPoint.mX, toPoint.mY);
				child.setScale(toScale.mX, toScale.mY, toScale.mZ);
			}

			@Override
			public void onAnimationRepeat(String arg0) {
			}

			@Override
			public void onAnimationStart(String arg0) {
			}
		});
		
		child.startAnimation(animationSet);
	}
	
	// set Region of child slides
	private void setChildSlideRegion() {
		// handling null exception
		if (mImage == null)
			return;
		for (int i = 0; i < mChildSlide.length; i++) {
			SVISlide childSlide = mChildSlide[i];
			if(childSlide == null){
				Log.d(TAG, "child slide is null");
				return;
			}
		}
		
		float leftRect_x = 0.0f;
		float centerRect_x = (float) mStretchableArea_x;
		float rightRect_x = (float) (mStretchableArea_x + mStretchableArea_width);

		float topRect_y = 0.0f;
		float centerRect_y = (float) mStretchableArea_y;
		float bottomRect_y = (float) (mStretchableArea_y + mStretchableArea_height);

		float leftRect_width = (float) mStretchableArea_x;
		float centerRect_width = (float) mStretchableArea_width;
		float rightRect_width = (mOriginalImg_width - rightRect_x);

		float topRect_height = (float) mStretchableArea_y;
		float centerRect_height = (float) mStretchableArea_height;
		float bottomRect_height = (mOriginalImg_height - bottomRect_y);
		

		float x = 0.0f, y = 0.0f, width = 0.0f, height = 0.0f;
		// y : top
		// ******************************************************************************************************************************
		y = topRect_y;
		height = topRect_height;
		// left top
		x = leftRect_x;
		width = leftRect_width;
		mChildSlide[0].setTextureRegion(x, y, width, height);
		mChildSlide[0].setRegion(0.0f, 0.0f, width, height);
		mChildSlide[0].setPosition(-centerRect_width * 0.5f, -centerRect_height * 0.5f);
		// middle top
		x = centerRect_x;
		width = centerRect_width;
		mChildSlide[1].setTextureRegion(x, y, width, height);
		mChildSlide[1].setRegion(0.0f, 0.0f, width, height);
		mChildSlide[1].setPosition(0.0f, -centerRect_height * 0.5f);
		// right top
		x = rightRect_x;
		width = rightRect_width;
		mChildSlide[2].setTextureRegion(x, y, width, height);
		mChildSlide[2].setRegion(0.0f, 0.0f, width, height);
		mChildSlide[2].setPosition(centerRect_width * 0.5f, -centerRect_height * 0.5f);

		// y : middle
		// ******************************************************************************************************************************
		y = centerRect_y;
		height = centerRect_height;
		// left middle
		x = leftRect_x;
		width = leftRect_width;
		mChildSlide[3].setTextureRegion(x, y, width, height);
		mChildSlide[3].setRegion(0.0f, 0.0f, width, height);
		mChildSlide[3].setPosition(-centerRect_width * 0.5f, 0.0f);
		// center
		x = centerRect_x;
		width = centerRect_width;
		mChildSlide[4].setTextureRegion(x, y, width, height);
		mChildSlide[4].setRegion(0.0f, 0.0f, width, height);
		mChildSlide[4].setPosition(0.0f, 0.0f);
		// right middle
		x = rightRect_x;
		width = rightRect_width;
		mChildSlide[5].setTextureRegion(x, y, width, height);
		mChildSlide[5].setRegion(0.0f, 0.0f, width, height);
		mChildSlide[5].setPosition(centerRect_width * 0.5f, 0.0f);

		// y : bottom
		// ******************************************************************************************************************************
		y = bottomRect_y;
		height = bottomRect_height;
		// left bottom
		x = leftRect_x;
		width = leftRect_width;
		mChildSlide[6].setTextureRegion(x, y, width, height);
		mChildSlide[6].setRegion(0.0f, 0.0f, width, height);
		mChildSlide[6].setPosition(-centerRect_width * 0.5f, centerRect_height * 0.5f);
		// middle bottom
		x = centerRect_x;
		width = centerRect_width;
		mChildSlide[7].setTextureRegion(x, y, width, height);
		mChildSlide[7].setRegion(0.0f, 0.0f, width, height);
		mChildSlide[7].setPosition(0.0f, centerRect_height * 0.5f);
		// right bottom
		x = rightRect_x;
		width = rightRect_width;
		mChildSlide[8].setTextureRegion(x, y, width, height);
		mChildSlide[8].setRegion(0.0f, 0.0f, width, height);
		mChildSlide[8].setPosition(centerRect_width * 0.5f, centerRect_height * 0.5f);
	}
	
	public void destroy(){
		destroyChildSlide();
		mImage = null;
	}	
	
	private boolean destroyChildSlide() {
		for (int i = this.getSubSlideCount() - 1; i >= 0; i--) {
			SVISlide removeSlide = this.getSubSlide(i);
			if (removeSlide != null){
				this.removeSlide(removeSlide);
				removeSlide = null;
			}
		}
		
		return true;
	}
	
	
	public static class NinePatchChunk {
		public static final int NO_COLOR = 0x00000001;
	    public static final int TRANSPARENT_COLOR = 0x00000000;

	    public Rect mPaddings = new Rect();

	    public int mDivX[];
	    public int mDivY[];
	    public int mColor[];

	    private static void readIntArray(int[] data, ByteBuffer buffer) {
	        for (int i = 0, n = data.length; i < n; ++i) {
	            data[i] = buffer.getInt();
	        }
	    }

	    private static void checkDivCount(int length) {
	        if (length == 0 || (length & 0x01) != 0) {
	            throw new RuntimeException("invalid nine-patch: " + length);
	        }
	    }

	    public static NinePatchChunk deserialize(byte[] data) {
	        ByteBuffer byteBuffer =
	                ByteBuffer.wrap(data).order(ByteOrder.nativeOrder());

	        byte wasSerialized = byteBuffer.get();
	        if (wasSerialized == 0) return null;

	        NinePatchChunk chunk = new NinePatchChunk();
	        chunk.mDivX = new int[byteBuffer.get()];
	        chunk.mDivY = new int[byteBuffer.get()];
	        chunk.mColor = new int[byteBuffer.get()];

	        checkDivCount(chunk.mDivX.length);
	        checkDivCount(chunk.mDivY.length);

	        // skip 8 bytes
	        byteBuffer.getInt();
	        byteBuffer.getInt();

	        chunk.mPaddings.left = byteBuffer.getInt();
	        chunk.mPaddings.right = byteBuffer.getInt();
	        chunk.mPaddings.top = byteBuffer.getInt();
	        chunk.mPaddings.bottom = byteBuffer.getInt();

	        // skip 4 bytes
	        byteBuffer.getInt();

	        readIntArray(chunk.mDivX, byteBuffer);
	        readIntArray(chunk.mDivY, byteBuffer);
	        readIntArray(chunk.mColor, byteBuffer);

	        return chunk;
	    }

	}
}
