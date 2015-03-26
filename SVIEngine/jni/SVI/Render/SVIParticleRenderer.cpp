
#define SGI_BASE

#define GLES20
//#define GL30

#ifdef SGI_BASE
#include "../SVICores.h"
#include "SVITexture.h"
#include "SVITextureManager.h"
#endif

#include "../Particle/SVIParticles.h"
#include "../Particle/SVIParticleEffect.h"
#include "../Particle/SVIParticleEffectManager.h"

#include "SVIParticleRenderer.h"

#define SVI_PARTICLE_COUNT		500

namespace SVI {

	SVIParticleCache::SVIParticleCache(){
		m_pPosition = NULL;
		m_pColor = NULL;
		m_pScale = NULL;
		m_pRotation = NULL;
		m_Distance = 0.0f;
	}
	SVIParticleCache::~SVIParticleCache(){
		m_pPosition = NULL;
		m_pColor = NULL;
		m_pScale = NULL;
		m_pRotation = NULL;
		m_Distance = 0.0f;
	}

	void SVIParticleCache::setDistance(SVIFloat fDistance){
		m_Distance = fDistance;
	}
	
	void SVIParticleCache::setPosition(SVIVector3* pPosition){
		m_pPosition = pPosition;
	}

	void SVIParticleCache::setColor(SVIColor* pColor){
		m_pColor = pColor;
	}

	void SVIParticleCache::setScale(SVIVector3* pScale){
		m_pScale = pScale;
	}

	void SVIParticleCache::setRotation(SVIVector3* pRotation){
		m_pRotation = pRotation;
	}
	
	SVIVector3 * SVIParticleCache::getPosition(){ 
		return m_pPosition; 
	}

	SVIColor * SVIParticleCache::getColor(){
		return m_pColor;
	}
	
	SVIVector3 * SVIParticleCache::getScale(){ 
		return m_pScale; 
	}

	SVIVector3 * SVIParticleCache::getRotation(){ 
		return m_pRotation; 
	}

	SVIFloat		SVIParticleCache::getDistance(){ 
		return m_Distance; 
	}

	//2012-06-01 masterkeaton27@gmail.com
	//Cache for 1 Particle Effect (1 Texture Only)

	SVIParticleEffectCache::SVIParticleEffectCache(){
		m_uTextureIndex = 0;

		m_pVertexBuffer = NULL;
		m_pTexcoordBuffer = NULL;
		m_pIndexBuffer = NULL;
		m_pViewMatrix = NULL;
		m_WorldMatrix.setIdentity();

		mVertexCount = 0;
		mTexcoordCount = 0;
		mIndexCount = 0;

		m_Color.r = 1.0f;
		m_Color.g = 1.0f;
		m_Color.b = 1.0f;
		m_Color.a = 1.0f;

		m_bHasTexture = SVIFALSE;
		m_bIsAddBlend = SVITRUE;
		m_pColorBuffer =NULL;
		m_uCurrentMemoryUsage =0;
	}

	SVIParticleEffectCache::~SVIParticleEffectCache(){
		clearAllCache();
	}

	void SVIParticleEffectCache::addParticleCache(SVIParticleCache * pCache){
		m_uCurrentMemoryUsage += sizeof(SVIParticleCache);

		//calculrate distance
		SVIVector3 vCameraDirection = SVIVector3(0.0f,0.0f,-1.0f);
		SVIFloat fDistance = pCache->getPosition()->dot(vCameraDirection);
		pCache->setDistance(fDistance);

		mParticleCaches.push_back(pCache);
	}

	void SVIParticleEffectCache::clearAllCache(){
		for (SVIUInt n = 0; n < mParticleCaches.size(); n++){
			SVIParticleCache * pCache = mParticleCaches[n];
			SVI_SVIFE_DELETE(pCache);
			m_uCurrentMemoryUsage -= sizeof(SVIParticleCache);
		}
		mParticleCaches.clear();
	}

	void SVIParticleEffectCache::build(){


		if (m_pVertexBuffer == NULL) {
			LOGE("m_pVertexBuffer is NULL");
			return;
		}
		if (mParticleCaches.empty()) return;


		sortParticleCaches();
		buildBuffer();
	}
				
	void SVIParticleEffectCache::setTextureIndex(SVIUInt uTextureIndex){
		m_uTextureIndex = uTextureIndex;
	}

	void SVIParticleEffectCache::setVertexBuffer(SVIVector3 * pBuffer){
		m_pVertexBuffer = pBuffer;
	}

	void SVIParticleEffectCache::setColorBuffer(SVIVector4 * pBuffer){
		m_pColorBuffer = pBuffer;
	}

