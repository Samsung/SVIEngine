#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../SVICores.h"
#include "SVITexture.h"
#include "SVITextureManager.h"
#include "SVIRenderer.h"
#include "../NativeInterface/nativeSVIImage.h"


#define IS_POWER_OF_TWO(x) ((x != 0) && ((x & (x - 1)) == 0)) ? SVITRUE : SVIFALSE

jmp_buf g_jumper;

void texture_handler(int aSigNum) {
    LOGE("Broken pointer");
}

void texture_sigaction(int aSigNum, siginfo_t *aInfo, void *aCustom) {
    longjmp(g_jumper, 1);
}

namespace SVI{
// Enble this flag to dump image buffer to file (debugging purpose only)
#define SVI_DUMP_ENABLE 0

	SVIUInt SVITexture::mUniqueId = 27;

	const static SVIBool DEBUG = SVIFALSE;

	SVITexture::SVITexture(SVIGLSurface* saGLSurface):
	mIsUploaded(SVIFALSE),
		mIsRequestedUpload(SVIFALSE),
		mTextureId(0),
		mImage(NULL){
			mSVIGLSurface = saGLSurface;
		
			//Initialize texture usage count and chain none.
			mUsageCount = 0;
			mSharedToTexture = NULL;
			mSharedFromTexture = NULL;

			mSVIGLSurface->getTextureManager()->addAliveTexture(this);
			
			mValidCheck = 0x27;
			mId = mUniqueId++;
			snprintf(mName,32,"UnNamedTexture %d",mId);
			mAlphaType = NORMAL;
	}

    SVITexture::~SVITexture(){
        //Remove this texture from SVITextureManager
        mSVIGLSurface->getTextureManager()->remove(this);
		mSVIGLSurface->getTextureManager()->removeAliveTexture(this);

        mValidCheck = 0x72;
        unload();

		mSVIGLSurface = NULL;
    }

	/**
	* Update the shared texture chain to register a new usage.
	*/
	void SVITexture::increamentUsageCount(){
		mUsageCount++;
		if(mSharedToTexture != NULL){
			mSharedToTexture->mSharedFromTexture = NULL;
			mSharedToTexture->increamentUsageCount();
			mSharedToTexture->mSharedFromTexture = this;
		}
		if(mSharedFromTexture != NULL){
			mSharedFromTexture->mSharedToTexture = NULL;
			mSharedFromTexture->increamentUsageCount();
			mSharedFromTexture->mSharedToTexture = this;
		}
	}

	/**
	* Update the shared texture chain to un-register a usage.
	*/
	void SVITexture::decreamentUsageCount(){
		mUsageCount--;
		if(mSharedToTexture != NULL){
			mSharedToTexture->mSharedFromTexture = NULL;
			mSharedToTexture->decreamentUsageCount();
			mSharedToTexture->mSharedFromTexture = this;
		}
		if(mSharedFromTexture != NULL){
			mSharedFromTexture->mSharedToTexture = NULL;
			mSharedFromTexture->decreamentUsageCount();
			mSharedFromTexture->mSharedToTexture = this;
		}
	}

	/**
	* Make this SVITexture to share opengl texture with 'sharingTexture'.
	* @param: sharingTexture - The SVITexture which is sharing its opengl texture.
	*/
    void SVITexture::share(SVITexture* sharingTexture){
		//Find the end of the chanin.
		SVITexture* endOfChain = sharingTexture;
		while(endOfChain->mSharedToTexture != NULL){
			endOfChain = endOfChain->mSharedToTexture;
		}
		
		//Unload any already loaded texture
		unload();
        mTextureId = endOfChain->mTextureId;
		mGLContextId = endOfChain->mGLContextId; //Textures are not deleted while Sharing

		//Update usage count
		endOfChain->increamentUsageCount();
		mUsageCount = endOfChain->mUsageCount;

		//Insert this SVITexture into the shared textures chain.
		mSharedFromTexture = endOfChain;
		mSharedToTexture = NULL;
		endOfChain->mSharedToTexture = this;

        mIsUploaded = SVITRUE;
        mIsRequestedUpload = SVIFALSE;
    }

    

