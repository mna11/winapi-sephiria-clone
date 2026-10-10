#include "pch.h"
#include "CStat.h"

#include "CPlayer.h"
#include "CWeaponController.h"
#include "CWeaponData.h"

#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CFontMgr.h"

CStat::CStat()
{
}

CStat::~CStat()
{
	Release();
}

void CStat::Initialize()
{
	m_tInfo = { 10.f, 0.f, 0.f, 0.f };
	m_fUIScale = PIXEL_SCALE * 0.5f;

	// 모든 UI보다 위
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 1;
}

int CStat::Update()
{
	if (m_bDead)
		return DEAD;

	if (!m_bView)
		return NOEVENT;

	return NOEVENT;
}

void CStat::LateUpdate()
{
	if (!m_bView)
		return;
}

void CStat::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	Image* pImg{ nullptr };
	VEC vCellSize{};
	VEC vImgSize{};
	VEC vRectSize{};
	VEC vOffset{};
	RectF DestRect{};
	RectF DestBgStr{};

	// 플레이어 정보 가져오기
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	// 스탯, 무기
	STAT tPlayerStat = pPlayer->GetStat();
	CWeaponController* pWeaponController = pPlayer->GetWeaponController();

	///// 베이스 그리기
	pImg = CImgMgr::GetInstance()->FindImg(L"Stat_Base");
	if (nullptr == pImg)
		return;

	float fCellTopSize = 100.f;
	float fCellMiddleSize = 100.f;
	float fCellBottomSize = 23.f;

	float fCellTopEnd = 100.f;
	float fCellMiddleEnd = 200.f;

	float fMiddleSize = 150.f;

	// 상
	vCellSize = { 160.f, fCellTopSize };
	vImgSize = vCellSize * m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX,
				m_tInfo.vPoint.fY,
				vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);
	// 중
	vCellSize = { 160.f, fCellMiddleSize };
	vImgSize = vCellSize * m_fUIScale;
	vImgSize.fY = fMiddleSize * m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX,
				m_tInfo.vPoint.fY + fCellTopEnd * m_fUIScale,
				vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(
		pImg, DestRect,
		0, fCellTopEnd, vCellSize.fX, vCellSize.fY, UnitPixel
	);
	// 하
	vCellSize = { 160.f, fCellBottomSize };
	vImgSize = vCellSize * m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX,
				m_tInfo.vPoint.fY + (fCellTopEnd + fMiddleSize - 1) * m_fUIScale, // 이론상 -1 안하는게 맞는데, 스케일링 단계에서 이슈가 있는건지 좀 커지면 틈이 보이길래 안전하게 함
				vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(
		pImg, DestRect,
		0, fCellMiddleEnd, vCellSize.fX, vCellSize.fY, UnitPixel
	);

	///// 이름 텍스트 그리기
	vOffset = VEC{ 20.f, 50.f } *m_fUIScale;
	vRectSize = VEC{ 120.f, 12.f } *m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
	wstring strName = pPlayer->GetName();
#ifdef _DEBUG
	SolidBrush blackBrush(Color{ 255, 0, 0, 0 });
	pGraphics->FillRectangle(&blackBrush, DestRect);
#endif // _DEBUG
	CFontMgr::GetInstance()->DrawString(pGraphics, strName, FONT_TYPE::NORMAL, DestRect, { 255, 255, 255, 255 }, 22.f);

	///// 레벨 텍스트 그리기
	vRectSize = VEC{ 25.f, 12.f } *m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
	wstring strLevel = L"LV " + to_wstring(pPlayer->GetLevel());
#ifdef _DEBUG
	pGraphics->FillRectangle(&blackBrush, DestRect);