	void SVIParticleEffectCache::setTexcoordBuffer(SVIVector2 * pBuffer){
		m_pTexcoordBuffer = pBuffer;
	}

	void SVIParticleEffectCache::setIndexBuffer(SVIUShort * pBuffer){
		m_pIndexBuffer = pBuffer;
	}
	
	SVIUInt	SVIParticleEffectCache::getTextureIndex(){
		return m_uTextureIndex;
	}

	SVIVector3 *	SVIParticleEffectCache::getVertexBuffer(){
		return m_pVertexBuffer;
	}

	SVIVector4 *	SVIParticleEffectCache::getColorBuffer(){
		return m_pColorBuffer;
	}

	SVIVector2 *	SVIParticleEffectCache::getTexcoordBuffer(){
		return m_pTexcoordBuffer;
	}

	SVIUShort	* SVIParticleEffectCache::getIndexBuffer(){
		return m_pIndexBuffer;
	}

	SVIUInt SVIParticleEffectCache::getMemoryUsage(){
		return m_uCurrentMemoryUsage;
	}

	SVIUInt SVIParticleEffectCache::getParticleCount(){
		SVIUInt particleCount = mParticleCaches.size();
		return particleCount;
	}

	SVIUInt	SVIParticleEffectCache::getVertexBufferSize(){
		return getParticleCount() * 4 * 3;
	}

	SVIUInt	SVIParticleEffectCache::getColorBufferSize(){
		return getParticleCount() * 4 * 4;
	}

	SVIUInt	SVIParticleEffectCache::getTexcoordBufferSize(){
		return getParticleCount() * 4 * 2;
	}

	SVIUInt	SVIParticleEffectCache::getIndexBufferSize(){
		return getParticleCount() * 2 * 6;
	}
		
	void SVIParticleEffectCache::buildBuffer(){
		buildBuffer(SVITRUE,SVITRUE);
	}

	void SVIParticleEffectCache::setColor(SVIVector4 color){
		m_Color.r = color.r;
		m_Color.g = color.g;
		m_Color.b = color.b;
		m_Color.a = color.a;
	}

	void SVIParticleEffectCache::setColor(SVIFloat  r, SVIFloat g, SVIFloat b, SVIFloat a){
		m_Color.r = r;
		m_Color.g = g;
		m_Color.b = b;
		m_Color.a = a;
	}

	void SVIParticleEffectCache::setHasTexture(SVIBool hasTexture){
		m_bHasTexture = hasTexture;
	}

	void SVIParticleEffectCache::setViewMatrix(SVIMatrix * pViewMatrix){
		m_pViewMatrix = pViewMatrix;
	}

	void SVIParticleEffectCache::setWorldMatrix(SVIMatrix & pWorldMatrix){
		m_WorldMatrix = pWorldMatrix;
	}

	void SVIParticleEffectCache::setAddBlend(SVIBool isAddBlend){
		m_bIsAddBlend = isAddBlend;
	}

	SVIVector4 &SVIParticleEffectCache::getColor(){
		return m_Color;
	}

	SVIBool SVIParticleEffectCache::getAddBlend(){
		return m_bIsAddBlend;
	}

	SVIBool SVIParticleEffectCache::getHasTexture(){
		return m_bHasTexture;
	}

	SVIMatrix * SVIParticleEffectCache::getWorldMatrix(){
		return &m_WorldMatrix;
	}

	int compareParticles(SVIParticleCache * lhs, SVIParticleCache * rhs){
		return lhs->getDistance() > rhs->getDistance() ? 1 : 0;
	}

	void SVIParticleEffectCache::sortParticleCaches(){
		std::sort(mParticleCaches.begin(),mParticleCaches.end(),compareParticles);
	}