	SVIBool SVITexture::upload(){
		//Make unload and upload atomic
		SVIMutex::SVIAutolock lock(mSVIGLSurface->getTextureManager()->mLockForBitmapPixels);

		unload();

		if( mImage == NULL ) {
			LOGE("SVITexture::upload mImage is null !!!");
			return SVIFALSE; 
		}

		DEBUG_CHECK_LOGI("SVITexture::upload() Image Handle : %d", (SVIInt)mImage);
		SVIImage* image = mImage;

		if (image->mBits == NULL ){
			DEBUG_CHECK_LOGI("image mBIts == NULL");
			mIsUploaded = SVIFALSE;
            mIsRequestedUpload = SVIFALSE;		
            return SVIFALSE;	
		}

		glGenTextures(1,&mTextureId);

		if( mTextureId == 0 ) {
			LOGE("SVITexture::upload glGenTexture is failed !!!");
			return SVIFALSE;
		}

		//Save gl context id in which texture was uploaded.
		mGLContextId = mSVIGLSurface->getTextureManager()->getGLContextId();

		//Set usage count and sharing chain.
		mUsageCount = 1;
		mSharedToTexture = NULL;
		mSharedFromTexture = NULL;

		glBindTexture(GL_TEXTURE_2D, mTextureId);		


		//2011-05-27 masterkeaton27@gmail.com
		//do not change wrap parameter. 
        //GLES20 not power of two texture does not support GL_REPEAT.
        /*if (IS_POWER_OF_TWO(image->mWidth) && IS_POWER_OF_TWO(image->mHeight)){
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }else{
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }*/
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//2011-05-30 select format
        if(image->mIsCompress == SVIFALSE)
		{
            if( mImage != NULL ) {
				//2011-08-01 masterkeaton27@gmail.com
				//to solve texture filtering issue
				if (mAlphaType == PREMULTIPLIED){
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}else{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR);//GL_NEAREST
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_LINEAR);					
				}

				//2012-04-29 masterkeaton27@gmail.com
				//to solve skewed image when alignment is not wrong
				if (image->mImageFormat == SVIImage::RGBA_8888){
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
				}else{
					glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				}
				                                
                /*struct sigaction act;

                act.sa_handler = texture_handler;
                act.sa_sigaction = texture_sigaction;

                sigfillset(&(act.sa_mask));
                sigaction(SIGSEGV, &act, NULL);

                SVIInt result = 0;

                result = setjmp(g_jumper);

                if(result == 0) {*/
                    GLint internalFormat = GL_RGBA;
                    GLint internalPixelType = GL_UNSIGNED_BYTE;

                    //2011-10-26 masterkeaton27@gmail.com
                    //typically 8bit alpha format is luminance texture. 
                    //but, should check with GL_ALPHA
                    if (image->mImageFormat == SVIImage::RGBA_8888) internalFormat = GL_RGBA;
                    else if (image->mImageFormat == SVIImage::RGBA_4444) internalFormat = GL_RGBA;
                    else if (image->mImageFormat == SVIImage::RGB_565) internalFormat = GL_RGB;
                    else if (image->mImageFormat == SVIImage::A_8) internalFormat = GL_LUMINANCE;

                    if (image->mImageFormat == SVIImage::RGBA_8888) internalPixelType = GL_UNSIGNED_BYTE;
                    else if (image->mImageFormat == SVIImage::RGBA_4444) internalPixelType = GL_UNSIGNED_SHORT_4_4_4_4;
                    else if (image->mImageFormat == SVIImage::RGB_565) internalPixelType = GL_UNSIGNED_SHORT_5_6_5;
                    else if (image->mImageFormat == SVIImage::A_8) internalPixelType = GL_UNSIGNED_BYTE;
					// Dump image buffer to file
					dumpImageBufferToFile(image);	

                    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image->mWidth, image->mHeight, 0, 
                        internalFormat, internalPixelType, image->mBits);
                //}
                //else if(result == 1) {
                //    //exception SIGSEGV
                //    LOGE("Broken Bitmap pointer - Using the App, please check recycle method of Bitmap");
                //    LOGE("Broken Bitmap pointer Address(%d) Width(%d), Height(%d)", (SVIInt)image->mBits, image->mWidth, image->mHeight);
                //    mImage->mBits = NULL;
                //    mImage = NULL;
                //    mTextureId = 0;
                //    mIsUploaded = SVITRUE;
                //    mIsRequestedUpload = SVIFALSE;
                //    return SVIFALSE;
                //}

                //2013-03-21 rareboy0112@facebook.com
                //this code do not use Android platform.
                //image->mBits = (SVIByte*)image;
                ////pointer valid check
                //int result = pointer_valid_p(image->mBits);

