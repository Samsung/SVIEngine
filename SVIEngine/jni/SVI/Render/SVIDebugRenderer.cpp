#include "../SVICores.h"
#include "SVIDebugRenderer.h"

#include "SVI3DCamera.h"

#include "SVIViewport.h"
#include "SVIProgramManager.h"
#include "SVIRenderer.h"



namespace SVI{
	SVIRectEntity::SVIRectEntity():
	mColor(1.0f,1.0f,1.0f,1.0f),
	mMin(0.0f,0.0f,0.0f),
	mMax(1.0f,1.0f,0.0f){
		mVertices[0] = SVIVector3(mMin.x,mMin.y,0.0f);
		mVertices[1] = SVIVector3(mMin.x,mMax.y,0.0f);
		mVertices[2] = SVIVector3(mMax.x,mMax.y,0.0f);
		mVertices[3] = SVIVector3(mMax.x,mMin.y,0.0f);
	}
	SVIRectEntity::~SVIRectEntity(){}		
	
	void SVIRectEntity::setMax(const SVIVector3& vMax){
		mMax = vMax;
		mVertices[0] = SVIVector3(mMin.x,mMin.y,0.0f);
		mVertices[1] = SVIVector3(mMin.x,mMax.y,0.0f);
		mVertices[2] = SVIVector3(mMax.x,mMax.y,0.0f);
		mVertices[3] = SVIVector3(mMax.x,mMin.y,0.0f);
	}
	void SVIRectEntity::setMin(const SVIVector3& vMin){
		mMin = vMin;
		mVertices[0] = SVIVector3(mMin.x,mMin.y,0.0f);
		mVertices[1] = SVIVector3(mMin.x,mMax.y,0.0f);
		mVertices[2] = SVIVector3(mMax.x,mMax.y,0.0f);
		mVertices[3] = SVIVector3(mMax.x,mMin.y,0.0f);
	}
	void SVIRectEntity::setColor(const SVIVector4& vColor){mColor = vColor;}
	

	SVIPointEntity::SVIPointEntity():
	mColor(1.0f,1.0f,1.0f,1.0f),
	mPosition(0.0f,0.0f,0.0f),
	mSize(1.0f){}

	SVIPointEntity::~SVIPointEntity(){}
		
	void SVIPointEntity::setPosition(const SVIVector3& vPosition){
		mPosition = vPosition; 
	}
	
	void SVIPointEntity::setSize(float size){
		mSize = size;
	}

	void SVIPointEntity::setColor(const SVIVector4& vColor){
		mColor = vColor;
	}

	SVILineEntity::SVILineEntity():
		m_Width(1.0f),
		mColor(1.0f,1.0f,1.0f,1.0f){
		mLine[0] = SVIVector3();
		mLine[1] = SVIVector3();		
	}

	SVILineEntity::~SVILineEntity(){}
	
	void SVILineEntity::setColor(const SVIVector4& vColor){
		mColor = vColor;
	}

	void SVILineEntity::setA(const SVIVector3& vA){
		mLine[0]= vA;
	}

	void SVILineEntity::setB(const SVIVector3& vB){
		mLine[1]= vB;
	}

	void SVILineEntity::setLine(const SVIVector3& vA,const SVIVector3& vB){
		mLine[0]= vA;
		mLine[1]= vB;
	}

	 SVIDebugRenderer::SVIDebugRenderer(SVIGLSurface *surface) {
	 	if(surface == NULL) {
			LOGE("Invalid argument to the SVIDebugRenderer::SVIDedugRenderer(SVIGLSurface *surface) \n");
			exit(0); // To check for errors till stabilization.. To be removed later. 
		} else {
			mSVIGLSurface = surface;
		}
	 }
	
#ifdef MULTIDRAW
	void SVIDebugRenderer::drawPoints(const std::vector<SVIVector3*>& vPoints, const SVIVector4& vColor){}
	void SVIDebugRenderer::drawLines(const std::vector<SVIVector3*>& vLines, const SVIVector4& vColor){}
	void SVIDebugRenderer::drawRects(const std::vector<SVIVector3*>& vPoints, const SVIVector4& vColor){}
#endif		
	void SVIDebugRenderer::drawPoint(const SVIVector3& vPoint, const SVIVector4& vColor){
		SVIPointEntity * pPoint = new SVIPointEntity();
		pPoint->setPosition(vPoint);
		pPoint->setColor(vColor);
		mPoints.push_back(pPoint);
	}
	
	void SVIDebugRenderer::drawLine(const SVIVector3& vA, const SVIVector3& vB, const SVIVector4& vColor){
		SVILineEntity * pLine = new SVILineEntity();
		pLine->setLine(vA,vB);
		pLine->setColor(vColor);
		mLines.push_back(pLine);
	}