	void SVIParticleEffectCache::buildBuffer(SVIBool isUsingUV, SVIBool isUsingIndex){
		
		SVIUInt particleCount = mParticleCaches.size();
		SVIUInt vbSize = particleCount * 4;
		SVIUInt tbSize = particleCount * 4;
		SVIUInt ibSize = particleCount * 6;

		SVIVector3 * pVB = &m_pVertexBuffer[0];
		SVIVector4 * pCB = &m_pColorBuffer[0];
		SVIVector2 * pTB = &m_pTexcoordBuffer[0];
		SVIUShort * pIB = &m_pIndexBuffer[0];

		SVIMatrix matWorld;
		matWorld.setIdentity();

		SVIMatrix matView;

		if (m_pViewMatrix != NULL){
			matView = *m_pViewMatrix;
		}else matView.setIdentity();

		matView.invert();

		SVIVector3 vXAxis;
		SVIVector3 vYAxis;
		
		vXAxis.x = matView._11; 
		vXAxis.y = matView._12;
		vXAxis.z = matView._13;

		vYAxis.x = matView._21;
		vYAxis.y = matView._22;
		vYAxis.z = matView._23;

		SVIUInt vertexCount = 0;

		for (SVIUInt n = 0; n < particleCount; n++){
			SVIParticleCache * pParticleCache = mParticleCaches[n];
			SVIVector3 * pPosition = pParticleCache->getPosition();
			SVIColor * pColor = pParticleCache->getColor();
			SVIVector3 * pSize = pParticleCache->getScale();
						
			//Vertex Buffer Generation
			pVB->x = pPosition->x - vXAxis.x * pSize->x + vYAxis.x * pSize->y;
			pVB->y = pPosition->y - vXAxis.y * pSize->x + vYAxis.y * pSize->y;
			pVB->z = pPosition->z - vXAxis.z * pSize->x + vYAxis.z * pSize->y; pVB++;
			pVB->x = pPosition->x + vXAxis.x * pSize->x + vYAxis.x * pSize->y;
			pVB->y = pPosition->y + vXAxis.y * pSize->x + vYAxis.y * pSize->y;
			pVB->z = pPosition->z + vXAxis.z * pSize->x + vYAxis.z * pSize->y; pVB++;
			pVB->x = pPosition->x + vXAxis.x * pSize->x - vYAxis.x * pSize->y;
			pVB->y = pPosition->y + vXAxis.y * pSize->x - vYAxis.y * pSize->y;
			pVB->z = pPosition->z + vXAxis.z * pSize->x - vYAxis.z * pSize->y; pVB++;
			pVB->x = pPosition->x - vXAxis.x * pSize->x - vYAxis.x * pSize->y;
			pVB->y = pPosition->y - vXAxis.y * pSize->x - vYAxis.y * pSize->y;
			pVB->z = pPosition->z - vXAxis.z * pSize->x - vYAxis.z * pSize->y; pVB++;

			//Color Buffer Generation
			pCB->r = pColor->mR; pCB->g = pColor->mG; pCB->b = pColor->mB; pCB->a = pColor->mA; pCB++;
			pCB->r = pColor->mR; pCB->g = pColor->mG; pCB->b = pColor->mB; pCB->a = pColor->mA; pCB++;
			pCB->r = pColor->mR; pCB->g = pColor->mG; pCB->b = pColor->mB; pCB->a = pColor->mA; pCB++;
			pCB->r = pColor->mR; pCB->g = pColor->mG; pCB->b = pColor->mB; pCB->a = pColor->mA; pCB++;

			if (isUsingUV){
				//Texture Buffer Generation
				pTB->x = 0.0f; pTB->y = 0.0f; pTB++;
				pTB->x = 1.0f; pTB->y = 0.0f; pTB++;
				pTB->x = 1.0f; pTB->y = 1.0f; pTB++;
				pTB->x = 0.0f; pTB->y = 1.0f; pTB++;
			}

			if (isUsingIndex){
				//Index Buffer Generation
				*pIB = vertexCount + 0; pIB++;
				*pIB = vertexCount + 1; pIB++;
				*pIB = vertexCount + 2; pIB++;
				*pIB = vertexCount + 0; pIB++;
				*pIB = vertexCount + 2; pIB++;
				*pIB = vertexCount + 3; pIB++;
			}

			vertexCount += 4;
		}
	}
	

	void SVIParticleRenderer::initialize(){
		initialize(SVI_PARTICLE_COUNT);
	}

	void SVIParticleRenderer::update(){
		/*LOGE("------------------------------------------------------");
		LOGE("Particle Renderer : Current Particle Count <%d>",m_uCurrentParticleCount);
		LOGE("Particle Renderer : Current Memory Usage <%.02f KB>", (SVIFloat)m_uCurrentMemoryUsage / 1024.0f);
		LOGE("------------------------------------------------------");*/
		m_uCurrentParticleCount = 0;
	}

