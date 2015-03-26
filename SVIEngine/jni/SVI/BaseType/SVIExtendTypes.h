#ifndef __SVI_EXTEND_TYPES_H_
#define __SVI_EXTEND_TYPES_H_

#include <jni.h>

#include "../SVIDefines.h"
#include "SVIBaseTypes.h"
#include "../Utils/SVIMutex.h"
#include "SVISharedPtr.h"

namespace SVI {
	
	class SVIPoint {
	public:
		SVIPoint() {
			mX = 0;
			mY = 0;
		}

		SVIPoint(SVIFloat x, SVIFloat y) {
			mX = x;
			mY = y;
		}

		SVIPoint(const SVIPoint& rhs) {
			mX = rhs.mX;
			mY = rhs.mY;
		}

		SVIPoint operator + (const SVIPoint& rhs) const {
			return SVIPoint(mX+rhs.mX, mY+rhs.mY);
		}

		SVIPoint operator + (const SVIPoint& rhs) {
			return SVIPoint(mX+rhs.mX, mY+rhs.mY);
		}

		SVIPoint operator + (SVIPoint& rhs) {
			return SVIPoint(mX+rhs.mX, mY+rhs.mY);
		}

		SVIPoint operator - (const SVIPoint& rhs) const {
			return SVIPoint(mX-rhs.mX, mY-rhs.mY);
		}

		SVIPoint operator - (const SVIPoint& rhs) {
			return SVIPoint(mX-rhs.mX, mY-rhs.mY);
		}

		SVIPoint operator - (SVIPoint& rhs) {
			return SVIPoint(mX-rhs.mX, mY-rhs.mY);
		}

		SVIPoint operator * (const SVIFloat& rhs) {
			return SVIPoint(mX*rhs, mY*rhs);
		}

		SVIPoint operator * (const SVIFloat& rhs) const {
			return SVIPoint(mX*rhs, mY*rhs);
		}

		SVIBool operator == (const SVIPoint& rhs) {
			if( mX != rhs.mX || mY != rhs.mY )
				return SVIFALSE;
			
			return SVITRUE;
		}

		SVIBool operator != (const SVIPoint& rhs) {
			if( *this == rhs )
				return SVIFALSE;
			return SVITRUE;
		}

	public:
		SVIFloat		mX;
		SVIFloat		mY;
	};
	
	class SVISize {
	public:
		SVISize(){
			mWidth = 0;
			mHeight = 0;
		}

		SVISize(SVIFloat w, SVIFloat h) {
			mWidth = w;
			mHeight = h;
		}

		SVISize(const SVISize& rhs) {
			mWidth = rhs.mWidth;
			mHeight = rhs.mHeight;
		}

		SVISize operator + (const SVISize& rhs) {
			return SVISize(mWidth+rhs.mWidth, mHeight+rhs.mHeight);
		}

		SVISize operator - (const SVISize& rhs) {
			return SVISize(mWidth-rhs.mWidth, mHeight-rhs.mHeight);
		}

		SVIBool operator == (const SVISize& rhs) {
			if( mWidth != rhs.mWidth || mHeight != rhs.mHeight )
				return SVIFALSE;
			
			return SVITRUE;
		}

		SVIBool operator != (const SVISize& rhs) {
			if( *this == rhs )
				return SVIFALSE;
			return SVITRUE;
		}

	public:
		SVIFloat		mWidth;
		SVIFloat		mHeight;
	};


	class SVIRect {
	public:
		SVIRect() {}

		SVIRect(SVIPoint origin, SVISize size) {
			mOrigin = origin;
			mSize = size;
		}

		SVIRect(SVIFloat x, SVIFloat y, SVIFloat w, SVIFloat h) {
			mOrigin.mX = x;
			mOrigin.mY = y;

			mSize.mWidth = w;
			mSize.mHeight = h;
		}

		SVIRect(const SVIRect& rhs) {
			mOrigin = rhs.mOrigin;
			mSize = rhs.mSize;
		}

		SVIBool operator == (const SVIRect& rhs) {
			if( mOrigin != rhs.mOrigin || mSize != rhs.mSize )
				return SVIFALSE;
			
			return SVITRUE;
		}

