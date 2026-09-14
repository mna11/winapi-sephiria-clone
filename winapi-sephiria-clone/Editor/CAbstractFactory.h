#pragma once

#include "CObj.h"

template<typename T>
class CAbstractFactory
{
public:
	static CObj* CreateObj()
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj* CreateObj(float fX, float fY, float fAngle = 0.f)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(fX, fY);
		pObj->SetAngle(fAngle);

		return pObj;
	}
};

