#include "pch.h"
#include "CSoundMgr.h"


CSoundMgr* CSoundMgr::m_pInstance = nullptr;
CSoundMgr::CSoundMgr()
{
	m_pSystem = nullptr;
	ZeroMemory(&m_pChannelGroup, sizeof(ChannelGroup*) * toUType(CHANNEL_GROUPID::END));
}


CSoundMgr::~CSoundMgr()
{
	Release();
}

void CSoundMgr::Initialize()
{
	// 사운드를 담당하는 대표객체를 생성하는 함수
	System_Create(&m_pSystem);
	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	m_pSystem->init(32, FMOD_INIT_NORMAL, nullptr);
	m_pSystem->createChannelGroup("BGM", &m_pChannelGroup[toUType(CHANNEL_GROUPID::BGM)]);
	m_pSystem->createChannelGroup("SFX", &m_pChannelGroup[toUType(CHANNEL_GROUPID::SFX)]);

	LoadSoundFile();
}

void CSoundMgr::Update()
{
	if (nullptr != m_pSystem)
		m_pSystem->update();
}

void CSoundMgr::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		Mypair.second->release();
	}
	m_mapSound.clear();

	for (auto& ChanGroup : m_pChannelGroup)
	{
		if (nullptr != ChanGroup)
		{
			ChanGroup->release();
			ChanGroup = nullptr;
		}
	}

	if (nullptr != m_pSystem)
	{
		m_pSystem->close();
		m_pSystem->release();
		m_pSystem = nullptr;
	}

}


void CSoundMgr::PlaySound(const TCHAR* pSoundKey, CHANNEL_GROUPID eID, float fVolume, float fSpeed)
{
	map<TCHAR*, Sound*>::iterator iter;

	// 사운드 찾기
	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	// BGM이면 일단 기존을 멈추고 실행
	if (eID == CHANNEL_GROUPID::BGM)
		StopSound(CHANNEL_GROUPID::BGM);

	Channel* pChannel = nullptr;
	FMOD_RESULT eResult = m_pSystem->playSound(iter->second, m_pChannelGroup[toUType(eID)], false, &pChannel);
	if (eResult == FMOD_OK)
	{
		pChannel->setVolume(fVolume);

		// 만약 BGM이었다면, 무한 루프를 켜줌
		if (eID == CHANNEL_GROUPID::BGM)
			pChannel->setMode(FMOD_LOOP_NORMAL);

		pChannel->setPitch(fSpeed);
	}

}

void CSoundMgr::StopSound(CHANNEL_GROUPID eID)
{
	if (nullptr != m_pChannelGroup[toUType(eID)])
		m_pChannelGroup[toUType(eID)]->stop();
}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < toUType(CHANNEL_GROUPID::END); ++i)
		StopSound(static_cast<CHANNEL_GROUPID>(i));
}

void CSoundMgr::SetChannelVolume(CHANNEL_GROUPID eID, float fVolume)
{
	if (nullptr != m_pChannelGroup[toUType(eID)])
		m_pChannelGroup[toUType(eID)]->setVolume(fVolume);
}

void CSoundMgr::LoadSoundFile()
{
	// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
	_finddata_t fd;

	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
	intptr_t handle = _findfirst("../Resource/Sound/BGM/*.*", &fd);

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "../Resource/Sound/BGM/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);

		// "../Sound/Success.wav"
		strcat_s(szFullPath, fd.name);

		Sound* pSound = nullptr;
	
		FMOD_RESULT eRes = m_pSystem->createSound(szFullPath, FMOD_DEFAULT, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			// 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.insert({ pSoundKey, pSound });
		}
		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 
		// 더이상 없다면 -1을 리턴
		iResult = _findnext(handle, &fd);
	}

	m_pSystem->update();

	_findclose(handle);
}
