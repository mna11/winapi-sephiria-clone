#pragma once

#include "CState.h"
#include "CWeapon.h"

class CWeaponController
	: public CWeapon, public CState<CWeapon::TYPE>
{
public:
	CWeaponController();
	~CWeaponController();

public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(Graphics*) override;
	void Release() override;

public:
	void Attack() override;
	void SpecialAttack() override;

public:
	void ApplyChange() override;

private:
	CWeapon* m_pWeapon;
};

