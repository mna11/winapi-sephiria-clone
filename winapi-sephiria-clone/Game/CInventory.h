#pragma once

class CPlayer;
class CItem;

class CInventory
{
public:
	CInventory();
	~CInventory();

public:
	void Initialize();
	void Release();

public:
	void InsertItem(int iIdx, int iID);
	void EraseItem(int iIdx);
	void MoveItem(int iStartIdx, int iEndIdx);

public:
	void SetOwner(CPlayer* pPlayer) { m_pOwner = pPlayer; }

public:
	const vector<CItem*>& GetItems() const { return m_vecItems; }
	CItem* GetItem(int iIdx) const { return m_vecItems[iIdx]; }

private:
	int				m_iInvenSize;
	vector<CItem*>	m_vecItems;

	CPlayer*		m_pOwner;

};

