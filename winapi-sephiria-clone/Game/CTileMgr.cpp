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

	float fViewLeft = -vScroll.fX;
	float fViewTop = -vScroll.fY;
	float fViewRight = fViewLeft + WINCX;
	float fViewBottom = fViewTop + WINCY;

	for (int l = 0; l < toUType(TILE_LAYER::END); ++l)
	{
		for (CObj* pTile : m_vecTile[l])
		{
			const INFO& tInfo = pTile->GetInfo();

			float fTileLeft = tInfo.vPoint.fX - TILECX * 0.5f;
			float fTileTop = tInfo.vPoint.fY - TILECY * 0.5f;
			float fTileRight = tInfo.vPoint.fX + TILECX * 0.5f;
			float fTileBottom = tInfo.vPoint.fY + TILECY * 0.5f;

			if (fTileRight < fViewLeft ||
				fTileLeft > fViewRight ||
				fTileBottom < fViewTop ||
				fTileTop > fViewBottom)
			{
				continue;
			}

			pTile->Render(pGraphics);
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


void CTileMgr::LoadTile(SCENEID eSceneID)
{
	wstring strFile = L"";

	switch (eSceneID)
	{
	case SCENEID::STAGE0:
		break;
	case SCENEID::STAGE1:
		strFile = L"../Data/Stage01/Tile_Client_Stage01.dat";
		break;
	case SCENEID::BOSS_STAGE:
		strFile = L"../Data/BossStage/Tile_Client_BossStage.dat";
		break;
	default:
		break;
	}

	HANDLE	hFile = CreateFile(strFile.c_str(), // 이름을 포함한 파일 경로
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
		TILE tTile{};
		INFO tTileInfo{};

		ReadFile(hFile, &tTile, sizeof(TILE), &dwbyte, nullptr);
		ReadFile(hFile, &tTileInfo, sizeof(INFO), &dwbyte, nullptr);
		if (dwbyte == 0)
			break;

		CObj* pTile = CAbstractFactory<CTile>::CreateObj(
			tTileInfo.vPoint.fX,
			tTileInfo.vPoint.fY
		);

		static_cast<CTile*>(pTile)->SetTile(tTile);  

		int iLayer = static_cast<int>(tTile.eTileLayer);
		m_vecTile[iLayer].push_back(pTile);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Tile Load 완료"), L"Success", MB_OK);
}
