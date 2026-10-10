#include "pch.h"
#include "CStageChange.h"

#include "CButton.h"
#include "CStageChangePlayerIcon.h"

#include "CAbstractFactory.h"
#include "CImgMgr.h"
#include "CSceneMgr.h"
#include "CKeyMgr.h"
#include "CObjMgr.h"
#include "CUIMgr.h"
#include "CTimeMgr.h"

CStageChange::CStageChange()
	: m_fHeight(WINCY * 2), m_iNodeNum(8), m_iCurStep(0), m_iCurNode(0), m_pPlayerIcon(nullptr)
{
	m_vecNode.reserve(m_iNodeNum);
	ZeroMemory(&m_vWheelScroll, sizeof(VEC));

	// 간선 모든 경우의 수
	// - 이 중 first가 현재 노드인 경우만 화면에 표시할 거임
	m_vecEdge = {
		{0, 1}, {0, 2}, {0, 3},
		{1, 4}, {1, 5}, {1, 6},
		{2, 4}, {2, 5}, {2, 6},
		{3, 4}, {3, 5}, {3, 6},
		{4, 7}, {4, 7}, {4, 7},
		{5, 7}, {5, 7}, {5, 7},
		{6, 7}, {6, 7}, {6, 7},
	};
}

CStageChange::~CStageChange()
{
	Release();
}

void CStageChange::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.5f;

	m_eRender = RENDERID::UI;
	m_iRenderLayer = -2;

	InitializeNode();
	InitializePlayerIcon(); 
	
	Hide();
}

int CStageChange::Update()
{
	if (!m_bView)
		return NOEVENT;

	// 화면에 나오는 동안 아이콘이 빙글빙글 돌게하기
	if (nullptr != m_pPlayerIcon)
		m_pPlayerIcon->AddAngle(0.01f);

	// 마우스 스크롤값 업데이트
	UpdateScroll();

	return NOEVENT;
}

void CStageChange::LateUpdate()
{
	if (!m_bView)
		return;
}

void CStageChange::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	// 배경 그리기
	Image* pImg{nullptr};
	VEC vCellSize = {320.f, 180.f};
	VEC vImgSize = { WINCX, vCellSize.fX * m_fUIScale }; // 가로로는 꽉차게, 세로로는 스케일만큼만
	RectF DestRect{};

	pImg = CImgMgr::GetInstance()->FindImg(L"StageChange_Background");
	if (nullptr == pImg)
		return;

	// 높이가 꽉차게 그리기
	// m_fHeight - vImgSize.fY로 했더니 높이가 안맞아서 덜 그리더라 그냥 넉넉하게 함
	for (float y = 0.f; y < m_fHeight; y += vImgSize.fY)
	{
		// 휠 스크롤 더해주기
		DestRect = { 0.f, y + m_vWheelScroll.fY, vImgSize.fX, vImgSize.fY };
		pGraphics->DrawImage(pImg, DestRect, 0, 0, vCellSize.fX, vCellSize.fY, UnitPixel);
	}

	// 간선 그리기
	pImg = CImgMgr::GetInstance()->FindImg(L"StageChange_Line");
	if (nullptr == pImg)
		return;

	vCellSize = { 14.f, 8.f };
	vImgSize = vCellSize * PIXEL_SCALE;

	for (int iIdx = 0; iIdx < m_vecEdge.size(); ++iIdx)
	{
		// 현재 노드와 관련이 있는 간선만 그림
		if (m_iCurNode != m_vecEdge[iIdx].first)
			continue;

		// A(현재 플레이어 위치)에서 갈 수 있는 노드(B) 사이의 거리와 방향을 구함
		VEC vA = m_vecNode[m_vecEdge[iIdx].first].pButton->GetInfo().vPoint;
		VEC vB = m_vecNode[m_vecEdge[iIdx].second].pButton->GetInfo().vPoint;
		float fDistance = (vB - vA).Norm();
		VEC vDir = (vB - vA).Normalize();

		// atan2f를 하게 되면, x축에서 방향 벡터 사이의 끼인각을 구해줘서 그 만큼 회전시키면 방향을 가리킴
		float fAngle = atan2f(vDir.fY, vDir.fX) * 180.f / PI;

		// 회전한 경우가 있어서 정확하지는 않지만 대충 나쁘지 않아서 그냥 이대로 함
		float fGap = vImgSize.fX;

		// 여기도 배경처럼 거리 사이에 꽉 차게 그림
		for (float d = fGap; d < fDistance; d += fGap)
		{
			// 시작점 + 방향 벡터 * 거리
			VEC vPoint = vA + vDir * d;

			// 아까 구한 각도만큼 좌표계 회전
			Matrix matRot{};
			matRot.RotateAt(fAngle, { vPoint.fX, vPoint.fY });
			pGraphics->SetTransform(&matRot);
			RectF DestRect{ vPoint.fX - vImgSize.fX * 0.5f, vPoint.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };

			pGraphics->DrawImage(
				pImg, DestRect,
				0, 0, vCellSize.fX, vCellSize.fY, UnitPixel
			);

			pGraphics->ResetTransform();
			matRot.Reset();
		}
	}

	// 아이콘하고 버튼은 CObjMgr가 알아서 그려줌
}

