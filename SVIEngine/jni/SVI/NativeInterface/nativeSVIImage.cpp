#include "nativeSVIImage.h"

#include "../SVIDefines.h"

#include "../BaseType/SVIExtendTypes.h"

#include "../Slide/SVIRenderPartManager.h"

#include "../Render/SVITextureManager.h"
#include "../Render/SVITexture.h"

using namespace SVI;



JNIEXPORT jint JNICALL Java_com_github_sviengine_basetype_SVIImage_nativeCreateSVIImage
  (JNIEnv *env, jclass object) {
	SVIImage *saImage;
	saImage = new SVIImage();
	return (jint)saImage;

}


JNIEXPORT void JNICALL Java_com_github_sviengine_basetype_SVIImage_nativeDeleteSVIImage
	(JNIEnv *env, jclass object, jint surfaceNativeHandle,  jint imageHandle) {

        if (imageHandle == 0) {
            LOGE("SVIImage_nativeDeleteSVIImage : imageHandle is null. error!!");	
            return;
        }	
	SVIGLSurface *surface = (SVIGLSurface*)surfaceNativeHandle;	

        SVIImage* image = (SVIImage*)imageHandle;

		//Thread protect access to bitmap pixel buffer.
		SVIMutex::SVIAutolock lock(surface->getTextureManager()->mLockForBitmapPixels);

		if(image != NULL){
		//Remove the SVIImage from any added SVITextures.
		surface->getTextureManager()->unlink(image);

		//Unlock the bitmap pixel buffer and delete the jni global reference.
		SVIImage *saImage = (SVIImage*) image;
		if(saImage != NULL && saImage->jbitmap != NULL){
			AndroidBitmap_unlockPixels(env, saImage->jbitmap);
			env->DeleteGlobalRef(saImage->jbitmap);
			saImage->jbitmap = NULL;
			saImage->mBits = NULL;
		}

		//Delete the SVIImage object.
		SVI_SVIFE_DELETE(image);
		}
}



JNIEXPORT void JNICALL Java_com_github_sviengine_basetype_SVIImage_nativeLock
    (JNIEnv *env, jclass object, jint imageHandle) {
        if (imageHandle == 0) {
            LOGE("SVIImage_nativeLock : imageHandle is null. error!!");
            return;

        }

        SVIImage *saImage = (SVIImage*) imageHandle;
        saImage->lock();
} 



JNIEXPORT void JNICALL Java_com_github_sviengine_basetype_SVIImage_nativeUnlock
   (JNIEnv *env, jclass object, jint imageHandle) {
	if (imageHandle == 0) {
		LOGE("SVIImage_nativeUnlock : imageHandle is null. error!!");
		return;
	}	
	SVIImage *saImage = (SVIImage*) imageHandle;
	saImage->unLock();
}	


JNIEXPORT void JNICALL Java_com_github_sviengine_basetype_SVIImage_nativeSetEmpty
    (JNIEnv *env, jclass object, jint imageHandle) {
        if (imageHandle == 0) {
            LOGE("SVIImage_nativeUnlock : imageHandle is null. error!!");
            return;
        }	
    SVIImage *saImage = (SVIImage*) imageHandle;
    saImage->mBits = NULL;
}	


JNIEXPORT void JNICALL Java_com_github_sviengine_basetype_SVIImage_nativeSetCopyBitmap
  (JNIEnv *env, jclass object, jint imageHandle, jobject bitmap) {
    if (imageHandle == 0 ) {
        LOGE("SVIImage_nativeSetBitmap : imageHandle is null. error!!");	
        return;
    }	

    SVIImage *saImage = (SVIImage*) imageHandle;

    if( bitmap == 0 ) {
        saImage->mBits = NULL;
        return;
    }

    if (bitmap != NULL && saImage != NULL ) {
        void* addr = NULL;
        AndroidBitmapInfo info;
        AndroidBitmap_getInfo(env, bitmap, &info);

        if( info.format == ANDROID_BITMAP_FORMAT_RGB_565  ||
            info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ) {
                           
            AndroidBitmap_lockPixels(env, bitmap, &addr);

            if( addr != NULL ) {
                saImage->mWidth = info.width;
                saImage->mHeight = info.height;
                saImage->mStride = info.stride;
                SVIInt pixelSize = 0;
                if( info.format == ANDROID_BITMAP_FORMAT_RGB_565 ) {
                    saImage->mImageFormat = SVIImage::RGB_565;
                    pixelSize = 2;
                }
                else if( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ) {
                    saImage->mImageFormat = SVIImage::RGBA_8888;
                    pixelSize = 4;
                }else if( info.format == ANDROID_BITMAP_FORMAT_RGBA_4444 ) {
                    saImage->mImageFormat = SVIImage::RGBA_4444;
                    pixelSize = 2;
                }else if( info.format == ANDROID_BITMAP_FORMAT_A_8 ) {
                    saImage->mImageFormat = SVIImage::A_8;
                    pixelSize = 1;
                }
                saImage->mIsCopied = SVITRUE;
                saImage->mBits = (SVIByte*)malloc(pixelSize * info.width * info.height * sizeof(SVIByte));
                memcpy(saImage->mBits,addr, pixelSize * info.width * info.height);
                AndroidBitmap_unlockPixels(env, bitmap);
            }
        }
    }
}



