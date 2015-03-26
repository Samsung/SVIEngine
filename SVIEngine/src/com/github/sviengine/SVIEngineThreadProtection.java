package com.github.sviengine;

import android.os.Looper;
import android.util.Log;

public class SVIEngineThreadProtection {
	
	private final static String TAG = SVIEngineThreadProtection.class.getSimpleName();
	
	/**
     *  Validate Thread's are running on same main thread or not
     */
	public static void validateMainThread() {
		 //Log.e(TAG," Main Thread : " +Looper.getMainLooper().getThread()+"    Current Thread : "+Thread.currentThread() );
		if (Looper.getMainLooper().getThread() != Thread.currentThread() ){
			Log.e(TAG,"Not called from Main Thread, Thread Id's : "+Looper.getMainLooper().getThread()+"  Current Thread :"+Thread.currentThread());
			// Print the Error Logs in case , it is not handled in runtime Exception
			Thread.dumpStack();
			throw new RuntimeException("SVI Engine Api's are not called from Main Thread");
		}
	}
}
