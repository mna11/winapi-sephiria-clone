#pragma once
class CMainGame
{
public:
	CMainGame();
	~CMainGame();
public:
	void Initialize();
	void Update();
	void LateUpdate();
	void Render();
	void Release();

private:
	HDC			m_hDC;
	HDC			m_hBackDC;		   // 더블 버퍼링 용도
	HBITMAP		m_hOldBit;         // 더블 버퍼링 용도
	HBITMAP     m_hBackBit;        // 더블 버퍼링 용도
	Graphics*	m_pBackGraphics;   // 더블 버퍼링 용도
};

