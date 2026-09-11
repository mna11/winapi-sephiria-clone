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
	virtual void Render(HDC hDC) PURE;
	virtual void Release()       PURE;

public:
	const INFO&		GetInfo() const { return m_tInfo; }
	const RECT&		GetRect() const { return m_tRect; }
	RENDERID		GetRenderID() const { return m_eRender; }

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

