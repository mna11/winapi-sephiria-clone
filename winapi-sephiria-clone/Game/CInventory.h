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
	bool InsertItem(int iIdx, int iID);
	void EraseItem(int iIdx);
	void MoveItem(int iStartIdx, int iEndIdx);

public:
	void SetOwner(CPlayer* pPlayer) { m_pOwner = pPlayer; }

public:
	const vector<CItem*>& GetItems() const { return m_vecItems; }
	const int& GetInventorySize() const { return m_iInvenSize; }
	CItem* GetItem(int iIdx) const { return m_vecItems[iIdx]; }
	const CPlayer* GetOwner() const { return m_pOwner; }

public:
	bool IsExistItem(int iIdx) { return (- 1 != iIdx && (nullptr != m_vecItems[iIdx])); }

private:
	const int		m_iInvenSize;
	vector<CItem*>	m_vecItems;

	CPlayer*		m_pOwner;
};

