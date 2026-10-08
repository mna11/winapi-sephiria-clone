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

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 1;      // UI 중에 최약체


	m_fUIScale = PIXEL_SCALE;
}

int CBasicInfo::Update()
{
	if (!m_bView)
		return NOEVENT;

	if (m_bDead)
		return DEAD;

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
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
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

	// 경험치바 이미지 가져오기
	Image* pExpFrameImg = CImgMgr::GetInstance()->FindImg(L"Exp_Bar");
	if (nullptr == pMpImg)
		return;

	// 경험치 이미지 가져오기
	Image* pExpFillImg = CImgMgr::GetInstance()->FindImg(L"Exp_Bar_Fill");
	if (nullptr == pMpImg)
		return;

	// 리프 이미지 가져오기
	Image* pLeafImg = CImgMgr::GetInstance()->FindImg(L"HUD_Leaf");
	if (nullptr == pLeafImg)
		return;

	// 주사위 이미지 가져오기
	Image* pDiceImg = CImgMgr::GetInstance()->FindImg(L"HUD_Dice");
	if (nullptr == pDiceImg)
		return;

	///////////////////////////////////// 본격적인 출력 전 설정 (출력? 렌더?)

	// 픽셀 갭 
	int iGap = 1.5f * m_fUIScale;
	// 프레임 사이즈
	VEC vFrameSize = VEC{ 63.f, 13.5f } *m_fUIScale;
	VEC vExpBarFrameSize = VEC{ WINCX, 2.f * m_fUIScale };
	// 각 바들 사이즈
	VEC vHpSize = VEC{ vFrameSize.fX - 2 * iGap, 5.f * m_fUIScale };
	VEC vMpSize = VEC{ vFrameSize.fX - 2 * iGap, 4.f * m_fUIScale };
	VEC vExpSize = VEC{ WINCX, 2.f * m_fUIScale };
	// 대시 사이즈 
	VEC vDashCellSize = { 11.f, 9.f };
	VEC vDashSize = vDashCellSize * m_fUIScale * 0.6; // 대시만 조금 더 작게 함


	// 띄울 위치
	VEC vStartPoint = m_tInfo.vPoint;
	// 플레이어 스탯 정보 받아오기
	const STAT& tPlayerStat = pPlayer->GetStat();
	// 텍스트 출력 string 설정
	// to_wstring 안했더니 L"/"을 주소 이동한게 되서 이상한 한자 나오더라
	wstring strHp = to_wstring(tPlayerStat.iHp) + L"/" + to_wstring(tPlayerStat.iMaxHp);
	wstring strMp = to_wstring(tPlayerStat.iMp) + L"/" + to_wstring(tPlayerStat.iMaxMp);
	wstring strLeaf = to_wstring(pPlayer->GetLeaf());
	wstring strDice = to_wstring(pPlayer->GetDice());
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
			double dElapse	= pPlayer->GetDashRecoveryElapse();
			double dInterval = pPlayer->GetDashRecoveryInterval();
			
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

	// Exp 그리기
	float fExpRatio = (float)pPlayer->GetExp() / (float)pPlayer->GetMaxExp();
	rcDest = { 0, WINCY - vExpSize.fY,
			   vExpSize.fX * fExpRatio, vExpSize.fY };
	pGraphics->DrawImage(
		pExpFillImg, rcDest, 0, 0, 1.f, 2.f, UnitPixel
	);

	// Exp 프레임 그리기
	rcDest = { 0, WINCY - vExpBarFrameSize.fY, vExpBarFrameSize.fX, vExpBarFrameSize.fY };
	VEC vCellSize{ 660.f, 2.f };
	pGraphics->DrawImage(
		pExpFrameImg, rcDest, 0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);

	// 리프 그리기
	vCellSize = { 11.f, 12.f };
	VEC vImgSize = vCellSize * m_fUIScale * 0.5f;
	rcDest = { WINCX - 50.f, WINCY - 50.f,
			   vImgSize.fX, vImgSize.fY };

	pGraphics->DrawImage(
		pLeafImg, rcDest, 0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);
	RectF rcStrDest = rcDest;
	rcStrDest.X -= 100; rcStrDest.Width = 100.f; rcStrDest.Height = 30.f;
#ifdef _DEBUG
	SolidBrush blackBrush(Color(255, 0, 0, 0));
	pGraphics->FillRectangle(&blackBrush, rcStrDest);
#endif // _DEBUG
	CFontMgr::GetInstance()->DrawString(pGraphics, strLeaf, FONT_TYPE::PIXEL_BIG, rcStrDest, Color(255, 255, 255, 255), 24.f, StringAlignmentFar, StringAlignmentFar);

	// 다이스 그리기
	vCellSize = { 11.f, 12.f };
	vImgSize = vCellSize * m_fUIScale * 0.5f;
	rcDest = { WINCX - 50.f, WINCY - 80.f,
			   vImgSize.fX, vImgSize.fY };

	pGraphics->DrawImage(
		pDiceImg, rcDest, 0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
	);
	rcStrDest.Y -= 30;
#ifdef _DEBUG
	pGraphics->FillRectangle(&blackBrush, rcStrDest);
#endif // _DEBUG
	CFontMgr::GetInstance()->DrawString(pGraphics, strDice, FONT_TYPE::PIXEL_BIG, rcStrDest, Color(255, 255, 255, 255), 24.f, StringAlignmentFar, StringAlignmentFar);
}

void CBasicInfo::Release()
{
}