	void SVIDebugRenderer::drawLine(const SVIVector3& vA, const SVIVector3& vB, const SVIVector4& vColor, SVIFloat fLineWidth){
		SVILineEntity * pLine = new SVILineEntity();
		pLine->setLine(vA,vB);
		pLine->setColor(vColor);
		pLine->setWidth(fLineWidth);
		mLines.push_back(pLine);
	}

	void SVIDebugRenderer::drawRect(const SVIVector3& vMin, const SVIVector3& vMax, const SVIVector4& vColor){
		SVIRectEntity * pRect = new SVIRectEntity();
		pRect->setMin(vMin);
		pRect->setMax(vMax);
		pRect->setColor(vColor);
		mRects.push_back(pRect);
	}

	
	void SVIDebugRenderer::initialize(){}

	void SVIDebugRenderer::update(){
		finalize();	
	}

	void makeZero(int * lineCount, int * pPoints){
		*lineCount = 4;
		pPoints[0] = 0;
		pPoints[1] = 1;	
		pPoints[2] = 1;
		pPoints[3] = 5;	
		pPoints[4] = 5;
		pPoints[5] = 4;	
		pPoints[6] = 4;
		pPoints[7] = 0;
	}

	void makeOne(int * lineCount, int * pPoints){
		*lineCount = 1;
		pPoints[0] = 1;
		pPoints[1] = 5;	
	}

	void makeTwo(int * lineCount, int * pPoints){
		*lineCount = 5;
		pPoints[0] = 0;
		pPoints[1] = 1;	
		pPoints[2] = 2;
		pPoints[3] = 3;		
		pPoints[4] = 4;
		pPoints[5] = 5;			
		pPoints[6] = 1;
		pPoints[7] = 3;			
		pPoints[8] = 2;
		pPoints[9] = 4;		
	}

	void makeThree(int * lineCount, int * pPoints){
		*lineCount = 4;
		pPoints[0] = 0;
		pPoints[1] = 1;	
		pPoints[2] = 2;
		pPoints[3] = 3;		
		pPoints[4] = 4;
		pPoints[5] = 5;	
		pPoints[6] = 1;
		pPoints[7] = 5;	
	}

	void makeFour(int * lineCount, int * pPoints){
		*lineCount = 3;
		pPoints[0] = 0;
		pPoints[1] = 2;	
		pPoints[2] = 2;
		pPoints[3] = 3;		
		pPoints[4] = 1;
		pPoints[5] = 5;	
	}

	void makeFive(int * lineCount, int * pPoints){
		*lineCount = 5;
		pPoints[0] = 0;
		pPoints[1] = 1;	
		pPoints[2] = 2;
		pPoints[3] = 3;		
		pPoints[4] = 4;
		pPoints[5] = 5;	
		pPoints[6] = 0;
		pPoints[7] = 2;	
		pPoints[8] = 3;
		pPoints[9] = 5;
	}

	void makeSix(int * lineCount, int * pPoints){
		*lineCount = 5;
		pPoints[0] = 0;
		pPoints[1] = 4;	
		pPoints[2] = 0;
		pPoints[3] = 1;	
		pPoints[4] = 2;
		pPoints[5] = 3;	
		pPoints[6] = 4;
		pPoints[7] = 5;	
		pPoints[8] = 3;
		pPoints[9] = 5;
	}

	void makeSeven(int * lineCount, int * pPoints){
		*lineCount = 2;
		pPoints[0] = 0;
		pPoints[1] = 1;	
		pPoints[2] = 1;
		pPoints[3] = 5;		
	}

	void makeEight(int * lineCount, int * pPoints){
		*lineCount = 5;
		pPoints[0] = 0;
		pPoints[1] = 1;	
		pPoints[2] = 0;
		pPoints[3] = 4;	
		pPoints[4] = 4;
		pPoints[5] = 5;	
		pPoints[6] = 5;
		pPoints[7] = 1;	
		pPoints[8] = 2;
		pPoints[9] = 3;
	}

	void makeNine(int * lineCount, int * pPoints){
		*lineCount = 4;
		pPoints[0] = 0;
		pPoints[1] = 1;	
		pPoints[2] = 2;
		pPoints[3] = 3;		
		pPoints[4] = 1;
		pPoints[5] = 5;
		pPoints[6] = 0;
		pPoints[7] = 2;
	}

	void makeDot(int * lineCount, int * pPoints){
		*lineCount = 1;
		pPoints[0] = 5;
		pPoints[1] = 6;	
	}

	void makeLeft(int * lineCount, int * pPoints){
		*lineCount = 2;
		pPoints[0] = 1;
		pPoints[1] = 2;	
		pPoints[2] = 2;
		pPoints[3] = 4;
	}

	void makeRight(int * lineCount, int * pPoints){
		*lineCount = 2;
		pPoints[0] = 0;
		pPoints[1] = 3;	
		pPoints[2] = 3;	
		pPoints[3] = 4;	
	}


