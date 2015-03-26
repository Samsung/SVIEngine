package com.github.visualview.unittest;

import java.io.InputStream;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

import com.github.visualview.R;
import com.github.visualview.SVColor;
import com.github.visualview.SVGLSurface;
import com.github.visualview.SVImage;
import com.github.visualview.SVSlide;
import com.github.visualview.SVVector3;

public class HelloVisualViewActivity extends Activity implements OnTouchListener{

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		
		mSurface = new SVGLSurface(this);
		
		setContentView(mSurface);
		
		
		mSlide = new SVSlide(null, 0.0f, 0.0f, 1000.0f, 1000.0f, null);
		mSurface.addSlide(mSlide);
		
		
		mSubSlide = new SVSlide(mSlide, 200.0f, 200.0f, 600.0f, 600.0f, new SVColor(1.0f, 1.0f, 1.0f, 1.0f));
		
		mSubSlide.setBackgroundColor(new SVColor(0.0f, 0.0f, 1.0f, 1.0f));

		mSubSlide.setProjectionType(SVSlide.PROJECTION_PERSPECTIVE);
		

		InputStream is;	
		is = getResources().openRawResource(R.drawable.icon);
		Bitmap bitmap = BitmapFactory.decodeStream(is);
		
		SVImage svImage = new SVImage(true);
		svImage.setBitmap(bitmap);

		mSubSlide.setScale(new SVVector3(-1.0f, -1.0f, -1.0f));
		mSubSlide.setImage(svImage);
		
		
		mSurface.setOnTouchListener(this);
		
	}	
	
	SVGLSurface mSurface = null;
	SVSlide mSlide = null;
	SVSlide mSubSlide = null;
	private boolean mIs = false;
	
	@Override
	public boolean onTouch(View v, MotionEvent event) {
		// TODO Auto-generated method stub
		if(event.getAction()== MotionEvent.ACTION_DOWN){
			if(!mIs){
				
				mSubSlide.setBlendType(SVSlide.BLEND_ADD);
				
				mSubSlide.setBackgroundColor(new SVColor(1.0f, 0.0f, 0.0f, 1.0f));
			}
			
		}
		return false;
	}
	
	
}
