package com.github.sviengine.unittest;

import android.content.res.Configuration;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

public class NonNormalMapActivity extends UnitTestBaseActivity implements SensorEventListener{
	
	
	public static float swRoll;
	public static float swPitch;
	public static float swAzimuth;

	public static Sensor accelerometer;
	public static Sensor magnetometer;
	public static Sensor gyrometer;
	public static Sensor gravitymeter;
	
	public static SensorManager mSensorManager;
	public static float[] mAccelerometer = null;
	public static float[] mGeomagnetic = null;
	public static float[] mGyro = null;
	public static float[] mGravity = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		//mSensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);
	    //accelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
	    //magnetometer = mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
	    //gyrometer = mSensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
	    //gravitymeter = mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY);

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
		
		//mSensorManager.unregisterListener(this, accelerometer);
	    //mSensorManager.unregisterListener(this, magnetometer);
	    //mSensorManager.unregisterListener(this, gyrometer);
	    //mSensorManager.unregisterListener(this, gravitymeter);
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		
		//mSensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_GAME); //SENSOR_DELAY_GAME
	    //mSensorManager.registerListener(this, magnetometer, SensorManager.SENSOR_DELAY_GAME);
	    //mSensorManager.registerListener(this, gyrometer, SensorManager.SENSOR_DELAY_GAME);
	    //mSensorManager.registerListener(this, gravitymeter, SensorManager.SENSOR_DELAY_GAME);
	}

	@Override
	public boolean createWindow() {
		// TODO Auto-generated method stub
		mWindow = new NonNormalMapWindow(this);
		
		if( mWindow == null )
			return false;
		
		return true;
	}
	
	@Override
	public boolean onCreateOptionsMenu (Menu menu) {				
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected (MenuItem item) {
		mWindow.onOptionsItemSelected(item);
		return true;
	}

	@Override
	public void onAccuracyChanged(Sensor arg0, int arg1) {
		// TODO Auto-generated method stub
		
	}

	
	@Override
	public void onSensorChanged(SensorEvent event) {
		
		if (event.sensor.getType() == Sensor.TYPE_GRAVITY) {
	        mGravity = event.values;
	        ((NormalMapWindow)mWindow).setSensorValueGrv(-mGravity[0], mGravity[1], mGravity[2]);
	        //Log.i("button","-x:"+-mGravity[0]+" y:"+mGravity[1]+" z:"+mGravity[2]);
	        return;
	    }
			    
	    if (event.sensor.getType() == Sensor.TYPE_GYROSCOPE) {
	        mGyro = event.values;	         
	    }

	    if (mGyro != null) {
	    	float azimuth = mGyro[0]; //z
            float pitch = mGyro[1]; //x
            float roll = mGyro[2]; //y
            
            float x = azimuth * (float)Math.sin(pitch);
            float y = 0.0f;
            float z = azimuth * (float)Math.cos(pitch);
            // ((NormalMapWindow)mWindow).setSensorValueAcc(x, y, z);        
	    	return;
	    }
	    /*
	    if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD) {
	        mGeomagnetic = event.values;
	    }
	    
	    if (event.sensor.getType() == Sensor.TYPE_GYROSCOPE) {
	        mGyro = event.values;
	         
	    }

	    if (mGyro != null) {
	    	float azimuth = mGyro[0]; //z
            float pitch = mGyro[1]; //x
            float roll = mGyro[2]; //y
            
            float x = azimuth * (float)Math.sin(pitch) * (float)Math.cos(roll);
            float y = azimuth * (float)Math.sin(pitch) * (float)Math.sin(roll);
            float z = azimuth * (float)Math.cos(pitch);
             ((NormalMapWindow)mWindow).setSensorValueAcc(-mAccelerometer[0], mAccelerometer[1], mAccelerometer[2]);
            ((DeformationWindow)mWindow).setSensorValue(x, y, z);
	    	return;
	    }
	    if (mAccelerometer != null && mGeomagnetic != null && mGyro != null) {
	        float R[] = new float[9];
	        float I[] = new float[9];
	        boolean success = SensorManager.getRotationMatrix(R, I, mAccelerometer, mGeomagnetic);
	        	        	        
	        if (success) {
	        	float orientation[] = new float[3];
	            SensorManager.getOrientation(R, orientation);
	            // at this point, orientation contains the azimuth(direction), pitch and roll values.
	            float azimuth = orientation[0]; //z
	            float pitch = orientation[1]; //x
	            float roll = orientation[2]; //y
	              
	            float x = azimuth * (float)Math.sin(pitch) * (float)Math.cos(roll);
	            float y = azimuth * (float)Math.sin(pitch) * (float)Math.sin(roll);
	            float z = azimuth * (float)Math.cos(pitch);
	              
	            ((DeformationWindow)mWindow).setSensorValue(x, y, z);
	        }
	    }
	    */
	}

}
