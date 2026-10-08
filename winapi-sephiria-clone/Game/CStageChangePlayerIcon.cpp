#include "pch.h"
#include "CStageChangePlayerIcon.h"
#include "CButton.h"

#include "CImgMgr.h"

CStageChangePlayerIcon::CStageChangePlayerIcon()
	: m_pAnchorBtn(nullptr)
{
}

CStageChangePlayerIcon::~CStageChangePlayerIcon()
{
	Release();
}

void CStageChangePlayerIcon::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.7f;

	m_tInfo = { 0.f, 0.f, 0.f, 0.f };

	m_eRender = RENDERID::UI;
	m_iRenderLayer = 0;
	m_pFrameKey = L"StageChange_PlayerIcon";
}

int CStageChangePlayerIcon::Update()
{
	if (m_bDead)
		return DEAD;

	if (!m_bView)
		return NOEVENT;

	if (!m_pAnchorBtn)
		return NOEVENT;

	// 노드의 중앙 위치로 위치시키기
	m_tInfo.vPoint = m_pAnchorBtn->GetInfo().vPoint;

	return NOEVENT;
}

void CStageChangePlayerIcon::LateUpdate()
{
	if (!m_bView)
		return;
}

void CStageChangePlayerIcon::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	if (!m_pAnchorBtn)
		return;

	VEC vCellSize{ 21.f, 21.f };
	VEC vImgSize = vCellSize * m_fUIScale;
	Image* pImg = CImgMgr::GetInstance()->FindImg(L"StageChange_PlayerIcon");
	if (nullptr == pImg)
		return;

	Matrix matRot{};
	matRot.RotateAt(m_fAngle * 180.f / PI, { m_tInfo.vPoint.fX, m_tInfo.vPoint.fY });
	pGraphics->SetTransform(&matRot);
	RectF DestRect{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f, m_tInfo.vPoint.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };

	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);

	pGraphics->ResetTransform();
	matRot.Reset();
}

void CStageChangePlayerIcon::Release()
{
}