	void SVIParticleRenderer::initialize(SVIUInt maxParticleCount){
        SVIUInt uVertexCount = maxParticleCount * 4;
        SVIUInt uTexcoordCount = maxParticleCount * 4;
        SVIUInt uColorCount = maxParticleCount * 4;
        SVIUInt uIndexCount = maxParticleCount * 6;

        SVI_SVIFE_DELETE_ARRAY(m_pReservedPositionBuffer);
        SVI_SVIFE_DELETE_ARRAY(m_pReservedColorBuffer);
        SVI_SVIFE_DELETE_ARRAY(m_pReservedTexcoordBuffer);
        SVI_SVIFE_DELETE_ARRAY(m_pReservedIndexBuffer);

        m_pReservedPositionBuffer = new SVIVector3[uVertexCount];
        m_pReservedColorBuffer = new SVIVector4[uColorCount];
        m_pReservedTexcoordBuffer = new SVIVector2[uTexcoordCount];
        m_pReservedIndexBuffer = new SVIUShort[uIndexCount];

        m_uReservedPositionCount = uVertexCount;
        m_uReservedTexcoordCount = uTexcoordCount;
        m_uReservedIndexCount = uIndexCount;
        m_uReservedColorCount = uColorCount;

        m_uCurrentMemoryUsage += m_uReservedColorCount * sizeof(SVIVector4);
        m_uCurrentMemoryUsage += m_uReservedPositionCount * sizeof(SVIVector3);
        m_uCurrentMemoryUsage += m_uReservedTexcoordCount * sizeof(SVIVector2);
        m_uCurrentMemoryUsage += m_uReservedIndexCount * sizeof(SVIUShort);
				
		m_uCurrentParticleCount = 0;
		createShader();
	}
	
	void SVIParticleRenderer::defferedRender(){
		for (SVIUInt n = 0; n < mParticleEffectCache.size(); n++){
			SVIParticleEffectCache * pCache = mParticleEffectCache[n];
			renderCache(pCache);
		}
		clearDefferd();
	}
	
	void SVIParticleRenderer::immediateRender(SVIParticleEffect* pEffect){

		SVIInt particlesCount = pEffect->getParticlesList().size(); //pEffect - get sub particles

		ParticlesList particlesList = pEffect->getParticlesList();
		ParticlesList::iterator iter = particlesList.begin();
		

		//2012-06-12 masterkeaton27@gmail.com
		//this would create and deletion may cause CPU memory allocation cost
		while (iter != particlesList.end()){
			SVIParticles * pParticles = *iter;
			// 2012/06/28 rareboy0112@facebook.com
			// check particle's status
			if( (*iter)->getStatus() == SVIParticles::ANIMATING) {
				SVIParticleEffectCache * pNewCache = generateRenderCache(pParticles);

				//2012-06-22 masterkeaton27@gmail.com
				pNewCache->setWorldMatrix(*pEffect->getParentTransform());
				renderCache(pNewCache);
				SVI_SVIFE_DELETE(pNewCache);
			}
			iter++;
		}
	}

	void SVIParticleRenderer::reset(){
		clearDefferd();
	}

	void SVIParticleRenderer::finalize(){
		SVI_SVIFE_DELETE_ARRAY(m_pReservedPositionBuffer);
		SVI_SVIFE_DELETE_ARRAY(m_pReservedColorBuffer);
		SVI_SVIFE_DELETE_ARRAY(m_pReservedTexcoordBuffer);
		SVI_SVIFE_DELETE_ARRAY(m_pReservedIndexBuffer);
		m_uReservedPositionCount = 0;
		m_uReservedColorCount = 0;
		m_uReservedTexcoordCount = 0;
		m_uReservedIndexCount = 0;
		reset();
		m_uCurrentMemoryUsage = 0;
		destroyShader();
	}
	
	void SVIParticleRenderer::createDefferedRender(SVIParticleEffect* pEffect){
		SVIInt particlesCount = 0;
		if(pEffect)
			particlesCount = pEffect->getParticlesList().size();
		for (SVIInt n = 0; n < particlesCount; n++){
			SVIParticles * pParticles = NULL;
			SVIParticleEffectCache * pNewCache = generateRenderCache(pParticles);
			
			mParticleEffectCache.push_back(pNewCache);
			m_uCurrentMemoryUsage += pNewCache->getMemoryUsage();
		}		
	}

	void SVIParticleRenderer::clearDefferd(){
		SVIParticleEffectCacheList::iterator iter = mParticleEffectCache.begin();

		while (iter != mParticleEffectCache.end()){
			SVIParticleEffectCache * pCache = *iter;
			iter++;
			m_uCurrentMemoryUsage -= pCache->getMemoryUsage();
			SVI_SVIFE_DELETE(pCache);
		}
		mParticleEffectCache.clear();
	}

	SVIUInt SVIParticleRenderer::getDefferedCount(){
		return mParticleEffectCache.size();
	}
	
