#include "pch.h"
#include "CLineMgr.h"
#include "CLine.h"

CLineMgr* CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
{
}

CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Initialize()
{
	LoadLine();
}

void CLineMgr::Render(HDC hDC)
{
	for (auto& pLine : m_LineList)
		pLine->Render(hDC);
}

void CLineMgr::Release()
{
	for_each(m_LineList.begin(), m_LineList.end(), SafeDelete<CLine*>);
	m_LineList.clear();
}

void CLineMgr::LoadLine()
{
	HANDLE	hFile = CreateFile(L"../Data/Line.dat", // 이름을 포함한 파일 경로
		GENERIC_READ,		// 파일 접근 모드
		NULL,				// 공유 방식, NULL로 지정하면 공유하지 않음
		NULL,				// 보안 속성, NULL인 기본 값 설정
		OPEN_EXISTING,		// 생성 방식, 쓰기(CREATE_ALWAYS), 읽기(OPEN_EXISTING)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성, 아무런 속성이 없는 일반 파일
		NULL);	// 생성될 파일의 속성을 제공할 템플릿(사용 안하기 때문에 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// _T : 아스키 코드를 유니코드로 변환시켜주는 매크로
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	Release();

	DWORD dwbyte(0);
	LINE   tLine{};

	while (true)
	{
		ReadFile(hFile, &tLine, sizeof(LINE), &dwbyte, nullptr);

		if (0 == dwbyte)
			break;

		m_LineList.push_back(new CLine(tLine.tLPoint, tLine.tRPoint));
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Load 완료"), L"Success", MB_OK);
}
