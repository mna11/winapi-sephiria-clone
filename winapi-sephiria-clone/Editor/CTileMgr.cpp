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

				m_vecTile[l].push_back(pTile);
			}
		}
	}

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Tile/Library/Library_BG.png", L"LIB_TILE_BG");

	for (int i = 0; i < EnumToInt(TILE_OPTION::END); ++i)
	{
		ColorMatrix colorMat = {
			1.f, 0.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 0.f, 1.f
		};

		switch ((TILE_OPTION)i)
		{
		case TILE_OPTION::WALL:
			colorMat.m[4][2] = 0.5f;
			break;
		case TILE_OPTION::AIR:
			colorMat.m[4][0] = 0.5f;
			break;
		case TILE_OPTION::INTERACTION:
			colorMat.m[4][1] = 0.5f;
			break;
		case TILE_OPTION::FLOOR:
			break;
		}

		m_tImgAttr[i].SetColorMatrix(
			&colorMat,
			ColorMatrixFlagsDefault,
			ColorAdjustTypeBitmap
		);
	}
}

void CTileMgr::Update()
{
	// 현재 타일 Update에서 하는 일이 없음 
	/*for (int l = 0; l < EnumToInt(TILE_LAYER::END); ++l)
	{
		for (auto& pTile : m_vecTile[l])
		{
			if(static_cast<CTile*>(pTile)->GetTile().bDraw)
				pTile->Update();
		}
	}*/
}

void CTileMgr::LateUpdate()
{
	// 현재 타일 LateUpdate에서 하는 일이 없음 
	/*for (int l = 0; l < EnumToInt(TILE_LAYER::END); ++l)
	{
		for (auto& pTile : m_vecTile[l])
		{
			if (static_cast<CTile*>(pTile)->GetTile().bDraw)
				pTile->LateUpdate();
		}
	}*/
}

