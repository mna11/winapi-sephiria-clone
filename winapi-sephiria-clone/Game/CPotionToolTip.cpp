#include "pch.h"
#include "CPotionToolTip.h"

#include "CImgMgr.h"
#include "CItem.h"
#include "CMouse.h"
#include "CFontMgr.h"
#include "CPotionData.h"

CPotionToolTip::CPotionToolTip()
{
}

CPotionToolTip::~CPotionToolTip()
{
	Release();
}

void CPotionToolTip::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.5f;

	m_tInfo = { 0.f, 0.f, 0.f, 0.f };

	m_eRender = RENDERID::UI;
	m_iRenderLayer = 3;
}

int CPotionToolTip::Update()
{
	if (m_bDead)
		return DEAD;

	if (!m_bView)
		return NOEVENT;

	return NOEVENT;
}

void CPotionToolTip::LateUpdate()
{
	if (!m_bView)
		return;
}

void CPotionToolTip::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	const int& iHoverItemID = m_pMouse->GetHoverReferItem().iID;
	if (-1 == iHoverItemID || m_pMouse->GetHoverReferItem().eItemType != ITEM_TYPE::POTION)
		return;

	const POTION_INFO* pPotionInfo = CPotionData::GetInstance()->FindPotionInfo(iHoverItemID);
	if (nullptr == pPotionInfo)
		return;

	Image* pImg(nullptr);
	VEC vCellSize{};
	VEC vImgSize{};
	RectF DestRect{};

	// 베이스 그리기
	pImg = CImgMgr::GetInstance()->FindImg(L"PotionTooltip_Base");
	if (nullptr == pImg)
		return;

	float fCellTopSize = 42.f;
	float fCellMiddleSize = 12.f;
	float fCellBottomSize = 8.f;

	float fCellTopEnd = 42.f;
	float fCellMiddleEnd = 54.f;

	float fMiddleSize = 15.f + pPotionInfo->vecStrDescription.size() * 10.f;

	float fMiddleEnd = fCellTopEnd + fMiddleSize;
	// 만약 바닥 밑으로 내려간다면 그 만큼 위로 올리기 - 화면 상에 다 보이게 보정하기
	float fScreenHeight = (fMiddleEnd + fCellBottomSize) * m_fUIScale;
	float fEndY = m_tInfo.vPoint.fY + fScreenHeight;
	if (fEndY > WINCY)
	{
		m_tInfo.vPoint.fY -= fEndY - WINCY;
	}

	// 상
	vCellSize = { 159.f, fCellTopSize }; // 총 67.f
	vImgSize = vCellSize * m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX,
				m_tInfo.vPoint.fY,
				vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);
	// 중
	vCellSize = { 159.f, fCellMiddleSize };
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
	vCellSize = { 159.f, fCellBottomSize };
	vImgSize = vCellSize * m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX,
				m_tInfo.vPoint.fY + (fCellTopEnd + fMiddleSize - 1) * m_fUIScale, // 이론상 -1 안하는게 맞는데, 스케일링 단계에서 이슈가 있는건지 좀 커지면 틈이 보이길래 안전하게 함
				vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(
		pImg, DestRect,
		0, fCellMiddleEnd, vCellSize.fX, vCellSize.fY, UnitPixel
	);

	/////////////////////////////// 포션 그리기 //////////////////////////////////

	VEC vOffset{};
	VEC vRectSize{};

	// 아이템 아이콘 그리기
	pImg = CImgMgr::GetInstance()->FindImg(pPotionInfo->strImg.c_str());
	if (nullptr == pImg)
		return;

	vCellSize = { 32.f, 32.f };
	vImgSize = vCellSize * m_fUIScale;
	vOffset = VEC{ 121.5f, 6.f } *m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vImgSize.fX, vImgSize.fY };
#ifdef _DEBUG
	SolidBrush whiteBrush(Color{ 255, 255, 255, 255 });
	pGraphics->FillRectangle(&whiteBrush, DestRect);
#endif // _DEBUG
	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);

	// 아이템 타이틀 텍스트 그리기
	vOffset = VEC{ 8.f, 6.f } *m_fUIScale;
	vRectSize = VEC{ 109.f, 30.f } *m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
	SolidBrush GreenBrush(Color{ 255, 0, 128, 0 });
	pGraphics->FillRectangle(&GreenBrush, DestRect);
#endif // _DEBUG
	CFontMgr::GetInstance()->DrawString(pGraphics, pPotionInfo->strName, FONT_TYPE::NORMAL, DestRect, Color{ 255, 255, 255, 255 }, 24.f);
	
	// 포션 텍스트 그리기
	vOffset = VEC{ 9.f, fCellTopEnd + 2.f } *m_fUIScale;
	vRectSize = VEC{ 23.f, 10.f } *m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
	SolidBrush PinkBrush(Color{ 255, 255, 51, 255 });
	pGraphics->FillRectangle(&PinkBrush, DestRect);
#endif // _DEBUG
	wstring strUnique = L"[포션]";
	CFontMgr::GetInstance()->DrawString(pGraphics, strUnique, FONT_TYPE::NORMAL, DestRect, Color{ 255, 144, 141, 148 }, 18.f, StringAlignmentNear, StringAlignmentNear);


	// 아이템 효과 텍스트 그리기
	vector<wstring> vecStrDescription = pPotionInfo->vecStrDescription;
	VEC vDashOffset{};
	VEC vDashRectSize{};
	RectF DashRect{};

	float fGap = 10.f;
	wstring strDash = L"-";
	for (int i = 0; i < vecStrDescription.size(); ++i)
	{
		vDashOffset = VEC{ 9.f, fCellTopEnd + 12.f + fGap * i } *m_fUIScale;
		vDashRectSize = VEC{ 6.f, 10.f } *m_fUIScale;
		DashRect = { m_tInfo.vPoint.fX + vDashOffset.fX, m_tInfo.vPoint.fY + vDashOffset.fY, vDashRectSize.fX, vDashRectSize.fY };
#ifdef _DEBUG
		SolidBrush GrayBrush(Color{ 255, 160, 160, 160 });
		pGraphics->FillRectangle(&GrayBrush, DashRect);
#endif // _DEBUG
		CFontMgr::GetInstance()->DrawString(pGraphics, strDash, FONT_TYPE::NORMAL, DashRect, Color{ 255, 255, 255, 255 }, 18.f, StringAlignmentNear, StringAlignmentNear);

		vOffset = VEC{ 15.f, fCellTopEnd + 12.f + fGap * i } *m_fUIScale;
		vRectSize = VEC{ 133.f, 10.f } *m_fUIScale;
		DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
		SolidBrush DoubleGrayBrush(Color{ 255, 80, 80, 80 });
		pGraphics->FillRectangle(&DoubleGrayBrush, DestRect);
#endif // _DEBUG
		CFontMgr::GetInstance()->DrawString(pGraphics, vecStrDescription[i], FONT_TYPE::NORMAL, DestRect, Color{ 255, 255, 255, 255 }, 18.f, StringAlignmentNear, StringAlignmentNear);
	}
}

void CPotionToolTip::Release()
{
}
