#include "../SVICores.h"
#include "SVI3DCamera.h"

//2011-11-07 masterkeaton27@gmail.com
namespace SVI{

	#define DEFAULT_FOV	SVI_DEGTORAD(60.0f);
	#define DEFAULT_ZNEAR 1.0f;
	#define DEFAULT_ZFAR 5000.0f;
	
	static const SVIFloat fOtrhoZMax = 50000.0f;

	//2011-11-07 masterkeaton27@gmail.com
	//should replace this delta value with real one.
	static const SVIFloat fDeltaTime = 1.0f / 60.0f;

	SVI3DCamera::SVI3DCamera(){
		m_Position = SVIVector3(0.0f, 0.0f, 0.0f);
		m_Direction = SVIVector3(0.0f, 0.0f, 1.0f);
		m_Up = SVIVector3(0.0f, 1.0f, 0.0f);
		m_FocalLength = 1.0f;
		memset(&m_Projection,0,sizeof(Projection));
	}

	SVI3DCamera::~SVI3DCamera(){}

	void SVI3DCamera::setFovXAspect(SVIFloat FovX, SVIFloat Aspect, SVIFloat ZNear, SVIFloat ZFar){
		m_Projection.m_ZNear = ZNear;
		m_Projection.m_ZFar = ZFar;
		m_Projection.m_Type = Type_Perspective;

		m_Projection.m_Aspect = Aspect;
		m_Projection.m_Fov = FovX;
		m_Projection.m_Width = 2.0f * m_Projection.m_ZNear * tanf(FovX/2.0f);
		m_Projection.m_Height = m_Projection.m_Width / Aspect;
	}

	void SVI3DCamera::setOrthogonal(SVIFloat Width, SVIFloat Height, SVIFloat ZNear, SVIFloat ZFar){
		m_Projection.m_ZNear = ZNear;
		m_Projection.m_ZFar = ZFar;
		m_Projection.m_Type = Type_Orthogonal;

		m_Projection.m_Width = Width;
		m_Projection.m_Height = Height;
	}

	float SVI3DCamera::getFovX(){
		return m_Projection.m_Fov;
	}

	SVIMatrix SVI3DCamera::getViewMatrix() {
		SVIMatrix cameraWorld;
		cameraWorld = m_CameraMatrix.inverse(NULL);
		cameraWorld._31 *= -1.0f;
		cameraWorld._32 *= -1.0f;
		cameraWorld._33 *= -1.0f;
		cameraWorld._34 *= -1.0f;
		return cameraWorld;
	}

	SVIMatrix SVI3DCamera::getProjMatrix() {
		if(m_Projection.m_Type == Type_Perspective) {
			SVIFloat fov = DEFAULT_FOV;
			SVIFloat zNear = DEFAULT_ZNEAR;
			SVIFloat zFar = DEFAULT_ZFAR;
			//return SVIMatrix::perspective(fov, (SVIFloat)m_Projection.m_Width / (SVIFloat)m_Projection.m_Height, zNear, zFar);
			return SVIMatrix::perspectiveLH(m_Projection.m_Width, m_Projection.m_Height, m_Projection.m_ZNear, m_Projection.m_ZFar);
		} else if(m_Projection.m_Type == Type_Orthogonal) {
			return SVIMatrix::orthoLH(m_Projection.m_Width, m_Projection.m_Height, m_Projection.m_ZNear, m_Projection.m_ZFar);
		}
		return SVIMatrix();
	}

