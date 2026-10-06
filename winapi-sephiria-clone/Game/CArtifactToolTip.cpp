#include "pch.h"
#include "CArtifactToolTip.h"
#include "CImgMgr.h"
#include "CItem.h"
#include "CMouse.h"
#include "CFontMgr.h"
#include "CArtifactData.h"

CArtifactToolTip::CArtifactToolTip()
{
}

CArtifactToolTip::~CArtifactToolTip()
{
	Release();
}

void CArtifactToolTip::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.5f;

	// 이 UI는 중점 좌표보다는 오히려 왼쪽 상단 위치로 두는게 배치시 코드가 더 깔끔할거 같아서
	// m_tInfo.vPoint가 중점이 아니라 LT임
	m_tInfo = { 0.f, 0.f, 0.f, 0.f };

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 3;
}

int CArtifactToolTip::Update()
{
	if (!m_bView)
		return NOEVENT;

	if (m_bDead)
		return DEAD;

	return NOEVENT;
}

void CArtifactToolTip::LateUpdate()
{
	if (!m_bView)
		return;
}

void CArtifactToolTip::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	const int& iHoverItemID = m_pMouse->GetHoverReferItem().iID;
	if (-1 == iHoverItemID || m_pMouse->GetHoverReferItem().eItemType != ITEM_TYPE::ARTIFACT)
		return;

	const ARTIFACT_INFO* pArtifactInfo = CArtifactData::GetInstance()->FindArtifactInfo(iHoverItemID);

	if (nullptr == pArtifactInfo)
		return;

	Image* pImg(nullptr);
	VEC vCellSize{};
	VEC vImgSize{};
	RectF DestRect{};

	// 베이스 그리기
	pImg = CImgMgr::GetInstance()->FindImg(L"ItemToolTip_Base");
	if (nullptr == pImg)
		return; 

	float fCellTopSize		= 43.f;
	float fCellMiddleSize	= 8.f;
	float fCellBottomSize	= 18.f;

	float fCellTopEnd		= 43.f;
	float fCellMiddleEnd	= 51.f;

	float fMiddleSize = 10.f + pArtifactInfo->vecStrDescription.size() * 10.f;

	// 상
	vCellSize = {160.f, fCellTopSize}; // 총 67.f
	vImgSize = vCellSize * m_fUIScale;
	DestRect = {m_tInfo.vPoint.fX,
				m_tInfo.vPoint.fY,
				vImgSize.fX, vImgSize.fY};
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

	/////////////////////////////// 아이템 그리기 //////////////////////////////////

	VEC vOffset{};
	VEC vRectSize{};

	// 아이템 아이콘 그리기
	pImg = CImgMgr::GetInstance()->FindImg(pArtifactInfo->strImg.c_str());
	if (nullptr == pImg)
		return;

	vCellSize = { 32.f, 32.f };
	vImgSize = vCellSize * m_fUIScale;
	vOffset = VEC{ 121.5f, 6.f } * m_fUIScale;
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
	vOffset = VEC{ 8.f, 6.5f } * m_fUIScale;
	vRectSize = VEC{ 105.f, 18.f } * m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
	SolidBrush GreenBrush(Color{ 255, 0, 128, 0 });
	pGraphics->FillRectangle(&GreenBrush, DestRect);
#endif // _DEBUG
	CFontMgr::GetInstance()->DrawString(pGraphics, pArtifactInfo->strName, FONT_TYPE::NORMAL, DestRect, Color{255, 255, 255, 255}, 24.f, StringAlignmentCenter, StringAlignmentFar);

	// 아이템 시너지 그리기
	// 1. 시너지 아이콘
	pImg = CImgMgr::GetInstance()->FindImg(pArtifactInfo->strCategoryImg.c_str());
	if (nullptr == pImg)
		return;

	vCellSize = { 19.f, 19.f };
	vImgSize = vCellSize * m_fUIScale * 0.5f;

	vOffset = VEC{ 45.f, 23.f } *m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vImgSize.fX, vImgSize.fY };
#ifdef _DEBUG
	SolidBrush PurpleBrush(Color{ 255, 153, 51, 155 });
	pGraphics->FillRectangle(&PurpleBrush, DestRect);
#endif // _DEBUG
	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);

	// 2. 시너지 텍스트
	vOffset = VEC{ 55.f, 24.f } * m_fUIScale;
	vRectSize = VEC{ 53.f, 10.f } * m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
	SolidBrush RedBrush(Color{ 255, 128, 0, 0 });
	pGraphics->FillRectangle(&RedBrush, DestRect);
#endif // _DEBUG
	CFontMgr::GetInstance()->DrawString(pGraphics, pArtifactInfo->strCategoryName, FONT_TYPE::NORMAL, DestRect, Color{ 255, 105, 159, 139 }, 18.f, StringAlignmentNear, StringAlignmentNear);

	// 고유 텍스트 그리기
	vOffset = VEC{ 12.f, fCellTopEnd + 5.f } *m_fUIScale;
	vRectSize = VEC{ 23.f, 10.f } *m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
	SolidBrush PinkBrush(Color{ 255, 255, 51, 255 });
	pGraphics->FillRectangle(&PinkBrush, DestRect);
#endif // _DEBUG
	wstring strUnique = L"[고유]";
	CFontMgr::GetInstance()->DrawString(pGraphics, strUnique, FONT_TYPE::NORMAL, DestRect, Color { 255, 200, 158, 121 }, 18.f, StringAlignmentNear, StringAlignmentNear);

	// 아이템 효과 텍스트 그리기
	vector<wstring> vecStrDescription = pArtifactInfo->vecStrDescription;
	VEC vDashOffset{};
	VEC vDashRectSize{};
	RectF DashRect{};

	float fGap = 10.f;
	wstring strDash = L"-";
	for (int i = 0; i < vecStrDescription.size(); ++i)
	{
		vDashOffset = VEC{ 12.f, fCellTopEnd + 15.f + fGap * i } *m_fUIScale;
		vDashRectSize = VEC{ 6.f, 10.f } *m_fUIScale;
		DashRect = { m_tInfo.vPoint.fX + vDashOffset.fX, m_tInfo.vPoint.fY + vDashOffset.fY, vDashRectSize.fX, vDashRectSize.fY };
#ifdef _DEBUG
		SolidBrush GrayBrush(Color{ 255, 160, 160, 160 });
		pGraphics->FillRectangle(&GrayBrush, DashRect);
#endif // _DEBUG
		CFontMgr::GetInstance()->DrawString(pGraphics, strDash, FONT_TYPE::NORMAL, DashRect, Color{ 255, 255, 255, 255 }, 18.f, StringAlignmentNear, StringAlignmentNear);

		vOffset = VEC{ 18.f, fCellTopEnd + 15.f + fGap * i } *m_fUIScale;
		vRectSize = VEC{ 133.f, 10.f } *m_fUIScale;
		DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
		SolidBrush DoubleGrayBrush(Color{ 255, 80, 80, 80 });
		pGraphics->FillRectangle(&DoubleGrayBrush, DestRect);
#endif // _DEBUG
		CFontMgr::GetInstance()->DrawString(pGraphics, vecStrDescription[i], FONT_TYPE::NORMAL, DestRect, Color{255, 255, 255, 255}, 18.f, StringAlignmentNear, StringAlignmentNear);

	}
}

void CArtifactToolTip::Release()
{
}
