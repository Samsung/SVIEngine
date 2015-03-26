#include "../../SVICores.h"

#include "SVIPolyClipInfo.h"

namespace SVI {
	SVIPolyClipInfo::SVIPolyClipInfo(SVIFloat x, SVIFloat y, SVIFloat w, SVIFloat h) {
		setRegion(x, y, w, h);
	}
	
	SVIPolyClipInfo::~SVIPolyClipInfo(){}

	void SVIPolyClipInfo::setRegion(SVIFloat x, SVIFloat y, SVIFloat w, SVIFloat h) {
		m_ClipRegion.setOrigin(SVIPoint(x, y));
		m_ClipRegion.setSize(SVISize(w, h));
	}
	void SVIPolyClipInfo::doBegin() {
		glEnable( GL_SCISSOR_TEST);
		glScissor(m_ClipRegion.mOrigin.mX, m_ClipRegion.mOrigin.mY,
			m_ClipRegion.mSize.mWidth, m_ClipRegion.mSize.mHeight);
	}

	void SVIPolyClipInfo::doEnd() {
		glDisable( GL_SCISSOR_TEST);
	}
}
