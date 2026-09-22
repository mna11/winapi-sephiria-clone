#pragma once

#include <random>

/////////////////////////////////////////

#define		PURE =0

#define		WINCX 1280
#define		WINCY 720

#define		DEAD    1
#define     NOEVENT 0

#define     TILEX   100
#define     TILEY   100 

#define		PIXEL_SCALE 5 // 도트 픽셀 배율

#define     TILECX  (16 * PIXEL_SCALE)
#define     TILECY  (16 * PIXEL_SCALE)

#define		VK_MAX	0xff

#define		PI		3.14f


// EffectMgr 옵션
#define		EFTMGR_IMAGE  0x0000
#define		EFTMGR_STRING 0x0001

#define		EFTMGR_FIXED  0x0000
#define		EFTMGR_FOLLOW 0x0010
#define		EFTMGR_MOVE   0x0020


/////////////////////////////////////////
// 매니저 싱글톤 단축
// - TimeMgr
#define		DT				CTimeMgr::GetInstance()->GetDeltaTime()
#define     GET_TIME()		CTimeMgr::GetInstance()->GetTime()
#define		GET_DURATION(x)	CTimeMgr::GetInstance()->GetTime(x)
//// - KeyMgr
#define		KEY_PRESS(x)	CKeyMgr::GetInstance()->KeyPress(x)
#define		KEY_DOWN(x)		CKeyMgr::GetInstance()->KeyDown(x)
#define		KEY_UP(x)		CKeyMgr::GetInstance()->KeyUp(x)
#define		KEY_HOLD(x)		CKeyMgr::GetInstance()->KeyHold(x)
//
/////////////////////////////////////////
// 열거체

enum class OBJID	{ PLAYER, MONSTER, MONSTER_BULLET, WEAPON, EFFECT, UI, CAMERA, END };
enum class RENDERID	{ PRIORITY, GAMEOBJECT, EFFECT, UI, CAMERA, END };
enum class UIID		{ BASIC_INFO, BOSS_HP, STAT_INFO, INVENTORY, SHOP, ITEM_INFO, REWARD, END };
enum class SCENEID	{ STAGE0, STAGE1, BOSS_STAGE, END };

enum class KEY_STATE { NONE, DOWN, HOLD, UP, END };

enum class TILE_OPTION { FLOOR, WALL, AIR, INTERACTION, END };
enum class TILE_LAYER { LAYER0, LAYER1, LAYER2, LAYER3, END };

/////////////////////////////////////////
// 구조체

// 2차원 벡터 - 원래 클래스로 만들었다가 INFO랑 변환이 힘들어 구조체로 만든 뒤, INFO도 VEC으로 바꿈
typedef struct tagVector
{
	// 멤버 변수
	float fX, fY;

	// 생성자
	tagVector() { ZeroMemory(this, sizeof(tagVector)); }
	tagVector(float X, float Y) : fX(X), fY(Y) {}

	// 연산자 오버로딩 - 추후 필요시 추가
	tagVector	operator+(const tagVector& rhs) const	{ return tagVector{ fX + rhs.fX, fY + rhs.fY }; }
	tagVector	operator-(const tagVector& rhs) const	{ return tagVector{ fX - rhs.fX, fY - rhs.fY }; }
	tagVector	operator*(float fScalar) const			{ return tagVector{ fScalar * fX, fScalar * fY }; }
	tagVector	operator*(const tagVector& rhs) const	{ return tagVector{ fX * rhs.fX, fY * rhs.fY }; }
	bool	    operator==(const tagVector& rhs) const  { return (fX == rhs.fX && fY == rhs.fY); }
	bool	    operator!=(const tagVector& rhs) const  { return (fX != rhs.fX || fY != rhs.fY); }
	tagVector& operator+=(const tagVector& rhs) {
		fX += rhs.fX;
		fY += rhs.fY;
		return *this;
	}
	tagVector& operator+=(int iScalar) {
		fX += iScalar;
		fY += iScalar;
		return *this;
	}
	tagVector& operator+=(float fScalar) {
		fX += fScalar;
		fY += fScalar;
		return *this;
	}
	tagVector& operator-=(const tagVector& rhs) {
		fX -= rhs.fX;
		fY -= rhs.fY;
		return *this;
	}
	tagVector& operator=(const tagVector& rhs) {
		fX = rhs.fX;
		fY = rhs.fY;
		return *this;
	}


	// 벡터 내적
	float		Dot(const tagVector& rhs) const			{ return fX * rhs.fX + fY * rhs.fY; }
	// 벡터 크기
	float		Norm() const							{ return sqrtf(fX * fX + fY * fY); }
	// 벡터 정규화
	tagVector	Normalize() const 
	{ 
		float fNorm = sqrtf(fX * fX + fY * fY);
		if (fNorm == 0) return tagVector{ 0.f, 0.f };
		else return tagVector{ fX / fNorm, fY / fNorm };
	}
} VEC;

