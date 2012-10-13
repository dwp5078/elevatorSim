#ifndef __CTIMEMANAGER_H__
#define __CTIMEMANAGER_H__

#include <windows.h>
#include <mmsystem.h>

class cTimeManager
{
private:
	DWORD	m_dwPrevTime;
	DWORD	m_dwCurrTime;

	DWORD	m_dwWorldTime;
	DWORD	m_dwElapsedTime;
	DWORD	m_dwFps;
	DWORD	m_dwFrame;

	DWORD	m_dwLimitTime;
	DWORD	m_dwFpsPlus;

	cTimeManager(void);

public:
	~cTimeManager(void);

	static	cTimeManager* GetInstance();

	inline	DWORD	GetElapsedTime()	{	return m_dwElapsedTime;	}
	inline	DWORD	GetWorldTime()		{	return m_dwWorldTime;	}
	inline	DWORD	GetFPS()			{	return m_dwFps;	}
   inline   DWORD GetTotalFrame()   {  return m_dwFpsPlus;  }


	void Setup();
	void Update();
	//void Render(HDC hdc);
};

#endif //__CTIMEMANAGER_H__