	void SVIDebugRenderer::drawDigit(float x, float y, int lineCount, int * pPointIndices, float * points, float fScale, float r,float g, float b, float a){
		float * vertices = NULL;	
        float * aaCoords = NULL;
        vertices = new float[lineCount*2*2];	
        aaCoords = new float[lineCount*2];	
        int verticeIndex = 0;
        for (int n = 0; n < lineCount*2; n++){
            int pointIndex = pPointIndices[n];		
            vertices[verticeIndex++] = x + points[pointIndex*2] * fScale;
            vertices[verticeIndex++] = y + points[pointIndex*2+1] * fScale * 1.3f; 
            aaCoords[n] = 1.0f;
        }


        SVIProgramParams renderParams;
        SVIProgramHandler * pHandler = NULL;
        SVIProgram * pProgram = NULL;
        renderParams.addElement(EM_COLOR);

        pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
		if (pProgram == NULL || !pProgram->getActivated()) {
			delete [] vertices;//delete the allocated memory in this case also
			delete [] aaCoords;//delete the allocated memory
			return;
		}
        pHandler = pProgram->getHandler();
        pProgram->use();

        pHandler->setAttribute(HD_A_AACOORD, 1, aaCoords);


        SVIVector4 rtColor = SVIVector4(r,g,b,a);
        pHandler->setUniform(HD_U_COLOR, rtColor);
        pHandler->setAttribute(HD_A_POSITION, 2, vertices);	

		SVIMatrix viewMat; viewMat.setIdentity();
		SVIMatrix projectionMat = mSVIGLSurface->getViewport()->getProjection();

        pHandler->setUniform(HD_U_PROJ, projectionMat);
        pHandler->setUniform(HD_U_VIEW, viewMat);

        glDrawArrays(GL_LINES, 0, lineCount*2);	

        pHandler->releaseAttribute(HD_A_POSITION);		
        pHandler->releaseAttribute(HD_A_AACOORD);

	glFinish();//Make sure vert data has been uploaded to GPU before deleting.
	delete [] vertices;
	delete [] aaCoords;
	}

	void SVIDebugRenderer::renderLineFPS(float fX, float fY, float fSize, float fLineWidth, float fFPS, SVIVector4 color ){
		float points[] = {
			0.0f,0.0f, 	//PT 0
			1.0f,0.0f, 	//PT 1
			0.0f,0.5f, 	//PT 2		
			1.0f,0.5f, 	//PT 3
			0.0f,1.0f, 	//PT 4
			1.0f,1.0f, 	//PT 5
			1.0f,0.8f, 	//PT 6
		};		




		char strFPS[64] = "";
		int pointIndex[64];
		int lineCount = 0;		
		int digitCount = 0;
		float fSideStep = 0.0f;

		//make string form..
		sprintf(strFPS,"%.02f",fFPS);
		digitCount = strlen(strFPS);

		glEnable(GL_BLEND);
		//glDisable(GL_TEXTURE_2D);    
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		
		glLineWidth(fLineWidth);    

		for (int n = 0; n < digitCount; n++){
			SVIBool isChecked = SVIFALSE;
			float stepDistance = fSize * 1.3f;
			switch (strFPS[n]){
			case '0': 
				makeZero(&lineCount,pointIndex); isChecked = true;
				break;
			case '1': 
				makeOne(&lineCount,pointIndex); isChecked = true;
				break;
			case '2': 
				makeTwo(&lineCount,pointIndex); isChecked = true;
				break;
			case '3': 
				makeThree(&lineCount,pointIndex); isChecked = true;
				break;
			case '4': 
				makeFour(&lineCount,pointIndex); isChecked = true;
				break;
			case '5': 
				makeFive(&lineCount,pointIndex); isChecked = true;
				break;
			case '6': 
				makeSix(&lineCount,pointIndex); isChecked = true;
				break;
			case '7': 
				makeSeven(&lineCount,pointIndex); isChecked = true;
				break;
			case '8': 
				makeEight(&lineCount,pointIndex); isChecked = true;
				break;
			case '9': 
				makeNine(&lineCount,pointIndex); isChecked = true;
				break;
			case '.': 
				makeDot(&lineCount,pointIndex); isChecked = true;
				stepDistance = fSize * 0.5f;
				break;
			}			

			if (isChecked){
				fSideStep += stepDistance;
				drawDigit(fX + fSideStep, fY,lineCount,pointIndex,points,fSize,color.x,color.y,color.z,color.w);
			}
		}		
		glDisable(GL_BLEND);

	}

	void SVIDebugRenderer::drawNumeric(float x,float y, float value, SVIVector4 vColor, SVIBool isBold){
		renderLineFPS(x,y,12.0f,isBold ? 3.0f : 2.0f,value,vColor);
	}

	void SVIDebugRenderer::render(){
		

		//no culling, no depth. just render over based on add ordering
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		//glDisable(GL_TEXTURE_2D);
		

		//render debug entities
		renderRects();
		renderLines();
		renderPoints();

	}

