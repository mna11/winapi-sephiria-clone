#include "pch.h"
#include "CStoneTabletToolTIp.h"

#include "CMouse.h"
#include "CItem.h"

#include "CImgMgr.h"
#include "CKeyMgr.h"
#include "CFontMgr.h"

#include "CStoneTabletData.h"

CStoneTabletToolTIp::CStoneTabletToolTIp()
{
}

CStoneTabletToolTIp::~CStoneTabletToolTIp()
{
	Release();
}

void CStoneTabletToolTIp::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.5f;

	// m_tInfo.vPoint가 중점이 아니라 LT임
	m_tInfo = { 0.f, 0.f, 0.f, 0.f };

	m_eRender = RENDERID::UI;
	m_iRenderLayer = 3;
}

int CStoneTabletToolTIp::Update()
{
	if (m_bDead)
		return DEAD;

	if (!m_bView)
		return NOEVENT;

	Rotate();

	return NOEVENT;
}

void CStoneTabletToolTIp::LateUpdate()
{
	if (!m_bView)
		return;
}

void CStoneTabletToolTIp::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	const int& iHoverItemID = m_pMouse->GetHoverReferItem().iID;
	if (-1 == iHoverItemID || m_pMouse->GetHoverReferItem().eItemType != ITEM_TYPE::STONE_TABLET)
		return;

	const STONE_TABLET_INFO* pStoneTabletInfo = CStoneTabletData::GetInstance()->FindStoneTabletInfo(iHoverItemID);

	if (nullptr == pStoneTabletInfo)
		return;

	Image* pImg(nullptr);
	VEC vCellSize{};
	VEC vImgSize{};
	RectF DestRect{};

	// 베이스 그리기
	pImg = CImgMgr::GetInstance()->FindImg(L"StoneTabletToolTip_Base");
	if (nullptr == pImg)
		return;

	float fCellTopSize = 62.f;
	float fCellMiddleSize = 57.f;
	float fCellBottomSize = 45.f;

	float fCellTopEnd = 62.f;
	float fCellMiddleEnd = 119.f;

	// 중앙 길이 크기 구하기
	int iAngle = static_cast<int>(round(m_fAngle / (PI * 0.5f)));
	// 가장 큰 상대 Y 구하기
	int iMaxAbsY = 0;
	for (const auto& pos : pStoneTabletInfo->arrRelativePos[iAngle])
	{
		iMaxAbsY = max(iMaxAbsY, std::abs(pos.second));
	}

	const float fSlotSize = 14.f;
	const float fPadding = 5.f; // y축 여백
	float fMiddleSize = max(40.f,(iMaxAbsY * 2 + 1) * fSlotSize + fPadding * 2); // 1은 중앙칸 땜시 더함

	float fMiddleEnd = fCellTopEnd + fMiddleSize;

	// 만약 바닥 밑으로 내려간다면 그 만큼 위로 올리기 - 화면 상에 다 보이게 보정하기
	float fScreenHeight = (fMiddleEnd + fCellBottomSize) * m_fUIScale;
	float fEndY = m_tInfo.vPoint.fY + fScreenHeight;
	if (fEndY > WINCY)
	{
		m_tInfo.vPoint.fY -= fEndY - WINCY;
	}

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

	/////////////////////////////// 석판 그리기 //////////////////////////////////

	VEC vOffset{};
	VEC vRectSize{};

	// 석판 아이콘 그리기
	pImg = CImgMgr::GetInstance()->FindImg(pStoneTabletInfo->strImg.c_str());
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

	Matrix matRot;
	PointF imgCenter{ DestRect.X + DestRect.Width * 0.5f, DestRect.Y + DestRect.Height * 0.5f };

	matRot.RotateAt(m_fAngle * 180.f / PI * -1, imgCenter);
	pGraphics->SetTransform(&matRot);
	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);
	pGraphics->ResetTransform();
	matRot.Reset();

	// 아이템 타이틀 텍스트 그리기
	vOffset = VEC{ 8.f, 6.f } * m_fUIScale;
	vRectSize = VEC{ 110.f, 31.f } * m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
	SolidBrush GreenBrush(Color{ 255, 0, 128, 0 });
	pGraphics->FillRectangle(&GreenBrush, DestRect);
