#pragma once
#include "CSword.h"

// 충돌 관련해서는 Sword And Shield
// 초기화(Initialize), 위치(Update)는 CSword에서 관리
// 하위 Sword에서는 렌더만 신경쓰면 된다.

class CNormalSword :
    public CSword
{
public:
    CNormalSword();
    ~CNormalSword();
};

