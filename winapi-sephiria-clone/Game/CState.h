#pragma once

#include "Define.h"

template<typename T>
class CState abstract
{
public:
	CState(T eCurState, T ePreState) : m_eCurState(eCurState), m_ePreState(ePreState) {}
	virtual ~CState() {}
public:
	virtual void ApplyChange() PURE;
public:
	void RequestChange(T eState) { m_eCurState = eState; }

protected:
	T m_eCurState;
	T m_ePreState;
};

