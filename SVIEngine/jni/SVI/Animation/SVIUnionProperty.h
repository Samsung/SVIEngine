#ifndef __SVI_UNION_PROPERTY_H_
#define __SVI_UNION_PROPERTY_H_

#include "../SVICores.h"

namespace SVI {

	struct SVIUnionProperty {

	public:

		enum PropertyType {
			NOT_DEFINED = 0,
			FLOAT,
			POINT,
			SIZE,
			COLOR,
			RECT,
			VECTOR3,
			VECTOR4
		};

		//container type may be extended.
		SVIFloat mData[4];
		PropertyType mPropertyType;

	public:
		SVIUnionProperty();
		SVIUnionProperty(const SVIUnionProperty& rhs);
		SVIUnionProperty(const SVIFloat& rhs);
		SVIUnionProperty(const SVIPoint& rhs);
		SVIUnionProperty(const SVISize& rhs);
		SVIUnionProperty(const SVIColor& rhs);
		SVIUnionProperty(const SVIRect& rhs);
		SVIUnionProperty(const SVIVector3& rhs);
		SVIUnionProperty(const SVIVector4& rhs);

		SVIUnionProperty& operator = (const SVIUnionProperty& rhs);
		SVIUnionProperty& operator = (const SVIFloat& rhs);
		SVIUnionProperty& operator = (const SVIPoint& rhs);
		SVIUnionProperty& operator = (const SVISize& rhs);
		SVIUnionProperty& operator = (const SVIColor& rhs);
		SVIUnionProperty& operator = (const SVIRect& rhs);
		SVIUnionProperty& operator = (const SVIVector3& rhs);
		SVIUnionProperty& operator = (const SVIVector4& rhs);

		void get(SVIFloat& rhs);
		void get(SVIPoint& rhs);
		void get(SVISize& rhs);
		void get(SVIColor& rhs);
		void get(SVIRect& rhs);
		void get(SVIVector3& rhs);
		void get(SVIVector4& rhs);
	};

};

#endif //__SVI_UNION_PROPERTY_H_
