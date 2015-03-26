#include "SVIUnionProperty.h"

namespace SVI {

	SVIUnionProperty::SVIUnionProperty() {
		memset(mData, 0x00, sizeof(mData));
		mPropertyType = NOT_DEFINED;
	}

	SVIUnionProperty::SVIUnionProperty(const SVIUnionProperty& rhs) {
		memcpy(mData, rhs.mData, sizeof(mData));
		mPropertyType = rhs.mPropertyType;
	}

	SVIUnionProperty::SVIUnionProperty(const SVIFloat& rhs) {
		memcpy(mData, &rhs, sizeof(rhs));
		mPropertyType = FLOAT;
	}

	SVIUnionProperty::SVIUnionProperty(const SVIPoint& rhs) {
		memcpy(mData, &rhs, sizeof(rhs));
		mPropertyType = POINT;
	}

	SVIUnionProperty::SVIUnionProperty(const SVISize& rhs) {
		memcpy(mData, &rhs, sizeof(rhs));
		mPropertyType = SIZE;
	}

	SVIUnionProperty::SVIUnionProperty(const SVIColor& rhs) {
		memcpy(mData, &rhs, sizeof(rhs));
		mPropertyType = COLOR;
	}

	SVIUnionProperty::SVIUnionProperty(const SVIRect& rhs) {	
		memcpy(mData, &rhs, sizeof(rhs));
		mPropertyType = RECT;
	}

	SVIUnionProperty::SVIUnionProperty(const SVIVector3& rhs) {
		memcpy(mData, &rhs, sizeof(rhs));
		mPropertyType = VECTOR3;
	}

	SVIUnionProperty::SVIUnionProperty(const SVIVector4& rhs) {
		memcpy(mData, &rhs, sizeof(rhs));
		mPropertyType = VECTOR4;
	}

	SVIUnionProperty& SVIUnionProperty::operator = (const SVIUnionProperty& rhs) {
		memcpy(mData, rhs.mData, sizeof(mData));
		mPropertyType = rhs.mPropertyType;
		return *this;
	}

	SVIUnionProperty& SVIUnionProperty::operator = (const SVIFloat& rhs) {
		memcpy(mData, &rhs, sizeof(rhs));
		mPropertyType = FLOAT;
		return *this;
	}

	SVIUnionProperty& SVIUnionProperty::operator = (const SVIPoint& rhs) {
		memcpy(mData, &rhs, sizeof(rhs));
		mPropertyType = POINT;
		return *this;
	}

	SVIUnionProperty& SVIUnionProperty::operator = (const SVISize& rhs) {
		memcpy(mData, &rhs, sizeof(rhs));
		mPropertyType = SIZE;
		return *this;
	}

	SVIUnionProperty& SVIUnionProperty::operator = (const SVIColor& rhs) {
		memcpy(mData, &rhs, sizeof(rhs));
		mPropertyType = COLOR;
		return *this;
	}

	SVIUnionProperty& SVIUnionProperty::operator = (const SVIRect& rhs) {
		memcpy(mData, &rhs, sizeof(rhs));
		mPropertyType = RECT;
		return *this;
	}

	SVIUnionProperty& SVIUnionProperty::operator = (const SVIVector3& rhs) {
		memcpy(mData, &rhs, sizeof(rhs));
		mPropertyType = VECTOR3;
		return *this;
	}

	SVIUnionProperty& SVIUnionProperty::operator = (const SVIVector4& rhs) {
		memcpy(mData, &rhs, sizeof(rhs));
		mPropertyType = VECTOR4;
		return *this;
	}

	void SVIUnionProperty::get(SVIFloat& rhs) {
		memcpy(&rhs, mData, sizeof(rhs));
	}

	void SVIUnionProperty::get(SVIPoint& rhs) {
		memcpy(&rhs, mData, sizeof(rhs));
	}

	void SVIUnionProperty::get(SVISize& rhs) {
		memcpy(&rhs, mData, sizeof(rhs));
	}

	void SVIUnionProperty::get(SVIColor& rhs) {
		memcpy(&rhs, mData, sizeof(rhs));
	}

	void SVIUnionProperty::get(SVIRect& rhs) {
		memcpy(&rhs, mData, sizeof(rhs));
	}

	void SVIUnionProperty::get(SVIVector3& rhs) {
		memcpy(&rhs, mData, sizeof(rhs));
	}

	void SVIUnionProperty::get(SVIVector4& rhs) {
		memcpy(&rhs, mData, sizeof(rhs));
	}

};