#pragma once

#include "CShield.h"

class CMagicStaffShield
	: public CShield
{
public:
	CMagicStaffShield();
	~CMagicStaffShield();
public:
	void HandleIdleRender(Graphics* pGraphics, Image* pImg, VEC& vScroll) override;
	void HandleAttackRender(Graphics* pGraphics, Image* pImg, VEC& vScroll) override;
	void HandleAttack1Render(Graphics* pGraphics, Image* pImg, VEC& vScroll) override;
	void HandleAttack2Render(Graphics* pGraphics, Image* pImg, VEC& vScroll) override;
	void HandleAttack3Render(Graphics* pGraphics, Image* pImg, VEC& vScroll) override;
	void HandleShieldRender(Graphics* pGraphics, Image* pImg, VEC& vScroll) override;
	void HandleCleaveRender(Graphics* pGraphics, Image* pImg, VEC& vScroll) override;
};

