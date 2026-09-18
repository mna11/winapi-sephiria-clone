#include "pch.h"
#include "CBasicInfo.h"

#include "CPlayer.h"

#include "CImgMgr.h"
#include "CObjMgr.h"

CBasicInfo::CBasicInfo()
{
}

CBasicInfo::~CBasicInfo()
{
	Release();
}

void CBasicInfo::Initialize()
{
	m_tInfo = { 35, 30, 0.f, 0.f }; // 임시 

	// 스프라이트 넣기 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/Frame.png", L"BasicInfo_Frame");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/HP_Bar_Fill.png", L"HP_Bar");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/MP_Bar_Fill.png", L"MP_Bar");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/Dash_Fill.png", L"Dash_Fill");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/Dash_Blank.png", L"Dash_Blank");
	
	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 0;      // UI 중에 최약체
}

int CBasicInfo::Update()
{
	if (!m_bView)
		return NOEVENT;

	return NOEVENT;
}

void CBasicInfo::LateUpdate()
{
	if (!m_bView)
		return;


}

void CBasicInfo::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	///////////////////////////////////// 정보, 이미지 가져오기

	// 플레이어 정보 가져오기
	CObj* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	// 프레임 이미지 가져오기
	Image* pFrameImg = CImgMgr::GetInstance()->FindImg(L"BasicInfo_Frame");
	if (nullptr == pFrameImg)
		return;

	// Hp바 이미지 가져오기
	Image* pHpImg = CImgMgr::GetInstance()->FindImg(L"HP_Bar");
	if (nullptr == pHpImg)
		return;

	// Mp바 이미지 가져오기
	Image* pMpImg = CImgMgr::GetInstance()->FindImg(L"MP_Bar");
	if (nullptr == pMpImg)
		return;

	// 대시 이미지 가져오기
	Image* pDashBlank = CImgMgr::GetInstance()->FindImg(L"Dash_Blank");
	Image* pDashFill = CImgMgr::GetInstance()->FindImg(L"Dash_Fill");
	if (nullptr == pDashBlank || nullptr == pDashFill)
		return;

	///////////////////////////////////// 본격적인 출력 전 설정 (출력? 렌더?)

	// 픽셀 갭 
	int iGap = 2 * PIXEL_SCALE;
	// 프레임 사이즈
	VEC vFrameSize = VEC{ 68.f, 16.f } * PIXEL_SCALE;
	// 각 바들 사이즈
	VEC vHpSize = VEC{ vFrameSize.fX - 2 * iGap, 6.f * PIXEL_SCALE };
	VEC vMpSize = VEC{ vFrameSize.fX - 2 * iGap, 4.f * PIXEL_SCALE };
	// 대시 사이즈 
	VEC vDashCellSize = { 11.f, 9.f };
	VEC vDashSize = vDashCellSize * PIXEL_SCALE * 0.75;


	// 띄울 위치
	VEC vStartPoint = m_tInfo.vPoint;
	// 플레이어 스탯 정보 받아오기
	const STAT& tPlayerStat = pPlayer->GetStat();
	// 텍스트 출력을 위한 폰트 설정
	FontFamily fontFamily;
	int iNumFamilies;
	g_pFontCollection->GetFamilies(1, &fontFamily, &iNumFamilies);
	Font font(&fontFamily, 18.f, FontStyleRegular, UnitPixel);

	SolidBrush outlineBrush(Color(255, 0, 0, 0));
	SolidBrush solidBrush(Color(255, 255, 255, 255));

	StringFormat sf;
	sf.SetAlignment(StringAlignmentCenter);
	sf.SetLineAlignment(StringAlignmentCenter);

	TCHAR szHp[32], szMp[32];
	swprintf_s(szHp, L"%d/%d", tPlayerStat.iHp, tPlayerStat.iMaxHp);
	swprintf_s(szMp, L"%d/%d", tPlayerStat.iMp, tPlayerStat.iMaxMp);

	pGraphics->SetTextRenderingHint(
		TextRenderingHintSingleBitPerPixelGridFit
	);

	///////////////////////////////////// 그리기

	// 프레임 그리기
	RectF rcDest{ vStartPoint.fX, vStartPoint.fY, vFrameSize.fX, vFrameSize.fY };
	pGraphics->DrawImage(
		pFrameImg, rcDest, 0, 0, 1.f, 1.f, UnitPixel
	);

	// HP 그리기
	vStartPoint += iGap;

	float fHpRatio = (float)tPlayerStat.iHp / (float)tPlayerStat.iMaxHp;

	rcDest = { vStartPoint.fX, vStartPoint.fY,
			   vHpSize.fX * fHpRatio, vHpSize.fY };
	pGraphics->DrawImage(
		pHpImg, rcDest, 0, 0, 1.f, 1.f, UnitPixel
	);

	pGraphics->DrawString(
		szHp, -1, &font, { vStartPoint.fX, vStartPoint.fY, vHpSize.fX, vHpSize.fY }, &sf, &solidBrush
	);

	// MP 그리기
	vStartPoint.fY += iGap + vHpSize.fY;

	float fMpRatio = (float)tPlayerStat.iMp / (float)tPlayerStat.iMaxMp;
	rcDest = { vStartPoint.fX, vStartPoint.fY,
			   vMpSize.fX * fMpRatio, vMpSize.fY };
	pGraphics->DrawImage(
		pMpImg, rcDest, 0, 0, 1.f, 1.f, UnitPixel
	);
	Font fontMp(&fontFamily, 16.f, FontStyleRegular, UnitPixel);

	pGraphics->DrawString(
		szMp, -1, &fontMp, { vStartPoint.fX, vStartPoint.fY, vMpSize.fX, vMpSize.fY }, &sf, &solidBrush
	);

	// Dash
	vStartPoint.fX -= iGap;
	vStartPoint.fY += vMpSize.fY + 2.f * iGap;

	 // 비교적 커서 좀 줄여줌

	for (int i = 0; i < tPlayerStat.iMaxDash; ++i)
	{
		rcDest = { vStartPoint.fX + vDashSize.fX * i, vStartPoint.fY,
			   vDashSize.fX, vDashSize.fY };

		pGraphics->DrawImage(
			pDashBlank, rcDest, 0, 0, vDashCellSize.fX, vDashCellSize.fY, UnitPixel
		);

		if (i < tPlayerStat.iDash)
		{
			pGraphics->DrawImage(
				pDashFill, rcDest, 0, 0, vDashCellSize.fX, vDashCellSize.fY, UnitPixel
			);
		}
		else if (i == tPlayerStat.iDash)
		{
			double dElapse	= static_cast<CPlayer*>(pPlayer)->GetDashRecoveryElapse();
			double dInterval = static_cast<CPlayer*>(pPlayer)->GetDashRecoveryInterval();
			
			double dRatio = dElapse / dInterval;
			double dImgWidth = vDashSize.fX * dRatio;
			double dCellWidth = vDashCellSize.fX * dRatio;

			RectF rcTemp = { vStartPoint.fX + vDashSize.fX * i, vStartPoint.fY,
							 (float)dImgWidth, vDashSize.fY };

			pGraphics->DrawImage(
				pDashFill, rcTemp, 0, 0, (float)dCellWidth, vDashCellSize.fY, UnitPixel
			);
		}
	}
}

void CBasicInfo::Release()
{
}
