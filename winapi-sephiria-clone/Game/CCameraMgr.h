#pragma once

class CCamera;
class CObj;

class CCameraMgr
{
private:
	CCameraMgr();
	~CCameraMgr();
	CCameraMgr(const CCameraMgr& rhs) = delete;
	CCameraMgr& operator=(CCameraMgr& rScrollMgr) = delete;

public:
	static CCameraMgr* GetInstance()
	{
		if (!m_pInstance) {
			m_pInstance = new CCameraMgr;
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
	/*void Update();
	void LateUpdate();
	void Render();*/
	void Release(); 

public:
	int GetScrollX() const { return m_iScrollX; }
	int GetScrollY() const { return m_iScrollY; }

public:
	void SetCameraPos(float fX, float fY);
	void SetCameraTarget(CObj* pObj);
	void MoveCamera(const CVector& rhs, float fSpeed);

public:
	bool Culling(CObj* pObj); // 카메라 화면 안에 들어오지 않는 Obj는 false를 리턴한다. 

private:
	static CCameraMgr* m_pInstance;

	CCamera*		m_pCamera;

	int			m_iScrollX;
	int			m_iScrollY;
};