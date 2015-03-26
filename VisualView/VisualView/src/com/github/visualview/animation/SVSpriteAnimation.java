/*
 * Copyright (C) 2013 SVV.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.github.visualview.animation;

import com.github.sviengine.animation.SVISpriteAnimation;
import com.github.sviengine.animation.SVISpriteAnimation.PlayType;
import com.github.sviengine.basetype.SVIImage;
import com.github.visualview.SVImage;


/**
 * This class provides the sprite animation functionality, which is created from a texture atlas.<p>
 * This Visual View package feature creates animations by using frames on the texture atlas.<p>
 * It uses one image resource and makes the animation by showing rectangles from that resource.
 * 
 */
public class SVSpriteAnimation extends SVAnimation {

    SVISpriteAnimation  mSpriteAnimation;
    private int mTotalCount = 0;
    
	
    /**
     * Creates an instance of SVSpriteAnimation.
     * <P> A Sprite image is a texture atlas consists of different texture which when shown after one another leads to the beautiful animation.
     * <p> framewidth is therefore the dimension of the one texture in the sprite Image. if the an Sprite Image contains 4 texture in a row then the frame
     * <p> width can be calculated as (FrameWidth = ImageBitmapWidth/NoOfFramesInaRow)
     * <p> Similarly Height can be calculated as (FrameWidth = ImageBitmapheight/NoOfFramesInaColoumn).
     * <p> Interesting point is total number of frame might not be always equal to (NoOfFramesInaRow * NoOfFramesInaColoumn). User must look on the sprite image so as to know the number of frames in the texture atlas.
     * <p> User can use the API SetInterval(fromframe, toframe)   to choose the frames to be animated ensuring the criteria  0=< fromframe/toframe =<total.
     * 
     * @param image SVImage object with the resource atlas with the images to be used. 
     * @param frameWidth Width of one frame.
     * @param frameHeight Height of one frame.
     * @throws IllegalArgumentException if frame width or height is passed as a negative value.
     * 
     * @see PlayType
     */
    public SVSpriteAnimation(SVImage image, int frameWidth, int frameHeight) {
	    
    	checkLowValue(frameWidth);
    	checkLowValue(frameHeight);
    	
    	if(image == null)
    		throw new IllegalArgumentException("SVSpriteAnimation:: NUll Sprite Image ");
    	else if(image.getBitmap() == null)
    		throw new IllegalArgumentException("SVSpriteAnimation:: NUll Bitmap ...Please set the Bitmap to the image ");;
    	
    	
    	SVIImage argImage = new SVIImage();
    	
    	argImage.copyBitmap(image.getBitmap());
    	mAnimation = new SVISpriteAnimation(PlayType.PLAY_PARTIAL, argImage, frameWidth, frameHeight);
    	mSpriteAnimation = (SVISpriteAnimation) mAnimation;
    	
    	mTotalCount = (argImage.mBitmap.getWidth() / frameWidth) * (argImage.mBitmap.getHeight() / frameHeight);
    	
    	mSpriteAnimation.setInterval(0, mTotalCount - 1);
    }
    
    /**
     * Get Total count of animation frame
     * @return animation frame's total count
     */
    public int getTotalCount(){
    	return mTotalCount;
    }
    
    
    
    /**
     * Set the playback interval in terms of frame indices.<p>
     * The from and to index value must be less then the total frame count value of Sprite Image
     * 
     * @param fromIndex Start frame index.
     * @param toIndex End frame index. 
     * @throws IllegalArgumentException if from /to index is negative or bigger then the total frame counts
     * 
     */
    public void setInterval(int fromIndex, int toIndex) {
	    checkLowValue(fromIndex);
	    checkLowValue(toIndex);
    	if(fromIndex > (mTotalCount-1) || toIndex > (mTotalCount-1)){
	    	throw new IllegalArgumentException("SVSpriteAnimation:: setInterval() : Incorrect index (must be less then total number of frames)  ");
	    }
    	
    	if(mSpriteAnimation != null){
		   mSpriteAnimation.setInterval(fromIndex,toIndex);
	   }
    }
    
    private void checkLowValue(float value) throws IllegalArgumentException{
  		if(value < 0.0f){
  			throw new IllegalArgumentException("Value must be bigger than 0.");
  		}
  	}
}
