#pragma once

#include "Define.h"

template<typename T>
class CState abstract
{
public:
	CState(T eCurState, T eNextState) : m_eCurState(eCurState), m_eNextState(eNextState) {}
	virtual ~CState() {}
public:
	virtual void ApplyChange() PURE;
public:
	void RequestChange(T eState) { m_eCurState = eState; }

protected:
	T m_eCurState;
	T m_eNextState;
};

