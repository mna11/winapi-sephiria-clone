#pragma once

class CImgMgr
{
private:
	CImgMgr();
	~CImgMgr();
	CImgMgr(const CImgMgr& rhs) = delete;
	CImgMgr& operator=(CImgMgr& rBmpMgr) = delete;

public:
	static CImgMgr* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new CImgMgr;

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
	void Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

public:
	void	InsertImg(const TCHAR* pFilePath, const TCHAR* pImgKey);
	Image*	FindImg(const TCHAR* pImgKey) const;

private:
	static CImgMgr* m_pInstance;
	map<const TCHAR*, Image*>	m_mapImg;
};

