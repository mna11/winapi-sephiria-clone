#include "pch.h"
#include "CMsgBox.h"

#include "CButton.h"
#include "CObjMgr.h"
#include "CFontMgr.h"
#include "CImgMgr.h"
#include "CAbstractFactory.h"

CMsgBox::CMsgBox()
	: m_eLayout(MSG_BOX_LAYOUT::END)
{
}

CMsgBox::~CMsgBox()
{
	Release();
}

void CMsgBox::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.5f;
	m_tInfo = { 0.f, 0.f, 0.f, 0.f };

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 4;
}

// 메세지 박스는 몇번째 버튼을 클릭했는지를 반환해줌 - 0은 아무것도 클릭 안함
int CMsgBox::Update()
{
	if (m_bDead)
		return DEAD;

	if (!m_bView)
		return NOEVENT;

	return NOEVENT;
}

void CMsgBox::LateUpdate()
{
	if (!m_bView)
		return;
}

void CMsgBox::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;
	
	Image* pImg(nullptr);
	pImg = CImgMgr::GetInstance()->FindImg(L"MsgBoxFrame");
	if (nullptr == pImg)
		return;

#ifdef _DEBUG
	SolidBrush whiteBrush(Color(255, 255, 255, 255));
	pGraphics->FillRectangle(&whiteBrush, m_rcPrint);
#endif // _DEBUG

	VEC vCellSize{220.f, 84.f};

	pGraphics->DrawImage(
		pImg, m_rcPrint,
		0,
		0,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);

	if (m_string.empty())
		return;

#ifdef _DEBUG
	SolidBrush magentaBrush(Color(255, 255, 0, 255));
	pGraphics->FillRectangle(&magentaBrush, m_rcStringPrint);
#endif // _DEBUG
	CFontMgr::GetInstance()->DrawString(pGraphics, m_string, FONT_TYPE::NORMAL, m_rcStringPrint, Color{ 255, 255, 255, 255 });	
}

void CMsgBox::Release()
{
	// 버튼 삭제
	for_each(m_vecButton.begin(), m_vecButton.end(), [](CButton*& pButton)
		{
			if (nullptr != pButton)
			{
				pButton->SetDead(true);
				pButton = nullptr;
			}
		});
}

void CMsgBox::SetButtonNumber(int iNum)
{
	// 버튼 개수 세팅이 되면 일단 마우스만 연결한 버튼을 일단 다 넣어준다.
	// CObjMgr에게도 알려준다.
	m_vecButton.reserve(iNum);

	CMouse* pMouse = CObjMgr::GetInstance()->GetMouse();
	for (int i = 0; i < iNum; ++i)
	{
		m_vecButton.push_back(CAbstractFactory<CButton>::CreateButton(pMouse, {}, {}));
		CObjMgr::GetInstance()->AddObject(OBJID::UI, m_vecButton.back());
		m_vecButton.back()->SetRenderLayer(m_iRenderLayer + 1);
	}
}

