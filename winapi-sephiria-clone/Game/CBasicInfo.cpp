#include "pch.h"
#include "CBasicInfo.h"

#include "CPlayer.h"

#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CFontMgr.h"

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
	int iGap = 1.5f * PIXEL_SCALE;
	// 프레임 사이즈
	VEC vFrameSize = VEC{ 63.f, 13.5f } * PIXEL_SCALE;
	// 각 바들 사이즈
	VEC vHpSize = VEC{ vFrameSize.fX - 2 * iGap, 5.f * PIXEL_SCALE };
	VEC vMpSize = VEC{ vFrameSize.fX - 2 * iGap, 4.f * PIXEL_SCALE };
	// 대시 사이즈 
	VEC vDashCellSize = { 11.f, 9.f };
	VEC vDashSize = vDashCellSize * PIXEL_SCALE * 0.6;


	// 띄울 위치
	VEC vStartPoint = m_tInfo.vPoint;
	// 플레이어 스탯 정보 받아오기
	const STAT& tPlayerStat = pPlayer->GetStat();
	// 텍스트 출력 string 설정
	// to_wstring 안했더니 L"/"을 주소 이동한게 되서 이상한 한자 나오더라
	wstring strHp = to_wstring(tPlayerStat.iHp) + L"/" + to_wstring(tPlayerStat.iMaxHp);
	wstring strMp = to_wstring(tPlayerStat.iMp) + L"/" + to_wstring(tPlayerStat.iMaxMp);

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
	CFontMgr::GetInstance()->DrawString(pGraphics, strHp, FONT_TYPE::PIXEL_BIG, RectF{ vStartPoint.fX + int(iGap * 0.5), vStartPoint.fY + int(iGap * 0.5), vHpSize.fX, vHpSize.fY }, Color(255, 0, 0, 0));
	CFontMgr::GetInstance()->DrawString(pGraphics, strHp, FONT_TYPE::PIXEL_BIG, RectF{ vStartPoint.fX, vStartPoint.fY, vHpSize.fX, vHpSize.fY }, Color(255, 255, 255, 255));

	// MP 그리기
	vStartPoint.fY += iGap + vHpSize.fY;

	float fMpRatio = (float)tPlayerStat.iMp / (float)tPlayerStat.iMaxMp;
	rcDest = { vStartPoint.fX, vStartPoint.fY,
			   vMpSize.fX * fMpRatio, vMpSize.fY };
	pGraphics->DrawImage(
		pMpImg, rcDest, 0, 0, 1.f, 1.f, UnitPixel
	);
	CFontMgr::GetInstance()->DrawString(pGraphics, strMp, FONT_TYPE::PIXEL_SMALL, RectF{ vStartPoint.fX + int(iGap * 0.5), vStartPoint.fY + int(iGap * 0.5), vMpSize.fX, vMpSize.fY }, Color(255, 0, 0, 0));
	CFontMgr::GetInstance()->DrawString(pGraphics, strMp, FONT_TYPE::PIXEL_SMALL, RectF{ vStartPoint.fX, vStartPoint.fY, vMpSize.fX, vMpSize.fY }, Color(255, 255, 255, 255));

	// Dash 그리기
	vStartPoint.fX -= iGap;
	vStartPoint.fY += vMpSize.fY + 2.f * iGap;

	for (int i = 0; i < tPlayerStat.iMaxDash; ++i)
	{
		rcDest = { vStartPoint.fX + vDashSize.fX * i, vStartPoint.fY,
			   vDashSize.fX, vDashSize.fY };

		// Blank 이미지는 매 위치 그림
		pGraphics->DrawImage(
			pDashBlank, rcDest, 0, 0, vDashCellSize.fX, vDashCellSize.fY, UnitPixel
		);

		// Fill 되어진 이미지는 iDash 개수만큼 그리고, 만약 쓴 대쉬가 있다면 점점 차오르게 보이게 만듬
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
