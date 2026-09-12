#include "pch.h"
#include "CTileMgr.h"
#include "CTile.h"
#include "CAbstractFactory.h"
#include "CCameraMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
	m_vecTile.reserve(TILEX * TILEY);
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = float((TILECX >> 1) + (TILECX * j));
			float fY = float((TILECY >> 1) + (TILECY * i));

			CObj* pTile = CAbstractFactory<CTile>::CreateObj(fX, fY);
			m_vecTile.push_back(pTile);
		}
	}

}

void CTileMgr::Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Update();
}

void CTileMgr::LateUpdate()
{
	for (auto& pTile : m_vecTile)
		pTile->LateUpdate();
}

void CTileMgr::Render(Graphics* pGraphics)
{
	int iCullX = abs((int)CCameraMgr::GetInstance()->GetScrollX() / TILECX);
	int iCullY = abs((int)CCameraMgr::GetInstance()->GetScrollY() / TILECY);

	int iMaxX = iCullX + (WINCX / TILECX) + 2;
	int iMaxY = iCullY + (WINCY / TILECY) + 2;

	for (int i = iCullY; i < iMaxY; ++i)
	{
		for (int j = iCullX; j < iMaxX; ++j)
		{
			int iIndex = i * TILEX + j;

			if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
				continue;

			m_vecTile[iIndex]->Render(pGraphics);
		}
	}
}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<CObj*>);
	m_vecTile.clear();
}

void CTileMgr::PickingTile(POINT pt, int iDrawID, int iOption)
{
	int x = pt.x / TILECX;
	int y = pt.y / TILECY;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->SetDrawID(iDrawID);
	dynamic_cast<CTile*>(m_vecTile[iIndex])->SetOption(iOption);
}

void CTileMgr::SaveTile()
{
	HANDLE	hFile = CreateFile(L"../Data/Tile.dat", // 이름을 포함한 파일 경로
		GENERIC_WRITE,		// 파일 접근 모드
		NULL,				// 공유 방식, NULL로 지정하면 공유하지 않음
		NULL,				// 보안 속성, NULL인 기본 값 설정
		CREATE_ALWAYS,		// 생성 방식, 쓰기(CREATE_ALWAYS), 읽기(OPEN_EXISTING)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성, 아무런 속성이 없는 일반 파일
		NULL);	// 생성될 파일의 속성을 제공할 템플릿(사용 안하기 때문에 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// _T : 아스키 코드를 유니코드로 변환시켜주는 매크로
		MessageBox(g_hWnd, _T("Tile Save File"), L"Fail", MB_OK);
		return;
	}

	DWORD dwbyte(0);
	int iDrawID(0), iOption(0);

	for (auto& pTile : m_vecTile)
	{
		iDrawID = dynamic_cast<CTile*>(pTile)->GetDrawID();
		iOption = dynamic_cast<CTile*>(pTile)->GetOption();

		WriteFile(hFile, &iDrawID, sizeof(int), &dwbyte, nullptr);
		WriteFile(hFile, &iOption, sizeof(int), &dwbyte, nullptr);
		WriteFile(hFile, &pTile->GetInfo(), sizeof(INFO), &dwbyte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Tile Save 완료"), L"Success", MB_OK);

}

void CTileMgr::LoadTile()
{
	HANDLE	hFile = CreateFile(L"../Data/Tile.dat", // 이름을 포함한 파일 경로
		GENERIC_READ,		// 파일 접근 모드
		NULL,				// 공유 방식, NULL로 지정하면 공유하지 않음
		NULL,				// 보안 속성, NULL인 기본 값 설정
		OPEN_EXISTING,		// 생성 방식, 쓰기(CREATE_ALWAYS), 읽기(OPEN_EXISTING)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성, 아무런 속성이 없는 일반 파일
		NULL);	// 생성될 파일의 속성을 제공할 템플릿(사용 안하기 때문에 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// _T : 아스키 코드를 유니코드로 변환시켜주는 매크로
		MessageBox(g_hWnd, _T("Tile Load File"), L"Fail", MB_OK);
		return;
	}

	Release();

	DWORD dwbyte(0);
	int		iDrawID(0), iOption(0);
	INFO	tTile{};

	while (true)
	{
		ReadFile(hFile, &iDrawID, sizeof(int), &dwbyte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwbyte, nullptr);
		ReadFile(hFile, &tTile, sizeof(INFO), &dwbyte, nullptr);

		if (0 == dwbyte)
			break;

		CObj* pTile = CAbstractFactory<CTile>::CreateObj(tTile.vPoint.fX, tTile.vPoint.fY);
		dynamic_cast<CTile*>(pTile)->SetDrawID(iDrawID);
		dynamic_cast<CTile*>(pTile)->SetOption(iOption);

		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Tile Load 완료"), L"Success", MB_OK);
}