#endif // _DEBUG
	CFontMgr::GetInstance()->DrawString(pGraphics, strLevel, FONT_TYPE::PIXEL_BIG, DestRect, { 255, 255, 255, 255 }, 22.f);

	////// 무기 아이콘 그리기
	// 배경
	pImg = CImgMgr::GetInstance()->FindImg(L"Inventory_Slot_Blank");
	if (nullptr == pImg)
		return; 
	vCellSize = { 32.f , 32.f };
	vImgSize = vCellSize * m_fUIScale;
	vOffset = VEC{ 20.f, 62.f } * m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);
	// 무기
	const WEAPON_INFO* pWeaponInfo = CWeaponData::GetInstance()->FindWeaponInfo(pWeaponController->GetWeapon()->GetWeaponID());
	if (nullptr == pWeaponInfo)
		return;
	pImg = CImgMgr::GetInstance()->FindImg(pWeaponInfo->strIconImg.c_str());
	if (nullptr == pImg)
		return;
	vCellSize = { 64.f, 64.f };
	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);

	///// 공격력 그리기
	vOffset = VEC{ 52.f, 62.f } *m_fUIScale; 
	vRectSize = VEC{ 88.f, 32.f } *m_fUIScale;

	pImg = CImgMgr::GetInstance()->FindImg(L"KeywordUI");
	if (!pImg)
		return;

	// 물리 공격력 아이콘
	vOffset += VEC{ vRectSize.fX * 0.1f, vRectSize.fY * 0.1f };
	RenderAttackStat(pGraphics, pImg, vOffset, 2, 4, tPlayerStat.iPhysicalAtk);

	// 화염 공격 아이콘
	vOffset.fX += vRectSize.fX * 0.25f;
	RenderAttackStat(pGraphics, pImg, vOffset, 1, 8, tPlayerStat.iFireAtk);

	// 얼음 공격 아이콘
	vOffset.fX += vRectSize.fX * 0.25f;
	RenderAttackStat(pGraphics, pImg, vOffset, 2, 3, tPlayerStat.iFrozenAtk);

	// 번개 공격 아이콘
	vOffset.fX += vRectSize.fX * 0.25f;
	RenderAttackStat(pGraphics, pImg, vOffset, 1, 7, tPlayerStat.iLightingAtk);

	///// 공통 스탯 그리기
	float fGap(0.);
	VEC vRowOffset{};
	float fRowGap = 9.f * m_fUIScale;
	wstring strTitle{};
	wstring strInfo{};

	// pImg는 아이콘이라 계속 재사용해야됨
	Image* pStrBgImg = CImgMgr::GetInstance()->FindImg(L"Stat_String_Bg");
	if (!pStrBgImg)
		return;

	// HP, MP, MP 재생
	vCellSize = { 49.f, 28.f };
	vOffset = VEC{ 16.f, 100.f } *m_fUIScale;
	vRectSize = VEC{ 129.f, 32.f } *m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
	pGraphics->DrawImage(
		pStrBgImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);
	vRowOffset = vOffset + VEC{4.f, 3.f} * m_fUIScale;
	float fWidth = vRectSize.fX - 4.f * 2.f;
	strTitle = L"HP"; strInfo = to_wstring(tPlayerStat.iHp) + L" / " + to_wstring(tPlayerStat.iMaxHp);
	RenderCommonStat(pGraphics, pImg, vRowOffset, fWidth, 0, 2, strTitle, strInfo);
	vRowOffset.fY += fRowGap;
	strTitle = L"MP"; strInfo = to_wstring(tPlayerStat.iMp) + L" / " + to_wstring(tPlayerStat.iMaxMp);
	RenderCommonStat(pGraphics, pImg, vRowOffset, fWidth, 0, 3,  strTitle, strInfo);
	vRowOffset.fY += fRowGap;
	strTitle = L"MP 재생"; strInfo = to_wstring(tPlayerStat.iMpRegeneration);
	RenderCommonStat(pGraphics, pImg, vRowOffset, fWidth, 1, 4, strTitle, strInfo);

	fGap = 36.f * m_fUIScale;


	// 방어력, 회피
	vOffset.fY += fGap;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY * 0.75f };
	pGraphics->DrawImage(
		pStrBgImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);
	vRowOffset = vOffset + VEC{ 4.f, 3.f } *m_fUIScale;
	strTitle = L"방어력"; strInfo = to_wstring(tPlayerStat.iDefense);
	RenderCommonStat(pGraphics, pImg, vRowOffset, fWidth, 2, 6, strTitle, strInfo);
	vRowOffset.fY += fRowGap;
	strTitle = L"회피"; strInfo = to_wstring(tPlayerStat.iEvasion);
	RenderCommonStat(pGraphics, pImg, vRowOffset, fWidth, 2, 5, strTitle, strInfo);

	fGap = 36.f * m_fUIScale * 0.75f;


	// 치명타 확률, 치명타 피해
	vOffset.fY += fGap;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY * 0.75f };
	pGraphics->DrawImage(
		pStrBgImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);
	vRowOffset = vOffset + VEC{ 4.f, 3.f } *m_fUIScale;
	strTitle = L"치명타 확률"; strInfo = to_wstring(static_cast<int>(round(tPlayerStat.fCriticalChange * 100.f))) + L"%";
	RenderCommonStat(pGraphics, pImg, vRowOffset, fWidth, 0, 5, strTitle, strInfo);
	vRowOffset.fY += fRowGap;
	strTitle = L"치명타 피해"; strInfo = to_wstring(static_cast<int>(round(tPlayerStat.fCriticalDamage * 100.f))) + L"%";
	RenderCommonStat(pGraphics, pImg, vRowOffset, fWidth, 0, 6, strTitle, strInfo);


	// 공속, 이속, 대시 회복 속도
	vOffset.fY += fGap;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
	pGraphics->DrawImage(
		pStrBgImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);
	vRowOffset = vOffset + VEC{ 4.f, 3.f } *m_fUIScale;
	strTitle = L"공격 속도"; strInfo = to_wstring(static_cast<int>(round(tPlayerStat.fAttackSpeed * 100.f))) + L"%";
	RenderCommonStat(pGraphics, pImg, vRowOffset, fWidth, 0, 7, strTitle, strInfo);
	vRowOffset.fY += fRowGap;
	strTitle = L"이동 속도"; strInfo = to_wstring(static_cast<int>(round(tPlayerStat.fMoveSpeed * 100.f))) + L"%";
	RenderCommonStat(pGraphics, pImg, vRowOffset, fWidth, 0, 8, strTitle, strInfo);
	vRowOffset.fY += fRowGap;
	strTitle = L"대시 회복 속도"; strInfo = to_wstring(static_cast<int>(round(tPlayerStat.fDashRecoverySpeed * 100.f))) + L"%";
	RenderCommonStat(pGraphics, pImg, vRowOffset, fWidth, 0, 9, strTitle, strInfo);

	fGap = 36.f * m_fUIScale;

	vOffset.fY += fGap;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
	pGraphics->DrawImage(
		pStrBgImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);
	vRowOffset = vOffset + VEC{ 4.f, 3.f } *m_fUIScale;
	strTitle = L"경험치 드롭"; strInfo = to_wstring(static_cast<int>(round(tPlayerStat.fExperienceDrop * 100.f))) + L"%";
	RenderCommonStat(pGraphics, pImg, vRowOffset, fWidth, 2, 2, strTitle, strInfo);
	vRowOffset.fY += fRowGap;
	strTitle = L"잎 드롭"; strInfo = to_wstring(static_cast<int>(round(tPlayerStat.fLeafDrop * 100.f))) + L"%";
	RenderCommonStat(pGraphics, pImg, vRowOffset, fWidth, 9, 4, strTitle, strInfo);
	vRowOffset.fY += fRowGap;
	strTitle = L"레벨업 시 HP 회복"; strInfo = to_wstring(static_cast<int>(round(tPlayerStat.fHpRestoredOnLevelUp * 100.f))) + L"%";
	RenderCommonStat(pGraphics, pImg, vRowOffset, fWidth, 1, 5, strTitle, strInfo);
}

