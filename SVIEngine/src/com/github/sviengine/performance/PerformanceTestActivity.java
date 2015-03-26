package com.github.sviengine.performance;

import android.app.Dialog;
import android.content.DialogInterface;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;

import com.github.sviengine.R;

public class PerformanceTestActivity extends TestCaseBaseActivity {
	

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
		
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		
	}
	

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	}

	
	@Override
	protected void onDestroy() {
		originWindow.release();
		originWindow = null;
		mWindow = null;
		super.onDestroy();
	}
	
	
	@Override
	public boolean onCreateOptionsMenu (Menu menu) {
		MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.performancetest_menu, menu);
        
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected (MenuItem item) {
		
		if( item.getItemId() == R.id.setprop ) {
			showDialog(R.id.setprop, null);        
		} else {
			super.onOptionsItemSelected(item);
		}
		 
		return true;
	}
	
	@Override
	protected Dialog onCreateDialog(int id, Bundle args) {
		// TODO Auto-generated method stub
		Dialog dialog = null;
		
		if( id == R.id.setprop ) {
			dialog = new PerformanceTestDialog(this, 0, new DialogInterface.OnClickListener() {
	            public void onClick(DialogInterface dialog, int id) {
	            	int duration = ((PerformanceTestDialog)dialog).mDuration;
	            	int mIconSize = ((PerformanceTestDialog)dialog).mIconSize;
	            	int mIconQuantity = ((PerformanceTestDialog)dialog).mQuantity;
	            	int mCornerRad = ((PerformanceTestDialog)dialog).mCornerRad;
	       
	            	boolean isShadow = ((PerformanceTestDialog)dialog).isShadowSet;
	            	boolean isLight = ((PerformanceTestDialog)dialog).isLightSet;
	            	boolean isBlend = ((PerformanceTestDialog)dialog).isBlendSet;
	            	boolean isScaletoFit = ((PerformanceTestDialog)dialog).isScaletoFitMat;
	            	boolean isBgColor = ((PerformanceTestDialog)dialog).isBgColorSet;
	            	setDialog(duration, mIconSize, mCornerRad, mIconQuantity, isShadow, isLight, isBlend, isScaletoFit, isBgColor);
	            	Log.i("test", duration+"  "+mIconSize+"  "+mCornerRad+"  "+mIconQuantity+"  "+isShadow+"  "+isLight+"  "+isBlend+"  "+isScaletoFit+"  "+isBgColor);
	            }
	        });
		}
	   
		return dialog;
	}



	protected void setDialog(int duration, int mIconSize, int mCornerRad, int mIconQuantity,
			boolean isShadow, boolean isLight, boolean isBlend, boolean isScaletoFit, boolean isBgColor) {
		
		
		originWindow.duration = duration;
		originWindow.setItemSize(mIconSize);
		originWindow.cornerRadSize = mCornerRad;
		
		originWindow.isShadow = isShadow;
		originWindow.isLight = isLight;
		originWindow.isBlendAdd = isBlend;
		originWindow.isScaletoFitMat = isScaletoFit;
		originWindow.isBgColor = isBgColor;
		
		
		if(mIconQuantity==50){
			originWindow.setItemQuantity(10, 5);
		}else if(mIconQuantity==100){
			originWindow.setItemQuantity(10, 10);
		}else if(mIconQuantity==200){
			originWindow.setItemQuantity(20, 10);
		}else if(mIconQuantity==300){
			originWindow.setItemQuantity(20, 15);
		}else{
			originWindow.setItemQuantity(1, 1);
		}
		originWindow.removeall();
		originWindow.buildSubSlide();
	}

	@Override
	boolean createWindow() {
		
		mWindow = new PerformanceTestWindow(this);
		
		originWindow = (PerformanceTestWindow)mWindow;
		
		if(mWindow == null)
			return false;
		
		return true;
	}
	
	PerformanceTestWindow originWindow = null;

}