	void SVIDebugRenderer::renderRects(){
		SVIProgramParams renderParams;
		SVIProgramHandler * pHandler = NULL;
		SVIProgram * pProgram = NULL;
		renderParams.addElement(EM_COLOR);

		pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
		if (pProgram == NULL || !pProgram->getActivated()) return;
		pHandler = pProgram->getHandler();
		pProgram->use();				
		

		SVIMatrix viewMat; viewMat.setIdentity();
		SVIMatrix projectionMat = mSVIGLSurface->getViewport()->getProjection();	

		pHandler->setUniform(HD_U_PROJ, projectionMat);
		pHandler->setUniform(HD_U_VIEW, viewMat);

		for (SVIUInt n = 0; n < mRects.size(); n++){
			SVIRectEntity * pRect = mRects[n];

			SVIVector4 colorValue = pRect->getColor();
			pHandler->setUniform(HD_U_COLOR, colorValue);
			pHandler->setAttribute(HD_A_POSITION, (SVIVector3*)pRect->getRect());	
			glDrawArrays( GL_TRIANGLE_FAN, 0, 4);			
		}

		pHandler->releaseAttribute(HD_A_POSITION);

	}

	void SVIDebugRenderer::renderLines(){
		SVIProgramParams renderParams;
		SVIProgramHandler * pHandler = NULL;
		SVIProgram * pProgram = NULL;
		renderParams.addElement(EM_COLOR);

		pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
		if (pProgram == NULL || !pProgram->getActivated()) return;
		pHandler = pProgram->getHandler();
		pProgram->use();	

		SVIMatrix viewMat; viewMat.setIdentity();
		SVIMatrix projectionMat = mSVIGLSurface->getViewport()->getProjection();	

		pHandler->setUniform(HD_U_PROJ, projectionMat);
		pHandler->setUniform(HD_U_VIEW, viewMat);

		for (SVIUInt n = 0; n < mLines.size(); n++){
			SVILineEntity * pLine = mLines[n];
			SVIVector4 colorValue = pLine->getColor();
			glLineWidth(pLine->getWidth());
			pHandler->setUniform(HD_U_COLOR, colorValue);
			pHandler->setAttribute(HD_A_POSITION, (SVIVector3*)pLine->getLine());	
			glDrawArrays( GL_LINES, 0, 2);
		}

		pHandler->releaseAttribute(HD_A_POSITION);	

	}


	void SVIDebugRenderer::render3DGrid(SVI3DCamera * pCamera, SVIFloat fGap, SVIInt step){
		SVIProgramParams renderParams;
		SVIProgramHandler * pHandler = NULL;
		SVIProgram * pProgram = NULL;
		renderParams.addElement(EM_COLOR);

		pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
		if (pProgram == NULL || !pProgram->getActivated()) return;
		pHandler = pProgram->getHandler();
		pProgram->use();	

		LOGE("3D: Set View Projection Matrix");
		SVIMatrix viewMat = pCamera->getViewMatrix();
		SVIMatrix projectionMat = pCamera->getProjMatrix();	

		pHandler->setUniform(HD_U_PROJ, projectionMat);
		pHandler->setUniform(HD_U_VIEW, viewMat);

		SVIVector4 color = SVIVector4(0.2f,0.2f,0.2f,1.0f);
		glLineWidth(1.0f);

		SVIInt index = 0;
		SVIInt cVerticesCount = ((step*2+1)*2);
		std::vector<SVIVector3> vertices;
		vertices.resize(cVerticesCount);
		
		LOGE("3D: render y lines of grid");
		//render y lines of grid
		for (int y=-step; y<=step; ++y){
			vertices[index++] = SVIVector3( y*fGap, 0, -(fGap*step));
			vertices[index++] =  SVIVector3( y*fGap, 0, (fGap*step));
		}

		pHandler->setUniform(HD_U_COLOR, color);
		pHandler->setAttribute(HD_A_POSITION, (SVIVector3*)&(*vertices.begin()));	
		glDrawArrays( GL_LINES, 0, vertices.size() );
		
		LOGE("3D: render x lines of grid");
		//render x lines of grid
		index = 0;
		for (int x=-step; x<=step; ++x){
			vertices[index++] = SVIVector3( -(fGap*step), 0.0f, x*fGap );
			vertices[index++] = SVIVector3( (fGap*step), 0.0f, x*fGap );
		}		
		pHandler->setAttribute(HD_A_POSITION, (SVIVector3*)&(*vertices.begin()));	
		glDrawArrays( GL_LINES, 0, vertices.size() );

		LOGE("3D: render main axises");
		//render main axises
		glLineWidth(3.0f);
		index = 0;
		vertices.resize(6);
		vertices[index++] = SVIVector3( 0.0f, 0.0f, 0.0f );
		vertices[index++] = SVIVector3(  (fGap*step), 0.0f, 0.0f );
		vertices[index++] = SVIVector3(  0.0f, 0.0f, 0.0f );
		vertices[index++] = SVIVector3(	0.0f, 0.0f,  (fGap*step) );
		vertices[index++] = SVIVector3(  0.0f, fGap*step * 0.5f, 0.0f );
		vertices[index++] = SVIVector3(	0.0f, 0.0f, 0.0f );

		color = SVIVector4(1.0f,1.0f,1.0f,1.0f);
		pHandler->setUniform(HD_U_COLOR, color);
		pHandler->setAttribute(HD_A_POSITION, (SVIVector3*)&(*vertices.begin()));	
		glDrawArrays( GL_LINES, 0, vertices.size() );
		
		pHandler->releaseAttribute(HD_A_POSITION);	
	}


