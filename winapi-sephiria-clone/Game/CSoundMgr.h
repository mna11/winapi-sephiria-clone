#pragma once

#include "Define.h"

class CSoundMgr
{
public:
	static CSoundMgr* GetInstance()
	{
		if (nullptr == m_pInstance)
		{
			m_pInstance = new CSoundMgr;
			m_pInstance->Initialize();
		}

		return m_pInstance;
	}
	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize();
	void Update();
	void Release();

public:
	void PlaySound(const TCHAR* pSoundKey, CHANNEL_GROUPID eID, float fVolume, float fSpeed = 1.f);
	void StopSound(CHANNEL_GROUPID eID);
	void StopAll();
	void SetChannelVolume(CHANNEL_GROUPID eID, float fVolume);

private:
	void LoadSoundFile();

private:
	static CSoundMgr* m_pInstance;

	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, Sound*> m_mapSound;

	// 브금 채널 그룹, 이펙트 채널 그룹
	ChannelGroup* m_pChannelGroup[toUType(CHANNEL_GROUPID::END)];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	System* m_pSystem;
};