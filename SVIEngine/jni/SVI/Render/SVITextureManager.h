#pragma once
#include "../Utils/SVIMutex.h"
#include "../GLSurface/SVIGLSurface.h"
namespace SVI{
	class SVIImage;
	class SVITexture;
	class SVIMutex;

	typedef std::vector<SVITexture*>			TextureList;
	typedef std::vector<SVIUInt>				TextureIDList;
	class SVITextureManager{
	public:
		SVITextureManager(SVIGLSurface* mSVIGLSurface);//Multi-Instance-Support
		virtual ~SVITextureManager();
		SVITexture*	add(const SVIPChar pName, SVIImage * pImage);
		void			add(SVITexture* pTexture);
		void			addToBeUnloadedTexture(SVIUInt textureID);
		void			remove(const SVIPChar pName);
		void			remove(SVITexture* pTexture);

	public:	
        const SVITexture*	find(SVIUInt id);
		const SVITexture*	find(SVITexture* pOnwer, SVIByte* pBits);
		const SVITexture*	find(const SVIPChar pName);
		const SVITexture* find(SVITexture* pTexture);

		/*
		 * Remove the SVIImage with specified pixel data from any SVITextures.
		 */
		void unlink(SVIByte* pBits);

	public:
		void			initialize();
		
		/**
		* Increment gl context id. This will be called when gl context has changed.
		*/
		void			incrementGLContextId(){
									mGLContextId = (mGLContextId+1) & 0xffff;
								}
								
		/**
		* Get the present gl context's id.
		*/
		SVIInt		getGLContextId(){
									return mGLContextId;
								}
		
		void			update();
		
		/*
		 * Unlock the bitmap pixel buffers so that the memory can be freed up.
		 */
		void 		unlockBitmapBuffers(JNIEnv *env);
		
		void			finalize();
		void			restore();
		SVIBool		getFirstLoadFinished();

		void 			addAliveTexture(SVITexture* saTexture);
		void 			removeAliveTexture(SVITexture* saTexture);

		/*
		 * Remove the SVIImage from any SVITextures using it.
		 * @param: image - The image to be removed.
		 */
		void unlink(SVIImage* image);
	public:
		SVIBool			mFirstTextureRequestFinished;
		TextureList		mTextures;
		TextureIDList	mToBeUnloadedTextures;
		SVIMutex*			mLockForTextureUpload;
		SVIMutex*			mLockForTextureUnload;
		SVIInt			mGLContextId;

		/**Mutex for synchronizing access to bitmap pixel data.*/ 		
		SVIMutex*			mLockForBitmapPixels;

	protected:
		SVIGLSurface * mSVIGLSurface;//Multi-Instance-Support

		/** Holds the list of all SVITextures sharing same surface as this manager. */
		std::vector<SVITexture*>		mAliveTextures;
	};
};