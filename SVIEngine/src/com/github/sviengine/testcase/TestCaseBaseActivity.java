package com.github.sviengine.testcase;


import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import com.github.sviengine.slide.SVISlide;

import android.app.Activity;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;

abstract public class TestCaseBaseActivity extends Activity {

	abstract boolean createWindow();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		if( createWindow() ) {
			setContentView(mWindow);
			boolean translucent = true;
			int depth = 16;
			int stencil = 8;
			mWindow.initialize(translucent, depth, stencil);
		}
		else
			Log.e("SVI UnitTest", "TestBed Window is not created!!!");
		
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
		if( mWindow != null ) {
			int rotation = getWindowManager().getDefaultDisplay().getRotation();
			mWindow.onConfigurationChanged(rotation);
		}
	}
	


	private void resetVRFlingerStatus(String val ){
		File file = null;
		FileWriter fw = null;
		BufferedWriter bw = null;
		try {
			file = new File("/data/data/com.gvrf.vrlauncher/files/flinger_status.txt");
			if(file != null) {
				fw = new FileWriter(file.getAbsoluteFile());
				if( fw != null){
					bw = new BufferedWriter(fw);
		            if( bw != null ) {
		            	bw.write(val);
		            }
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if( bw != null ) try { bw.close(); } catch (IOException e) {}
			bw = null;
			if( fw != null ) try { fw.close(); } catch (IOException e) {}
			fw = null;
			file = null;
		}
	}
			

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		if(mWindow!=null)
			mWindow.onPause();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		if( mWindow != null )
			mWindow.onResume();
	}
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		Log.e("SVI", "Activity Destroyed");
		SVISlide.nativeSetRootSlideSVISlide(0);
		super.onDestroy();
	}
	




	protected PlatformWindow			mWindow = null;
}