/*
* Class:     com_github_sviengine_basetype_SVIImage
* Method:    nativeSetTextureIdDirect
* Signature: (I, ILandroid/graphics/Bitmap;)V
*/
JNIEXPORT void JNICALL Java_com_github_sviengine_basetype_SVIImage_nativeSetTextureIdDirect
    (JNIEnv *env, jclass object, jint imageHandle,    jint width, jint height, jint texIndex){
    if (imageHandle == 0 ) {
        LOGE("SVIImage_nativeSetBitmap : imageHandle is null. error!!");
        return;
    }	

    SVIImage *saImage = (SVIImage*)imageHandle;
    saImage->mWidth = width;
    saImage->mHeight = height;
    saImage->mTextureID = texIndex;
    saImage->mIsID = SVITRUE;

    if (saImage->mIsCopied){
        if (saImage->mBits != NULL) free(saImage->mBits);
        saImage->mBits = NULL;
        saImage->mIsCopied = SVIFALSE;
    }
}

/*
 * Class:     com_github_sviengine_basetype_SVIImage
 * Method:    nativeSetRawBufferDirect
 * Signature: (I, ILandroid/graphics/Bitmap;)V
 */
JNIEXPORT void JNICALL Java_com_github_sviengine_basetype_SVIImage_nativeSetRawBufferDirect
  (JNIEnv *env, jclass object, jint imageHandle, jint width, jint height, jint stride, jint format, jbyteArray javaBytes){

    if (imageHandle == 0 ) {
        LOGE("SVIImage_nativeSetRawBufferDirect : imageHandle is null. error!!");
        return;
    }	

    jboolean isCopy;
    SVIImage *saImage = (SVIImage*)imageHandle;
    jbyte *nativeBytes = env->GetByteArrayElements(javaBytes, &isCopy);


    if( nativeBytes == NULL ) {
        saImage->mBits = NULL;
        return;
    }
    saImage->mWidth = width;
    saImage->mHeight = height;
    saImage->mStride = stride;

    SVIInt pixelSize = 0;
    if( format == ANDROID_BITMAP_FORMAT_RGB_565 ) {
        saImage->mImageFormat = SVIImage::RGB_565;
        pixelSize = 2;
    }
    else if( format == ANDROID_BITMAP_FORMAT_RGBA_8888 ) {
        saImage->mImageFormat = SVIImage::RGBA_8888;
        pixelSize = 4;
    }else if( format == ANDROID_BITMAP_FORMAT_RGBA_4444 ) {
        saImage->mImageFormat = SVIImage::RGBA_4444;
        pixelSize = 2;
    }else if( format == ANDROID_BITMAP_FORMAT_A_8 ) {
        saImage->mImageFormat = SVIImage::A_8;
        pixelSize = 1;
    }
    saImage->mIsCopied = SVITRUE;
    saImage->mBits = (SVIByte*)malloc(pixelSize * width * height * sizeof(SVIByte));

    memcpy(saImage->mBits, nativeBytes, pixelSize * width * height * sizeof(SVIByte));
}




JNIEXPORT void JNICALL Java_com_github_sviengine_basetype_SVIImage_nativeSetBitmapCompress
    (JNIEnv *env, jclass object, jint imageHandle, jint compressType, jbyteArray javaBytes) {
        
    if (imageHandle == 0 ) {
        LOGE("SVIImage_nativeSetBitmap : imageHandle is null. error!!");
        return;
    }	

    jboolean isCopy;
    SVIImage *saImage = (SVIImage*)imageHandle;

    jbyte *nativeBytes = env->GetByteArrayElements(javaBytes, &isCopy);

    if( nativeBytes == NULL ) {
        saImage->mBits = NULL;
        return;
    }
    else {

        //2013-03-07 masterkeaton27@gmail.com
        //deallocate copied memory.
        if (saImage->mIsCopied && saImage->mBits != NULL){
            free(saImage->mBits);
            saImage->mBits = NULL;
            saImage->mIsCopied = SVIFALSE;
        }

        if((SVIInt)compressType == (SVIInt)SVIImage::COMPRESS_ATC) {
            
            SVIUInt offset = 0;
            SVIUInt value = 0;
            //check SVI compress file
            memcpy((void *)&(saImage->mCompressHeader.signature), nativeBytes + offset, sizeof(SVIUInt));
            if(SVIImage::COMPRESS_SIGNATURE != saImage->mCompressHeader.signature) {
                LOGE("This compressed texture not svi format !!!");

                return;
            }
            
            offset += sizeof(SVIUInt);

            memcpy((void *)&(saImage->mCompressHeader.width), nativeBytes + offset, sizeof(SVIUInt));
            offset += sizeof(SVIUInt);
            
            memcpy((void *)&(saImage->mCompressHeader.height), nativeBytes + offset, sizeof(SVIUInt));
            offset += sizeof(SVIUInt);
            
            SVIUInt flags;
            memcpy((void *)&flags, nativeBytes + offset, sizeof(SVIUInt));
            if(flags == SVIImage::RGB_565) {
                saImage->mCompressHeader.format = (SVIUInt)SVIImage::ATC_RGB;
            }
            else {
                saImage->mCompressHeader.format = (SVIUInt)SVIImage::ATC_RGBA;
            }
            offset += sizeof(SVIUInt);
            
            memcpy((void *)&(saImage->mCompressHeader.dataOffset), nativeBytes + offset, sizeof(SVIUInt));
            offset += sizeof(SVIUInt);

            SVIUInt nTotalBlocks, nBytesPerBlock;
            nTotalBlocks = ((saImage->mCompressHeader.width + 3) >> 2) * ((saImage->mCompressHeader.height + 3) >> 2);
            if(saImage->mCompressHeader.format == (SVIUInt)SVIImage::ATC_RGBA) {
                nBytesPerBlock = 16;
            }
            else {
                nBytesPerBlock = 8;
            }

            saImage->mCompressHeader.imageSize = nTotalBlocks * nBytesPerBlock;

            saImage->mBits = (SVIByte*)nativeBytes + saImage->mCompressHeader.dataOffset;

            saImage->mWidth = saImage->mCompressHeader.width;
            saImage->mHeight = saImage->mCompressHeader.height;

            saImage->mIsCompress = SVITRUE;
        }
        else {
            LOGE("Compress Type = NONE !!!");
        }
    }
}

