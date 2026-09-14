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
	void Update();	   // Obj 이동이 시작하기 전 카메라 위치 기억용
	void LateUpdate(); // m_vScroll 업데이트 용
	//void Render();
	void Release(); 

public:
	VEC  GetScroll() const { return m_vScroll; }

public:
	void SetCameraPos(VEC vPoint);
	void SetCameraTarget(CObj* pObj);
	void MoveCamera(VEC vPoint);
	void AddCameraPos(VEC vPoint);
	void CameraShaking(int iStrength, double dDuration);

public:
	bool Culling(CObj* pObj) {} // 타일맵까지 하고 구현 예정, 카메라 화면 안에 들어오지 않는 Obj는 false를 리턴한다. 

private:
	static CCameraMgr* m_pInstance;

	VEC				m_vPreCameraPos; // 이번 프레임 시작하기 전에 카메라 위치
									 // 카메라가 이동하면 그걸 스크롤에 반영해야되기 때문에 기억해둠
	CCamera*		m_pCamera;
	VEC				m_vScroll;
};