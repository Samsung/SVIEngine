package com.github.svieffect.speaker.findColor;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Random;

import android.content.res.ColorStateList;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;


public class CollectColor {
    private final int TargetSize = 60;
    private final int ColorMask = 0xFFE0E0E0;
    
    private int mResultCount;
    private Resources mResources;
    private int mResourceID;
    private Bitmap mBitmap;
    private Bitmap mBitmapSmall;
    private int mWidth;
    private int mHeight;
    
    private Hashtable<Integer, Integer> mHash;
    private CollectColorSort mSort;
    
    private int[] mResult;
    
    // 2013-08-08 teja.kim
    private int[] mDefaultColors;
    
    private Random mRand;
    private int mFindResultIndex;
    
    public CollectColor(Resources res) {
        if(res == null)
            return;
        
        mResources = res;
        mHash = new Hashtable<Integer, Integer>();
        mSort = new CollectColorSort();
        
        mRand = new Random();
        mRand.setSeed(System.currentTimeMillis());
        
        
        // 2013-08-08 teja.kim
        mDefaultColors = new int[39];
        for(int i=0; i<39; i++)
        	mDefaultColors[i] = 0;
    }
    
    public int[] setImage(int resoureID, int resultCount) {
        if(resultCount < 1)
            return null;
        
        mFindResultIndex = 0;
        mResourceID = resoureID;
        mResultCount = resultCount;
        decodeImage();
        mHash.clear();
        runDetail();
        
        CollectColorNode[] nodes = copyToArrayForThread(mHash);
        mSort.sort(nodes);
        int[] colors = getResult(nodes);
        mResult = getRGB(colors);

        return mResult;
    }
    
    // 2013-08-08 teja.kim
    public void setDefaultColor(int[] colors)
    {
    	for(int i=0; i<colors.length; i++)
    	{
    		mDefaultColors[i] = colors[i];	
    	}
    	
    }
    
    public int[] setImage(Bitmap bitmap, int resultCount) {
        if(resultCount < 1)
            return null;
        
        // 2013-08-08 teja.kim
        if(bitmap == null)
        	return mDefaultColors;
        	
        mFindResultIndex = 0;
        mBitmap = bitmap;
        mResultCount = resultCount;
        decodeImage();
        mHash.clear();
        runDetail();
		Log.d("teja.kim", "start_albumjacket thread");
		
        CollectColorNode[] nodes = copyToArrayForThread(mHash);
        mSort.sort(nodes);
        int[] colors = getResult(nodes);
        mResult = getRGB(colors);

        return mResult;
    }
    
    private void decodeImage() {
        if(mBitmap == null) {
            BitmapFactory.Options options = new BitmapFactory.Options();
            options.inJustDecodeBounds = true;
            mBitmap = BitmapFactory.decodeResource(mResources, mResourceID, options);
            int length = options.outWidth > options.outHeight ? options.outWidth : options.outHeight;
            length /= TargetSize;
            int sampleSize = 0x8000;
            while((sampleSize & length) == 0) {
                sampleSize >>= 1;
                if(sampleSize == 0)
                    break;
            }
            if(sampleSize == 0)
                options.inSampleSize = 1;
            else
                options.inSampleSize = sampleSize;
            options.inJustDecodeBounds = false;
            
            mBitmapSmall = BitmapFactory.decodeResource(mResources, mResourceID, options);
        } else {            
            mBitmapSmall = Bitmap.createScaledBitmap(mBitmap, TargetSize, TargetSize, true);
            mBitmap.recycle();
        }
        
        mWidth = mBitmapSmall.getWidth();
        mHeight = mBitmapSmall.getHeight();
    }
    
    private void runDetail() {
        int key;
        for(int i=0; i<mHeight; i++) {
            for(int j=0; j<mWidth; j++) {
                key = mBitmapSmall.getPixel(j, i);
                addHash(key);
            }
        }
    }
    
