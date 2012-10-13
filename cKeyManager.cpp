#include "cKeyManager.hpp"

cKeyManager::cKeyManager(void)
{
	for(int i=0; i<128; i++)
	{
		m_bOnceKeyDown[i]	= false;
		m_bOnceKeyUp[i]		= false;
		m_bKeyPress[i]		= false;
	}
}

cKeyManager::~cKeyManager(void)
{
}

cKeyManager* cKeyManager::GetInstance()
{
	static cKeyManager Instance;
	return &Instance;
}

bool cKeyManager::OnceKeyUp(int key)
{
	/* if(g_hWnd != GetFocus())	return false; */
	if(GetKeyState(key) & 8000)
	{
		if(m_bOnceKeyUp[key] == false)
		{
			m_bOnceKeyUp[key] = true;
		}
	}

	else if(m_bOnceKeyUp[key] == true)
	{
		m_bOnceKeyUp[key] = false;
		return true;
	}

	return false;
}

bool cKeyManager::OnceKeyDown(int key)
{
	/* if(g_hWnd != GetFocus())	return false; */
	if(GetKeyState(key) & 8000)
	{
		if(m_bOnceKeyDown[key] == false)
		{
			m_bOnceKeyDown[key] = true;
			return true;
		}
	}

	else if(m_bOnceKeyDown[key] == true)
	{
		m_bOnceKeyDown[key] = false;
	}

	return false;
}

bool cKeyManager::KeyPress(int key)
{
	/* if(g_hWnd != GetFocus())	return false; */

	if(GetKeyState(key) & 8000)
	{
		return true;
	}

	return false;
}