	void SVIParticleRenderer::renderCache(SVIParticleEffectCache * pCache){
		if (pCache == NULL) return;

		//set reserved buffer to cache
		pCache->setVertexBuffer(m_pReservedPositionBuffer);
		pCache->setColorBuffer(m_pReservedColorBuffer);
		pCache->setTexcoordBuffer(m_pReservedTexcoordBuffer);
		pCache->setIndexBuffer(m_pReservedIndexBuffer);

		//build polygon
		pCache->build();

		//let's render
		useShader();

		//2012-06-25 masterkeaton27@gmail.com
		//set adaptive color uniform.
		//if particle effect does not have texture then 
		//it will shows up as a colored rectangle which had given color.
		SVIVector4 adaptiveColor = (pCache->getHasTexture() == SVITRUE) ? 
			SVIVector4(0.0f,0.0f,0.0f,0.0f) : SVIVector4(1.0f,1.0f,1.0f,1.0f);
		SVIVector4 color = pCache->getColor();
		
		//multiply with parent's transform
		SVIMatrix worldMat = (*pCache->getWorldMatrix()) * (*m_pWorld);

		setBlend(pCache->getAddBlend());
		setSampler(pCache->getTextureIndex());
		setUniforms(m_pProjection,m_pView, &worldMat,&color,&adaptiveColor);
		setAttributes(pCache->getVertexBuffer(),pCache->getColorBuffer(), pCache->getTexcoordBuffer());
		
		render(pCache->getParticleCount() * 6,pCache->getIndexBuffer());

		unsetAttributes();
		unsetBlend();
		
		m_uCurrentParticleCount += pCache->getParticleCount();
	}

	void SVIParticleRenderer::setViewMatrix(SVIMatrix * pMatrix){
		m_pView = pMatrix;
	}
	
	void SVIParticleRenderer::setWorldMatrix(SVIMatrix * pMatrix){
		m_pWorld = pMatrix;
	}

	void SVIParticleRenderer::setProjectionMatrix(SVIMatrix * pMatrix){
		m_pProjection = pMatrix;
	}

	

	SVIParticleEffectCache * SVIParticleRenderer::generateRenderCache(SVIParticles* pEffect){
		
		SVIParticleEffectCache * pNewCache = new SVIParticleEffectCache();

#ifdef SGI_BASE
		// 1. SVIParticles - get particle texture name

		SVIPChar pTextureName = (SVIPChar)"Unknown Texture";
		if (pEffect != NULL){
			pTextureName = (SVIPChar)pEffect->getTextureFileName().c_str();
		}
		SVITexture * pTexture = (SVITexture *)mSVIGLSurface->getTextureManager()->find(pTextureName);
		SVIUInt uTextureIndex = 0;
		if (pTexture != NULL){
			uTextureIndex = pTexture->getTextureId();
			pNewCache->setTextureIndex(uTextureIndex);	
			pNewCache->setHasTexture(SVITRUE);
		}else{
			pNewCache->setHasTexture(SVIFALSE);
		}
#endif		
		//1-1.should  check texture existence here.
					
		//2. SVIParticles - get color type;
		SVIColor cColor = SVIColor(1.0f,1.0f,1.0f,1.0f); 
		
		pNewCache->setColor(cColor.mR,cColor.mG,cColor.mB,cColor.mA);

		//3. SVIParticles - get blend type;
		SVIBool isAddBlend = SVITRUE; 
		pNewCache->setAddBlend(isAddBlend);

		//4. SVIParticles - get particle count
		SVIInt particleCount = 0;
		if(pEffect !=NULL)
			particleCount = pEffect->getParticleCount(); 
		
		//5. SVIParticles - get value from children and set value to particle cache. (copy)
		for (int n = 0; n < particleCount; n++){
			SVIParticle & particle = pEffect->getParticles()[n];

			if (!particle._active) continue;

			SVIParticleCache * pNewParticleCache = new SVIParticleCache();
			pNewParticleCache->setPosition(&particle._position);
			pNewParticleCache->setColor(&particle._particleColor);
			pNewParticleCache->setScale(&particle._size);

			pNewCache->addParticleCache(pNewParticleCache);						
		}

		//6. return new cache
		return pNewCache;
	}
		
	SVIParticleRenderer::SVIParticleRenderer(SVIGLSurface* saGLSurface){
		m_pReservedPositionBuffer = NULL;
		m_pReservedColorBuffer = NULL;
		m_pReservedTexcoordBuffer = NULL;
		m_pReservedIndexBuffer = NULL;
		m_uReservedPositionCount = 0;
		m_uReservedColorCount = 0;
		m_uReservedTexcoordCount = 0;
		m_uReservedIndexCount = 0;
		m_uCurrentMemoryUsage = 0;

		m_pWorld = NULL;
		m_pView = NULL;
		m_pProjection = NULL;
		m_iFragmentShaderID = 0;
		m_iProgramID =0;
		m_iVertexShaderID =0;
		m_uCurrentParticleCount =0;
		mSVIGLSurface = saGLSurface;
	}

