#ifndef __SVI_TEXTURE_H_
#define __SVI_TEXTURE_H_

#include <signal.h>
#include <setjmp.h>
#include "../Utils/SVIMutex.h"
#include "../GLSurface/SVIGLSurface.h"

//2013/03/07 rareboy0112@facebook.com
//signal structure
//struct sigaction {
//    void (*sa_handler) (int);
//    void (*sa_sigaction) (int, siginfo_t *, void *);
//    sigset_t sa_mask;
//    int sa_flags;
//    void (*sa_restorer) (void);
//};
//
//struct siginfo_t {
//    int         si_signo;
//    int         si_errno;
//    int         si_code;
//    pid_t       si_pid;
//    uid_t       si_uid;
//    int         si_status;
//    clock_t     si_utime;
//    clock_t     si_stime;
//    sigval_t    si_value;
//    int         si_int;
//    void        *si_ptr;
//    void        *si_addr;
//    int         si_band;
//    int         si_fd;
//};

namespace SVI{

	class SVITexture{
    
    protected:
        int pointer_valid_p(void *something);

    public:
		SVITexture(SVIGLSurface* saGLSurface);//Multi-Instance-Support
		virtual ~SVITexture();

		enum { NORMAL, PREMULTIPLIED };
		
	public:
		SVIBool  upload();
		SVIBool  restore();
        SVIBool  unload();	
		
		/**
		* Make this SVITexture to share opengl texture with 'sharingTexture'.
		* @param: sharingTexture - The SVITexture which is sharing its opengl texture.
		*/
        void    share(SVITexture* sharingTexture);

        void setImage(SVIImage * pImage);
				
        void setName(const SVIPChar pName);
        void requestUpload();
    public:
        SVIBool	        getValid(){return mValidCheck == 0x27 ? SVITRUE : SVIFALSE;}
        SVIBool	        getRequestedUpload(){return mIsRequestedUpload;}
        SVIBool	        getUploaded(){return mIsUploaded;}
        SVIUInt	        getId(){ return mId; }
        SVIUInt	        getTextureId(){ return mTextureId; }
        SVIPChar	        getName(){ return mName; }
        SVIPVoid	        getImageData(){ return mImage != NULL ? mImage->mBits : NULL;}
        SVIUInt	        getWidth(){ return mImage != NULL ? mImage->mWidth : 0;}
        SVIUInt	        getHeight(){ return mImage != NULL ? mImage->mHeight : 0;}
        SVIInt	        getAlphaType() { return mAlphaType; }
        SVIBool		    getHasAlpha();
        SVIImage*		getImage() { return mImage; }

		/**
		* Update the shared texture chain to register a new usage.
		*/
		void			increamentUsageCount();
		
		/**
		* Update the shared texture chain to un-register a usage.
		*/
		void			decreamentUsageCount();

		/**
		*  Dump image buffer to file
		*/
		void 			dumpImageBufferToFile(SVIImage* image);

		
    protected:
        static SVIUInt		mUniqueId;

        SVIImage*			mImage;

        SVIUInt				mTextureId;
        SVIUInt				mId;
        SVIChar				mName[32];
        SVIByte				mValidCheck;
        SVIInt				mAlphaType;
	protected:
		/** Holds gl context id in which this SVITexture was uploaded.*/
		SVIInt				mGLContextId;
		
		/** Holds usage count for opengl texture used by this SVITexture.*/
		SVIInt				mUsageCount;
		
		/** Holds link to next SVITexture in the texture sharing chain.*/
		SVITexture*			mSharedToTexture;
		
		/** Holds link to previous SVITexture in the texture sharing chain.*/
		SVITexture*			mSharedFromTexture;
		
		SVIBool				mIsUploaded;
		SVIBool				mIsRequestedUpload;

		SVIGLSurface* mSVIGLSurface;
	};
};

#endif
