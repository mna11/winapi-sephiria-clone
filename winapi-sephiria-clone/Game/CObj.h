#pragma once
class CObj abstract
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void	Initialize()		PURE;
	virtual int		Update()			PURE;
	virtual void	LateUpdate()		PURE;
	virtual void	Render(Graphics*)	PURE;
	virtual void	Release()			PURE;

public:
	const INFO&		GetInfo()		const			{ return m_tInfo; }
	const RECT&		GetRect()		const			{ return m_tRect; }
	const RENDERID&	GetRenderID()	const			{ return m_eRender; }
	const float&	GetSpeed()		const			{ return m_fSpeed; }
	const int&		GetRenderLayer()const			{ return m_iRenderLayer; }
	const float&	GetAngle()		const			{ return m_fAngle; }
	const VEC&		GetPrePoint()	const			{ return m_vPrePoint; }

public:
	void			SetPos(float _fX, float _fY)	{ m_tInfo.vPoint.fX = _fX; m_tInfo.vPoint.fY = _fY; }
	void			SetPos(VEC vec)					{ m_tInfo.vPoint = { vec }; }
	void			SetDead(bool bDead)				{ m_bDead = bDead; }
	void			SetAngle(float _fAngle)			{ m_fAngle = _fAngle; }
	void			SetFrameKey(const TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }
	void			SetFrame(int iStart, int iEnd, int iMotion, double dFrameSpeed);
	void			SetSpeed(float fSpeed)			{ m_fSpeed = fSpeed; }
	void			SetSize(VEC vec)				{ m_tInfo.vSize = vec; }
	virtual void	SetTarget(CObj* pObj) { if (nullptr != pObj) m_pTarget = pObj; }
	virtual	void	SetDamage(int iDamage, CObj* pObj = nullptr);

public:
	void			AddPos(float fDx, float fDy)	{ m_tInfo.vPoint.fX += fDx; m_tInfo.vPoint.fY += fDy; }
	void			AddPos(VEC vec) { m_tInfo.vPoint += vec; }

public:
	void			RefreshRect()					{ UpdateRect(); }

protected:
	virtual void	UpdateRect();
	virtual void	UpdateFrame(); 

protected:
	INFO			m_tInfo;
	RECT			m_tRect;
	FRAME			m_tFrame;
	RENDERID		m_eRender;
	
	VEC				m_vPrePoint;		// 이번 프레임 이동 전 위치

	int				m_iRenderLayer;		// RENDERID 내에서 레이어 위치 (0이 가장 낮은 것)
	float			m_fSpeed;	
	float			m_fAngle;
	bool			m_bDead;
	int				m_iHp;				// 체력

	CObj*			m_pTarget;

	const TCHAR*	m_pFrameKey;

	bool			m_bHit;				 // 피해를 입은 플래그
	double			m_dIframeTime;       // 피해를 입지 않는 i-frame 시간
	double			m_dHitElapseTime;    // 피해를 입은 뒤 경과한 시간
};

