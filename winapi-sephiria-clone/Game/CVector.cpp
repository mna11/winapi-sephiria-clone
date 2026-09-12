#include "pch.h"
#include "CVector.h"

CVector::CVector()
	: m_fX(0.f), m_fY(0.f)
{
}

CVector::CVector(float x, float y)
	: m_fX(x), m_fY(y)
{
}

CVector::~CVector()
{
}

CVector CVector::operator+(const CVector& rhs) const
{
	return CVector{ this->m_fX + rhs.m_fX, this->m_fY + rhs.m_fY };
}

CVector CVector::operator-(const CVector& rhs) const
{
	return CVector{ this->m_fX - rhs.m_fX, this->m_fY - rhs.m_fY };
}

CVector CVector::operator*(float fScalar) const
{
	return CVector( this->m_fX * fScalar, this->m_fY * fScalar);
}

bool CVector::operator==(const CVector& rhs) const
{
	return (this->m_fX == rhs.m_fX && this->m_fY == rhs.m_fY);
}

float CVector::Dot(const CVector& rhs) const
{
	return this->m_fX * rhs.m_fY +  this->m_fY * rhs.m_fX;
}

float CVector::GetAngle() const
{
	return atan2f(m_fY, m_fX) * 180.f / PI;
}
