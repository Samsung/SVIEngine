#pragma once
#include "../GLSurface/SVIGLSurface.h"

namespace SVI {

	//2012-06-01 masterkeaton27@gmail.com
	//Cache for 1 Particle
	class SVIParticleCache{
	public:
		SVIParticleCache();
		virtual ~SVIParticleCache();

	public:
		void setPosition(SVIVector3* pPosition);
		void setColor(SVIColor* pColor);
		void setScale(SVIVector3* pScale);
		void setRotation(SVIVector3* pRotation);		
		void setDistance(SVIFloat fDistance);

	public:
		SVIVector3 * getPosition();
		SVIVector3 * getScale();
		SVIVector3 * getRotation();
		SVIColor * getColor();

		SVIFloat		getDistance();
		

	private:
		SVIVector3*	m_pPosition;
		SVIVector3*	m_pScale;
		SVIVector3*	m_pRotation;
		SVIColor*	m_pColor;
		SVIFloat		m_Distance;
	};

	//2012-06-01 masterkeaton27@gmail.com
	//Cache for 1 Particle Effect (1 Texture Only)
	class SVIParticleEffectCache{
	public:
		SVIParticleEffectCache();
		virtual ~SVIParticleEffectCache();

	public:
		void addParticleCache(SVIParticleCache * pCache);
		void clearAllCache();
		void build();

	public:
		void		setTextureIndex(SVIUInt uTextureIndex);
		void		setVertexBuffer(SVIVector3 * pBuffer);
		void		setColorBuffer(SVIVector4 * pBuffer);
		void		setTexcoordBuffer(SVIVector2 * pBuffer);
		void		setIndexBuffer(SVIUShort * pBuffer);
		void		setColor(SVIVector4 color);
		void		setColor(SVIFloat  r, SVIFloat g, SVIFloat b, SVIFloat a);
		void		setAddBlend(SVIBool isAdd);
		void		setViewMatrix(SVIMatrix * pViewMatrix);
		void		setWorldMatrix(SVIMatrix & WorldMatrix);
		void		setHasTexture(SVIBool hasTexture);

	public:	
		SVIUInt		getMemoryUsage();
		SVIUInt		getTextureIndex();
		SVIUInt		getParticleCount();
		SVIUInt		getVertexBufferSize();
		SVIUInt		getColorBufferSize();
		SVIUInt		getTexcoordBufferSize();
		SVIUInt		getIndexBufferSize();
				
		SVIVector4 & getColor();
		SVIVector4 *	getColorBuffer();
		SVIVector3 *	getVertexBuffer();
		SVIVector2 *	getTexcoordBuffer();
		SVIUShort *	getIndexBuffer();
		SVIMatrix *  getWorldMatrix();
		SVIBool		getAddBlend();
		SVIBool		getHasTexture();

	private:
		void buildBuffer(SVIBool isUsingUV, SVIBool isUsingIndex);
		void buildBuffer();
		void sortParticleCaches();

	private:	
		std::vector<SVIParticleCache*>	mParticleCaches;
		SVIUInt							m_uTextureIndex;

		SVIVector4						m_Color;

		SVIMatrix 						m_WorldMatrix;
		SVIMatrix *						m_pViewMatrix;
		SVIVector3 *						m_pVertexBuffer;
		SVIVector4 *						m_pColorBuffer;
		SVIVector2 *						m_pTexcoordBuffer;
		SVIUShort *						m_pIndexBuffer;

		SVIUInt							mVertexCount;
		SVIUInt							mTexcoordCount;
		SVIUInt							mIndexCount;
		SVIUInt							m_uCurrentMemoryUsage;

		SVIBool							m_bIsAddBlend;
		SVIBool							m_bHasTexture;
	};

	class SVIParticleEffect; 
	class SVIParticles;

	typedef std::vector<SVIParticleEffectCache*> SVIParticleEffectCacheList;

	class SVIParticleRenderer{
		typedef std::list<SVIParticles*> ParticlesList;

	public:
		void initialize();
		void initialize(SVIUInt maxParticleCount);

		void update();

		void defferedRender();
		void immediateRender(SVIParticleEffect* pEffect);
		void reset();
		void finalize();
		
		void setViewMatrix(SVIMatrix * pMatrix);
		void setWorldMatrix(SVIMatrix * pMatrix);
		void setProjectionMatrix(SVIMatrix * pMatrix);
	public:
		void createDefferedRender(SVIParticleEffect* pEffect);

	public:
		SVIUInt getDefferedCount();

	private:
		void clearDefferd();
		void renderCache(SVIParticleEffectCache * pCache);
		SVIParticleEffectCache * generateRenderCache(SVIParticles* pEffect);

	private: //shader parameters.
		void createShader();
		void destroyShader();
		void useShader();
		void setBlend(SVIBool isAddBlend);
		void setSampler(SVIUInt samplerIndex);
		void setUniforms(SVIMatrix* pProjMatrix, SVIMatrix* pViewMatrix, SVIMatrix* pWorldMatrix, SVIVector4* pColor, SVIVector4* pAdaptiveColor );
		void setAttributes(SVIVector3 * pVB,SVIVector4 * pCB, SVIVector2 * pTB);
		void render(SVIUInt indexCount, SVIUShort* pIB);
		void unsetAttributes();
		void unsetBlend();
	public:
		 SVIParticleRenderer(SVIGLSurface* mSVIGLSurface);//Multi-Instance-Support
		virtual ~SVIParticleRenderer();
		SVIParticleRenderer(const SVIParticleRenderer& rhs){}
		void operator=(const SVIParticleRenderer& rhs){}

	private:
		SVIParticleEffectCacheList mParticleEffectCache;

	private:
		SVIGLSurface * mSVIGLSurface;//Multi-Instance-Support

	private:
		SVIInt		m_iProgramID;
		SVIInt		m_iVertexShaderID;
		SVIInt		m_iFragmentShaderID;

	private:
		SVIUInt		m_uReservedPositionCount;
		SVIUInt		m_uReservedColorCount;
		SVIUInt		m_uReservedTexcoordCount;
		SVIUInt		m_uReservedIndexCount;

		SVIVector3 * m_pReservedPositionBuffer;
		SVIVector4 * m_pReservedColorBuffer;
		SVIVector2 * m_pReservedTexcoordBuffer;
		SVIUShort *	m_pReservedIndexBuffer;


		SVIUInt		m_uCurrentParticleCount;
		SVIUInt		m_uCurrentMemoryUsage;

		SVIMatrix *	m_pWorld;
		SVIMatrix *	m_pView;
		SVIMatrix *	m_pProjection;
	};
};