void CStageChange::Release()
{
	// 버튼 삭제 예정
	for (auto& node : m_vecNode)
	{
		if (nullptr == node.pButton)
			continue;

		node.pButton->SetDead(true);
		node.pButton = nullptr;
	}

	// 아이콘 삭제 예정
	if (nullptr != m_pPlayerIcon)
	{
		m_pPlayerIcon->SetDead(true);
		m_pPlayerIcon = nullptr;
	}
}

void CStageChange::Show()
{
	m_bView = true;

	// 씬 매니저에서 현재 어떤 노드인지 세팅
	SetCurrentNode(CSceneMgr::GetInstance()->GetCurrentNodeID());

	// 버튼, 아이콘 보이게 하기
	for (auto& node : m_vecNode)
	{
		if (nullptr == node.pButton)
			continue;

		node.pButton->Show();

		if (nullptr == m_pPlayerIcon)
			continue;

		if (node.iNodeID == m_iCurNode)
		{
			m_pPlayerIcon->SetAnchorButton(node.pButton);
			m_pPlayerIcon->SetAngle(0.f);
			m_pPlayerIcon->Show();
		}
	}

	CUIMgr::GetInstance()->ShowUI(UIID::FADE);
}

void CStageChange::Hide()
{
	m_bView = false;

	// 버튼하고 아이콘 숨기기
	for (auto& node : m_vecNode)
	{
		if (nullptr == node.pButton)
			continue;

		node.pButton->Hide();
	}
	if (nullptr != m_pPlayerIcon)
		m_pPlayerIcon->Hide();
}

void CStageChange::SetCurrentNode(int iNodeID)
{
	for (const auto& node : m_vecNode)
	{
		if (node.iNodeID != iNodeID)
			continue;

		// 노드, 단계, 아이콘 세팅
		m_iCurNode = node.iNodeID;
		m_iCurStep = node.iStep;

		if (m_pPlayerIcon)
			m_pPlayerIcon->SetAnchorButton(node.pButton);

		UpdateNode();
		return;
	}
}

void CStageChange::InitializeNode()
{
	// 만약 노드가 있다면 일단 초기화
	for (auto& node : m_vecNode)
	{
		if (nullptr == node.pButton)
			continue;

		node.pButton->SetDead(true);
		node.pButton = nullptr;
	}
	m_vecNode.clear();

	// 노드 생성해서 넣어주기
	for (int i = 0; i < m_iNodeNum; ++i)
	{
		m_vecNode.push_back(CreateNode(i));
	}
}

void CStageChange::InitializePlayerIcon()
{
	// 플레이어 아이콘 세팅
	m_pPlayerIcon = static_cast<CStageChangePlayerIcon*>(CAbstractFactory<CStageChangePlayerIcon>::CreateObj());
	m_pPlayerIcon->SetRenderLayer(0);
	CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pPlayerIcon);
}

void CStageChange::UpdateScroll()
{
	// 휠 한번 굴리면 WHEEL_DELTA만큼 나옴, 그래서 몇번 굴렸냐?를 구할려고 나눈거
	// 그래서 한번 굴릴때 60 만큼 이동하게 할려고 60으로 정한거
	m_vWheelScroll.fY += (CKeyMgr::GetInstance()->GetWheelScroll() / WHEEL_DELTA) * 60.f;

	// 배경의 시작점이 스크롤의 좌표라고 생각하면 쉽더라
	// 배경이 0부터 시작하는데 + 방향으로 가면 대참사임 그래서 Max는 0
	// Min은 m_fHeight가 높이니깐 m_fHeight에서 WINCY를 뺀 만큼의 음수임
	float fMin = -max(0.f, m_fHeight - static_cast<float>(WINCY));
	float fMax = 0.f;
	// 스크롤은 min max clamp 검
	m_vWheelScroll.fY = clamp(m_vWheelScroll.fY, fMin, fMax);

	// 버튼에 스크롤 오프셋 값을 적용시킴
	for (auto& node : m_vecNode)
	{
		if (node.pButton)
			node.pButton->SetScrollOffset(m_vWheelScroll);
	}
}

