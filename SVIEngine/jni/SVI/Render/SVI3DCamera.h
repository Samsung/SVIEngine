#pragma once


namespace SVI{

	struct SVIVector3;
	struct SVIMatrix;


	enum ProjectionType {
		Type_Perspective = 0,
		Type_Orthogonal,
	};


	struct Projection {
		ProjectionType	m_Type;
		float	m_Fov, m_Aspect;
		float	m_Width, m_Height;
		float	m_ZNear, m_ZFar;
	};

	class SVI3DCamera {
	public:
		SVI3DCamera();
		virtual ~SVI3DCamera();

	public:
		Projection	m_Projection;

		void		setPosition(SVIVector3& Position); 
		SVIVector3&	getPosition() { return m_Position; };
		void		setDirection(SVIVector3& Direction); 
		SVIVector3&	getDirection() { return m_Direction; };
		void		setUp(SVIVector3& Up); 
		SVIVector3&	getUp() { return m_Up; };

	protected:
		SVIVector3		m_Position;
		SVIVector3		m_Direction;
		SVIVector3		m_Up;
		SVIMatrix	m_CameraMatrix;

	protected:
		void updateCameraMatrixFrom();

	public:
		SVIFloat		m_FocalLength;

	public:
		void setFovXAspect(SVIFloat FovX, SVIFloat Aspect, SVIFloat ZNear, SVIFloat ZFar);
		void setOrthogonal(SVIFloat Width, SVIFloat Height, SVIFloat ZNear, SVIFloat ZFar);
		SVIFloat getFovX();

		SVIMatrix getProjMatrix();
		SVIMatrix getViewMatrix();
	};


	class SVI3DFreeCamera : public SVI3DCamera {
	public:
		SVI3DFreeCamera();
		virtual ~SVI3DFreeCamera();

		enum ViewMode {
			ViewMode_Perspective = 0,
			ViewMode_Top,
			ViewMode_Left,
			ViewMode_Front,
			ViewMode_Null
		};
		struct Input {
			Input() 
			{
				m_RotateX = m_RotateY = m_MoveUpAndDown = m_MoveForwardAndBackward = m_MoveLeftAndRight = 0.0f;
			};
			SVIFloat	m_RotateX, m_RotateY;
			SVIFloat	m_MoveUpAndDown;
			SVIFloat	m_MoveForwardAndBackward;
			SVIFloat	m_MoveLeftAndRight;
		};
		Input		m_Input;

	private:
		ViewMode	m_ViewMode;

	public:
		void updateCamera(SVIFloat fAspect, SVIFloat fFov, SVIFloat fZNear, SVIFloat fZFar);
		void updateCameraForPerspective(SVIFloat fAspect, SVIFloat fFov, SVIFloat fZNear, SVIFloat fZFar);
		void updateCameraForOrtho(SVIFloat fAspect);
		void setViewMode(ViewMode Mode);
		ViewMode getViewMode() { return m_ViewMode; };
		void resetInput();

	public:
		SVIFloat	m_BaseSpeed;
		SVIFloat	m_RotationSpeed;
		SVIFloat	m_MoveSpeed;
	};


}