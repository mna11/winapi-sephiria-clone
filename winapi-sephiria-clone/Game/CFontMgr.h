#pragma once

// 폰트 만들 때마다 FontFamily만들고, Font만들고, StringFormat 선언하는거 귀찮아서 만듬
// 원하는 폰트, 사이즈, 위치 넣어주면 DrawString 대신 해줌

enum class FONT_TYPE
{
	PIXEL_BIG,		// Unity TMP 아틀라스 추출한거 - 숫자용
	PIXEL_BOLD,		// ...
	PIXEL_SMALL,	// ...
	NORMAL,			// 갈무리 7
	END
};

class CFontMgr
{
private:
	CFontMgr();
	~CFontMgr();
	CFontMgr(const CFontMgr& rhs) = delete;
	CFontMgr& operator=(CFontMgr& rTileMgr) = delete;

public:
	static CFontMgr* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CFontMgr;
			m_pInstance->Initialize();
		}

		return m_pInstance;
	}

	static void	DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void Initialize();
	void Release(); 
	
public:
	void DrawString(Graphics* pGraphics,									// 그래픽스 (그릴 곳) 
					wstring& wstr,											// 출력할 문자열
					FONT_TYPE eFont,										// 폰트 타입
					RectF rc,												// 출력할 위치
					Color color,											// 문자열 색깔
					float fSize = -1.,										// 폰트 사이즈,    디폴트 값 m_vecFont 기준 사이즈
					StringAlignment alignment = StringAlignmentCenter,		// 가로 정렬 타입, 디폴트 값 중앙 
					StringAlignment lineAlignment = StringAlignmentCenter);	// 세로 정렬 타입, 디폴트 값 중앙 

private:
	static CFontMgr*	m_pInstance;

	vector<FontFamily*> m_vecFontFamily;
	vector<Font*>		m_vecFont;
	StringFormat		m_sf;
	SolidBrush			m_solidBrush;
};

