package com.github.sviengine.scene;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.SVIEngineThreadProtection;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.scene.SVISceneNode;


/**
 * SVISceneNode class control 3dModel
 * if import 3d model made from 3d making application (etc. MAYA, 3D Max ...), this class can control them.
 * read filename and contain them to node.
 * @Undefined class.
 */
public class SVISceneNode {
	static public SVISceneNode createSceneNodeFromModelFile(String fileName) {
		return createSceneNodeFromModelFile(fileName, SVIGLSurface.getSurface(null));
	}
	
	/**
	 * create scene node from model file
	 *
	 * @param fileName > 3d model's file name
	 * @return SVISceneNode outNode
	 */
	static public SVISceneNode createSceneNodeFromModelFile(String fileName, SVIGLSurface surface) {
		SVIEngineThreadProtection.validateMainThread();
		if(fileName == null)
			return null;
		
		surface = SVIGLSurface.getSurface(surface);
		
		SVISceneNode outNode = new SVISceneNode(surface);
		int result = nativeLoad3dModelFile(surface.getNativeHandle(), fileName);
		
		if(result != -1) {
			outNode.mId = fileName;
			return outNode;
		}
		else {
			outNode.mId = null;
			return outNode;
		}
	}
	
	/**
	 * get id
	 *
	 * @return String mId
	 */
	public final String getId() {
		return mId;
	}
	
	protected SVISceneNode() {
		this(null);
	}
	
	protected SVISceneNode(SVIGLSurface surface) {
		surface = SVIGLSurface.getSurface(surface);
		mNatSurfaceHandle = surface.getNativeHandle();
		mId = null;
	}
	
	private String mId;
	
	/*****************************************************************************************************************/
	/**
	  * SVI3dModel native interface.
	  *
	  */
	static {
		System.loadLibrary(SVIEngineDesc.mName);
	}

	private int mNatSurfaceHandle = 0;
	private static native int nativeLoad3dModelFile(int mNatSurfaceHandle, String fileName);
}