    private void addHash(int key) {
        key &= ColorMask;
        if(mHash.containsKey(key)) {
            mHash.put(key, ((Integer)mHash.get(key)) + 1);
        } else {
            mHash.put(key, 1);
        }
    }
    
    private CollectColorNode[] copyToArrayForThread(Hashtable<Integer, Integer> hash) {
        CollectColorNode[] nodes = new CollectColorNode[mResultCount];
        int nodes_index = 0;
        int temp_key;
        boolean nodesIsFull = false;
        Enumeration<Integer> e = hash.keys();
        
        for(int i=0; i<nodes.length; i++) {
            nodes[i] = new CollectColorNode();
        }
        while(e.hasMoreElements()) {
            temp_key = e.nextElement();
            
            if(nodesIsFull){
                insert(temp_key, hash.get(temp_key), nodes);
            } else {
                insert(temp_key, hash.get(temp_key), nodes, nodes_index);
                nodes_index++;
                if(nodes_index == nodes.length) {
                    nodesIsFull = true;
                    mSort.makeTree(nodes);
                }
            }
        }
        
        mFindResultIndex = nodes_index;
        
        mSort.makeTree(nodes);
		Log.d("teja.kim", "end_albumjacket thread");
		
        return nodes;
    }
    
    private boolean insert(int key, int value, CollectColorNode[] nodes) {
        if(nodes[0].getValue() < value) {
            nodes[0].setNode(key, value);
            mSort.checkTree(nodes);
            return true;
        }
        return false;
    }
    
    private boolean insert(int key, int value, CollectColorNode[] nodes, int nodes_count) {
        nodes[nodes_count].setKey(key);
        nodes[nodes_count].setValue(value);
        return true;
    }
    
    private int[] getResult(CollectColorNode[] nodes) {
        int[] result = new int[mResultCount];
        for(int i=0; i<result.length; i++) {
            if(mFindResultIndex <= i) {
                result[i] = getSimAverage(result[0]);
                continue;
            }
            
            result[i] = nodes[i].getKey();
        }
        return result;
    }
    
    private int getSimAverage(int color) {
        int multi = 8; 
        int r = (color & 0x00FF0000) >> 16;
        int g = (color & 0x0000FF00) >> 8;
        int b = color & 0x000000FF;
        
        // 2013-08-08 teja.kim
        float avrF = (r + g + b) / 3 / 2 / multi;
        int average = (int)avrF;
        
        int result = 0; 
        if(average == 0){
        	result = getInt(0, 0, 0);
        	return result;
        }
        //
        
        if(mRand.nextBoolean() == true)
            r = r + mRand.nextInt(average) * multi;
        else
            r = r - mRand.nextInt(average) * multi;
        
        if(r > 255)
            r = 255;
        else if(r < 0)
            r = 0;
        
        if(mRand.nextBoolean() == true)
            g = g + mRand.nextInt(average) * multi;
        else
            g = g - mRand.nextInt(average) * multi;
        if(g > 255)
            g = 255;
        else if(g < 0)
            g = 0;
        
        if(mRand.nextBoolean() == true)
            b = b + mRand.nextInt(average) * multi;
        else
            b = b - mRand.nextInt(average) * multi;
        if(b > 255)
            b = 255;
        else if(b < 0)
            b = 0;
        
        result = getInt(r,g,b);
        return result;
    }
    
    private int getInt(int r, int g, int b) {
        int result = (0xFF000000) | (r << 16) | (g << 8) | b;
        return result;
    }
    
    private int[] getRGB(int[] color) {
        int[] result = new int[color.length * 3];
        int index = 0;
        for(int i=0; i<color.length; i++) {
            result[index++] = (color[i] & 0x00FF0000) >> 16;
            result[index++] = (color[i] & 0x0000FF00) >> 8;
            result[index++] = color[i] & 0x000000FF;
        }
        return result;
    }
}
