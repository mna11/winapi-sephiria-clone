#include "pch.h"
#include "CItem.h"

CItem::CItem()
{
}

CItem::~CItem()
{
	Release();
}

void CItem::Initialize()
{
}

int CItem::Update()
{
	return 0;
}

void CItem::LateUpdate()
{
}

void CItem::Render(Graphics*)
{
}

void CItem::Release()
{
}