	void SVIDebugRenderer::render3DBox(SVI3DCamera * pCamera, const SVIVector3& cMin, const SVIVector3& cMax, SVIVector4 color){
		SVIVector3 pVb[36] = {
			SVIVector3( cMax.x, cMax.y, cMax.z),
			SVIVector3( cMax.x, cMax.y, cMin.z),
			SVIVector3( cMin.x, cMax.y, cMin.z),

			SVIVector3( cMin.x, cMax.y, cMin.z),
			SVIVector3( cMin.x, cMax.y, cMax.z),
			SVIVector3( cMax.x, cMax.y, cMax.z),

			SVIVector3( cMax.x, cMin.y, cMin.z),
			SVIVector3( cMax.x, cMin.y, cMax.z),
			SVIVector3( cMin.x, cMin.y, cMin.z),

			SVIVector3( cMin.x, cMin.y, cMax.z),
			SVIVector3( cMin.x, cMin.y, cMin.z),
			SVIVector3( cMax.x, cMin.y, cMax.z),

			SVIVector3( cMax.x, cMax.y, cMin.z),
			SVIVector3( cMax.x, cMin.y, cMin.z),
			SVIVector3( cMin.x, cMin.y, cMin.z),

			SVIVector3( cMin.x, cMin.y, cMin.z),
			SVIVector3( cMin.x, cMax.y, cMin.z),
			SVIVector3( cMax.x, cMax.y, cMin.z),

			SVIVector3( cMax.x, cMin.y, cMax.z),
			SVIVector3( cMax.x, cMax.y, cMax.z),
			SVIVector3( cMin.x, cMin.y, cMax.z),

			SVIVector3( cMin.x, cMax.y, cMax.z),
			SVIVector3( cMin.x, cMin.y, cMax.z),
			SVIVector3( cMax.x, cMax.y, cMax.z),

			SVIVector3( cMin.x, cMax.y, cMin.z),
			SVIVector3( cMin.x, cMin.y, cMin.z),
			SVIVector3( cMin.x, cMin.y, cMax.z),

			SVIVector3( cMin.x, cMax.y, cMin.z),
			SVIVector3( cMin.x, cMin.y, cMax.z),
			SVIVector3( cMin.x, cMax.y, cMax.z),

			SVIVector3( cMax.x, cMin.y, cMin.z),
			SVIVector3( cMax.x, cMax.y, cMin.z),
			SVIVector3( cMax.x, cMin.y, cMax.z),

			SVIVector3( cMax.x, cMin.y, cMax.z),
			SVIVector3( cMax.x, cMax.y, cMin.z),
			SVIVector3( cMax.x, cMax.y, cMax.z)
		};

		SVIProgramParams renderParams;
		SVIProgramHandler * pHandler = NULL;
		SVIProgram * pProgram = NULL;
		renderParams.addElement(EM_COLOR);

		pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
		if (pProgram == NULL || !pProgram->getActivated()) return;
		pHandler = pProgram->getHandler();
		pProgram->use();	

		SVIMatrix viewMat = pCamera->getViewMatrix();
		SVIMatrix projectionMat = pCamera->getProjMatrix();	

		pHandler->setUniform(HD_U_PROJ, projectionMat);
		pHandler->setUniform(HD_U_VIEW, viewMat);

		pHandler->setUniform(HD_U_COLOR, color);

		pHandler->setAttribute(HD_A_POSITION, (SVIVector3*)pVb);	
		glDrawArrays( GL_TRIANGLES, 0, 12 );
		pHandler->releaseAttribute(HD_A_POSITION);	
	}

