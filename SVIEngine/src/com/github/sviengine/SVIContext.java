package com.github.sviengine;

import java.util.ArrayList;

import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIBasicAnimation;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIParticleAnimation;
import com.github.sviengine.animation.SVISpriteAnimation;
import com.github.sviengine.animation.SVITransitionAnimation;
import com.github.sviengine.animation._PackageAccessAnimation;
import com.github.sviengine.animation.SVISpriteAnimation.PlayType;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.basetype._PackageAccessBasetype;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.slide.SVIImageSlide;
import com.github.sviengine.slide.SVINinePatchSlide;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide._PackageAccessSlide;

import android.graphics.Bitmap;

public class SVIContext {
	SVIGLSurface mGLSurface;
	
	public SVIContext(SVIGLSurface sviGLSurface){
		if(sviGLSurface == null){
			throw new NullPointerException("SVIGLSurface cannot be null !");
		}
		mGLSurface = sviGLSurface;
	}
	
	///////
	public SVIBasicAnimation createSVIBasicAnimation(int type, float from[], float to[]){
		return _PackageAccessAnimation.createSVIBasicAnimation(mGLSurface, type, from, to);
	}
	
	public SVIBasicAnimation createSVIBasicAnimation(int type, float from[], float to[], int lightType){
		return _PackageAccessAnimation.createSVIBasicAnimation(mGLSurface, type, from, to, lightType);
	}
	
	public SVIBasicAnimation createSVIBasicAnimation(int type, float from, float to){
		return _PackageAccessAnimation.createSVIBasicAnimation(mGLSurface, type, from, to);
	}
	
	public SVIBasicAnimation createSVIBasicAnimation(int type, float from, float to, int lightType){
		return _PackageAccessAnimation.createSVIBasicAnimation(mGLSurface, type, from, to, lightType);
	}
	
	public SVIBasicAnimation createSVIBasicAnimation(int type, SVIPoint from, SVIPoint to){
		return _PackageAccessAnimation.createSVIBasicAnimation(mGLSurface, type, from, to);
	}
	
	public SVIBasicAnimation createSVIBasicAnimation(int type, SVIPoint from, SVIPoint to, int lightType){
		return _PackageAccessAnimation.createSVIBasicAnimation(mGLSurface, type, from, to, lightType);
	}
	
	public SVIBasicAnimation createSVIBasicAnimation(int type, SVIRect from, SVIRect to){
		return _PackageAccessAnimation.createSVIBasicAnimation(mGLSurface, type, from, to);
	}
	
	public SVIBasicAnimation createSVIBasicAnimation(int type, SVIColor from, SVIColor to){
		return _PackageAccessAnimation.createSVIBasicAnimation(mGLSurface, type, from, to);
	}
	
	public SVIBasicAnimation createSVIBasicAnimation(int type, SVIVector3 from, SVIVector3 to){
		return _PackageAccessAnimation.createSVIBasicAnimation(mGLSurface, type, from, to);
	}
	
	/////
	public SVIAnimationSet createSVIAnimationSet(){
		return _PackageAccessAnimation.createSVIAnimationSet(mGLSurface);
	}
	
	public SVIAnimationSet createSVIAnimationSet(String tag){
		return _PackageAccessAnimation.createSVIAnimationSet(mGLSurface, tag);
	}
	
	public SVIAnimationSet createSVIAnimationSet(int duration, int repeatCount, boolean autoReverse, int offset){
		return _PackageAccessAnimation.createSVIAnimationSet(mGLSurface, duration, repeatCount, autoReverse, offset);
	}
	
	public SVIAnimationSet createSVIAnimationSet(int duration, int repeatCount, boolean autoReverse, int offset, String tag){
		return _PackageAccessAnimation.createSVIAnimationSet(mGLSurface, duration, repeatCount, autoReverse, offset, tag);
	}
	
	/////////
	public SVIParticleAnimation createSVIParticleAnimation(){
		return _PackageAccessAnimation.createSVIParticleAnimation(mGLSurface);
	}
	
	///////
	public SVIKeyFrameAnimation createSVIKeyFrameAnimation(int type){
		return _PackageAccessAnimation.createSVIKeyFrameAnimation(mGLSurface, type);
	}
	
	public SVIKeyFrameAnimation createSVIKeyFrameAnimation(int type, int lightType){
		return _PackageAccessAnimation.createSVIKeyFrameAnimation(mGLSurface, type, lightType);
	}
	
	/////////
	public SVISpriteAnimation createSVISpriteAnimation(PlayType playType, SVIImage image, int frameWidth, int frameHeight){
		return _PackageAccessAnimation.createSVISpriteAnimation(mGLSurface, playType, image, frameWidth, frameHeight);
	}
	
	public SVISpriteAnimation createSVISpriteAnimation(PlayType playType, ArrayList<Bitmap> listBitmap){
		return _PackageAccessAnimation.createSVISpriteAnimation(mGLSurface, playType, listBitmap);
	}
	
	/////////
	public SVITransitionAnimation createSVITransitionAnimation(){
		return _PackageAccessAnimation.createSVITransitionAnimation(mGLSurface);
	}
	
	public SVITransitionAnimation createSVITransitionAnimation(int transitionType){
		return _PackageAccessAnimation.createSVITransitionAnimation(mGLSurface, transitionType);
	}
	
	////////
	public SVIImage createSVIImage(){
		return _PackageAccessBasetype.createSVIImage(mGLSurface);
	}
	
	///////
	public SVISlide createSVISlide(){
		return _PackageAccessSlide.createSVISlide(mGLSurface);
	}
	
	public SVISlide createSVISlide(int type){
		return _PackageAccessSlide.createSVISlide(mGLSurface, type);
	}
	
	public SVISlide createSVISlide(int type, Bitmap bitmap){
		return _PackageAccessSlide.createSVISlide(mGLSurface, type, bitmap);
	}
	
	public SVISlide createSVISlide(int type, int parent, float x, float y, float width, float height, float[] color){
		return _PackageAccessSlide.createSVISlide(mGLSurface, type, parent, x, y, width, height, color);
	}
	
	public SVISlide createSVISlide(int type, int parent, float x, float y, float width, float height, float[] color, Bitmap bitmap){
		return _PackageAccessSlide.createSVISlide(mGLSurface, type, parent, x, y, width, height, color, bitmap);
	}
	
	//////
	public SVIImageSlide createSVIImageSlide(){
		return _PackageAccessSlide.createSVIImageSlide(mGLSurface);
	}
	
	public SVIImageSlide createSVIImageSlide(Bitmap bitmap){
		return _PackageAccessSlide.createSVIImageSlide(mGLSurface, bitmap);
	}
	
	/////
	public SVINinePatchSlide createSVINinePatchSlide(Bitmap bitmap){
		return _PackageAccessSlide.createSVINinePatchSlide(mGLSurface, bitmap);
	}
}
