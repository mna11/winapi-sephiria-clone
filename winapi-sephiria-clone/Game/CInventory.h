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
	void Update();
	void Release();

public:
	bool InsertItem(int iIdx, int iID, ITEM_TYPE eItemType );
	void ReduceItem(int iIdx);
	void EraseItem(int iIdx);
	void MoveItem(int iStartIdx, int iEndIdx);
	int  FindItem(int iID, ITEM_TYPE eItemType);
	bool UsePotion();

public:
	void					SetOwner(CPlayer* pPlayer) { m_pOwner = pPlayer; }

public:
	const vector<CItem*>&	GetItems() const { return m_vecItems; }
	const vector<int>&		GetLevels() const { return m_vecLevels; }
	const int&				GetInventorySize() const { return m_iInvenSize; }
	CItem*					GetItem(int iIdx) const { return m_vecItems[iIdx]; }
	const CPlayer*			GetOwner() const { return m_pOwner; }
	const int&				GetUsePotionSlot() const { return m_iUsePotionSlot; }

public:
	bool					IsExistItem(int iIdx) { return (- 1 != iIdx && (nullptr != m_vecItems[iIdx])); }
	void 					ChangePotion(int iWheelScroll, bool bUse = false);

private:
	void					UpdateLevel();

private:
	const int		m_iInvenSize;
	vector<CItem*>	m_vecItems;
	vector<int>		m_vecLevels;

	CPlayer*		m_pOwner;

	int				m_iUsePotionSlot; // 현재 BasicInfo 퀵 슬롯에 저장되어 있는 사용할 Potion	
};

