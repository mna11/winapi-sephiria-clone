#include "pch.h"
#include "CCollisionMgr.h"

#include "CObj.h"

void CCollisionMgr::CollisionRect(list<CObj*>& DstList, list<CObj*>& SrcList)
{
	RECT rc{};

	for (auto& Dst : DstList)
	{
		for (auto& Src : SrcList)
		{
			if (IntersectRect(&rc, &Dst->GetRect(), &Src->GetRect()))
			{
				Dst->SetDead(true);
				Src->SetDead(true);
			}
		}
	}
}