	void SVI3DCamera::updateCameraMatrixFrom(){ 
		SVIVector3 X = m_Up.Cross(m_Direction);
		m_CameraMatrix = SVIMatrix(X.x, X.y, X.z, 0.0f,
			m_Up.x, m_Up.y, m_Up.z, 0.0f,
			m_Direction.x, m_Direction.y, m_Direction.z, 0.0f,
			m_Position.x, m_Position.y, m_Position.z, 1.0f);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	SVI3DFreeCamera::SVI3DFreeCamera()
	{
		m_BaseSpeed = 1.0f;
		m_RotationSpeed = 0.0015f;
		m_MoveSpeed = 100.0f;
	
		m_ViewMode = ViewMode_Null;
		setViewMode(ViewMode_Perspective);
	}

	SVI3DFreeCamera::~SVI3DFreeCamera(){}

	void SVI3DFreeCamera::setViewMode(ViewMode Mode)
	{
		if(m_ViewMode == Mode) return;
		m_ViewMode = Mode;
		switch(m_ViewMode) {
		case ViewMode_Perspective:
			m_Position = SVIVector3(100.0f, 100.0f, -100.0f);
			m_Direction = -m_Position;
			m_Direction.normalize();
			m_Up = SVIVector3(0.0f, 1.0f, 0.0f);
			m_Up = m_Direction.Cross(m_Direction.Cross(m_Up));
			m_Projection.m_Type = Type_Perspective;
			updateCameraMatrixFrom();
			break;
		case ViewMode_Top:
			m_Direction = SVIVector3(0.0f, -1.0f, 0.0f);
			m_Up = SVIVector3(0.0f, 0.0f, 1.0f);
			m_Position = SVIVector3(0.0f, fOtrhoZMax*0.5f, 0.0f);
			m_Projection.m_Width = 250.0f;
			m_Projection.m_Type = Type_Orthogonal;
			updateCameraMatrixFrom();
			break;
		case ViewMode_Left:
			m_Direction = SVIVector3(1.0f, 0.0f, 0.0f);
			m_Up = SVIVector3(0.0f, 1.0f, 0.0f);
			m_Position = SVIVector3(-fOtrhoZMax*0.5f, 0.0f, 0.0f);
			m_Projection.m_Width = 250.0f;
			m_Projection.m_Type = Type_Orthogonal;
			updateCameraMatrixFrom();
			break;
		case ViewMode_Front:
			m_Direction = SVIVector3(0.0f, 0.0f, 1.0f);
			m_Up = SVIVector3(0.0f, 1.0f, 0.0f);
			m_Position = SVIVector3(0.0f, 0.0f, -fOtrhoZMax*0.5f);
			m_Projection.m_Width = 250.0f;
			m_Projection.m_Type = Type_Orthogonal;
			updateCameraMatrixFrom();
			break;

        default:
            break;
		}
	}

	void SVI3DFreeCamera::updateCamera(SVIFloat fAspect, SVIFloat fFov, SVIFloat fZNear, SVIFloat fZFar)
	{
		if(m_Projection.m_Type == Type_Perspective) updateCameraForPerspective(fAspect, fFov, fZNear, fZFar);
		else if(m_Projection.m_Type == Type_Orthogonal) updateCameraForOrtho(fAspect);
	}

	void SVI3DFreeCamera::updateCameraForOrtho(SVIFloat fAspect)
	{
		const SVIFloat fWidthMin = 1.0f;
		const SVIFloat fRef = 250.0f;
		const SVIFloat fFar = fOtrhoZMax;
		const SVIFloat fNear = 1.0f;

		SVIFloat fWidth = m_Projection.m_Width;

		const SVIFloat TRANSLATION_VECTOR_THREHOLD = 0.1f;
		{
			
			SVIFloat fTranslationSpeed = m_MoveSpeed*m_BaseSpeed*fDeltaTime*(fWidth/fRef);

			fWidth += m_Input.m_RotateY * fTranslationSpeed / 100.0f;
			fWidth = SVIMAX(fWidthMin, fWidth);

			SVIVector3 Xv = *(SVIVector3*)&m_CameraMatrix[0];
			SVIVector3 Yv = *(SVIVector3*)&m_CameraMatrix[4];
			
			SVIVector3 Offset(0.0f, 0.0f, 0.0f);
			if(Yv.getlength() > TRANSLATION_VECTOR_THREHOLD) {
				Yv.normalize();
				Offset += Yv * m_Input.m_MoveForwardAndBackward * fTranslationSpeed;
			}
			if(Xv.getlength() > TRANSLATION_VECTOR_THREHOLD) {
				Xv.normalize();
				Offset += Xv * m_Input.m_MoveLeftAndRight * fTranslationSpeed;
			}
			SVIMatrix Trans;
			Trans._41 = Offset.x;
			Trans._42 = Offset.y;
			Trans._43 = Offset.z;
			m_CameraMatrix = m_CameraMatrix * Trans;
		}

		setOrthogonal(fWidth, fWidth/fAspect, fNear, fFar);

		
		//2011-11-07 masterkeaton27@gmail.com
		//with orthogonal mode, up vector calculation would have a strange value. 
		//so, should maintain with a fixed value.
		// m_Up = GetYBasis(m_CameraMatrix);
		m_Direction = Util::getZBasis(m_CameraMatrix);
		m_Position = Util::getWBasis(m_CameraMatrix);

		m_Direction.normalize();

		resetInput();
	}

	void SVI3DFreeCamera::updateCameraForPerspective(SVIFloat fAspect, SVIFloat fFov, SVIFloat fZNear, SVIFloat fZFar)
	{
		setFovXAspect(fFov, fAspect, fZNear, fZFar);

		const SVIFloat TRANSLATION_VECTOR_THREHOLD = 0.1f;
		const SVIFloat ROTATION_VECTOR_THREHOLD = 0.1f;

		SVIFloat fRotationSpeed = m_RotationSpeed*fDeltaTime;
		SVIFloat fTranslationSpeed = m_MoveSpeed*m_BaseSpeed*fDeltaTime;

		SVIVector3 Rot(0.0f, 0.0f, 0.0f);
		{
			SVIVector3 Zv = *(SVIVector3*)&m_CameraMatrix[8];
			Zv.normalize();

			SVIVector3 Pv = *(SVIVector3*)&m_CameraMatrix[12];
			SVIVector3 LookAt = Pv + Zv*m_FocalLength;
			SVIMatrix Trans;
			Trans._41 = LookAt.x;
			Trans._42 = LookAt.y;
			Trans._43 = LookAt.z;
			SVIMatrix InvTrans;
			InvTrans._41 = -LookAt.x;
			InvTrans._42 = -LookAt.y;
			InvTrans._43 = -LookAt.z;
			Rot.x -= m_Input.m_RotateX * fRotationSpeed;
			Rot.y -= m_Input.m_RotateY * fRotationSpeed;

			SVIVector3 Xv = *(SVIVector3*)&m_CameraMatrix[0];
			SVIVector3 Yv = *(SVIVector3*)&m_CameraMatrix[4];

			Xv[1] = 0.0f;
			Yv = SVIVector3(0.0f, 1.0f, 0.0f);

			m_CameraMatrix = m_CameraMatrix * InvTrans;

			SVIMatrix MatRot;
			if(Xv.getlength() > ROTATION_VECTOR_THREHOLD && fabsf(Rot.y) > 0.00001f) {
				Xv.normalize();
				MatRot.setRotation(Xv, Rot.y);
				m_CameraMatrix = m_CameraMatrix * MatRot;
			}
			if(Yv.getlength() > ROTATION_VECTOR_THREHOLD && fabsf(Rot.x) > 0.00001f) {
				Yv.normalize();
				MatRot.setRotation(Yv, Rot.x);
				m_CameraMatrix = m_CameraMatrix * MatRot;
			}
			//MatRot.SetRotation(Zv, Rot.z);
			//m_CameraMatrix = m_CameraMatrix * MatRot;
			m_CameraMatrix = m_CameraMatrix * Trans;
		}
		{
			SVIVector3 Xv = *(SVIVector3*)&m_CameraMatrix[0];
			SVIVector3 Yv = *(SVIVector3*)&m_CameraMatrix[4];
			SVIVector3 Zv = *(SVIVector3*)&m_CameraMatrix[8];

			Xv[1] = 0.0f;
			Yv = SVIVector3(0.0f, 1.0f, 0.0f);
			Zv[1] = 0.0f;

			SVIVector3 Offset(0.0f, 0.0f, 0.0f);
			if(Yv.getlength() > TRANSLATION_VECTOR_THREHOLD) {
				Yv.normalize();
				Offset += Yv * m_Input.m_MoveUpAndDown * fTranslationSpeed;
			}
			if(Xv.getlength() > TRANSLATION_VECTOR_THREHOLD) {
				Xv.normalize();
				Offset += Xv * m_Input.m_MoveLeftAndRight * fTranslationSpeed;
			}
			if(Zv.getlength() > TRANSLATION_VECTOR_THREHOLD) {
				Zv.normalize();
				Offset += Zv * m_Input.m_MoveForwardAndBackward * fTranslationSpeed;
			}
			SVIMatrix Trans;
			Trans._41 = Offset.x;
			Trans._42 = Offset.y;
			Trans._43 = Offset.z;
			m_CameraMatrix = m_CameraMatrix * Trans;
		}

		m_Up = Util::getYBasis(m_CameraMatrix);
		m_Direction = Util::getZBasis(m_CameraMatrix);
		m_Position = Util::getWBasis(m_CameraMatrix);

		resetInput();
	}

	void SVI3DFreeCamera::resetInput()
	{
		m_Input = Input();
	}


}