	SVIParticleRenderer::~SVIParticleRenderer(){
		finalize();
		mSVIGLSurface = NULL;
	}



#ifndef GL_INFO_LOG_LENGTH
#define GL_INFO_LOG_LENGTH GL_INFO_LOG_length
#endif

	static char SVIParticleVertexShader[] =
#ifdef GL30
		"#version 330\n"
		"layout(location=0) in vec3		aPosition;\n"
		"layout(location=1) in vec2		aTexcoord;\n"
		"out vec2		vTexcoord;\n"
		"uniform mat4		uWVP;\n"
		"void main() {\n"					
		"	vec4 position = uWVP * vec4(aPosition.xyz,1.0);\n"
		"	vTexcoord = aTexcoord;\n"
		"	gl_Position = position;\n"
		"}\n";
#else
		"uniform mat4		uProjection;\n"
		"uniform mat4		uView;\n"
		"uniform mat4		uWorld;\n"
		"attribute vec3		aPosition;\n"
		"attribute vec4		aColor;\n"		
		"attribute vec2		aTexcoord;\n"	
		"varying vec4		vColor;\n"
		"varying vec2		vTexcoord;\n"
		"void main() {\n"					
		"	vec4 position = uProjection * uView * uWorld * vec4(aPosition.xyz,1.0);\n"
		"	vTexcoord = aTexcoord;\n"
		"	vColor = aColor;\n"
		"	gl_Position = position;\n"
		"}\n";
#endif



	static char SVIParticleFragmentShader[] =	
#ifdef GL30
		"#version 330\n"
		"in vec2			vTexcoord;\n"
		"uniform vec4		uColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"out vec4			fragColor;\n"
		"void main() {\n"
		"	vec4 texColor = texture2D( uSampler0, vTexcoord );\n"		
		"	fragColor = texColor * uColor;\n"		
		"}\n";
#else
		"precision mediump float;\n"
		"varying vec2		vTexcoord;\n"
		"varying vec4		vColor;\n"
		"uniform vec4		uColor;\n"
		"uniform vec4		uAdaptiveColor;\n"
		"uniform sampler2D	uSampler0;\n"
		"void main() {\n"
		"	vec4 texColor = texture2D( uSampler0, vTexcoord ) + uAdaptiveColor;\n"		
		"	gl_FragColor = texColor * uColor * vColor;\n"		
		"}\n";
#endif
			

	SVIInt compileShader(SVIInt shaderType, SVIPChar srcShader){
		SVIInt compiled = -1;
		SVIUInt shaderId = 0;
#ifdef GLES20
		shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, (const char**)&srcShader, NULL);
		glCompileShader(shaderId);
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = (char*) malloc(infoLen);
				if (buf) {
					glGetShaderInfoLog(shaderId, infoLen, NULL, buf);
					LOGE("SVIProgram: Could not compile shader %d:\n%s\n",
						shaderType, buf);
					free(buf);
				}
				glDeleteShader(shaderId);
				shaderId = 0;
				return 0;
			}
		}
#endif //#ifdef GLES20

#ifdef GL30
        shaderId = glCreateShader(shaderType);
        glShaderSource(shaderId, 1, (const char**)&srcShader, NULL);
        glCompileShader(shaderId);
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shaderId, infoLen, NULL, buf);
                    LOGE("SVIProgram: Could not compile shader %d:\n%s\n",
                        shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shaderId);
                shaderId = 0;
                return 0;
            }
        }