	void SVIDebugRenderer::render3DWireSphere(SVI3DCamera * pCamera, const SVIVector3& center, SVIFloat radius, SVIUInt res, SVIVector4 color){
		if(res<=0) return;
		SVIVector3 posTemp;
		// XY
		SVIFloat fBaseAngle = 0;
		SVIFloat fDiffAngle = SVI_2_PI/SVIFloat(res*4);
		std::vector<SVIVector3> vertices; 
		vertices.resize(res*4 + 1);

		SVIProgramParams renderParams;
		SVIProgramHandler * pHandler = NULL;
		SVIProgram * pProgram = NULL;
		renderParams.addElement(EM_COLOR);

		pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
		if (pProgram == NULL || !pProgram->getActivated()) return;
		pHandler = pProgram->getHandler();
		pProgram->use();	

		SVIMatrix viewMat = pCamera->getViewMatrix();
		SVIMatrix projectionMat = pCamera->getProjMatrix();	

		pHandler->setUniform(HD_U_PROJ, projectionMat);
		pHandler->setUniform(HD_U_VIEW, viewMat);

		pHandler->setUniform(HD_U_COLOR, color);

		posTemp.z = center.z;
		for(SVIUInt i = 0; i < res * 4 + 1; ++i)
		{
			if(i == res * 4) fBaseAngle = 0;
			posTemp.x = center.x + radius * sinf(fBaseAngle);
			posTemp.y = center.y + radius * cosf(fBaseAngle);
			vertices[i] = posTemp;
			fBaseAngle += fDiffAngle;
		}
		pHandler->setAttribute(HD_A_POSITION, (SVIVector3*)&(*vertices.begin()));	
		glDrawArrays( GL_LINE_STRIP, 0, vertices.size() );

		fBaseAngle = 0;
		posTemp.y = center.y;
		for(SVIUInt i = 0; i < res * 4 + 1; ++i)
		{
			if(i == res * 4) fBaseAngle = 0;
			
			posTemp.x = center.x + radius * sinf(fBaseAngle);
			posTemp.z = center.z + radius * cosf(fBaseAngle);
			vertices[i] = posTemp;
			fBaseAngle += fDiffAngle;
		}
		pHandler->setAttribute(HD_A_POSITION, (SVIVector3*)&(*vertices.begin()));	
		glDrawArrays( GL_LINE_STRIP, 0, vertices.size() );

		fBaseAngle = 0;
		posTemp.x = center.x;
		for(SVIUInt i = 0; i < res * 4 + 1; ++i)
		{
			if(i == res * 4) fBaseAngle = 0;
			posTemp.y = center.y + radius * sinf(fBaseAngle);
			posTemp.z = center.z + radius  * cosf(fBaseAngle);
			vertices[i] = posTemp;
			fBaseAngle += fDiffAngle;
		}
		pHandler->setAttribute(HD_A_POSITION, (SVIVector3*)&(*vertices.begin()));	
		glDrawArrays( GL_LINE_STRIP, 0, vertices.size() );

		pHandler->releaseAttribute(HD_A_POSITION);	
	}
	
	void SVIDebugRenderer::render3DSphere(SVI3DCamera * pCamera, const SVIVector3& center,SVIFloat radius, SVIUInt res, SVIVector4 color){
		SVIMatrix Pose;
		render3DHemisphere(pCamera,radius, res, center,&Pose, color);
		Pose.setEuler(SVIVector3(0.f,SVI_PI,0.f));
		render3DHemisphere(pCamera,radius, res, center,&Pose, color);
	}

	void SVIDebugRenderer::render3DHemisphere(SVI3DCamera * pCamera, SVIFloat radius, SVIUInt res, const SVIVector3& center, const SVIMatrix * pPose, SVIVector4 color){
		if(res<=0) return;
		std::vector<SVIVector3> pVb;
		SVIFloat fAngle = SVI_2_PI/SVIFloat(res*4);
		SVIFloat fSinAngle = sinf(fAngle);
		SVIFloat fCosAngle = cosf(fAngle);
		SVIFloat fStartNx=0.f, fStartNy=-1.f;

		for(SVIUInt i=0; i<res; ++i) {
			SVIFloat fStartNx2 =  fCosAngle*fStartNx + fSinAngle*fStartNy;
			SVIFloat fStartNy2 = -fSinAngle*fStartNx + fCosAngle*fStartNy;
			SVIFloat fX = fStartNx, fY = fStartNy, fZ = 0.f;
			SVIFloat fX2 = fStartNx2, fY2 = fStartNy2, fZ2 = 0.f;
			for(SVIUInt j=0; j<=res*4; ++j) {
				SVIVector3 Point1(fY2*radius, fZ2*radius,fX2*radius);
				SVIVector3 Point2(fY*radius, fZ*radius,fX*radius);
				if(pPose) {
					Point1 = pPose->transform(Point1);
					Point2 = pPose->transform(Point2);
				}

				Point1 += center;
				Point2 += center;

				pVb.push_back(Point2);
				pVb.push_back(Point1);

				SVIFloat fTemp = fCosAngle * fY - fSinAngle*fZ;
				fZ = fSinAngle*fY + fCosAngle*fZ;
				fY = fTemp;
				fTemp = fCosAngle*fY2 - fSinAngle*fZ2;
				fZ2 = fSinAngle*fY2 + fCosAngle*fZ2;
				fY2 = fTemp;
			}
			fStartNx = fStartNx2;
			fStartNy = fStartNy2;
		}

		SVIUInt nSize=pVb.size();
		SVIUInt nOffset=0;

		SVIProgramParams renderParams;
		SVIProgramHandler * pHandler = NULL;
		SVIProgram * pProgram = NULL;
		renderParams.addElement(EM_COLOR);

		pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
		if (pProgram == NULL || !pProgram->getActivated()) return;
		pHandler = pProgram->getHandler();
		pProgram->use();	

		SVIMatrix viewMat = pCamera->getViewMatrix();
        SVIMatrix projectionMat = pCamera->getProjMatrix();	

        pHandler->setUniform(HD_U_PROJ, projectionMat);
        pHandler->setUniform(HD_U_VIEW, viewMat);
        pHandler->setUniform(HD_U_COLOR, color);

        for(SVIUInt i=0; i<res; ++i) { 
            pHandler->setAttribute(HD_A_POSITION, (SVIVector3*)(&(*pVb.begin()) + nOffset));	
            glDrawArrays( GL_TRIANGLE_STRIP, 0, res*4*2 );
            nOffset+=(res*4*2)+2;
        }

        pHandler->releaseAttribute(HD_A_POSITION);	
	}


