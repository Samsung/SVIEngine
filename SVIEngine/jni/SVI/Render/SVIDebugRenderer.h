#pragma  once

namespace SVI{
	
	class SVIRectEntity{
	public:
		SVIRectEntity();
		virtual ~SVIRectEntity();

	public:
		const SVIVector4& getColor(){ return mColor;}
		const SVIVector3* getRect(){ return mVertices;}
		const SVIVector3& getMax(){ return mMax;}
		const SVIVector3& getMin(){ return mMin;}

	public:
		void setMax(const SVIVector3& vMax);
		void setMin(const SVIVector3& vMin);
		void setColor(const SVIVector4& vColor);

	protected:
		SVIVector4	mColor;
		SVIVector3	mMin;
		SVIVector3	mMax;
		SVIVector3   mVertices[4];
	};

	class SVIPointEntity{
	public:
		SVIPointEntity();
		virtual ~SVIPointEntity();

	public:
		const SVIVector4& getColor(){ return mColor;}
		const SVIVector3& getPosition(){ return mPosition;}
		float getSize(){ return mSize;}

	public:
		void setPosition(const SVIVector3& vPosition);
		void setSize(float size);
		void setColor(const SVIVector4& vColor);

	protected:
		SVIFloat		mSize;
		SVIVector3	mPosition; 
		SVIVector4	mColor;
	};

	class SVILineEntity{
	public:
		SVILineEntity();
		virtual ~SVILineEntity();

	public:
		const SVIVector4&    getColor(){ return mColor;}
		const SVIVector3&    getA(){ return mLine[0];}
		const SVIVector3&    getB(){ return mLine[1];}
		const SVIVector3*    getLine(){ return mLine;}
		SVIFloat	            getWidth(){return m_Width;}

	public:
		void setWidth(const float fWidth){m_Width = fWidth;}
		void setColor(const SVIVector4& vColor);
		void setA(const SVIVector3& vA);
		void setB(const SVIVector3& vB);
		void setLine(const SVIVector3& vA,const SVIVector3& vB);

	protected:
		SVIFloat		m_Width;
		SVIVector4	mColor;
		SVIVector3	mLine[2]; 
	};

	class SVI3DCamera;
	class SVIProgramHandler;
	class SVIGLSurface;
	class SVIDebugRenderer{

	public:
		 SVIDebugRenderer(SVIGLSurface *surface);
#ifdef MULTIDRAW
		 void drawPoints(const std::vector<SVIVector3*>& vPoints, const SVIVector4& vColor);
		 void drawLines(const std::vector<SVIVector3*>& vLines, const SVIVector4& vColor);
		 void drawRects(const std::vector<SVIVector3*>& vPoints, const SVIVector4& vColor);
#endif		
		 void drawDigit(float x, float y, int lineCount, int * pPointIndices, float * points, float fScale, float r,float g, float b, float a);
		 void drawPoint(const SVIVector3& vPoint, const SVIVector4& vColor);
		 void drawLine(const SVIVector3& vA, const SVIVector3& vB, const SVIVector4& vColor);
		 void drawLine(const SVIVector3& vA, const SVIVector3& vB, const SVIVector4& vColor, SVIFloat fLineWidth);
		 void drawRect(const SVIVector3& vMin, const SVIVector3& vMax, const SVIVector4& vColor);
		
		 void drawNumeric(float x,float y, float value, SVIVector4 vColor, SVIBool isBold = SVIFALSE);
	public:
		 void render3DGrid(SVI3DCamera * pCamera, SVIFloat fGap, SVIInt step);
		 void render3DBox(SVI3DCamera * pCamera, const SVIVector3& min, const SVIVector3& max, SVIVector4 color = SVIVector4(0.0f,0.0f,0.0f,0.0f));

		 void render3DCylinder(SVI3DCamera * pCamera, const SVIVector3& center, SVIFloat radius, SVIFloat height,SVIUInt res, SVIVector4 color = SVIVector4(0.0f,0.0f,0.0f,0.0f));
		 void render3DCapsule(SVI3DCamera * pCamera, const SVIVector3& center, SVIFloat radius, SVIFloat height, SVIUInt res, SVIVector4 color = SVIVector4(0.0f,0.0f,0.0f,0.0f));
		 void render3DSphere(SVI3DCamera * pCamera, const SVIVector3& center,SVIFloat radius, SVIUInt res, SVIVector4 color = SVIVector4(0.0f,0.0f,0.0f,0.0f));

		 void render3DHemisphere(SVI3DCamera * pCamera, SVIFloat radius, SVIUInt res, const SVIVector3& center ,const SVIMatrix * pPose, SVIVector4 color = SVIVector4(0.0f,0.0f,0.0f,0.0f));
		 void render3DCircle(SVI3DCamera * pCamera, const SVIVector3& center, SVIFloat radius, SVIUInt res, SVIVector4 color = SVIVector4(0.0f,0.0f,0.0f,0.0f));
		 void render3DCylinderBody(SVI3DCamera * pCamera, const SVIVector3& center, SVIFloat radius, SVIFloat height,SVIUInt res, SVIVector4 color = SVIVector4(0.0f,0.0f,0.0f,0.0f));

		 void render3DWireSphere(SVI3DCamera * pCamera, const SVIVector3& center, SVIFloat radius, SVIUInt res, SVIVector4 color = SVIVector4(0.0f,0.0f,0.0f,0.0f));
		 void render3DWireCircle(SVI3DCamera * pCamera, const SVIVector3& center, SVIFloat radius, SVIUInt res, SVIVector4 color = SVIVector4(0.0f,0.0f,0.0f,0.0f));
	public:
		 void initialize();
		 void update();
		 void render();
		 void finalize();

	private:
		 void renderRects();
		 void renderLines();
		 void renderPoints();
		void  renderLineFPS(float fX, float fY, float fSize, float fLineWidth, float fFPS, SVIVector4 color); //Multi-Instance Support

	private:
		 std::vector<SVIRectEntity*> mRects;
		 std::vector<SVIPointEntity*> mPoints;
		 std::vector<SVILineEntity*> mLines;
		 SVIGLSurface *mSVIGLSurface;
	};

};
