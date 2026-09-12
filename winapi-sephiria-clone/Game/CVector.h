#pragma once
class CVector
{
public:
	CVector();
	CVector(float x, float y);
	~CVector();
	
public:
	CVector operator+(const CVector&) const;
	CVector operator-(const CVector&) const; 
	CVector operator*(float fScalar) const;
	bool    operator==(const CVector&) const;

public:
	float Dot(const CVector&) const;
	float GetAngle() const;

public:
	float m_fX;
	float m_fY;
};

