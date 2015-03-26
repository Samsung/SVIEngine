#pragma once


namespace SVI{
	class SVIGLSurface;

	class SVIViewport{
	public:
		SVIViewport(SVIGLSurface* surface);
		~SVIViewport();
		void initialize();
		void initializeOrtho(SVIFloat width, SVIFloat height, SVIFloat near, SVIFloat far);
		void setOrtho(SVIFloat width, SVIFloat height, SVIFloat near, SVIFloat far, SVIBool bReverse = SVIFALSE);
		void update();
		void restore();
		void resize(SVIFloat width, SVIFloat height);
		void finalize();

	public:
		SVIMatrix& getProjection(){return mProjection;}
		SVIMatrix& getModelView(){return mModelView;}
		const SVIMatrix& getModelViewProjection(){return mModelViewProjection;}

	public:
		void setProjectionRatio(SVIFloat value);

	protected:
		SVIMatrix mProjection;
		SVIMatrix mModelView;
		SVIMatrix mModelViewProjection;
		
		//basic viewport settings
		SVIFloat mViedWidth;
		SVIFloat mViedHeight;

		SVIFloat mNear;
		SVIFloat mFar;

		SVIFloat mFieldOfView;
		SVIGLSurface *mSVIGLSurface;
	};

}