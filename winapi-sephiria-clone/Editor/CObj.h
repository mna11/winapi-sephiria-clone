#pragma once
class CObj abstract
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize()	 PURE;
	virtual int  Update()	     PURE;
	virtual void LateUpdate()	 PURE;
	virtual void Render(Graphics*) PURE;
	virtual void Release()       PURE;

public:
	const INFO&		GetInfo() const		{ return m_tInfo; }
	const RECT&		GetRect() const		{ return m_tRect; }
	RENDERID		GetRenderID() const { return m_eRender; }
	float			GetSpeed() const	{ return m_fSpeed; }

public:
	void			SetPos(float _fX, float _fY) { m_tInfo.vPoint.fX = _fX; m_tInfo.vPoint.fY = _fY; }
	void			SetPos(VEC vec)				 { m_tInfo.vPoint = { vec }; }
	void			AddPos(VEC vec)				 { m_tInfo.vPoint += vec; }
	void			SetDead(bool bDead) { m_bDead = bDead; }
	void			SetFrame(int iStart, int iEnd, int iMotion, double dFrameSpeed);
	void			SetAngle(float _fAngle) { m_fAngle = _fAngle; }
	void			SetTarget(CObj* pObj) { if(nullptr != pObj) m_pTarget = pObj; }

protected:
	virtual void UpdateRect();
	virtual void MoveFrame(); 

protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	FRAME		m_tFrame;
	RENDERID	m_eRender;

	float		m_fSpeed;
	float		m_fAngle;
	bool		m_bDead;

	CObj*		m_pTarget;

	const TCHAR* m_pFrameKey;
};

