#include "../SVICores.h"
#include "SVIRenderer.h"
#include "SVIFrameRenderer.h"
#include "SVIProgramManager.h"
#include "SVIViewport.h"
#include "SVIPostEffectRenderer.h"
#include "SVITexture.h"
#include "SVITextureManager.h"
#include "../Slide/SVIRenderPartManager.h"

//2013-03-27 masterkeaton27@gmail.com
//due to reducing initialization cost.

namespace SVI {

static const SVIBool DEBUG = SVIFALSE;


SVIPostEffectRenderer::SVIPostEffectRenderer(SVIGLSurface* saGLSurface) ://Multi-Instance-Support
		mSize(0.0f, 0.0f) {
	mActivated = false;
	mDebugDraw = false;
	mSVIGLSurface=saGLSurface;//Multi-Instance-Support
	mScene = new SVICaptureData(mSVIGLSurface);

	mInteractionCount = 0;
	for (int n = 0; n < MAX_INTERACTION; n++) {
		mInteractionPoints[n].x = 0.0f;
		mInteractionPoints[n].y = 0.0f;
	}

	mInteractionPoints[0].z = 0.02;
	mInteractionPoints[0].w = 0.3f;

	mInteractionPoints[1].z = 0.02;
	mInteractionPoints[1].w = 0.4f;

	mInteractionPoints[2].z = 0.03;
	mInteractionPoints[2].w = 0.3f;

	mInteractionPoints[3].z = 0.5;
	mInteractionPoints[3].w = 0.4f;
}

SVIPostEffectRenderer::~SVIPostEffectRenderer() {
	SVI_SVIFE_DELETE(mScene);
	mSVIGLSurface = NULL; // Multi-Instance-Support

	finalize();
}

void SVIPostEffectRenderer::initialize(float width, float height) {
	//restore(width,height);
	//SVIRippleRenderer::getInstance()->Initialize();
}

void SVIPostEffectRenderer::restore(float width, float height) {
	if (mScene == NULL)
		return;

	if (mScene)
		mScene->destroy();

	mScene->create(width, height);

	mActivated = SVITRUE;
}

void SVIPostEffectRenderer::setDebugDraw(SVIBool isDebugDraw) {

}

void SVIPostEffectRenderer::setActivated(SVIBool isActivated) {

}

void SVIPostEffectRenderer::finalize() {
	if (mScene)
		mScene->destroy();

}

void SVIPostEffectRenderer::update(float width, float height) {

}

void SVIPostEffectRenderer::renderFullScreenQuad(SVIVector2 size, SVIProgram * pProgram, SVIUInt sampler0, SVIUInt sampler1,
		SVIUInt sampler2) {
	renderQuad(SVIVector2(0.0f, 0.0f), size, pProgram, sampler0, sampler1, sampler2);
}

void SVIPostEffectRenderer::renderQuad(SVIVector2 offset, SVIVector2 size, SVIProgram * pProgram, SVIUInt sampler0,
		SVIUInt sampler1, SVIUInt sampler2) {

	SVIVector2 vertices[4] = { SVIVector2(0.0f, 0.0f), SVIVector2(0.0f, size.y), SVIVector2(size.x, size.y), SVIVector2(
			size.x, 0.0f) };

	float aaCoords[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	SVIProgramHandler * pHandler = pProgram->getHandler();

	pProgram->use();

	if (sampler0)
		pHandler->setSampler(HD_U_SVIMPLER_0, sampler0);
	if (sampler1)
		pHandler->setSampler(HD_U_SVIMPLER_1, sampler1);
	if (sampler2)
		pHandler->setSampler(HD_U_SVIMPLER_2, sampler2);

	SVIVector2 sideSize = size;
	SVIVector2 sidePivot(0.0f, 0.0f);        // = offset;
	pHandler->setUniform(HD_U_SIZE, sideSize);
	pHandler->setUniform(HD_U_PIVOT, sidePivot);

	SVIVector4 texInfo = SVIVector4(0.0f, 0.0f, 1.0f, 1.0f);
	pHandler->setUniform(HD_U_TEXTUREINFO, texInfo);

	SVIVector4 color = SVIVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pHandler->setUniform(HD_U_COLOR, color);

	SVIMatrix viewMat;
	SVIMatrix projMat = mSVIGLSurface->getViewport()->getProjection();
	viewMat.setIdentity();
	viewMat._41 = offset.x;
	viewMat._42 = offset.y;

	pHandler->setUniform(HD_U_PROJ, projMat);
	pHandler->setUniform(HD_U_VIEW, viewMat);

	pHandler->setAttribute(HD_A_POSITION, vertices);
	pHandler->setAttribute(HD_A_AACOORD, 1, aaCoords);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	pHandler->releaseAttribute(HD_A_AACOORD);
	pHandler->releaseAttribute(HD_A_POSITION);
}

void SVIPostEffectRenderer::beginFrame() {

	if (mScene)
		mScene->bind();
}

void SVIPostEffectRenderer::endFrame() {
	if (mScene)
		mScene->unbind();
}

void SVIPostEffectRenderer::_drawRipple() {
	mSVIGLSurface->getRippleRenderer()->update();
	mSVIGLSurface->getRippleRenderer()->render(mScene->getId());
}

void SVIPostEffectRenderer::drawPostRippleEffect() {
	_drawRipple();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Large Ripple Resource
float rippleVelocity[] = { 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000001, 0.000001, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000006, 0.000073, 0.000074, 0.000007, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000024, 0.000551, 0.003299, 0.003297, 0.000552, 0.000026, 0.000001,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000001, 0.000056, 0.001866, 0.020681, 0.021763, 0.021768, 0.020677, 0.001863,
		0.000062, 0.000001, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000001, 0.000082, 0.003637, 0.058989, 0.064969, 0.072713, 0.072702, 0.064976,
		0.058978, 0.003622, 0.000095, 0.000002, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000001, 0.000080, 0.004445, 0.095078, 0.115728, 0.082494, 0.077263, 0.077270,
		0.082490, 0.115745, 0.095063, 0.004417, 0.000096, 0.000001, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000052, 0.003486, 0.092771, 0.135830, 0.036451, -0.016153, -0.056132,
		-0.056129, -0.016141, 0.036458, 0.135857, 0.092759, 0.003458, 0.000065, 0.000001, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000022, 0.001714, 0.054778, 0.107818, 0.044509, -0.077469, -0.140713,
		-0.115216, -0.115232, -0.140729, -0.077449, 0.044521, 0.107827, 0.054776, 0.001699, 0.000028, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000005, 0.000485, 0.018249, 0.056158, 0.091149, -0.005198,
		-0.144135, -0.064754, -0.030516, -0.030500, -0.064773, -0.144169, -0.005165, 0.091157, 0.056142, 0.018253,
		0.000482, 0.000007, 0.000000, 0.000000, 0.000000, 0.000000, 0.000001, 0.000062, 0.002749, 0.017339, 0.070803,
		0.077529, -0.042919, -0.107093, -0.041399, -0.042570, -0.042566, -0.041398, -0.107095, -0.042922, 0.077543,
		0.070804, 0.017327, 0.002752, 0.000062, 0.000001, 0.000000, 0.000000, 0.000000, 0.000001, 0.000053, 0.002405,
		0.019099, 0.061391, 0.089359, -0.034342, -0.130646, -0.033094, -0.032908, -0.032897, -0.033092, -0.130675,
		-0.034327, 0.089379, 0.061375, 0.019098, 0.002405, 0.000053, 0.000001, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000005, 0.000389, 0.014788, 0.059871, 0.085182, 0.010952, -0.134152, -0.096711, -0.044963, -0.044951,
		-0.096740, -0.134166, 0.010979, 0.085181, 0.059869, 0.014784, 0.000389, 0.000006, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000018, 0.001303, 0.041715, 0.110572, 0.070191, -0.046645, -0.136159, -0.121210,
		-0.121230, -0.136165, -0.046625, 0.070208, 0.110582, 0.041701, 0.001296, 0.000023, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000043, 0.002515, 0.066655, 0.131539, 0.077356, 0.021005,
		-0.020877, -0.020870, 0.021016, 0.077370, 0.131550, 0.066634, 0.002502, 0.000052, 0.000001, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000001, 0.000065, 0.003048, 0.064555, 0.102866,
		0.092755, 0.088737, 0.088741, 0.092755, 0.102869, 0.064538, 0.003035, 0.000077, 0.000001, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000001, 0.000066, 0.002372,
		0.037880, 0.051340, 0.059473, 0.059467, 0.051341, 0.037872, 0.002365, 0.000075, 0.000002, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000001,
		0.000044, 0.001159, 0.012572, 0.014853, 0.014856, 0.012569, 0.001158, 0.000049, 0.000001, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000001, 0.000019, 0.000327, 0.001904, 0.001903, 0.000328, 0.000020, 0.000001, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000005, 0.000042, 0.000042, 0.000005, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000001, 0.000001, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000

};
float rippleHeight[] = { 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000001, 0.000001, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000009, 0.000088, 0.000088, 0.000009, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000001, 0.000034, 0.000656, 0.003539, 0.003538, 0.000657, 0.000037, 0.000001,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000001, 0.000078, 0.002213, 0.021949, 0.029402, 0.029405, 0.021946, 0.002214,
		0.000086, 0.000002, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000002, 0.000114, 0.004308, 0.062379, 0.105182, 0.127822, 0.127816, 0.105183,
		0.062367, 0.004305, 0.000130, 0.000002, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000001, 0.000112, 0.005263, 0.100409, 0.210622, 0.252118, 0.278006, 0.278006,
		0.252112, 0.210618, 0.100383, 0.005255, 0.000132, 0.000002, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000001, 0.000074, 0.004128, 0.097967, 0.257962, 0.318192, 0.318859, 0.301717,
		0.301724, 0.318869, 0.318195, 0.257957, 0.097940, 0.004120, 0.000089, 0.000001, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000031, 0.002031, 0.057915, 0.197556, 0.316065, 0.299150, 0.217869,
		0.199755, 0.199747, 0.217876, 0.299179, 0.316065, 0.197544, 0.057905, 0.002029, 0.000039, 0.000001, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000008, 0.000576, 0.019358, 0.092103, 0.242483, 0.322628, 0.220152,
		0.172825, 0.181997, 0.181998, 0.172809, 0.220156, 0.322656, 0.242468, 0.092086, 0.019363, 0.000577, 0.000010,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000001, 0.000074, 0.002946, 0.023780, 0.115763, 0.262480, 0.304605,
		0.213786, 0.181666, 0.185072, 0.185072, 0.181663, 0.213794, 0.304615, 0.262477, 0.115752, 0.023776, 0.002952,
		0.000075, 0.000001, 0.000000, 0.000000, 0.000000, 0.000001, 0.000063, 0.002592, 0.024661, 0.107968, 0.262655,
		0.313236, 0.205501, 0.179858, 0.186113, 0.186116, 0.179851, 0.205500, 0.313259, 0.262650, 0.107948, 0.024664,
		0.002595, 0.000065, 0.000001, 0.000000, 0.000000, 0.000000, 0.000000, 0.000006, 0.000464, 0.015751, 0.088565,
		0.228605, 0.324039, 0.241967, 0.169412, 0.179524, 0.179522, 0.169398, 0.241985, 0.324058, 0.228588, 0.088557,
		0.015751, 0.000468, 0.000008, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000026, 0.001552,
		0.044253, 0.177479, 0.307959, 0.320521, 0.244386, 0.220372, 0.220363, 0.244401, 0.320546, 0.307958, 0.177465,
		0.044240, 0.001555, 0.000031, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000001,
		0.000060, 0.002996, 0.070605, 0.216849, 0.305456, 0.326363, 0.315190, 0.315198, 0.326370, 0.305455, 0.216833,
		0.070581, 0.002998, 0.000071, 0.000001, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000001, 0.000090, 0.003633, 0.068378, 0.165077, 0.218809, 0.246703, 0.246701, 0.218803, 0.165067,
		0.068361, 0.003635, 0.000104, 0.000002, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000002, 0.000090, 0.002832, 0.040182, 0.076148, 0.096405, 0.096401, 0.076146,
		0.040177, 0.002834, 0.000101, 0.000002, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000001, 0.000061, 0.001387, 0.013390, 0.019327, 0.019328,
		0.013389, 0.001390, 0.000066, 0.000002, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000001, 0.000026, 0.000393, 0.002052,
		0.002052, 0.000394, 0.000028, 0.000001, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000007,
		0.000051, 0.000051, 0.000007, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000001, 0.000001, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
		0.000000, 0.000000, 0.000000

};

#define MIN_DETAIL 90
#define REDUCTION_RATE 0.83f
#define REDUCTION_SPEED 0.56f

#define spanXForLandscape 2
#define spanYForLandscape 2
#define spanXForPortrait 2
#define spanYForPortrait 2

SVIRippleRenderer::SVIRippleRenderer(SVIGLSurface* surface) :
		mVertices(NULL), mIndices(NULL), mHeights(NULL), mHeightMap(NULL), mVelocityMap(NULL), mEnvTexture(NULL)
{
	mVerticesCount = 0;
	mIndicesCount = 0;
	mMaxCount = 0;
	mReseted = SVIFALSE;
	mReductionRatio = REDUCTION_RATE;
	mThreshhold = 0.01f;
    	mStable = SVITRUE;
	mLock = new SVIMutex("SVIRippleRenderer");
	mSVIGLSurface = surface;
}

SVIRippleRenderer::~SVIRippleRenderer() {
	SVI_SVIFE_DELETE(mLock);
	mSVIGLSurface = NULL;
}


void SVIRippleRenderer::Initialize() {

	int i, j;
	int SURFACE_DETAILS_WIDTH = mSurfaceDetailWidth;
	int SURFACE_DETAILS_HEIGHT = mSurfaceDetailHeight;
	int MESH_SIZE_WIDTH = mWidth;
	int MESH_SIZE_HEIGHT = mHeight;

	long VCOUNT = mVerticesCount;
	long ICOUNT = mIndicesCount;

	int cnt = 0;
	int idx;
	float ypos;
	float xpos;

	float xStep = (float) MESH_SIZE_WIDTH / (float) (SURFACE_DETAILS_WIDTH - 1);
	float yStep = (float) MESH_SIZE_HEIGHT / (float) (SURFACE_DETAILS_HEIGHT - 1);

	for (i = 0; i < SURFACE_DETAILS_HEIGHT; i++) {
		for (j = 0; j < SURFACE_DETAILS_WIDTH; j++) {
			idx = j * SURFACE_DETAILS_HEIGHT + i;

			mVertices[idx].x = j * xStep;
			mVertices[idx].y = i * yStep;
			mVertices[idx].z = 0.0f;

			mHeights[idx].x = 0.0f;
			mHeights[idx].y = 0.0f;
			mHeights[idx].z = 0.0f;
		}
	}

	cnt = 0;

	for (i = 1; i < SURFACE_DETAILS_HEIGHT; i++) {
		for (j = 1; j < SURFACE_DETAILS_WIDTH; j++) {
			idx = j * SURFACE_DETAILS_HEIGHT + i;
			mIndices[cnt] = (SVIUShort) (idx - SURFACE_DETAILS_HEIGHT - 1);
			mIndices[cnt + 1] = (SVIUShort) (idx - SURFACE_DETAILS_HEIGHT);
			mIndices[cnt + 2] = (SVIUShort) (idx);
			mIndices[cnt + 3] = (SVIUShort) (idx - SURFACE_DETAILS_HEIGHT - 1);
			mIndices[cnt + 4] = (SVIUShort) (idx);
			mIndices[cnt + 5] = (SVIUShort) (idx - 1);
			cnt += 6;
		}
	}

    mStable = SVITRUE;
}

void SVIRippleRenderer::processRippleInputs() {

	int MESH_SIZE_WIDTH = mWidth;
	int MESH_SIZE_HEIGHT = mHeight;

	int i, j, k, l, imin, jmin, imax, jmax;
	float dx, dy, acc = 0, innerAcc;

	size_t requestCount = mRippleRequest.size();

	for (size_t n = 0; n < requestCount; n++) {
		SVIVector3 info = mRippleRequest[n];
		float mx = info.x;
		float my = info.y;
		float intensity = info.z;

		mx = (mx / MESH_SIZE_WIDTH) * mNumDetailWidth;
		my = (my / MESH_SIZE_HEIGHT) * mNumDetailHeight;

		jmin = (mx > 5) ? (int) floor(mx - 3) : 2;
		imin = (my > 5) ? (int) floor(my - 3) : 2;

		jmax = (mx < mNumDetailWidth - 5) ? (int) floor(mx + 4) : (mNumDetailWidth - 1);
		imax = (my < mNumDetailHeight - 5) ? (int) floor(my + 4) : (mNumDetailHeight - 1);
		/*
		 for (i = imin; i < imax; i++) {
		 for (j = jmin; j < jmax; j++) {
		 dx = mx - j;
		 dy = my - i;
		 acc = 3 - (float) sqrtf(dx * dx + dy * dy);
		 if (acc > 0) {
		 mVelocityMap[j * mNumDetailHeight + i] += acc * intensity;
		 }
		 }
		 }
		 */
		// Large Ripple Model
		const int range = 8;

		if (imin - range < 2 || jmin - range < 2 || imax + range > mNumDetailHeight - 2
				|| jmax + range > mNumDetailWidth - 2) {
			for (i = imin; i < imax; i++) {
				for (j = jmin; j < jmax; j++) {
					dx = mx - j;
					dy = my - i;
					acc = 2 - (float) sqrtf(dx * dx + dy * dy);
					if (acc > 0) {
						mVelocityMap[j * mNumDetailHeight + i] += acc * intensity;
					}
				}
			}
		} else {
			// Large Ripple Model
			for (i = imin - range, k = 0; i < imax + range; i++, k++) {
				for (j = jmin - range, l = 0; j < jmax + range; j++, l++) {
					int nIndex = j * mNumDetailHeight + i;

					if (i < 5 || j < 5 || i > mNumDetailHeight - 5 || j > mNumDetailWidth - 5)
						continue;

					mVelocityMap[nIndex] += rippleVelocity[l * 23 + k] * 1.75;
					mHeightMap[nIndex] += rippleHeight[l * 23 + k] * 1.75;
				}
			}
		}
        mStable = SVIFALSE;
	}

	mRippleRequest.clear();
}

void SVIRippleRenderer::update() {

	processRippleInputs();

	int i, j, idx, imax, jmax;
	int xSpan = 1;
	int ySpan = 1;

	float tempVelocity;
	float reductionRate = mReductionRatio;
	float speed = REDUCTION_SPEED;
	float * heights_temp = mHeightMap;

	xSpan = spanXForPortrait;
	ySpan = spanYForPortrait;

	imax = mNumDetailHeight - ySpan;
	jmax = mNumDetailWidth - xSpan;

	bool bVelocityCheck = false;
	bool bControl = true;

	for (i = ySpan; i < imax; i++) {
		for (j = xSpan; j < jmax; j++) {
			idx = j * mNumDetailHeight + i;

			tempVelocity = -mHeightMap[idx] * 4.0f;

			idx -= mNumDetailHeight;
			tempVelocity += mHeightMap[idx];
			idx += mNumDetailHeight - 1;
			tempVelocity += mHeightMap[idx];

			idx += 2;
			tempVelocity += mHeightMap[idx];
			idx += mNumDetailHeight - 1;
			tempVelocity += mHeightMap[idx];
			idx -= mNumDetailHeight;

			mVelocityMap[idx] = (mVelocityMap[idx] + tempVelocity * speed) * reductionRate; //reductionRate

			if (bControl) {
				const float clip = mThreshhold;
				if (!bVelocityCheck && (mVelocityMap[idx] > clip || mVelocityMap[idx] < -clip))
					bVelocityCheck = true;
			}
		}
	}

	for (i = ySpan; i < imax; i++) {
		for (j = xSpan; j < jmax; j++) {
			idx = j * mNumDetailHeight + i;
			mHeightMap[idx] += mVelocityMap[idx];
			if (mHeightMap[idx] > 100)
				mHeightMap[idx] = 100;
			else if (mHeightMap[idx] < -100)
				mHeightMap[idx] = -100;
			heights_temp[idx] = mHeightMap[idx];
		}
	}

	// (DMC) Jacobi
	float reduction = 0.004f;
	float tempheight = 0.0f;

	for (i = ySpan; i < imax; i++) {
		for (j = xSpan; j < jmax; j++) {
			idx = j * mNumDetailHeight + i;
			tempheight = -heights_temp[idx] * 4.0f;
			idx -= mNumDetailHeight;
			tempheight += heights_temp[idx];
			idx += mNumDetailHeight - 1;
			tempheight += heights_temp[idx];
			idx += 2;
			tempheight += heights_temp[idx];
			idx += mNumDetailHeight - 1;
			tempheight += heights_temp[idx];
			idx -= mNumDetailHeight;

			mHeightMap[idx] = mHeightMap[idx] + tempheight * reduction;
		}
	}

	if (bVelocityCheck) {
        mStable = SVIFALSE;
		if (!mSVIGLSurface->getRenderPartManager()->isAnimating())
			mSVIGLSurface->getRenderPartManager()->requestRender();
	} else {
        if (mStable == SVIFALSE){
            if (!mSVIGLSurface->getRenderPartManager()->isAnimating())
                mSVIGLSurface->getRenderPartManager()->requestRender();
             mStable = SVITRUE;
        }
       
		for (i = ySpan; i < imax; i++) {
			for (j = xSpan; j < jmax; j++) {
				idx = j * mNumDetailHeight + i;
				mHeightMap[idx] = 0.0f;
				mVelocityMap[idx] = 0.0f;
			}
		}
        
	}

	int SURFACE_DETAILS_WIDTH = mSurfaceDetailWidth;
	int SURFACE_DETAILS_HEIGHT = mSurfaceDetailHeight;

	for (i = 0; i < SURFACE_DETAILS_HEIGHT; i++) {
		for (j = 0; j < SURFACE_DETAILS_WIDTH; j++) {
			idx = (j * SURFACE_DETAILS_HEIGHT + i);
			mHeights[idx].x = mHeightMap[(j + 2) * mNumDetailHeight + i + 2];
			mHeights[idx].y = mHeightMap[(j + 2) * mNumDetailHeight + i + 1];
			mHeights[idx].z = mHeightMap[(j + 1) * mNumDetailHeight + i + 2];
		}
	}
}

void SVIRippleRenderer::setReductionRatio(float ratio) {
	mReductionRatio = ratio;
}

void SVIRippleRenderer::setThreshhold(float t) {
	mThreshhold = t;
}

void SVIRippleRenderer::setEnvImage(SVIImage* image) {
	//2013-02-08 masterkeaton27@gmail.com
	//just for the test.
	//this thread lock should be treat with transaction mechanism

    if (image != NULL) {
        if (mEnvTexture == NULL) {
            mEnvTexture = new SVITexture(mSVIGLSurface);
        }
        image->mAlphaType = SVIImage::NORMAL;
        mEnvTexture->setImage(image);
        mSVIGLSurface->getTextureManager()->add(mEnvTexture);//Multi-Instance-Support
    } else {
        mEnvTexture->setImage(NULL);
        mSVIGLSurface->getTextureManager()->remove(mEnvTexture);//Multi-Instance-Support
    }
}

void SVIRippleRenderer::ripple(float mx, float my, float intensity) {
    mRippleRequest.push_back(SVIVector3(mx, my, intensity));

    if (!mSVIGLSurface->getRenderPartManager()->isAnimating())
        mSVIGLSurface->getRenderPartManager()->requestRender();

    mStable = SVIFALSE;
}

SVIBool SVIRippleRenderer::isAnimating(){
    return (mStable == SVITRUE) ? SVIFALSE : SVITRUE;
}

void SVIRippleRenderer::render(SVIUInt diffuseMap) {

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
//	  GL_ONE
//	  GL_SRC_COLOR
//	  GL_ONE_MINUS_SRC_COLOR
//	  GL_SRC_ALPHA
//	  GL_ONE_MINUS_SRC_ALPHA
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);


	SVIProgram * pProgram = mSVIGLSurface->getProgramManager()->getProgram(PR_POST_RIPPLE);

	pProgram->use();

	SVIProgramHandler * pHandler = pProgram->getHandler();

	//2013-02-08 masterkeaton27@gmail.com
	SVIUInt reflectionMap = (mEnvTexture != NULL) ? mEnvTexture->getTextureId() : diffuseMap;

	pHandler->setSampler(HD_U_SVIMPLER_0, reflectionMap);
	pHandler->setSampler(HD_U_SVIMPLER_1, diffuseMap);

	SVIMatrix viewMat;
	SVIMatrix projMat = mSVIGLSurface->getViewport()->getProjection();
	viewMat.setIdentity();
	viewMat._41 = (float) 0.0f;
	viewMat._42 = (float) 0.0f;

	pHandler->setUniform(HD_U_PROJ, projMat);
	pHandler->setUniform(HD_U_VIEW, viewMat);

	pHandler->setAttribute(HD_A_POSITION, mVertices);
	pHandler->setAttribute(HD_A_HEIGHTS, mHeights);

	float refractiveIndex = 1 - 0.07f; // wave distortion degree
	float reflectionRatio = 0.13f; // wave reflection degree

	SVIVector4 reserve01(1.01f, 1.0f, 1.0f, 1.0f); // alphaRatio1, alphaRatio2
	SVIVector4 reserve02(mWidth, mHeight, mNumDetailWidth, mNumDetailHeight);
	SVIVector4 reserve03(refractiveIndex, 1.0f, 1.0f, 1.0f);
	SVIVector4 reserve04(6.0f, 40.0f, 0.1f, 1.0f); //specularRatio , exponent, fresnelRatio

	pHandler->setUniform(HD_U_RESERVE01, reserve01);
	pHandler->setUniform(HD_U_RESERVE02, reserve02);
	pHandler->setUniform(HD_U_RESERVE03, reserve03);
	pHandler->setUniform(HD_U_RESERVE04, reserve04);

	glDrawElements(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_SHORT, mIndices);

	pHandler->releaseAttribute(HD_A_POSITION);
	pHandler->releaseAttribute(HD_A_HEIGHTS);
}

//2013-03-18 masterkeaton27@gmail.com
void SVIRippleRenderer::reset(){
    if (mVelocityMap == NULL || mHeightMap == NULL) return;

    mStable = SVITRUE;
    long MAX = mNumDetailWidth * mNumDetailHeight;
    for (long n = 0; n < MAX; n++) {
        mVelocityMap[n] = mHeightMap[n] = 0.0f;
    }
    mRippleRequest.clear();
}

void SVIRippleRenderer::reset(float width, float height) {

	if (height > width) {
		mNumDetailWidth = MIN_DETAIL;
		mNumDetailHeight = (int) ((float) mNumDetailWidth * (height / width));
	} else {
		mNumDetailHeight = MIN_DETAIL;
		mNumDetailWidth = (int) ((float) mNumDetailHeight * (width / height));
	}

    mStable = SVITRUE;
	mWidth = width;
	mHeight = height;

	// mesh initialize
	mSurfaceDetailWidth = mNumDetailWidth - 4;
	mSurfaceDetailHeight = mNumDetailHeight - 4;

	bool dataChanged = SVIFALSE;

	mVerticesCount = mSurfaceDetailWidth * mSurfaceDetailHeight;
	mIndicesCount = (mSurfaceDetailWidth - 1) * (mSurfaceDetailHeight - 1) * 6;

	long VCOUNT = mVerticesCount;
	long ICOUNT = mIndicesCount;
	long MAX = mNumDetailWidth * mNumDetailHeight;

	if (MAX != mMaxCount) {
		dataChanged = SVITRUE;
	}

	mReseted = SVITRUE;
	mMaxCount = MAX;

	if (dataChanged || mVertices) {
		SVI_SVIFE_DELETE_ARRAY(mVertices);
		SVI_SVIFE_DELETE_ARRAY(mHeights);
		SVI_SVIFE_DELETE_ARRAY(mIndices);
		SVI_SVIFE_DELETE_ARRAY(mHeightMap);
		SVI_SVIFE_DELETE_ARRAY(mVelocityMap);

		mVertices = new SVIVector3[VCOUNT];
		mHeights = new SVIVector3[VCOUNT];
		mIndices = new SVIUShort[ICOUNT];
		mHeightMap = new SVIFloat[MAX];
		mVelocityMap = new SVIFloat[MAX];
	}

	for (long n = 0; n < MAX; n++) {
		mVelocityMap[n] = mHeightMap[n] = 0.0f;
	}

	mRippleRequest.clear();

	Initialize();
}

void SVIRippleRenderer::finalize() {
	SVI_SVIFE_DELETE_ARRAY(mVertices);
	SVI_SVIFE_DELETE_ARRAY(mHeights);
	SVI_SVIFE_DELETE_ARRAY(mIndices);
	SVI_SVIFE_DELETE_ARRAY(mHeightMap);
	SVI_SVIFE_DELETE_ARRAY(mVelocityMap);
}

};