void CTileMgr::Render(Graphics* pGraphics)
{
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

	VEC vStart{ 0.f, 0.f };
	VEC vEnd{ WINCX / TILECX + 2.f, WINCY / TILECY + 2.f };
	VEC vAdd{ -vScroll.fX / TILECX, -vScroll.fY / TILECY };

	vStart	+= vAdd;
	vEnd	+= vAdd;
	
	// 빌드는 문제 없는데, 인텔리전스가 모호하다고 해서 std 붙여줌
	// iIndex 검사를 하긴 하지만, i가 -1이고 j가 abs(-TILEX) 보다 크면 문제가 생기므로 clamp해줌 
	int iStartX = std::clamp((int)vStart.fX, 0, TILEX);
	int iStartY = std::clamp((int)vStart.fY, 0, TILEY);
	int iEndX   = std::clamp((int)vEnd.fX, 0, TILEX);
	int iEndY   = std::clamp((int)vEnd.fY, 0, TILEY);

	// 최적화를 위해 타일 각각이 아닌, 매니저에서 일괄적으로 호출하게 변경s
	Image* pTile = CImgMgr::GetInstance()->FindImg(L"LIB_TILE_BG");
	if (nullptr == pTile)
		return;

	for (int i = iStartY; i < iEndY; ++i)
	{
		for (int j = iStartX; j < iEndX; ++j)
		{
			int iIndex = i * TILEX + j;
			
			for (int l = 0; l < EnumToInt(TILE_LAYER::END); ++l)
			{
				if (0 > iIndex || m_vecTile[l].size() <= (size_t)iIndex)
					continue;

				if (static_cast<CTile*>(m_vecTile[l][iIndex])->GetTile().bDraw)
				{
					if (m_bPreview)
					{
						TILE_OPTION tmp = static_cast<CTile*>(m_vecTile[l][iIndex])->GetTile().eTileOption;
						m_vecTile[l][iIndex]->Render(pGraphics, pTile, vScroll, m_tImgAttr[EnumToInt(tmp)]);
					}
					else
					{
						m_vecTile[l][iIndex]->Render(pGraphics, pTile, vScroll);
					}
				}
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

	for (int l = 0; l < EnumToInt(TILE_LAYER::END); ++l)
	{
		for (auto& pTile : m_vecTile[l])
		{
			tTile = static_cast<CTile*>(pTile)->GetTile();

			WriteFile(hFile, &tTile, sizeof(TILE), &dwbyte, nullptr);
			WriteFile(hFile, &pTile->GetInfo(), sizeof(INFO), &dwbyte, nullptr);
		}
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Tile Save 완료"), L"Success", MB_OK);

}

void CTileMgr::SaveTileToClient()
{
	HANDLE	hFile = CreateFile(L"../Data/Tile_Client.dat", // 이름을 포함한 파일 경로
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

	for (int l = 0; l < EnumToInt(TILE_LAYER::END); ++l)
	{
		for (auto& pTile : m_vecTile[l])
		{
			tTile = static_cast<CTile*>(pTile)->GetTile();

			WriteFile(hFile, &tTile.eTileOption, sizeof(TILE_OPTION), &dwbyte, nullptr);
			WriteFile(hFile, &tTile.eTileLayer, sizeof(TILE_LAYER), &dwbyte, nullptr);
			WriteFile(hFile, &tTile.bDraw, sizeof(bool), &dwbyte, nullptr);
			WriteFile(hFile, &pTile->GetInfo(), sizeof(INFO), &dwbyte, nullptr);
		}
	}

	CloseHandle(hFile);

	ExtractPNG(); // 이미지 출력

	MessageBox(g_hWnd, _T("Tile Save 완료"), L"Success", MB_OK);
}

void CTileMgr::ExtractPNG()
{
	Bitmap bitmap(TILEX * TILECX, TILEY * TILECY, PixelFormat32bppARGB);
	Graphics graphics(&bitmap);
	
	VEC vScroll{ 0.f, 0.f };
	// 모드 설정
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	graphics.SetPixelOffsetMode(PixelOffsetModeHalf);

	// 배경은 투명색으로 칠함
	graphics.Clear(Color(0, 0, 0, 0));

	// 이미지 불러오기
	Image* pTile = CImgMgr::GetInstance()->FindImg(L"LIB_TILE_BG");
	if (nullptr == pTile)
		return;

	// 그리기 
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int iIndex = i * TILEX + j;

			for (int l = 0; l < EnumToInt(TILE_LAYER::END); ++l)
			{
				if (0 > iIndex || m_vecTile[l].size() <= (size_t)iIndex)
					continue;

				if (static_cast<CTile*>(m_vecTile[l][iIndex])->GetTile().bDraw)
				{
					m_vecTile[l][iIndex]->Render(&graphics, pTile, vScroll);
				}
			}
		}
	}

	CLSID pngClsid;
	int result = GetEncoderClsid(L"image/png", &pngClsid);
	if (result == -1)
	{
		MessageBox(g_hWnd, L"PNG EXTRACTOR ERR", L"추출 실패", MB_OK);
	}
	bitmap.Save(L"../Resource/Image/Stage/Map.png", &pngClsid, NULL);
}

// MSDN 코드
int CTileMgr::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
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
	INFO		tTileInfo{};

	int			l(0);

	while (true)
	{
		ReadFile(hFile, &tTile, sizeof(TILE), &dwbyte, nullptr);
		ReadFile(hFile, &tTileInfo, sizeof(INFO), &dwbyte, nullptr);

		if (0 == dwbyte)
			break;

		CObj* pTile = CAbstractFactory<CTile>::CreateObj(tTileInfo.vPoint.fX, tTileInfo.vPoint.fY);
		static_cast<CTile*>(pTile)->SetTile(tTile);

		m_vecTile[l].push_back(pTile);
		if (m_vecTile[l].size() == TILEX * TILEY)
			++l;
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Tile Load 완료"), L"Success", MB_OK);
}
