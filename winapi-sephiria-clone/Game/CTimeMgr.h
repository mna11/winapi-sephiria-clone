#pragma once

class CTimeMgr
{
private:
	CTimeMgr();
	CTimeMgr(const CTimeMgr&) = delete;
	const CTimeMgr& operator=(const CTimeMgr&) = delete;
	~CTimeMgr();
public:
	static CTimeMgr* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTimeMgr;
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

public:
	void Initialize();
	void Update();
	void Release();

public:
	double GetDeltaTime() const { return m_dDT; }
	double GetTime(LARGE_INTEGER llStartTick) const
	{ 
		return static_cast<double>(m_llCurCount.QuadPart - llStartTick.QuadPart) / static_cast<double>(m_llFrequncyCount.QuadPart); 
	}
	double GetTime() const
	{
		return GetTime(m_llStartCount);
	}

private:
	static CTimeMgr* m_pInstance;

	LARGE_INTEGER m_llStartCount;    // 프로그램 시작 시 카운트
	LARGE_INTEGER m_llCurCount;      // 지금 프레임 카운트
	LARGE_INTEGER m_llPrevCount;     // 이전 프레임 카운트
	LARGE_INTEGER m_llFrequncyCount; // 1초 카운트 빈도

	TCHAR m_szFPS[32];
	UINT m_iFPS;

	int m_iCallCount;				 // 1초 동안 Update가 수행된 횟수

	double m_dDT;                    // 델타 타임 - 프레임 간 사이 시간
	double m_dAcc;                   // 델타 타임을 누적하는 변수 - 1초 판단 근거
};