void CMsgBox::UpdateLayout()
{
	switch (m_eLayout)
	{
	case MSG_BOX_LAYOUT::NORMAL:
	{
		// 패딩
		VEC vPadding{ 25.f, 25.f };
		VEC vUsableSize{
			m_rcPrint.Width - vPadding.fX * 2.f,
			m_rcPrint.Height - vPadding.fY * 2.f
		};

		// 문자열 출력 Rect 위치 설정 / 문자열 : 버튼 = 6 : 4 (비율)
		m_rcStringPrint = {
			m_rcPrint.X + vPadding.fX, m_rcPrint.Y + vPadding.fY,
			m_rcPrint.Width - vPadding.fX * 2.f, vUsableSize.fY * 0.6f
		};

		// 버튼 출력 Rect 위치 설정
		int iNum = m_vecButton.size();
		float fBtnWidth = 61.f * m_fUIScale;
		float fBtnHeight = 24.f * m_fUIScale;
		float fGap = m_rcPrint.Width * 0.05f;

		// 버튼 전체가 차지하는 너비 = 버튼 개수 * 버튼 Width + 사이 갭들 더하기
		float fAllBtnWidth = iNum * fBtnWidth + (iNum - 1) * fGap;

		// 버튼의 원래 크기를 유지하고 싶으나
		// 만약 Width가 그정도가 안되면 비율로 줄임 
		if (fAllBtnWidth >= vUsableSize.fX)
		{
			float fScale = vUsableSize.fX / fAllBtnWidth;
			fBtnWidth *= fScale;
			fGap *= fScale;

			fAllBtnWidth = iNum * fBtnWidth + (iNum - 1) * fGap;
		}

		// 버튼 중앙 정렬을 위해 첫번째 버튼의 x 좌표 세팅
		// y는 vPadding.fY만큼 바닥에서 띄워있음
		VEC vOffset = { m_rcPrint.X + vPadding.fX + (vUsableSize.fX - fAllBtnWidth) * 0.5f,
						m_rcPrint.Y + vPadding.fY + (vUsableSize.fY - fBtnHeight) };

		for (int i = 0; i < m_vecButton.size(); ++i)
		{
			m_vecButton[i]->SetPrintRect({
					vOffset.fX + (fBtnWidth + fGap) * i, vOffset.fY, fBtnWidth, fBtnHeight
				});
		}

		break;
	}
	case MSG_BOX_LAYOUT::HORIZONTAL:
	{
		// 패딩
		VEC vPadding{ 25.f, 25.f };
		VEC vUsableSize{
			m_rcPrint.Width - vPadding.fX * 2.f,
			m_rcPrint.Height - vPadding.fY * 2.f
		};

		// 버튼 출력 Rect 위치 설정
		int iNum = m_vecButton.size();
		float fBtnWidth = 61.f * m_fUIScale;
		float fBtnHeight = vUsableSize.fY;		// 세로로는 가용영역 꽉 차도 됨!
		float fGap = m_rcPrint.Width * 0.05f;

		// 버튼 전체가 차지하는 너비 = 버튼 개수 * 버튼 Width + 사이 갭들 더하기
		float fAllBtnWidth = iNum * fBtnWidth + (iNum - 1) * fGap;

		// 버튼의 원래 가로를 유지하고 싶으나
		// 만약 Width가 그정도가 안되면 비율로 줄임 
		if (fAllBtnWidth >= vUsableSize.fX)
		{
			float fScale = vUsableSize.fX / fAllBtnWidth;
			fBtnWidth *= fScale;
			fGap *= fScale;

			fAllBtnWidth = iNum * fBtnWidth + (iNum - 1) * fGap;
		}

		// 버튼 중앙 정렬을 위해 첫번째 버튼의 x 좌표 세팅
		// y는 패딩 정도만 띄움 
		VEC vOffset = { m_rcPrint.X + vPadding.fX + (vUsableSize.fX - fAllBtnWidth) * 0.5f,
						m_rcPrint.Y + vPadding.fY };

		for (int i = 0; i < m_vecButton.size(); ++i)
		{
			m_vecButton[i]->SetPrintRect({
					vOffset.fX + (fBtnWidth + fGap) * i, vOffset.fY, fBtnWidth, fBtnHeight
				});
		}

		break;
	}
	case MSG_BOX_LAYOUT::VERTICAL:
	{
		// 패딩
		VEC vPadding{ 25.f, 25.f };
		VEC vUsableSize{
			m_rcPrint.Width - vPadding.fX * 2.f,
			m_rcPrint.Height - vPadding.fY * 2.f
		};

		// 버튼 출력 Rect 위치 설정
		int iNum = m_vecButton.size();
		float fBtnWidth = vUsableSize.fX;	  // 가로는 가용영역만큼 꽉 차있어도 됨!
		float fBtnHeight = 24.f * m_fUIScale;
		float fGap = m_rcPrint.Width * 0.05f;

		// 버튼 전체가 차지하는 높이 = 버튼 개수 * 버튼 Height + 사이 갭들 더하기
		float fAllBtnHeight = iNum * fBtnHeight + (iNum - 1) * fGap;

		// 버튼의 원래 크기를 유지하고 싶으나
		// 만약 Height가 그정도가 안되면 비율로 줄임 
		if (fAllBtnHeight >= vUsableSize.fY)
		{
			float fScale = vUsableSize.fY / fAllBtnHeight;
			fBtnHeight *= fScale;
			fGap *= fScale;

			fAllBtnHeight = iNum * fBtnHeight + (iNum - 1) * fGap;
		}

		// 버튼 중앙 정렬을 위해 첫번째 버튼의 x 좌표 세팅
		// x는 패딩 정도만 띄움
		// y는 중앙 정렬되게
		VEC vOffset = { m_rcPrint.X + vPadding.fX,
						m_rcPrint.Y + vPadding.fY + (vUsableSize.fY - fAllBtnHeight) * 0.5f };

		for (int i = 0; i < m_vecButton.size(); ++i)
		{
			m_vecButton[i]->SetPrintRect({
					vOffset.fX, vOffset.fY + (fBtnHeight + fGap)* i, fBtnWidth, fBtnHeight
				});
		}
	}
		break;
	default:
		break;
	}
}
