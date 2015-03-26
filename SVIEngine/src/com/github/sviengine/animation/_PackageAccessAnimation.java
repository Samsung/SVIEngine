package com.github.sviengine.animation;

import java.util.ArrayList;

import com.github.sviengine.animation.SVISpriteAnimation.PlayType;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.glsurface.SVIGLSurface;

import android.graphics.Bitmap;

public class _PackageAccessAnimation {
	public static SVIBasicAnimation createSVIBasicAnimation(SVIGLSurface saGLSurface, int type, float from[], float to[]){
		return new SVIBasicAnimation(saGLSurface, type, from, to);
	}
	
	public static SVIBasicAnimation createSVIBasicAnimation(SVIGLSurface saGLSurface, int type, float from[], float to[], int lightType){
		return new SVIBasicAnimation(saGLSurface, type, from, to, lightType);
	}
	
	public static SVIBasicAnimation createSVIBasicAnimation(SVIGLSurface saGLSurface, int type, float from, float to){
		return new SVIBasicAnimation(saGLSurface, type, from, to);
	}
	
	public static SVIBasicAnimation createSVIBasicAnimation(SVIGLSurface saGLSurface, int type, float from, float to, int lightType){
		return new SVIBasicAnimation(saGLSurface, type, from, to, lightType);
	}
	
	public static SVIBasicAnimation createSVIBasicAnimation(SVIGLSurface saGLSurface, int type, SVIPoint from, SVIPoint to){
		return new SVIBasicAnimation(saGLSurface, type, from, to);
	}
	
	public static SVIBasicAnimation createSVIBasicAnimation(SVIGLSurface saGLSurface, int type, SVIPoint from, SVIPoint to, int lightType){
		return new SVIBasicAnimation(saGLSurface, type, from, to, lightType);
	}
	
	public static SVIBasicAnimation createSVIBasicAnimation(SVIGLSurface saGLSurface, int type, SVIRect from, SVIRect to){
		return new SVIBasicAnimation(saGLSurface, type, from, to);
	}
	
	public static SVIBasicAnimation createSVIBasicAnimation(SVIGLSurface saGLSurface, int type, SVIColor from, SVIColor to){
		return new SVIBasicAnimation(saGLSurface, type, from, to);
	}
	
	public static SVIBasicAnimation createSVIBasicAnimation(SVIGLSurface saGLSurface, int type, SVIVector3 from, SVIVector3 to){
		return new SVIBasicAnimation(saGLSurface, type, from, to);
	}
	
	//////////
	public static SVIAnimationSet createSVIAnimationSet(SVIGLSurface saGLSurface){
		return new SVIAnimationSet(saGLSurface);
	}
	
	public static SVIAnimationSet createSVIAnimationSet(SVIGLSurface saGLSurface, String tag){
		return new SVIAnimationSet(saGLSurface, tag);
	}
	
	public static SVIAnimationSet createSVIAnimationSet(SVIGLSurface saGLSurface, int duration, int repeatCount, boolean autoReverse, int offset){
		return new SVIAnimationSet(saGLSurface, duration, repeatCount, autoReverse, offset);
	}
	
	public static SVIAnimationSet createSVIAnimationSet(SVIGLSurface saGLSurface, int duration, int repeatCount, boolean autoReverse, int offset, String tag){
		return new SVIAnimationSet(saGLSurface, duration, repeatCount, autoReverse, offset, tag);
	}
	
	/////////
	public static SVIParticleAnimation createSVIParticleAnimation(SVIGLSurface saGLSurface){
		return new SVIParticleAnimation(saGLSurface);
	}
	
	///////
	public static SVIKeyFrameAnimation createSVIKeyFrameAnimation(SVIGLSurface saGLSurface, int type){
		return new SVIKeyFrameAnimation(saGLSurface, type);
	}
	
	public static SVIKeyFrameAnimation createSVIKeyFrameAnimation(SVIGLSurface saGLSurface, int type, int lightType){
		return new SVIKeyFrameAnimation(saGLSurface, type, lightType);
	}
	
	/////////
	public static SVISpriteAnimation createSVISpriteAnimation(SVIGLSurface saGLSurface, PlayType playType, SVIImage image, int frameWidth, int frameHeight){
		return new SVISpriteAnimation(saGLSurface, playType, image, frameWidth, frameHeight);
	}
	
	public static SVISpriteAnimation createSVISpriteAnimation(SVIGLSurface saGLSurface, PlayType playType, ArrayList<Bitmap> listBitmap){
		return new SVISpriteAnimation(saGLSurface, playType, listBitmap);
	}
	
	/////////
	public static SVITransitionAnimation createSVITransitionAnimation(SVIGLSurface saGLSurface){
		return new SVITransitionAnimation(saGLSurface);
	}
	
	public static SVITransitionAnimation createSVITransitionAnimation(SVIGLSurface saGLSurface, int transitionType){
		return new SVITransitionAnimation(saGLSurface, transitionType);
	}
}
