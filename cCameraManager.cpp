#include "ElevatorSim.hpp"
#include "cCameraManager.hpp"
#include "cKeyManager.hpp"
#include "cTimeManager.hpp"

#include <FL/Enumerations.H>

namespace elevatorSim {
	const float MOVE = 0.07f;
	const float ROT = 0.1f;

	cCameraManager::cCameraManager(
		const cKeyManager& _keyManager, 
		const cTimeManager& _timeManager) :
	keyManager(_keyManager), 
		timeManager(_timeManager) {
			ReinitCamera();
	}

	void cCameraManager::Update()
	{
		/* TODO: use timeManager.getLastFrameTime().total_milliseconds() */

		float move = MOVE;
		float rot = ROT;

		Vec3f right = GetRight() * move;

		if(keyManager.isDown(FL_Page_Up)) {
			//m_vecCamPos += m_vecCamUp * move;
			//m_vecCamLookAt += m_vecCamUp * move;
			m_vecCamPos.SetY(m_vecCamPos.y() + move);
			m_vecCamLookAt.SetY(m_vecCamLookAt.y() + move);
		}

		if(keyManager.isDown(FL_Page_Down))    {
			//m_vecCamPos -= m_vecCamUp * move;
			//m_vecCamLookAt -= m_vecCamUp * move;
			m_vecCamPos.SetY(m_vecCamPos.y() - move);
			m_vecCamLookAt.SetY(m_vecCamLookAt.y() - move);
		}

		if(keyManager.isDown(FL_Right))    {
			//m_vecCamPos -= right * move;
			//m_vecCamLookAt -= right * move;
			m_vecCamPos.SetX(m_vecCamPos.x() + move);
			m_vecCamLookAt.SetX(m_vecCamLookAt.x() + move);
		}

		if(keyManager.isDown(FL_Left))    {
			//m_vecCamPos += right * move;
			//m_vecCamLookAt += right * move;
			m_vecCamPos.SetX(m_vecCamPos.x() - move);
			m_vecCamLookAt.SetX(m_vecCamLookAt.x() - move);
		}

		if(keyManager.isDown(FL_Up))    {
			m_vecCamPos.SetZ(m_vecCamPos.z() - move);
		}

		if(keyManager.isDown(FL_Down))    {
			m_vecCamPos.SetZ(m_vecCamPos.z() + move);
		}

		if(keyManager.isDown('W'))      Pitch(-rot);
		if(keyManager.isDown('S'))      Pitch(rot);

		if(keyManager.isDown('A'))      Yaw(-rot);
		if(keyManager.isDown('D'))      Yaw(rot);

		if(keyManager.isDown('R'))      Roll(-rot);
		if(keyManager.isDown('F'))      Roll(rot);

		if(keyManager.isDown(' ')) ReinitCamera();
	}


	void cCameraManager::Render()
	{
		glRotatef(m_fPitchAngle, 1, 0, 0);
		glRotatef(m_fYawAngle, 0, 1, 0);
		glRotatef(m_fRollAngle, 0, 0, 1);

		gluLookAt(m_vecCamPos.x(), m_vecCamPos.y(), m_vecCamPos.z(),
			m_vecCamLookAt.x(), m_vecCamLookAt.y(), m_vecCamLookAt.z(),
			m_vecCamUp.x(), m_vecCamUp.y(), m_vecCamUp.z());
	}


	void cCameraManager::Yaw(float fAngle)
	{
		m_fYawAngle += fAngle;
	}


	void cCameraManager::Pitch(float fAngle)
	{
		m_fPitchAngle += fAngle;
	}


	void cCameraManager::Roll(float fAngle)
	{
		m_fRollAngle += fAngle;
	}


	void cCameraManager::ReinitCamera()
	{
		m_vecCamPos.Set(0.f, 0.f, 20.f);
		m_vecCamLookAt.Set(0.f, 0.f, 0.f);
		m_vecCamUp.Set(0.0f, 1.0f, 0.0f);

		Vec3f lookingVec = m_vecCamPos - m_vecCamLookAt;
		Vec3f right = GetRight();
		Vec3f::Cross3(m_vecCamUp, right, lookingVec);

		m_fPitchAngle = 0.f;
		m_fYawAngle = 0.f;
		m_fRollAngle = 0.f;
	}


	Vec3f cCameraManager::GetRight()
	{
		Vec3f right;
		Vec3f lookingVec = m_vecCamPos - m_vecCamLookAt;
		lookingVec.Normalize();
		Vec3f::Cross3(right, lookingVec, m_vecCamUp);
		right.Normalize();

		return right;
	}
} /* namespace elevatorSim */