#endif //#ifdef GL30
		return shaderId;
	}

	SVIInt linkShader(SVIInt program){
		SVIInt linked = -1;
#ifdef GLES20
        glLinkProgram(program);	
        glGetProgramiv(program, GL_LINK_STATUS, &linked);	
#endif //#ifdef GLES20

#ifdef GL30
        glLinkProgram(program);	
        glGetProgramiv(program, GL_LINK_STATUS, &linked);	
#endif //#ifdef GL30

		if (linked == 0) {
			LOGE("SVIProgram: Could not link shader %d",program);
			return -1;
		}
		return 1;
    }

    void SVIParticleRenderer::createShader(){	
#ifdef GLES20
        m_iProgramID = glCreateProgram();
        m_iVertexShaderID = compileShader(GL_VERTEX_SHADER,SVIParticleVertexShader);
        m_iFragmentShaderID = compileShader(GL_FRAGMENT_SHADER,SVIParticleFragmentShader);
        glAttachShader(m_iProgramID, m_iVertexShaderID);  
        glAttachShader(m_iProgramID, m_iFragmentShaderID);  
        linkShader(m_iProgramID);
#endif //#ifdef GLES20

#ifdef GL30
        m_iProgramID = glCreateProgram();
        m_iVertexShaderID = compileShader(GL_VERTEX_SHADER,SVIParticleVertexShader);
        m_iFragmentShaderID = compileShader(GL_FRAGMENT_SHADER,SVIParticleFragmentShader);
        glAttachShader(m_iProgramID, m_iVertexShaderID);  
        glAttachShader(m_iProgramID, m_iFragmentShaderID);  
        linkShader(m_iProgramID);
#endif //#ifdef GL30

    }

    void SVIParticleRenderer::destroyShader(){
#ifdef GLES20
		glUseProgram(0);
		
		glDetachShader(m_iProgramID, m_iVertexShaderID);
		glDetachShader(m_iProgramID, m_iFragmentShaderID);

		glDeleteShader(m_iVertexShaderID);
		glDeleteShader(m_iFragmentShaderID);

		glDeleteProgram(m_iProgramID);
#endif //#ifdef GLES20

#ifdef GL30
        glUseProgram(0);

        glDetachShader(m_iProgramID, m_iVertexShaderID);
        glDetachShader(m_iProgramID, m_iFragmentShaderID);

        glDeleteShader(m_iVertexShaderID);
        glDeleteShader(m_iFragmentShaderID);

        glDeleteProgram(m_iProgramID);
#endif //#ifdef GL30

	}

	void SVIParticleRenderer::useShader(){
#ifdef GLES20
		glUseProgram(m_iProgramID);
#endif //#ifdef GLES20

#ifdef GL30
        glUseProgram(m_iProgramID);
#endif //#ifdef GL30

	}


	void SVIParticleRenderer::setBlend(SVIBool isAddBlend){
#ifdef GLES20
		glEnable(GL_BLEND);
		if (isAddBlend){			
			glBlendFunc(GL_ONE, GL_ONE);
		}else{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}		
#endif //#ifdef GLES20

#ifdef GL30
        glEnable(GL_BLEND);
        if (isAddBlend){			
            glBlendFunc(GL_ONE, GL_ONE);
        }else{
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }		
#endif //#ifdef GL30

	}

	void SVIParticleRenderer::setSampler(SVIUInt samplerIndex){
#ifdef GLES20
		glActiveTexture ( GL_TEXTURE1 );				
		glBindTexture( GL_TEXTURE_2D, samplerIndex);
		SVIInt iVBHandle = glGetUniformLocation(m_iProgramID, "uSampler0");
		glUniform1i( iVBHandle, 1 );
#endif //#ifdef GLES20

#ifdef GL30
        glActiveTexture ( GL_TEXTURE1 );				
        glBindTexture( GL_TEXTURE_2D, samplerIndex);
        SVIInt iVBHandle = glGetUniformLocation(m_iProgramID, "uSampler0");
        glUniform1i( iVBHandle, 1 );
#endif //#ifdef GL30

	}

	void SVIParticleRenderer::setUniforms(SVIMatrix* pProjMatrix, SVIMatrix* pViewMatrix, 
		SVIMatrix* pWorldMatrix, SVIVector4* pColor, SVIVector4 * pAdaptiveColor){
#ifdef GLES20
		SVIInt iPHandle = glGetUniformLocation(m_iProgramID, "uProjection");
		glUniformMatrix4fv(iPHandle, 1, GL_FALSE, (GLfloat*)pProjMatrix);

		SVIInt iVHandle = glGetUniformLocation(m_iProgramID, "uView");
		glUniformMatrix4fv(iVHandle, 1, GL_FALSE, (GLfloat*)pViewMatrix);

		SVIInt iWHandle = glGetUniformLocation(m_iProgramID, "uWorld");
		glUniformMatrix4fv(iWHandle, 1, GL_FALSE, (GLfloat*)pWorldMatrix);

		SVIInt iColorHandle = glGetUniformLocation(m_iProgramID, "uColor");
		glUniform4f(iColorHandle, pColor->r, pColor->g, pColor->b, pColor->a);

		//2012-06-25 masterkeaton27@gmail.com
		//adaptive color for non-texture situation
		SVIInt iAdaptiveColorHandle = glGetUniformLocation(m_iProgramID, "uAdaptiveColor");
		glUniform4f(iAdaptiveColorHandle, pAdaptiveColor->r, pAdaptiveColor->g, pAdaptiveColor->b, pAdaptiveColor->a);		
#endif //#ifdef GLES20

#ifdef GL30	
        SVIInt iPHandle = glGetUniformLocation(m_iProgramID, "uProjection");
        glUniformMatrix4fv(iPHandle, 1, GL_FALSE, (GLfloat*)pProjMatrix);

        SVIInt iVHandle = glGetUniformLocation(m_iProgramID, "uView");
        glUniformMatrix4fv(iVHandle, 1, GL_FALSE, (GLfloat*)pViewMatrix);

        SVIInt iWHandle = glGetUniformLocation(m_iProgramID, "uWorld");
        glUniformMatrix4fv(iWHandle, 1, GL_FALSE, (GLfloat*)pWorldMatrix);

        SVIInt iColorHandle = glGetUniformLocation(m_iProgramID, "uColor");
        glUniform4f(iColorHandle, pColor->r, pColor->g, pColor->b, pColor->a);

        //2012-06-25 masterkeaton27@gmail.com
        //adaptive color for non-texture situation
        SVIInt iAdaptiveColorHandle = glGetUniformLocation(m_iProgramID, "uAdaptiveColor");
        glUniform4f(iAdaptiveColorHandle, pAdaptiveColor->r, pAdaptiveColor->g, pAdaptiveColor->b, pAdaptiveColor->a);		
#endif //#ifdef GL30

	}

	void SVIParticleRenderer::setAttributes(SVIVector3 * pVB, SVIVector4 * pCB, SVIVector2 * pTB){
#ifdef GLES20
        SVIInt iVBHandle = glGetAttribLocation(m_iProgramID,"aPosition");
        SVIInt iCBHandle = glGetAttribLocation(m_iProgramID,"aColor");		
        SVIInt iTBHandle = glGetAttribLocation(m_iProgramID,"aTexcoord");

        glEnableVertexAttribArray( iVBHandle );
        glEnableVertexAttribArray( iCBHandle );
        glEnableVertexAttribArray( iTBHandle );

        glVertexAttribPointer( iVBHandle, 3, GL_FLOAT, GL_FALSE, 0, pVB);	
        glVertexAttribPointer( iCBHandle, 4, GL_FLOAT, GL_FALSE, 0, pCB);	
        glVertexAttribPointer( iTBHandle, 2, GL_FLOAT, GL_FALSE, 0, pTB);	

#endif //#ifdef GLES20

#ifdef GL30		
        SVIInt iVBHandle = glGetAttribLocation(m_iProgramID,"aPosition");
        SVIInt iCBHandle = glGetAttribLocation(m_iProgramID,"aColor");		
        SVIInt iTBHandle = glGetAttribLocation(m_iProgramID,"aTexcoord");

        glEnableVertexAttribArray( iVBHandle );
        glEnableVertexAttribArray( iCBHandle );
        glEnableVertexAttribArray( iTBHandle );

        glVertexAttribPointer( iVBHandle, 3, GL_FLOAT, GL_FALSE, 0, pVB);	
        glVertexAttribPointer( iCBHandle, 4, GL_FLOAT, GL_FALSE, 0, pCB);	
        glVertexAttribPointer( iTBHandle, 2, GL_FLOAT, GL_FALSE, 0, pTB);	

#endif //#ifdef GL30

	}

	void SVIParticleRenderer::render(SVIUInt indexCount, SVIUShort* pIB){
		
#ifdef GLES20
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, pIB);
#endif //#ifdef GLES20