                //if(result == 1) {
                //    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image->mWidth, image->mHeight, 0, 
                //        internalFormat, internalPixelType, image->mBits);
                //}
                //else {
                //    LOGE("Broken pointer Address");
                //}
			}
		}
        //2013/01/30 rareboy0112@facebook.com
        else {
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glCompressedTexImage2D( 
                GL_TEXTURE_2D, 0, 
                mImage->mCompressHeader.format, mImage->mCompressHeader.width, mImage->mCompressHeader.height, 
                0, mImage->mCompressHeader.imageSize, mImage->mBits );
        }
		mIsUploaded = SVITRUE;
		mIsRequestedUpload = SVIFALSE;

		return SVITRUE;
	}
	
	SVIBool SVITexture::restore(){
		if (unload()){
			return upload();
		}
		return SVIFALSE;
	}

	void SVITexture::setImage(SVIImage * pImage) { 
		//Thread protect access to bitmap pixel buffer.
		//SVIMutex::SVIAutolock lock(mSVIGLSurface->getTextureManager()->mLockForBitmapPixels); 	// BONNIE .. Need to check whether this lock is needed or not.

       	 unload();

		mImage = pImage; 

        if( mImage != NULL ) {
            //2013-03-19 masterkeaton27@gmail.com
            if (mImage->mIsID && mImage->mTextureID > 0){
                mTextureId = mImage->mTextureID;                 
            }
            
            mIsRequestedUpload = SVITRUE;
			if( pImage->mAlphaType == SVIImage::NORMAL ) 
				mAlphaType = NORMAL;
			else //SVIImage::PREMULTIPLIED
				mAlphaType = PREMULTIPLIED;
		}
	}

	void SVITexture::setName(const SVIPChar pName){
		snprintf(mName,32,"%s",pName);
	}

	void SVITexture::requestUpload(){
		mIsRequestedUpload = SVITRUE;
	}

	SVIBool SVITexture::getHasAlpha(){
		if (mImage != NULL){
			if (mImage->mImageFormat == SVIImage::RGBA_8888 || 
				mImage->mImageFormat == SVIImage::RGBA_4444 || 
				mImage->mImageFormat == SVIImage::A_8) return SVITRUE;
		}
		return SVIFALSE;
    }

    SVIBool SVITexture::unload(){

        if ((mIsUploaded == SVITRUE )&& (mTextureId != 0) ){ 

            //2013-02-26 masterkeaton27@gmail.com
            //just set 0 to unused texture
            SVITexture * pExistData = (SVITexture*)mSVIGLSurface->getTextureManager()->find(this, (SVIByte* )getImageData());
            if (pExistData != NULL){
                mTextureId = 0;
                mIsUploaded = SVIFALSE;
            }

            DEBUG_CHECK_LOGI("SVITexture::unload texture:%x, textureID:%d", (SVIInt)this, mTextureId);
            //Unload opengl texture only if this SVITexture belongs to present gl context 
			//and if nobody is sharing this opengl texture.
			if (mGLContextId == mSVIGLSurface->getTextureManager()->getGLContextId() && mUsageCount == 1){
				mSVIGLSurface->getTextureManager()->addToBeUnloadedTexture(mTextureId);
            }
            mTextureId = 0;
            mIsUploaded = SVIFALSE;

			//Remove this SVITexture from texture sharing chain.
			if(mUsageCount > 0){
				decreamentUsageCount();
				if(mSharedToTexture != NULL){
					mSharedToTexture->mSharedFromTexture = mSharedFromTexture;
				}
				if(mSharedFromTexture != NULL){
					mSharedFromTexture->mSharedToTexture = mSharedToTexture;
				}
			}
			
        }

		//Remove links to texture sharing chain.
		mSharedFromTexture = NULL;
		mSharedToTexture = NULL;

        return SVITRUE;
    }




	/*
	 * dumpBuffer to file to debug for any image curroption cases.
	 * @param: image - The image to be stored to file, File name contains width & height of buffer
	 * and stored in sdcard/svi folder. we can view using irfanview to check image  is corrupted or not
	 */
	void SVITexture::dumpImageBufferToFile(SVIImage* image){
#if SVI_DUMP_ENABLE
        GLchar MyFileName[120] = "";
		int resultcode = mkdir("/sdcard/svidump/", 0770); // It will be created once .. even though we tried everytime times
		static GLint FileCount=0,PresentFileCount=0, RemoveFileCount=0;
		GLint PixelSize=0;
		if (image == NULL) return;
        if (image->mImageFormat == SVIImage::RGBA_8888) PixelSize=4;
        else if ((image->mImageFormat == SVIImage::RGBA_4444) || (image->mImageFormat == SVIImage::RGB_565)) PixelSize=2;
        else if (image->mImageFormat == SVIImage::A_8) PixelSize=1;
					
		sprintf(MyFileName,"/sdcard/svidump/svidump%d_w_%d_h_%d_pixal_%d.raw",FileCount,image->mWidth,image->mHeight,PixelSize);
		FILE* outfile = fopen(MyFileName, "wb");
		fwrite(image->mBits, (PixelSize * image->mWidth * image->mHeight), 1, outfile);
		if (outfile) {
			fclose(outfile);
		}
		FileCount++;		
#endif
	}
    //2013-03-21 rareboy0112@facebook.com
    //this code do not use Android platform.
    /*int SVITexture::pointer_valid_p(void *something) {
        int   the_status;
        char  dummy_data;
        pid_t the_child_1;
        pid_t the_child_2;

        the_child_1 = fork();

        if(the_child_1 < 0)
        {
            LOGE("fork fail\n");

            return -1;
        }

        if(the_child_1 == 0)
        {
            dummy_data=*(char *)something;

            exit;
        }

        the_child_2 = waitpid(the_child_1, &the_status, 0);

        return the_status == 0;
    }*/
}
