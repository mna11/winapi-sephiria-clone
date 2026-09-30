#include "pch.h"
#include "CItemToolTip.h"
#include "CImgMgr.h"
#include "CItem.h"
#include "CMouse.h"
#include "CFontMgr.h"

CItemToolTip::CItemToolTip()
	: m_fMiddleSize(50.f)
{
}

CItemToolTip::~CItemToolTip()
{
	Release();
}

void CItemToolTip::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.5f;

	// 이 UI는 중점 좌표보다는 오히려 왼쪽 상단 위치로 두는게 배치시 코드가 더 깔끔할거 같아서
	// m_tInfo.vPoint가 중점이 아니라 LT임
	m_tInfo = { 100.f, 100.f, 0.f, 0.f };

	// 스프라이트 넣기 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/ItemToolTip/ItemToolTip_Base.png", L"ItemToolTip_Base");

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 5;
}

int CItemToolTip::Update()
{
	if (!m_bView)
		return NOEVENT;

	return NOEVENT;
}

void CItemToolTip::LateUpdate()
{
	if (!m_bView)
		return;
}

void CItemToolTip::Render(Graphics* pGraphics)
{
	if (!m_bView)
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
	vImgSize.fY = m_fMiddleSize * m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX,
				m_tInfo.vPoint.fY + fCellTopEnd * m_fUIScale,
				vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(
		pImg, DestRect,
		0, fCellTopEnd, vCellSize.fX, vCellSize.fY, UnitPixel
	);
#ifdef _DEBUG
	SolidBrush wBrush(Color{ 255, 255, 255, 255 });
	pGraphics->FillRectangle(&wBrush, DestRect);
#endif // _DEBUG
	// 하
	vCellSize = { 160.f, fCellBottomSize };
	vImgSize = vCellSize * m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX,
				m_tInfo.vPoint.fY + (fCellTopEnd + m_fMiddleSize - 1) * m_fUIScale, // 이론상 -1 안하는게 맞는데, 스케일링 단계에서 이슈가 있어서 안전하게 함
				vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(
		pImg, DestRect,
		0, fCellMiddleEnd, vCellSize.fX, vCellSize.fY, UnitPixel
	);

	// 아이템 그리기
	VEC vOffset{};
	VEC vRectSize{};

	const CItem* pItem = m_pMouse->GetHoverItem();
	if (nullptr == pItem)
		return;

	// 아이템 아이콘 그리기
	pImg = CImgMgr::GetInstance()->FindImg(pItem->GetItemInfo().strImg.c_str());
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

	// 아이템 타이틀 그리기
	vOffset = VEC{ 8.f, 6.5f } * m_fUIScale;
	vRectSize = VEC{ 105.f, 18.f } * m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
	SolidBrush GreenBrush(Color{ 255, 0, 128, 0 });
	pGraphics->FillRectangle(&GreenBrush, DestRect);
#endif // _DEBUG
	CFontMgr::GetInstance()->DrawString(pGraphics, pItem->GetItemInfo().strName, FONT_TYPE::NORMAL, DestRect, Color{255, 255, 255, 255}, 24.f, StringAlignmentCenter, StringAlignmentFar);

	// 아이템 시너지 그리기
	vOffset = VEC{ 8.f, 24.f } *m_fUIScale;
	vRectSize = VEC{ 105.f, 10.f }*m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
	SolidBrush RedBrush(Color{ 255, 128, 0, 0 });
	pGraphics->FillRectangle(&RedBrush, DestRect);
#endif // _DEBUG
	wstring str = L"정밀"; // 임시
	CFontMgr::GetInstance()->DrawString(pGraphics, str, FONT_TYPE::NORMAL, DestRect, Color{ 255, 105, 159, 139 }, 16.f, StringAlignmentCenter, StringAlignmentNear);

	// 아이템 효과 그리기
	vOffset = VEC{ 8.f, 24.f } *m_fUIScale;
	vRectSize = VEC{ 105.f, 10.f }*m_fUIScale;
	DestRect = { m_tInfo.vPoint.fX + vOffset.fX, m_tInfo.vPoint.fY + vOffset.fY, vRectSize.fX, vRectSize.fY };
#ifdef _DEBUG
	SolidBrush RBrush(Color{ 255, 128, 0, 0 });
	pGraphics->FillRectangle(&RBrush, DestRect);
#endif // _DEBUG
}

void CItemToolTip::Release()
{
}
