#pragma once

namespace SVI {


class SVIPolyClipInfo {
public:
	SVIPolyClipInfo(SVIFloat x, SVIFloat y, SVIFloat w, SVIFloat h) {
		setRegion(x, y, w, h);
	}
	virtual ~SVIPolyClipInfo() {

	}
public:
	SVIRect m_ClipRegion;
	void setRegion(SVIFloat x, SVIFloat y, SVIFloat w, SVIFloat h) {
		m_ClipRegion.setOrigin(SVIPoint(x, y));
		m_ClipRegion.setSize(SVISize(w, h));
	}
	void doBegin() {
		glEnable( GL_SCISSOR_TEST);
		glScissor(m_ClipRegion.mOrigin.mX, m_ClipRegion.mOrigin.mY,
				m_ClipRegion.mSize.mWidth, m_ClipRegion.mSize.mHeight);
	}

	void doEnd() {
		glDisable( GL_SCISSOR_TEST);
	}
};

}
;
