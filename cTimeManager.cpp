#include "cTimeManager.hpp"
#include <ctime>

cTimeManager::cTimeManager(void)
{
}

cTimeManager::~cTimeManager(void)
{
}

cTimeManager* cTimeManager::GetInstance()
{
	static cTimeManager Instance;
	return &Instance;
}

void cTimeManager::Setup()
{
	m_dwCurrTime = clock();
	m_dwPrevTime = m_dwCurrTime;

	m_dwWorldTime = 0;
	m_dwElapsedTime = 0;
	m_dwFps = 0;
	m_dwFrame = 0;

	m_dwLimitTime = 0;
	m_dwFpsPlus = 0;
}

void cTimeManager::Update()
{
	m_dwPrevTime = m_dwCurrTime;
	m_dwCurrTime = clock();

	m_dwElapsedTime = m_dwCurrTime - m_dwPrevTime;
	m_dwWorldTime += m_dwElapsedTime;

	if(m_dwLimitTime < m_dwWorldTime)
	{
		m_dwLimitTime = m_dwWorldTime + 1000;
		m_dwFps = m_dwFrame;
		m_dwFpsPlus += m_dwFrame;
		m_dwFrame = 0;
	}

	else
	{
		m_dwFrame++;
	}
}

/*void cTimeManager::Render(HDC hdc)
{
	char	szBuff[128];
	SetBkMode(hdc, TRANSPARENT);

	wsprintf(szBuff, "World Time    : %d", m_dwWorldTime);
	TextOut(hdc, 10, 10, szBuff, strlen(szBuff));

	wsprintf(szBuff, "Elapsed Time  : %d", m_dwElapsedTime);
	TextOut(hdc, 10, 30, szBuff, strlen(szBuff));

	wsprintf(szBuff, "FPS           : %d", m_dwFps);
	TextOut(hdc, 10, 50, szBuff, strlen(szBuff));

	wsprintf(szBuff, "Total Frame   : %d", m_dwFpsPlus);
	TextOut(hdc, 10, 70, szBuff, strlen(szBuff));
}*/