#ifdef GL30
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, pIB);
#endif //#ifdef GL30

	}

	void SVIParticleRenderer::unsetAttributes(){
#ifdef GLES20
        SVIInt iVBHandle = glGetAttribLocation(m_iProgramID,"aPosition");
        SVIInt iCBHandle = glGetAttribLocation(m_iProgramID,"aColor");
        SVIInt iTBHandle = glGetAttribLocation(m_iProgramID,"aTexcoord");
        glDisableVertexAttribArray( iVBHandle );
        glDisableVertexAttribArray( iCBHandle );
        glDisableVertexAttribArray( iTBHandle );
#endif //#ifdef GLES20

#ifdef GL30
        SVIInt iVBHandle = glGetAttribLocation(m_iProgramID,"aPosition");
        SVIInt iCBHandle = glGetAttribLocation(m_iProgramID,"aColor");
        SVIInt iTBHandle = glGetAttribLocation(m_iProgramID,"aTexcoord");
        glDisableVertexAttribArray( iVBHandle );
        glDisableVertexAttribArray( iCBHandle );
        glDisableVertexAttribArray( iTBHandle );
#endif //#ifdef GL30

	}

	void SVIParticleRenderer::unsetBlend(){
#ifdef GLES20
		glDisable(GL_BLEND);
#endif //#ifdef GLES20

#ifdef GL30
        glDisable(GL_BLEND);
#endif //#ifdef GL30

	}
	
	

}
