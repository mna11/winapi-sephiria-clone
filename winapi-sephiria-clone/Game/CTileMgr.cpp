#include "pch.h"
#include "CTileMgr.h"
#include "CTile.h"
#include "CAbstractFactory.h"
#include "CCameraMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
	for (int l = 0; l < toUType(TILE_LAYER::END); ++l)
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
	LoadTile();
}

void CTileMgr::Update()
{
}

void CTileMgr::LateUpdate()
{
}

void CTileMgr::Render(Graphics* pGraphics)
{
#ifdef _DEBUG
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

	VEC vStart{ 0.f, 0.f };
	VEC vEnd{ WINCX / TILECX + 2.f, WINCY / TILECY + 2.f };
	VEC vAdd{ -vScroll.fX / TILECX, -vScroll.fY / TILECY };

	vStart += vAdd;
	vEnd += vAdd;

	// 빌드는 문제 없는데, 인텔리전스가 모호하다고 해서 std 붙여줌
	// iIndex 검사를 하긴 하지만, i가 -1이고 j가 abs(-TILEX) 보다 크면 문제가 생기므로 clamp해줌 
	int iStartX = std::clamp((int)vStart.fX, 0, TILEX);
	int iStartY = std::clamp((int)vStart.fY, 0, TILEY);
	int iEndX = std::clamp((int)vEnd.fX, 0, TILEX);
	int iEndY = std::clamp((int)vEnd.fY, 0, TILEY);

	for (int i = iStartY; i < iEndY; ++i)
	{
		for (int j = iStartX; j < iEndX; ++j)
		{
			int iIndex = i * TILEX + j;

			for (int l = 0; l < toUType(TILE_LAYER::END); ++l)
			{
				if (0 > iIndex || m_vecTile[l].size() <= (size_t)iIndex)
					continue;

				m_vecTile[l][iIndex]->Render(pGraphics);
			}
		}
	}
#endif
}

void CTileMgr::Release()
{
	for (int l = 0; l < toUType(TILE_LAYER::END); ++l)
	{
		for_each(m_vecTile[l].begin(), m_vecTile[l].end(), SafeDelete<CObj*>);
		m_vecTile[l].clear();
	}
}

void CTileMgr::LoadTile()
{
	HANDLE	hFile = CreateFile(L"../Data/Tile_Client.dat", // 이름을 포함한 파일 경로
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
	int		iTemp(0);

	TILE		tTile{};
	bool		bDraw(false);
	INFO		tTileInfo{};
	int			iCnt(0);

	while (true)
	{
		ReadFile(hFile, &tTile, sizeof(TILE), &dwbyte, nullptr);
		ReadFile(hFile, &bDraw, sizeof(bool), &dwbyte, nullptr);
		ReadFile(hFile, &tTileInfo, sizeof(INFO), &dwbyte, nullptr);

		if (dwbyte == 0)
			break;

		int iLayer = iCnt / (TILEX * TILEY);

		CObj* pTile = CAbstractFactory<CTile>::CreateObj(
			tTileInfo.vPoint.fX,
			tTileInfo.vPoint.fY
		);

		tTile.eTileLayer = static_cast<TILE_LAYER>(iLayer);
		static_cast<CTile*>(pTile)->SetTile(tTile);

		m_vecTile[iLayer].push_back(pTile);

		++iCnt;
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Tile Load 완료"), L"Success", MB_OK);
}