	void SVIDebugRenderer::render3DCylinder(SVI3DCamera * pCamera, const SVIVector3& center, SVIFloat radius, SVIFloat height,SVIUInt res, SVIVector4 color){
		render3DCylinderBody(pCamera, center, radius, height, res, color);
	
		SVIVector3 newCenter = center;

		newCenter.y += height;
		render3DCircle(pCamera, newCenter, radius, res, color);
		newCenter.y -= height;
		render3DCircle(pCamera, newCenter, radius, res, color);
	}
	
	void SVIDebugRenderer::render3DCapsule(SVI3DCamera * pCamera, const SVIVector3& center, SVIFloat radius, SVIFloat height, SVIUInt res, SVIVector4 color){
		render3DCylinderBody(pCamera, center, radius, height, res, color);
		SVIMatrix Pose;
		SVIVector3 newCenter = center;
		newCenter.y += height;
		render3DHemisphere(pCamera,radius, res, newCenter,&Pose, color);
		Pose.setEuler(SVIVector3(0.f,SVI_PI,0.f));
		newCenter.y -= height;
		render3DHemisphere(pCamera,radius, res, newCenter,&Pose, color);
	}

	void SVIDebugRenderer::render3DWireCircle(SVI3DCamera * pCamera, const SVIVector3& center, SVIFloat radius, SVIUInt res, SVIVector4 color){
		SVIProgramParams renderParams;
		SVIProgramHandler * pHandler = NULL;
		SVIProgram * pProgram = NULL;
		renderParams.addElement(EM_COLOR);

		pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
		if (pProgram == NULL || !pProgram->getActivated()) return;
		pHandler = pProgram->getHandler();
		pProgram->use();	

		SVIMatrix viewMat = pCamera->getViewMatrix();
		SVIMatrix projectionMat = pCamera->getProjMatrix();	

		pHandler->setUniform(HD_U_PROJ, projectionMat);
		pHandler->setUniform(HD_U_VIEW, viewMat);

		std::vector<SVIVector3> vertices;

		SVIFloat fStep = SVI_2_PI/res;
		vertices.resize(res+1);
		for(SVIUInt i=0; i<=res; ++i) {
			SVIFloat fAngle = i*fStep;
			vertices[i] = SVIVector3(center.x + radius*sinf(fAngle), center.y + 0.0f, center.z + radius*cosf(fAngle));
		}

		pHandler->setUniform(HD_U_COLOR, color);
		pHandler->setAttribute(HD_A_POSITION, (SVIVector3*)&(*vertices.begin()));	
		glDrawArrays( GL_LINE_STRIP, 0, vertices.size()-1 );

		pHandler->releaseAttribute(HD_A_POSITION);	
	}

	void SVIDebugRenderer::render3DCircle(SVI3DCamera * pCamera, const SVIVector3& center, SVIFloat radius, SVIUInt res, SVIVector4 color){
		SVIProgramParams renderParams;
		SVIProgramHandler * pHandler = NULL;
		SVIProgram * pProgram = NULL;
		renderParams.addElement(EM_COLOR);

		pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
		if (pProgram == NULL || !pProgram->getActivated()) return;
		pHandler = pProgram->getHandler();
		pProgram->use();	

		SVIMatrix viewMat = pCamera->getViewMatrix();
		SVIMatrix projectionMat = pCamera->getProjMatrix();	

		pHandler->setUniform(HD_U_PROJ, projectionMat);
		pHandler->setUniform(HD_U_VIEW, viewMat);

		std::vector<SVIVector3> vertices;

		if(res<=2) return;
		SVIFloat fAngle = SVI_2_PI/SVIFloat(res);
		SVIFloat fSinAngle = sinf(fAngle);
		SVIFloat fCosAngle = cosf(fAngle);
		SVIFloat fY=1.f, fZ=0.f;

		for(SVIUInt i=0; i<=res; ++i) {
			SVIVector3 Point(center.x + fY*radius, center.y, center.z + fZ*radius);
			vertices.push_back(Point);
			if(i!=0) vertices.push_back(center);
			SVIFloat fTemp = fCosAngle*fY - fSinAngle*fZ;
			fZ = fSinAngle*fY + fCosAngle*fZ;
			fY = fTemp;
		}

		pHandler->setUniform(HD_U_COLOR, color);
		pHandler->setAttribute(HD_A_POSITION, (SVIVector3*)&(*vertices.begin()));	
		glDrawArrays( GL_TRIANGLE_STRIP, 0, vertices.size() );
		
		pHandler->releaseAttribute(HD_A_POSITION);	
	}
	
