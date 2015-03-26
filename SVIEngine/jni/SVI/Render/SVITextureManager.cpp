#include "../SVIDefines.h"
#include "../SVICores.h"
#include "SVITexture.h"
#include "SVITextureManager.h"
#include "../Utils/SVIMutex.h"
#include "SVIRenderer.h"
#include "../NativeInterface/nativeSVIImage.h"

#define USE_SHARING_METHOD

namespace SVI{

#define USE_TEXTURE_LOCK

    static const SVIBool DEBUG = SVIFALSE;
   #define SVI_MAX_TEXTURE_COUNT 256
     





	SVITextureManager::SVITextureManager(SVIGLSurface* saGLSurface) {//Multi-Instance-Support
		mSVIGLSurface=saGLSurface;
		mGLContextId = 0;

		mLockForBitmapPixels =  new SVIMutex("Bitmap Pixels lock");
		mFirstTextureRequestFinished = SVIFALSE;
   		mLockForTextureUpload = new SVIMutex("TextureManager Upload");
    	mLockForTextureUnload = new SVIMutex("TextureManager Unload");
	}
	
	SVITextureManager::~SVITextureManager() {
		mSVIGLSurface=NULL;
		SVI_SVIFE_DELETE(mLockForBitmapPixels);
		SVI_SVIFE_DELETE(mLockForTextureUpload);
		SVI_SVIFE_DELETE(mLockForTextureUnload);
	}

	void SVITextureManager::remove(const SVIPChar pName){
		SVIMutex::SVIAutolock lock(mLockForTextureUpload);
		std::vector<SVITexture*>::iterator iter = mTextures.begin();
		while (iter != mTextures.end()){
			SVITexture * pTexture = *iter;
			if (strcmp(pTexture->getName(),pName) == 0){
				mTextures.erase(iter);
				//2011-08-05 masterkeaton27@gmail.com
				//for safe deletion 
				SVI_SVIFE_DELETE(pTexture);
				return;
			}
			iter++;
		}
	}

	void SVITextureManager::remove(SVITexture* pTexture){
        SVIMutex::SVIAutolock lock(mLockForTextureUpload);
		std::vector<SVITexture*>::iterator iter = mTextures.begin();
		while (iter != mTextures.end()){
			if ((*iter) == pTexture){
				mTextures.erase(iter);
				return;
			}
			iter++;
		}
	}

	SVITexture*	SVITextureManager::add(const SVIPChar pName, SVIImage * pImage){
		SVITexture * pTexture = new SVITexture(mSVIGLSurface);
		pTexture->setImage(pImage);
		// 2012/06/12/rareboy0112@facebook.com
		// add setName code
		pTexture->setName(pName);

		SVIMutex::SVIAutolock lock(mLockForTextureUpload);
		mTextures.push_back(pTexture);

        return pTexture;
	}

	void SVITextureManager::add(SVITexture* SVITexture){
		SVIMutex::SVIAutolock lock(mLockForTextureUpload);
		//2011-08-07 masterkeaton27@gmail.com
		if (find(SVITexture) != NULL) return;
		mTextures.push_back(SVITexture);
	}

	void SVITextureManager::addToBeUnloadedTexture(SVIUInt textureID) {
		SVIMutex::SVIAutolock lock(mLockForTextureUnload);
		mToBeUnloadedTextures.push_back(textureID);
	}

    //2013-02-26 masterkeaton27@gmail.com
    const SVITexture*	SVITextureManager::find(SVITexture* pOnwer, SVIByte* pBits){
#ifdef USE_SHARING_METHOD
        SVITexture* pTexture = NULL;

        for (SVIUInt n = 0; n < mTextures.size(); ++n){
            pTexture = mTextures[n];
            if (pTexture == pOnwer) continue;

            if (pTexture->getImageData() == pBits ){
                return pTexture->getUploaded() ? pTexture : NULL;
            }
        }
#endif
        return NULL;
    }