#endif // _DEBUG
	CFontMgr::GetInstance()->DrawString(pGraphics, pStoneTabletInfo->strName, FONT_TYPE::NORMAL, DestRect, Color{ 255, 255, 255, 255 }, 24.f);

	// 내부 석판 안내 그리기
	
	// 아이콘을 그릴 중앙 
	pImg = CImgMgr::GetInstance()->FindImg(L"StoneTabletToolTip_Slot");
	vOffset = VEC{ 80.f , fCellTopEnd + fMiddleSize * 0.5f } * m_fUIScale;
	vCellSize = { 14.f, 14.f };
	vImgSize = vCellSize * m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX - vImgSize.fX * 0.5f, m_tInfo.vPoint.fY + vOffset.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);

	VEC vCenter = { DestRect.X + DestRect.Width * 0.5f, DestRect.Y + DestRect.Height * 0.5f };
	VEC vSlotSize = vImgSize;

	// 나머지 적용점들
	for (int i = 0; i < pStoneTabletInfo->arrRelativePos[iAngle].size(); ++i)
	{
		auto& prPos = pStoneTabletInfo->arrRelativePos[iAngle][i];

		DestRect.X = vCenter.fX + prPos.first * vSlotSize.fX - vSlotSize.fX * 0.5f;
		DestRect.Y = vCenter.fY + prPos.second * vSlotSize.fY - vSlotSize.fY * 0.5f;

		pGraphics->DrawImage(
			pImg, DestRect,
			0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
		);

		int iApplyLevel = pStoneTabletInfo->vecApplyLevel[i];
		wstring strLevel{};
		Color tColor{};
		if (iApplyLevel > 0)
		{
			strLevel = L"+" + to_wstring(iApplyLevel);
			tColor = { 255, 0, 255, 0 };
		}
		else 
		{
			strLevel = to_wstring(iApplyLevel);
			tColor = { 255, 255, 0, 0 };
		}
		DestRect.X += 1.f * m_fUIScale;
		DestRect.Y += 1.f * m_fUIScale;
		CFontMgr::GetInstance()->DrawString(pGraphics, strLevel, FONT_TYPE::PIXEL_SMALL, DestRect, tColor);
	}

	// 석판 아이콘
	pImg = CImgMgr::GetInstance()->FindImg(pStoneTabletInfo->strImg.c_str());
	vCellSize = { 32.f, 32.f };
	vImgSize = vImgSize * 1.5f;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX - vImgSize.fX * 0.5f, m_tInfo.vPoint.fY + vOffset.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };
	matRot.RotateAt(m_fAngle * 180.f / PI * -1, {vCenter.fX, vCenter.fY});
	pGraphics->SetTransform(&matRot);
	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);
	pGraphics->ResetTransform();
	matRot.Reset();

	// 회전 안내
	pImg = CImgMgr::GetInstance()->FindImg(L"KeyUI");

	float fKeyImgWidth = 15.f * m_fUIScale;
	float fTextWidth = 13.f * m_fUIScale;
	float fGap = 6.f * m_fUIScale;
	float fAllWidth = fKeyImgWidth + fTextWidth + fGap;
	float fKeyHeight = 10.f * m_fUIScale;
	float fTextHeight = 20.f * m_fUIScale;

	wstring strRotate = L"회전";
	vOffset = VEC{ 80.f, fMiddleEnd + 15.f } * m_fUIScale;
	RectF rcRotate = { m_tInfo.vPoint.fX + vOffset.fX - fAllWidth * 0.6f, m_tInfo.vPoint.fY + vOffset.fY,
						fAllWidth, fTextHeight };

	RectF rcRotateImg = rcRotate;
	rcRotateImg.Y += fTextHeight * 0.5f - fKeyHeight * 0.5f;
	rcRotateImg.Width = fKeyImgWidth;
	rcRotateImg.Height = fKeyHeight;

#ifdef _DEBUG
	SolidBrush BlackBrush(Color(255, 0, 0, 0));
	pGraphics->FillRectangle(&BlackBrush, rcRotate);
	SolidBrush WhiteBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&WhiteBrush, rcRotateImg);
#endif // _DEBUG

	vCellSize = { 13.f, 9.f };
	pGraphics->DrawImage(
		pImg, rcRotateImg,
		3 * vCellSize.fX,
		2 * vCellSize.fY,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);
	CFontMgr::GetInstance()->DrawString(pGraphics, strRotate, FONT_TYPE::NORMAL, rcRotate, Color{ 255, 255, 255, 255 }, 18.f, StringAlignmentFar);
}

void CStoneTabletToolTIp::Release()
{
}

void CStoneTabletToolTIp::Show()
{
	m_bView = true;

	// 인벤토리에 있는 석판이면, 현재 각도를 그에 맞게 세팅해준다.
	if (m_pMouse != nullptr && m_pMouse->GetHoverReferItem().eItemSource == ITEM_SOURCE::INVENTORY)
	{
		if (nullptr != m_pMouse->GetHoverItem())
		{
			m_fAngle = m_pMouse->GetHoverItem()->GetAngle();
		}
	}
}

void CStoneTabletToolTIp::Hide()
{
	m_bView = false;
	m_fAngle = 0.f;
}

void CStoneTabletToolTIp::Rotate()
{
	if (KEY_DOWN('R'))
	{
		AddAngle(PI * 0.5f);

		// 인벤토리에 있는 석판이면, 실제 인벤토리에 있는 아이템도 회전시켜준다.
		if (m_pMouse != nullptr && m_pMouse->GetHoverReferItem().eItemSource == ITEM_SOURCE::INVENTORY)
		{
			if (nullptr != m_pMouse->GetHoverItem())
			{
				m_pMouse->GetHoverItem()->AddAngle(PI * 0.5f);
			}
		}
	}
}