	void SVIDebugRenderer::render3DCylinderBody(SVI3DCamera * pCamera, const SVIVector3& center, SVIFloat radius, SVIFloat height,SVIUInt res, SVIVector4 color){
		if(res<=2) return;
		SVIFloat fAngle = SVI_2_PI/SVIFloat(res);
		SVIFloat fSinAngle = sinf(fAngle);
		SVIFloat fCosAngle = cosf(fAngle);
		SVIFloat fY=1.f, fZ=0.f;
		std::vector<SVIVector3> vertices;

		for(SVIUInt i=0; i<=res; ++i) {

			vertices.push_back(SVIVector3(center.x + fY*radius, center.y + height, center.z + fZ*radius));
			vertices.push_back(SVIVector3(center.x + fY*radius, center.y -height, center.z + fZ*radius));
			SVIFloat fTemp = fCosAngle*fY - fSinAngle*fZ;
			fZ = fSinAngle*fY + fCosAngle*fZ;
			fY = fTemp;
		}

		SVIProgramParams renderParams;
		SVIProgramHandler * pHandler = NULL;
		SVIProgram * pProgram = NULL;
		renderParams.addElement(EM_COLOR);

		pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
		if (pProgram == NULL || !pProgram->getActivated()) return;
		pHandler = pProgram->getHandler();
		pProgram->use();	

		SVIMatrix viewMat = pCamera->getViewMatrix();
		SVIMatrix projectionMat = pCamera->getProjMatrix();	

		pHandler->setUniform(HD_U_PROJ, projectionMat);
		pHandler->setUniform(HD_U_VIEW, viewMat);

		pHandler->setUniform(HD_U_COLOR, color);
		pHandler->setAttribute(HD_A_POSITION, (SVIVector3*)&(*vertices.begin()));	
		glDrawArrays( GL_TRIANGLE_STRIP, 0, res * 2 );
		pHandler->releaseAttribute(HD_A_POSITION);	
	}


    void SVIDebugRenderer::renderPoints(){
        SVIProgramParams renderParams;
        SVIProgramHandler * pHandler = NULL;
        SVIProgram * pProgram = NULL;
        renderParams.addElement(EM_COLOR);

		pProgram = mSVIGLSurface->getProgramManager()->getProgram(&renderParams);
        if (pProgram == NULL || !pProgram->getActivated()) return;
        pHandler = pProgram->getHandler();
        pProgram->use();	

		SVIMatrix viewMat; viewMat.setIdentity();
		SVIMatrix projectionMat =  mSVIGLSurface->getViewport()->getProjection();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

        glLineWidth(4);    

        float * vertices = NULL;	
        float * aaCoords = NULL;
        vertices = new float[mPoints.size()*2];	
        aaCoords = new float[mPoints.size()];	
        int verticeIndex = 0;
        for (int n = 0; n < mPoints.size(); n++){
            SVIPointEntity * pPoint = mPoints[n];
            vertices[verticeIndex++] = pPoint->getPosition().x;
            vertices[verticeIndex++] = pPoint->getPosition().y; 
            aaCoords[n] = 1.0f;
        }

        SVIVector4 colorValue(1.0f,1.0f,1.0f,1.0f);
        pHandler->setUniform(HD_U_COLOR, colorValue);

        pHandler->setAttribute(HD_A_AACOORD, 1, aaCoords);
        pHandler->setAttribute(HD_A_POSITION, 2, vertices);	

        pHandler->setUniform(HD_U_PROJ, projectionMat);
        pHandler->setUniform(HD_U_VIEW, viewMat);        

        glDrawArrays( GL_TRIANGLE_STRIP, 0, mPoints.size());

        pHandler->releaseAttribute(HD_A_POSITION);	
        pHandler->releaseAttribute(HD_A_AACOORD);

        delete [] vertices;
        delete [] aaCoords;

        glDisable(GL_BLEND);
    }

	void SVIDebugRenderer::finalize(){
		for (SVIUInt n = 0; n < mRects.size(); n++){
			SVI_SVIFE_DELETE(mRects[n]);
		}
		mRects.clear();

		for (SVIUInt n = 0; n < mLines.size(); n++){
			SVI_SVIFE_DELETE(mLines[n]);
		}
		mLines.clear();

		for (SVIUInt n = 0; n < mPoints.size(); n++){
			SVI_SVIFE_DELETE(mPoints[n]);
		}
		mPoints.clear();
	}
};

