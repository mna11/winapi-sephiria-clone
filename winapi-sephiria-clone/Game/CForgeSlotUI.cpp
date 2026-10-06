#include "pch.h"
#include "CForgeSlotUI.h"

#include "CImgMgr.h"
#include "CWeaponData.h"
#include "CFontMgr.h"

CForgeSlotUI::CForgeSlotUI()
	: m_iWeaponID(-1), m_bCol(false)
{
}

CForgeSlotUI::~CForgeSlotUI()
{
	Release();
}

void CForgeSlotUI::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.5f;
	m_tInfo = { 0.f, 0.f, 292.f * m_fUIScale, 84.f * m_fUIScale };

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 1;
	m_pFrameKey = L"Forge_Slot";

	m_iWeaponID = -1;
	m_bCol = false; 
}

int CForgeSlotUI::Update()
{
	if (m_bDead)
		return DEAD;

	__super::UpdateRect();
	return NOEVENT;
}

void CForgeSlotUI::LateUpdate()
{
}

void CForgeSlotUI::Render(Graphics* pGraphics)
{
	Image* pImg(nullptr);
	VEC vCellSize = m_tInfo.vSize / m_fUIScale;
	VEC vImgSize = m_tInfo.vSize;

	pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;

	RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f,
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f,
					vImgSize.fX,
					vImgSize.fY };

	pGraphics->DrawImage(
		pImg, rcDest,
		(m_bCol ? vCellSize.fX : 0),
		0,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);

	if (-1 == m_iWeaponID)
		return;

	const WEAPON_INFO* pWeaponInfo = CWeaponData::GetInstance()->FindWeaponInfo(m_iWeaponID);

	// 무기 아이콘 그리기
	pImg = CImgMgr::GetInstance()->FindImg(pWeaponInfo->strIconImg.c_str());

	VEC vIconCellSize = { 64.f, 64.f };
	VEC vIconImgSize = vIconCellSize * m_fUIScale * 0.35f;
	VEC vOffset = VEC{ -17.f, 16.f } * m_fUIScale;
	rcDest = { m_tInfo.vPoint.fX - vIconImgSize.fX * 0.5f + vImgSize.fX * 0.5f + vOffset.fX,
			   m_tInfo.vPoint.fY - vIconImgSize.fY * 0.5f - vImgSize.fY * 0.5f + vOffset.fY,
			   vIconImgSize.fX, vIconImgSize.fY };

	pGraphics->DrawImage(
		pImg, rcDest,
		0,
		0,
		vIconCellSize.fX,
		vIconCellSize.fY,
		UnitPixel
	);

	// 무기 이름 텍스트 그리기
	VEC vRectSize{};
	RectF rcBgStr{};

	vRectSize = VEC{ 105.f, 18.f } * m_fUIScale;
	vOffset = VEC{ 10.f, 5.f } *m_fUIScale;
	rcDest = { m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vOffset.fX, m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vOffset.fY, vRectSize.fX, vRectSize.fY };
	rcBgStr = rcDest;
	rcBgStr.X += 3.f;
	rcBgStr.Y += 3.f;
#ifdef _DEBUG
	SolidBrush GreenBrush(Color{ 255, 0, 128, 0 });
	pGraphics->FillRectangle(&GreenBrush, rcDest);
#endif // _DEBUG
	CFontMgr::GetInstance()->DrawString(pGraphics, pWeaponInfo->strName, FONT_TYPE::NORMAL, rcBgStr, Color{255, 0, 0, 0}, 22.f, StringAlignmentNear);
	CFontMgr::GetInstance()->DrawString(pGraphics, pWeaponInfo->strName, FONT_TYPE::NORMAL, rcDest, Color{ 255, 250, 175, 89 }, 22.f, StringAlignmentNear);

	// 무기 설명 텍스트 그리기
	vOffset = VEC{ 10.f, 27.f } *m_fUIScale;
	vRectSize = VEC{ 250.f, 50.f } *m_fUIScale;
	rcDest = { m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vOffset.fX, m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
	SolidBrush RedBrush(Color{ 255, 128, 0, 0 });
	pGraphics->FillRectangle(&RedBrush, rcDest);
#endif // _DEBUG
	CFontMgr::GetInstance()->DrawString(pGraphics, pWeaponInfo->strDescription, FONT_TYPE::NORMAL, rcDest, Color{ 255, 255, 255, 255 }, 22.f, StringAlignmentNear, StringAlignmentNear);
}

void CForgeSlotUI::Release()
{
}