		SVIBool pointInRect(SVIPoint point) {
			if( mOrigin.mX > point.mX || mOrigin.mY < point.mY )
				return SVIFALSE;
			if( mOrigin.mY > point.mY || mOrigin.mY < point.mY )
				return SVIFALSE;

			return SVITRUE;
		}

		void setOrigin(const SVIPoint& point) { mOrigin = point; }
		const SVIPoint& getOrigin() { return mOrigin; }
		
		void setSize(const SVISize& size) { mSize = size; }
		const SVISize& getSize() { return mSize; }

	public:
		SVIPoint		mOrigin;
		SVISize		mSize;
	};


#define CHECK_RANGE(v)	if(v > 1.0f) v = 1.0f; \
						if(v < 0.0f) v = 0.0f; 
	class SVIColor {
	public:
		SVIColor() {
			mR = 0.0f;
			mG = 0.0f;
			mB = 0.0f;
			mA = 1.0f;
		}

		SVIColor(const SVIFloat& r, const SVIFloat& g, const SVIFloat& b, const SVIFloat& a) {
			mR = r;
			mG = g;
			mB = b;
			mA = a;
		}

		SVIColor(const SVIColor& rhs) {
			mR = rhs.mR;
			mG = rhs.mG;
			mB = rhs.mB;
			mA = rhs.mA;
		}

		const SVIColor& operator = (const SVIColor& rhs) {
			mR = rhs.mR;
			mG = rhs.mG;
			mB = rhs.mB;
			mA = rhs.mA;
			return *this;
		}

		SVIColor operator + (const SVIColor& rhs) {
			return SVIColor(mR+rhs.mR, mG+rhs.mG, mB+rhs.mB, mA+rhs.mA);
		}

		SVIColor operator + (const SVIColor& rhs) const{
			return SVIColor(mR+rhs.mR, mG+rhs.mG, mB+rhs.mB, mA+rhs.mA);;
		}

		SVIColor operator - (const SVIColor& rhs) {
			return SVIColor(mR-rhs.mR, mG-rhs.mG, mB-rhs.mB, mA-rhs.mA);
		}

		SVIColor operator - (const SVIColor& rhs) const{
			return SVIColor(mR-rhs.mR, mG-rhs.mG, mB-rhs.mB, mA-rhs.mA);
		}

		SVIColor operator * (const SVIFloat& rhs) {
			return SVIColor(mR*rhs, mG*rhs, mB*rhs, mA*rhs);
		}

		SVIColor operator * (const SVIFloat& rhs) const {
			return SVIColor(mR*rhs, mG*rhs, mB*rhs, mA*rhs);
		}

		SVIBool operator == (const SVIColor& rhs) {
			if( mR != rhs.mR || mG != rhs.mG || 
				mB != rhs.mB || mA != rhs.mA )
				return SVIFALSE;
			
			return SVITRUE;
		}

		SVIBool operator != (const SVIColor& rhs) {
			return (*this == rhs) ? SVIFALSE : SVITRUE;
		}

		void filterColorRange() {
			CHECK_RANGE(mR);
			CHECK_RANGE(mG);
			CHECK_RANGE(mB);
			CHECK_RANGE(mA);
		}
	
	public:
		SVIFloat		mR;
		SVIFloat		mG;
		SVIFloat		mB;
		SVIFloat		mA;
	};


	/* SVIImage class does not take care memory handling(allocation, free).
	 * So, User should be careful to use this class.
	*/
	class SVIImage {
	public:
        struct SVICompressHeader
        {
            SVIUInt	signature;
            SVIUInt	width;
            SVIUInt	height;
            SVIUInt	format;
            //From start of header file
            SVIUInt	dataOffset;
            SVIUInt  imageSize;
        };

        //SVI Compress file signature
        static const SVIUInt COMPRESS_SIGNATURE = 0xCCC40002;

		enum ImageFormat { NONE, RGB_565, RGBA_8888, RGBA_4444, A_8 };
		enum ImageType { IMAGE_TEXT, IMAGE_RAW };
		enum AlphaType { NORMAL, PREMULTIPLIED };
        enum CompressTextureType { COMPRESS_ATC = 0, COMPRESS_PVR = 1 };
        //this same atc and imagination format
        enum CompressTextureFormat {
            ATC_RGB = 0x8C92,
            ATC_RGBA = 0x8C93
        };

