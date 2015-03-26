package com.github.sviengine.slide;

import com.github.sviengine.glsurface.SVIGLSurface;

import android.graphics.Bitmap;

public class _PackageAccessSlide {
	public static SVISlide createSVISlide(SVIGLSurface saGLSurface){
		return new SVISlide(saGLSurface);
	}
	
	public static SVISlide createSVISlide(SVIGLSurface saGLSurface, int type){
		return new SVISlide(saGLSurface, type);
	}
	
	public static SVISlide createSVISlide(SVIGLSurface saGLSurface, int type, Bitmap bitmap){
		return new SVISlide(saGLSurface, type, bitmap);
	}
	
	public static SVISlide createSVISlide(SVIGLSurface saGLSurface, int type, int parent, float x, float y, float width, float height, float[] color){
		return new SVISlide(saGLSurface, type, parent, x, y, width, height, color);
	}
	
	public static SVISlide createSVISlide(SVIGLSurface saGLSurface, int type, int parent, float x, float y, float width, float height, float[] color, Bitmap bitmap){
		return new SVISlide(saGLSurface, type, parent, x, y, width, height, color, bitmap);
	}
	
	//////
	public static SVIImageSlide createSVIImageSlide(SVIGLSurface saGLSurface){
		return new SVIImageSlide(saGLSurface);
	}
	
	public static SVIImageSlide createSVIImageSlide(SVIGLSurface saGLSurface, Bitmap bitmap){
		return new SVIImageSlide(saGLSurface, bitmap);
	}
	
	/////
	public static SVINinePatchSlide createSVINinePatchSlide(SVIGLSurface saGLSurface, Bitmap bitmap){
		return new SVINinePatchSlide(saGLSurface, bitmap);
	}
}