void CStat::Release()
{
}

void CStat::RenderAttackStat(Graphics* pGraphics, Image* pImg, VEC vOffset, int iImgCol, int iImgRow, int iAtk)
{
	VEC vCellSize{ 10.f, 10.f };
	VEC vImgSize = vCellSize * m_fUIScale;
	RectF DestRect{ m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(
		pImg, DestRect,
		iImgCol * vCellSize.fX, iImgRow * vCellSize.fY, vCellSize.fX, vCellSize.fY, UnitPixel
	);

	float fTextWidth = 23.f * m_fUIScale;
	RectF DestStrRect{ DestRect.X + (DestRect.Width - fTextWidth) * 0.5f,  DestRect.Y + DestRect.Height + 3.f * m_fUIScale, fTextWidth, 12.f * m_fUIScale };
	RectF DestStrShadowRect = DestStrRect;
	DestStrShadowRect.X += 3.f;
	DestStrShadowRect.Y += 3.f;

#ifdef _DEBUG
	SolidBrush greenBrush(Color{ 255, 0, 255, 0 });
	pGraphics->FillRectangle(&greenBrush, DestStrRect);
#endif

	wstring strAtk = to_wstring(iAtk);
	CFontMgr::GetInstance()->DrawString(pGraphics, strAtk, FONT_TYPE::PIXEL_BIG, DestStrShadowRect, { 255, 0, 0, 0 }, 28.f);
	CFontMgr::GetInstance()->DrawString(pGraphics, strAtk, FONT_TYPE::PIXEL_BIG, DestStrRect, { 255, 255, 255, 255 }, 28.f);
}

void CStat::RenderCommonStat(Graphics* pGraphics, Image* pImg, VEC vOffset, float fWidth, int iImgCol, int iImgRow, wstring strTitle, wstring strInfo)
{
	// 아이콘 그리기
	VEC vCellSize{ 10.f, 10.f };
	VEC vImgSize = VEC{8.f, 8.f} * m_fUIScale; // 실제 그리는건 살짝 줄임
	RectF DestRect{ m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(
		pImg, DestRect,
		iImgCol * vCellSize.fX, iImgRow * vCellSize.fY, vCellSize.fX, vCellSize.fY, UnitPixel
	);

	float fTitleOffset = 10.f * m_fUIScale; // 아이콘 사이즈 + 약간의 여백
	float fInfoWidth = 40.f * m_fUIScale;  // Info 값 적을 사이즈
	float fRPad = 4.f * m_fUIScale;
	RectF DestStrTitleRect{ m_tInfo.vPoint.fX + vOffset.fX + fTitleOffset, m_tInfo.vPoint.fY + vOffset.fY, fWidth - fTitleOffset - fInfoWidth - fRPad, vImgSize.fY };
	RectF DestStrInfoRect{ m_tInfo.vPoint.fX + vOffset.fX + fWidth - fInfoWidth - fRPad, m_tInfo.vPoint.fY + vOffset.fY, fInfoWidth, vImgSize.fY };
#ifdef _DEBUG
	SolidBrush greenBrush(Color{ 255, 0, 255, 0 });
	pGraphics->FillRectangle(&greenBrush, DestStrTitleRect);
	SolidBrush redBrush(Color{ 255, 255, 0, 0 });
	pGraphics->FillRectangle(&redBrush, DestStrInfoRect);

#endif

	CFontMgr::GetInstance()->DrawString(pGraphics, strTitle, FONT_TYPE::NORMAL, DestStrTitleRect, { 255, 208, 202, 207 }, 18.f, StringAlignmentNear);
	CFontMgr::GetInstance()->DrawString(pGraphics, strInfo, FONT_TYPE::NORMAL, DestStrInfoRect, { 255, 255, 255, 255 }, 18.f, StringAlignmentFar);
}