		SVIImage() {
            mIsID = SVIFALSE;
            mIsCopied = SVIFALSE;
            mIsCompress = SVIFALSE;
			mBits = NULL;
			mWidth = 0;
			mHeight = 0;
			mStride = 0;
			mKey = -1;
            mTextureID = 0;
			mImageFormat = NONE;
			mImageType = IMAGE_RAW;
			mAlphaType = NORMAL;
			mLock = new SVIMutex("SVIImage");
			jbitmap = NULL;
		}

		SVIImage(const SVIImage& rhs) {
            mIsCompress = rhs.mIsCompress;
            mIsCopied = rhs.mIsCopied;
            mIsID = rhs.mIsID; 
            mTextureID = rhs.mTextureID;
			mBits = rhs.mBits;
			mWidth = rhs.mWidth;
			mHeight = rhs.mHeight;
			mStride = rhs.mStride;
			mKey = rhs.mKey;
			mImageFormat = rhs.mImageFormat;
			mImageType = rhs.mImageType;
			mAlphaType = rhs.mAlphaType;
			jbitmap = rhs.jbitmap;
		}

		
		virtual ~SVIImage() {
            mTextureID = 0;
            mIsID = SVIFALSE;
            mIsCopied = SVIFALSE;
            mIsCompress = SVIFALSE;
			mBits = NULL;
			mWidth = 0;
			mHeight = 0;
			mStride = 0;
			mKey = -1;
			mImageFormat = NONE;
			mImageType = IMAGE_RAW;
			SVI_SVIFE_DELETE(mLock);

            if (mIsCopied && mBits != NULL) {
                free(mBits);
                mBits = NULL;
                mIsCopied = SVIFALSE;
            }

            jbitmap = NULL;
        }

        void lock() { mLock->lock(); }
        void unLock() { mLock->unlock(); }
        SVIMutex* getLock() { return mLock; }

    public:
        SVIBool              mIsID;
        SVIBool              mIsCopied;
        SVIBool              mIsCompress;
        CompressTextureType mCompressType;
        SVICompressHeader    mCompressHeader;
        SVIInt			    mWidth;
        SVIInt			    mHeight;
        SVIInt			    mStride;
        SVIInt			    mKey;
        SVIUInt              mTextureID;
        ImageFormat		    mImageFormat;
        ImageType		    mImageType;
        AlphaType		    mAlphaType;
        SVIByte*			    mBits;
		
        /*
         * Holds the jni global reference to the java bitmap object.
         */
		jobject 		jbitmap;
    private:
        SVIMutex*		    mLock;
	};

	class SVISlide;
	class SVIMessage {
	public:
		enum {
			TOP = 0,
			NORMAL,
			BOTTOM
		};

		SVIMessage() {
			mSlide = NULL;
			mMessage = 0;
			mParam1 = 0;
			mParam2 = 0;
			//memset(&mPropertyParam, 0x00, sizeof(SVIFloat)*4);

			mMessageID = 0;
			mPriority = NORMAL;
		}

		SVIMessage(const SVIMessage& rhs) {
			mSlide = rhs.mSlide;
			mMessage = rhs.mMessage;
			mParam1 = rhs.mParam1;
			mParam2 = rhs.mParam2;
			//memcpy(&mPropertyParam, rhs.mPropertyParam, sizeof(SVIFloat)*4);
			mMessageID = rhs.mMessageID;
			mPriority = rhs.mPriority;
		}

		SVIBool operator < (const SVIMessage& rhs) const{
			if( mPriority < rhs.mPriority )
				return SVITRUE;
			else if( mPriority == rhs.mPriority ) {
				if( mMessageID < rhs.mMessageID )
					return SVITRUE;
			}

			return SVIFALSE;
		}

		SVIUInt getID() { return mMessageID; }
		SVIUInt getPriority() { return mPriority; }

		~SVIMessage() {}

	public:
		SVISlide*		mSlide;
		SVIInt			mMessage;
		SVIInt			mParam1;
		SVIInt			mParam2;
		//SVIFloat			mPropertyParam[4];

	private:
		SVIUInt			mMessageID;
		SVIUInt			mPriority;

	friend class SVIMessageHandler;
	};
};

#endif