	const SVITexture*	SVITextureManager::find(SVIUInt id){
		SVITexture* pTexture = NULL;
		for (SVIUInt n = 0; n < mTextures.size(); ++n){
			pTexture = mTextures[n];
			if (pTexture->getId() == id ) return pTexture;
		}

        return NULL;
	}

	const SVITexture* SVITextureManager::find(SVITexture* pTexture){
		for (SVIUInt n = 0; n < mTextures.size(); ++n){
			if (pTexture == mTextures[n]) return pTexture;
		}
		return NULL;
	}

	const SVITexture*	SVITextureManager::find(const SVIPChar pName){
		SVITexture* pTexture = NULL;
		for (SVIUInt n = 0; n < mTextures.size(); ++n){
			pTexture = mTextures[n];
			if (pTexture == NULL) {
				continue;
			}			
			if (strcmp(pTexture->getName(),pName) == 0) {
				return pTexture;				
			}
		}
		return NULL;
	}

	void SVITextureManager::initialize(){
		mTextures.reserve(SVI_MAX_TEXTURE_COUNT);
		mFirstTextureRequestFinished = SVIFALSE;
	}

	//2011-08-07 masterkeaton27@gmail.com
	void SVITextureManager::update(){
		SVITexture* pTexture = NULL;
        SVIUInt n=0;
        {
            SVIMutex::SVIAutolock lock(mLockForTextureUnload);
            SVIUInt textureID = 0;
            for (n = 0; n < mToBeUnloadedTextures.size(); ++n){
                textureID = mToBeUnloadedTextures[n];
                DEBUG_CHECK_LOGI("SVITextureManager::update deleteTexture ID : %d", textureID);
                if( textureID > 0 ) {
                    glDeleteTextures(1,&textureID);
                }
            }
            mToBeUnloadedTextures.clear();
        }

        {
            SVIMutex::SVIAutolock lock(mLockForTextureUpload);
            for (n = 0; n < mTextures.size(); ++n){	
				SVITexture * pTexture = mTextures[n];

                if (pTexture->getValid() && !pTexture->getUploaded() && pTexture->getRequestedUpload()){					
					
                    //2013-02-26 masterkeaton27@gmail.com
                    SVITexture * pExistingTexture = (SVITexture *)find(pTexture, (SVIByte*)pTexture->getImageData());
                    
                    if (pExistingTexture != NULL){
                        pTexture->share(pExistingTexture);
                        continue;
                    }
                    
                    mFirstTextureRequestFinished = SVIFALSE;
					if (pTexture->upload()){
						DEBUG_CHECK_LOGI("SVITextureManager::update Texture(%s)  successfully uploaded.",pTexture->getName());
					}else{
						DEBUG_CHECK_LOGI("SVITextureManager::update Texture(%s)  upload failed.",pTexture->getName());
					}				
				}		
			}			
			if (mTextures.size() > 0) mFirstTextureRequestFinished = SVITRUE;
		}

		//2011-08-07 masterkeaton27@gmail.com
		//following codes are no longer need.
		//dangling texture pointer problem was fixed.
		/*
		{		
			SVIMutex::SVIAutolock lock(mLockForTextureUpload);
			std::vector<SVITexture*>::iterator iter = mTextures.begin();
			while (iter != mTextures.end()){
				SVITexture * pTexture = *iter;
				if (!pTexture->getValid()){
					iter = mTextures.erase(iter);
					LOGE("Exception Remove: Texture Count %d after ",mTextures.size());
					continue;
				}
				iter++;
			}
		}
		*/

    }
	
	
	/*
	 * Unlock the bitmap pixel buffers so that the memory can be freed up.
	 */
	void SVITextureManager::unlockBitmapBuffers(JNIEnv *env){
		//Thread protect access to bitmap pixel buffer.
		SVIMutex::SVIAutolock lock(mLockForBitmapPixels);

		//Do for all the textures that has been just uploaded.
		for (SVIUInt n = 0; n < mTextures.size(); ++n){
			SVITexture * pTexture = mTextures[n];

			//Unlock the bitmap pixel buffer and delete the jni global reference.
			if((pTexture != NULL && pTexture->getImage() != NULL) && pTexture->getImage()->jbitmap != NULL){
				AndroidBitmap_unlockPixels(env, pTexture->getImage()->jbitmap);
				env->DeleteGlobalRef(pTexture->getImage()->jbitmap);
				pTexture->getImage()->jbitmap = NULL;
			}
		}

		//Erase uploaded textures from the list.
		mTextures.erase(mTextures.begin(), mTextures.end());
		
	}