// OBJ 정보
typedef struct tagInfo
{
	VEC vPoint;
	VEC vSize;

	tagInfo() { ZeroMemory(this, sizeof(tagInfo)); }
	tagInfo(float fX, float fY, float fCX, float fCY) : vPoint{ fX, fY }, vSize{ fCX, fCY } {}
} INFO;


// 스탯
typedef struct tagStat
{
	int				iHp;		// 체력
	int				iMaxHp;		// 최대 체력
	int				iAtk;		// 공격력

	int				iMp;		// MP
	int				iMaxMp;		// 최대 MP

	int				iDefense;   // 방어력
	int				iEvasion;   // 회피율

	int				iDash;		// 대시 가능 횟수
	int				iMaxDash;	// 대시 최대 보유 횟수

	// 백분율형
	float			fCriticalChange;	// 치확
	float			fCriticalDamage;	// 치뎀
	float			fAttackSpeed;		// 공속
	float			fMoveSpeed;			// 이속
	float			fDashRecoverySpeed; // 대시 회복 속도

	int				iBarter;			// 교섭력
	int				iLuck;				// 행운

	// 백분율형
	float			fExperienceDrop;	  // 경험치 드랍율
	float			fLeafDrop;			  // 돈(리프) 드랍율
	float			fHpRestoredOnLevelUp; // 레벨업시 체력 회복률
} STAT;

// 무기공격 정보
typedef struct tagAttackInfo
{
	int		iLevel;				// 공격 레벨
	int		iMaxLevel;			// 최대 연격 횟수
	bool	bNextAtk;			// 다음 연격 가능 플래그
	double	dElapseTime;		// 무기 공격 시작 후 지나간 시간
	double  dMaxTime;			// 무기별 공격 시간
	double  dComboTime;			// 끝나기 전에 공격 클릭할 시, 연격이 있다면 연격을 함
								// 끝나고 클릭, 끝나고 클릭 -> Attack1 - Attack1 ....
								// 끝나기 전 클릭, 끝나기 전 클릭 -> Attack1 - Attack2 - Attack3
} ATK_INFO;


// 스프라이트 애니메이션용
// QueryPerfomanceCount 사용으로 변경
typedef struct tagFrame
{
	int iStart;
	int iEnd;
	int iMotion;
	double		  dFrameSpeed;
	double		  dFrameElapsedTime;
} FRAME;

typedef struct tagLinePoint
{
	VEC   vPoint;

	tagLinePoint() { ZeroMemory(this, sizeof(tagLinePoint)); }
	tagLinePoint(float X, float Y) : vPoint{X, Y} {}

}LINEPOINT;

typedef struct tagLine
{
	tagLinePoint	tLPoint;
	tagLinePoint	tRPoint;

	tagLine() { ZeroMemory(this, sizeof(tagLine)); }
	tagLine(tagLinePoint& LPoint, tagLinePoint& RPoint)
		: tLPoint(LPoint), tRPoint(RPoint) {
	}
}LINE;

typedef struct tagTileInfo
{
	TILE_OPTION	eTileOption;
	TILE_LAYER	eTileLayer;
} TILE;

/////////////////////////////////////////
// 함수

template<typename T>
void SafeDelete(T& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

// C++14부터 추가된, enum class에서 값 뽑는 법
template<typename E> 
constexpr auto
toUType(E enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E>>(enumerator);
}

extern HWND g_hWnd;
extern std::mt19937 g_engine;
extern Gdiplus::PrivateFontCollection* g_pFontCollection;