JNIEXPORT void JNICALL Java_com_github_sviengine_basetype_SVIImage_nativeSetBitmap
    (JNIEnv *env, jclass object, jint surfaceNativeHandle,  jint imageHandle, jobject bitmap) {
        if (imageHandle == 0 ) {
            LOGE("SVIImage_nativeSetBitmap : imageHandle is null. error!!");	
            return;
        }	

	SVIGLSurface *surface = (SVIGLSurface*)surfaceNativeHandle;
        SVIImage *saImage = (SVIImage*) imageHandle;

        //Thread protect access to bitmap pixel buffer.
       SVIMutex::SVIAutolock lock(surface->getTextureManager()->mLockForBitmapPixels);

        if(saImage != NULL){
        	//Unlock the bitmap pixel buffer and delete the jni global reference for OLD bitmap.
        	if(saImage->jbitmap != NULL){
				AndroidBitmap_unlockPixels(env, saImage->jbitmap);
				env->DeleteGlobalRef(saImage->jbitmap);
				saImage->mBits = NULL;
			}

        	//Save jni global reference for NEW bitmap.
			if(bitmap != NULL){
				saImage->jbitmap = env->NewGlobalRef(bitmap);
			}else{
				saImage->jbitmap = NULL;
			}
        

        //2013-03-07 masterkeaton27@gmail.com
        //deallocate copied memory.
        if (saImage->mIsCopied && saImage->mBits != NULL){
            free(saImage->mBits);
            saImage->mBits = NULL;
            saImage->mIsCopied = SVIFALSE;
        }

        if( bitmap == 0 ) {
            saImage->mBits = NULL;
            return;
        }

		}
        if (bitmap != NULL && saImage != NULL ) {
            void* addr = NULL;
            AndroidBitmapInfo info;
            AndroidBitmap_getInfo(env, bitmap, &info);

            if( info.format == ANDROID_BITMAP_FORMAT_RGB_565  ||
                info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ) {
                    AndroidBitmap_lockPixels(env, bitmap, &addr);
                    
                    if( addr != NULL ) {
                        saImage->mWidth = info.width;
                        saImage->mHeight = info.height;
                        saImage->mStride = info.stride;
                        if( info.format == ANDROID_BITMAP_FORMAT_RGB_565 ) {
                            saImage->mImageFormat = SVIImage::RGB_565;
                        }
                        else if( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ) {
                            saImage->mImageFormat = SVIImage::RGBA_8888;
                        }else if( info.format == ANDROID_BITMAP_FORMAT_RGBA_4444 ) {
                            saImage->mImageFormat = SVIImage::RGBA_4444;
                        }else if( info.format == ANDROID_BITMAP_FORMAT_A_8 ) {
                            saImage->mImageFormat = SVIImage::A_8;
                        }
                        saImage->mBits = (SVIByte*)addr;

                        //We won't unlock the bitmap pixel buffer here. We wait until:
                        //Either another bitmap/null is set ti this SVIImage,
                        //Or this SVIImage is GCed
                        //Or texture upload is over.
                    }
            }
        }
}

JNIEXPORT void JNICALL Java_com_github_sviengine_basetype_SVIImage_nativeSetAlphaType
	(JNIEnv *env, jclass object, jint imageHandle, jint alphaType) {
		if (imageHandle == 0 ) {
			LOGE("SVIImage_nativeSetAlphaType : imageHandle is null. error!!");	
			return;
		}	

		SVIImage *image= (SVIImage*) imageHandle;

		if( alphaType == 0 )
			image->mAlphaType = SVIImage::NORMAL;
		else
			image->mAlphaType = SVIImage::PREMULTIPLIED;
}

  

  