void CStageChange::UpdateNode()
{
	// 현재 단계에서 다음 단계만 버튼 활성화
	for (auto& node : m_vecNode)
	{
		bool bSelectable = node.iStep == m_iCurStep + 1;
		node.pButton->SetEnable(bSelectable);
	}
}

NODE_INFO CStageChange::CreateNode(int iID)
{
	// 노드 생성 
	// 노드 ID, 스텝, 어떤 씬으로 전환할건지, 버튼 생성
	NODE_INFO tNodeInfo{};

	switch (iID)
	{
	case 0:
		tNodeInfo = { 0, 0, SCENEID::STAGE1, CreateButton(iID) };
		break;
	case 1:
		tNodeInfo = { 1, 1, SCENEID::STAGE_TEST, CreateButton(iID) };
		break;
	case 2:
		tNodeInfo = { 2, 1, SCENEID::STAGE_TEST, CreateButton(iID) };
		break;
	case 3:
		tNodeInfo = { 3, 1, SCENEID::STAGE_TEST, CreateButton(iID) };
		break;
	case 4:
		tNodeInfo = { 4, 2, SCENEID::STAGE_TEST, CreateButton(iID) };
		break;
	case 5:
		tNodeInfo = { 5, 2, SCENEID::STAGE_TEST, CreateButton(iID) };
		break;
	case 6:
		tNodeInfo = { 6, 2, SCENEID::STAGE_TEST, CreateButton(iID) };
		break;
	case 7:
		tNodeInfo = { 7, 3, SCENEID::BOSS_STAGE, CreateButton(iID) };
		break;
	}

	// 버튼 클릭 콜백 함수 등록
	tNodeInfo.pButton->SetOnClick([tNodeInfo]() {
		// 현재 씬 매니저의 노드를 변경하고 RequestChange(SCENEID)를 호출하는 함수
		// - 씬 매니저의 노드는 현재 플레이 중인 스테이지 노드임
		CSceneMgr::GetInstance()->RequestNodeChange(tNodeInfo.iNodeID, tNodeInfo.eSceneID);
	});

	return move(tNodeInfo);
}

CButton* CStageChange::CreateButton(int iID)
{
	CButton* pButton{nullptr};
	const TCHAR* pImgKey = nullptr;
	Image* pImg{ nullptr };
	RectF DestRect{};
	VEC vCenter{};

	// 버튼 위치
	// y축 0.2차이
	// x축 0.25차이
	switch (iID)
	{
	case 0:
		pImgKey = L"StageChange_Node01";
		vCenter = { WINCX * 0.5f, m_fHeight * 0.2f };
		break;
	case 1:
		pImgKey = L"StageChange_Node01";
		vCenter = { WINCX * 0.25f, m_fHeight * 0.4f };
		break;
	case 2:
		pImgKey = L"StageChange_Node02";
		vCenter = { WINCX * 0.5f, m_fHeight * 0.4f };
		break;
	case 3:
		pImgKey = L"StageChange_Node03";
		vCenter = { WINCX * 0.75f, m_fHeight * 0.4f };
		break;
	case 4:
		pImgKey = L"StageChange_Node01";
		vCenter = { WINCX * 0.25f, m_fHeight * 0.6f };
		break;
	case 5:
		pImgKey = L"StageChange_Node02";
		vCenter = { WINCX * 0.5f, m_fHeight * 0.6f };
		break;
	case 6:
		pImgKey = L"StageChange_Node03";
		vCenter = { WINCX * 0.75f, m_fHeight * 0.6f };
		break;
	case 7:
		pImgKey = L"StageChange_NodeBoss";
		vCenter = { WINCX * 0.5f, m_fHeight * 0.8f };
		break;
	default:
		return nullptr;
	}

	pImg = CImgMgr::GetInstance()->FindImg(pImgKey);
	if (nullptr == pImg)
		return nullptr;

	// 버튼 생성
	VEC vCellSize{ static_cast<float>(pImg->GetWidth()) / 3.f, static_cast<float>(pImg->GetHeight()) };
	VEC vDrawSize = vCellSize * PIXEL_SCALE * 0.8f;
	DestRect = { vCenter.fX - vDrawSize.fX * 0.5f, vCenter.fY - vDrawSize.fY * 0.5f, vDrawSize.fX, vDrawSize.fY };
	pButton = CAbstractFactory<CButton>::CreateButton(m_pMouse, DestRect, {}, pImgKey, { vCellSize.fX, vCellSize.fY});
	pButton->SetRenderLayer(0);
	CObjMgr::GetInstance()->AddObject(OBJID::UI, pButton);

	return pButton;
}
