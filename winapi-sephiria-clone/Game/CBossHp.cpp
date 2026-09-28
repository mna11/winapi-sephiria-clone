#include "pch.h"
#include "CBossHp.h"

#include "CPlayer.h"

#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CFontMgr.h"

CBossHp::CBossHp()
{
}

CBossHp::~CBossHp()
{
	Release();
}

void CBossHp::Initialize()
{
	m_tInfo = { 300.f, 600.f, 0.f, 0.f }; // 임시 

	// 스프라이트 넣기 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BossHp/BossHp.png", L"BossHp_Frame");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/HP_Bar_Fill.png", L"HP_Bar");

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 0;      // UI 중에 최약체
}

int CBossHp::Update()
{
	if (!m_bView)
		return NOEVENT;

	return NOEVENT;
}

void CBossHp::LateUpdate()
{
	if (!m_bView)
		return;
}

void CBossHp::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	///////////////////////////////////// 정보, 이미지 가져오기

	// 정보 가져오기
	if (nullptr == m_pTarget)
		return;

	// 프레임 이미지 가져오기
	Image* pFrameImg = CImgMgr::GetInstance()->FindImg(L"BossHp_Frame");
	if (nullptr == pFrameImg)
		return;

	// Hp바 이미지 가져오기
	Image* pHpImg = CImgMgr::GetInstance()->FindImg(L"HP_Bar");
	if (nullptr == pHpImg)
		return;

	///////////////////////////////////// 본격적인 출력 전 설정 (출력? 렌더?)

	// 픽셀 갭 
	int iGap = 5.f * PIXEL_SCALE;
	// 프레임 사이즈
	VEC vFrameSize = VEC{ 254.f, 35.f } *PIXEL_SCALE * 0.5f;
	// 바 사이즈
	VEC vHpSize = VEC{ vFrameSize.fX - 3 * iGap, 10.f * PIXEL_SCALE * 0.5f };

	// 띄울 위치
	VEC vStartPoint = m_tInfo.vPoint;
	// 플레이어 스탯 정보 받아오기
	const STAT& tPlayerStat = m_pTarget->GetStat();
	// 텍스트 출력 string 설정
	wstring strName = L"Erma, the Mad Scientist";

	///////////////////////////////////// 그리기

	// 프레임 그리기
	RectF rcDest{ vStartPoint.fX, vStartPoint.fY, vFrameSize.fX, vFrameSize.fY };
	pGraphics->DrawImage(
		pFrameImg, rcDest, 0, 0, 254.f, 35.f, UnitPixel
	);

	// HP 그리기
	vStartPoint += iGap * 1.5f;

	float fHpRatio = (float)tPlayerStat.iHp / (float)tPlayerStat.iMaxHp;
	rcDest = { vStartPoint.fX, vStartPoint.fY,
			   vHpSize.fX * fHpRatio, vHpSize.fY };
	pGraphics->DrawImage(
		pHpImg, rcDest, 0, 0, 1.f, 1.f, UnitPixel
	);

	rcDest = { vStartPoint.fX, vStartPoint.fY - iGap * 3.f, vFrameSize.fX, vFrameSize.fY };
	// 텍스트 (보스 이름)
	CFontMgr::GetInstance()->DrawString(pGraphics, strName, FONT_TYPE::NORMAL, rcDest, Color(255, 255, 255, 255), -1.0f, StringAlignmentNear, StringAlignmentCenter);

}

void CBossHp::Release()
{
}
