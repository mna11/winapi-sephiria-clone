#include "pch.h"
#include "CImgMgr.h"

CImgMgr* CImgMgr::m_pInstance = nullptr;

CImgMgr::CImgMgr()
{
}

CImgMgr::~CImgMgr()
{
	Release();
}

void CImgMgr::InsertImg(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapImg.begin(), m_mapImg.end(), [&pImgKey](auto& p)
		{
			return !lstrcmp(pImgKey, p.first);
		});


	if (iter == m_mapImg.end())
	{
		Image* pImg = Image::FromFile(pFilePath);
		m_mapImg.insert({ pImgKey, pImg });
	}
}

Image* CImgMgr::FindImg(const TCHAR* pImgKey) const
{
	auto	iter = find_if(m_mapImg.begin(), m_mapImg.end(), [&pImgKey](auto& p)
		{
			return !lstrcmp(pImgKey, p.first);
		});

	if (iter == m_mapImg.end())
		return nullptr;

	return iter->second;
}

void CImgMgr::Release()
{
	for_each(m_mapImg.begin(), m_mapImg.end(), [](auto& p)
		{
			if (p.second)
			{
				delete p.second;
				p.second = nullptr; 
			}
		});

	m_mapImg.clear();
}