    void SVITextureManager::finalize(){
        SVIMutex::SVIAutolock lock(mLockForTextureUpload);
        SVITexture* pTexture = NULL;
        for (SVIUInt n = 0; n < mTextures.size(); ++n){
            pTexture = mTextures[n];
            if (pTexture->getUploaded()){
                pTexture->unload();
            }
            SVI_SVIFE_DELETE(pTexture);
        }
        mTextures.clear();
        mFirstTextureRequestFinished = SVIFALSE;
	}


	SVIBool		SVITextureManager::getFirstLoadFinished(){
		SVIMutex::SVIAutolock lock(mLockForTextureUpload);
		return mFirstTextureRequestFinished;
	}



	void SVITextureManager::restore(){
        SVIMutex::SVIAutolock lock(mLockForTextureUpload);
		mFirstTextureRequestFinished = SVIFALSE;

		SVITexture* pTexture = NULL;
		for (SVIUInt n = 0; n < mTextures.size(); ++n){
			pTexture = mTextures[n];
			pTexture->unload();
			//2012-06-13 masterkeaton27@gmail.com
			//when store called then should re-flag request upload
			pTexture->requestUpload();
		}


		//2012-04-02 masterkeaton27@gmail.com
		//remove re-upload routine
		/*
		SVIUInt textureID = 0;
		for (int n = 0; n < mToBeUnloadedTextures.size(); ++n){
			textureID = mToBeUnloadedTextures[n];
			if( textureID > 0 ) {
				glDeleteTextures(1,&textureID);
			}
		}
		*/
		mToBeUnloadedTextures.clear();


		//2012-04-02 masterkeaton27@gmail.com
		//remove re-upload routine
		/*
		for (int n = 0; n < mTextures.size(); ++n){
			pTexture = mTextures[n];
			if(pTexture->upload()){
				DEBUG_CHECK_LOGI("SVITextureManager::restore Texture(%s)  successfully re-uploaded.",pTexture->getName());
			}else{
				DEBUG_CHECK_LOGI("SVITextureManager::restore Texture(%s)  Re-Upload Failed",pTexture->getName());
			}			
		}
		if (mTextures.size() > 0) mFirstTextureRequestFinished = SVITRUE;
		*/
	}

	void SVITextureManager::addAliveTexture(SVITexture* saTexture){
		mAliveTextures.push_back(saTexture);
	}
	
	void SVITextureManager::removeAliveTexture(SVITexture* saTexture){
		std::vector<SVITexture*>::iterator iter = mAliveTextures.begin();
		while (iter != mAliveTextures.end()){
			if ((*iter) == saTexture){
				mAliveTextures.erase(iter);
				return;
			}
			iter++;
		}
	}

	/*
	 * Remove the SVIImage from any SVITextures using it.
	 * @param: image - The image to be removed.
	 */
	void SVITextureManager::unlink(SVIImage* image){
        std::vector<SVITexture*>::iterator iter = mAliveTextures.begin();
        while (iter != mAliveTextures.end()){
            SVITexture * pTexture = *iter;
            if (pTexture->getImage() == image){                
                pTexture->setImage(NULL);                      
            }
            iter++;
        }
    }
}