#pragma once

class CBossErma;

class CErmaPhase abstract
{
public:
	explicit CErmaPhase(CBossErma* pBoss);
	virtual ~CErmaPhase();

public:
	virtual void Initialize()	PURE;
	virtual void Update()		PURE;
	virtual void LateUpdate()   PURE;
	virtual void Release()		PURE;

protected:
	CBossErma* m_pBoss;
};

