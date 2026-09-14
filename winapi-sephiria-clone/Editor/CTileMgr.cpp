#include "pch.h"
#include "CTileMgr.h"
#include "CTile.h"
#include "CAbstractFactory.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
	: m_bPreview(false)
{
	for (int l = 0; l < EnumToInt(TILE_LAYER::END); ++l)
	{
		m_vecTile[l].reserve(TILEX * TILEY);
	}
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	for (int l = 0; l < EnumToInt(TILE_LAYER::END); ++l)
	{
		for (int i = 0; i < TILEY; ++i)
		{
			for (int j = 0; j < TILEX; ++j)
			{
				float fX = float((TILECX >> 1) + (TILECX * j));
				float fY = float((TILECY >> 1) + (TILECY * i));

				CObj* pTile = CAbstractFactory<CTile>::CreateObj(fX, fY);

				/*if ((TILE_LAYER)l == TILE_LAYER::LAYER0)
				{
					static_cast<CTile*>(pTile)->SetTile({ TILE_TYPE::LIB_WALL, 8, TILE_OPTION::FLOOR, TILE_LAYER::LAYER0});
					static_cast<CTile*>(pTile)->SetIsDraw(true);
				}*/

				m_vecTile[l].push_back(pTile);
			}
		}
	}

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Tile/Library/Library_BG.png", L"LIB_TILE_BG");
}

void CTileMgr::Update()
{
	for (int l = 0; l < EnumToInt(TILE_LAYER::END); ++l)
	{
		for (auto& pTile : m_vecTile[l])
			pTile->Update();
	}
}

void CTileMgr::LateUpdate()
{
	for (int l = 0; l < EnumToInt(TILE_LAYER::END); ++l)
	{
		for (auto& pTile : m_vecTile[l])
			pTile->LateUpdate();
	}
}

void CTileMgr::Render(Graphics* pGraphics)
{
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

	int iCullX = abs(vScroll.fX / TILECX);
	int iCullY = abs(vScroll.fY / TILECY);

	int iMaxX = iCullX + (WINCX / TILECX) + 2;
	int iMaxY = iCullY + (WINCY / TILECY) + 2;

	for (int i = iCullY; i < iMaxY; ++i)
	{
		for (int j = iCullX; j < iMaxX; ++j)
		{
			int iIndex = i * TILEX + j;
			
			for (int l = 0; l < EnumToInt(TILE_LAYER::END); ++l)
			{
				if (0 > iIndex || m_vecTile[l].size() <= (size_t)iIndex)
					continue;

				m_vecTile[l][iIndex]->Render(pGraphics);
			}
		}
	}
}

void CTileMgr::Release()
{
	for (int l = 0; l < EnumToInt(TILE_LAYER::END); ++l)
	{
		for_each(m_vecTile[l].begin(), m_vecTile[l].end(), SafeDelete<CObj*>);
		m_vecTile[l].clear();
	}
}

void CTileMgr::PickingTile(POINT pt, TILE tTile)
{
	int x = pt.x / TILECX;
	int y = pt.y / TILECY;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || m_vecTile[EnumToInt(tTile.eTileLayer)].size() <= (size_t)iIndex)
		return;
	
	static_cast<CTile*>(m_vecTile[EnumToInt(tTile.eTileLayer)][iIndex])->SetTile(tTile);
	static_cast<CTile*>(m_vecTile[EnumToInt(tTile.eTileLayer)][iIndex])->SetIsDraw(true);
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

	DWORD		dwbyte(0);
	TILE		tTile{};
	bool		bIsDraw(false);

	for (int l = 0; l < EnumToInt(TILE_LAYER::END); ++l)
	{
		for (auto& pTile : m_vecTile[l])
		{
			tTile = static_cast<CTile*>(pTile)->GetTile();
			bIsDraw = static_cast<CTile*>(pTile)->GetIsDraw();

			WriteFile(hFile, &tTile, sizeof(TILE), &dwbyte, nullptr);
			WriteFile(hFile, &bIsDraw, sizeof(bool), &dwbyte, nullptr);
			WriteFile(hFile, &pTile->GetInfo(), sizeof(INFO), &dwbyte, nullptr);
		}
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
	
	DWORD		dwbyte(0);
	TILE		tTile{};
	bool		bIsDraw(false);
	INFO		tTileInfo{};

	int			l(0);

	while (true)
	{
		ReadFile(hFile, &tTile, sizeof(TILE), &dwbyte, nullptr);
		ReadFile(hFile, &bIsDraw, sizeof(bool), &dwbyte, nullptr);
		ReadFile(hFile, &tTile, sizeof(INFO), &dwbyte, nullptr);

		if (0 == dwbyte)
			break;

		CObj* pTile = CAbstractFactory<CTile>::CreateObj(tTileInfo.vPoint.fX, tTileInfo.vPoint.fY);
		static_cast<CTile*>(pTile)->SetTile(tTile);
		static_cast<CTile*>(pTile)->SetIsDraw(bIsDraw);

		m_vecTile[l].push_back(pTile);
		if (m_vecTile[l].size() == TILEX * TILEY)
			++l;
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Tile Load 완료"), L"Success", MB_OK);
}
