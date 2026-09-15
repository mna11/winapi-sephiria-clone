#pragma once

class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void Initialize()				PURE;
	virtual void Update()					PURE;
	virtual void LateUpdate()				PURE;
	virtual void Render(Graphics* pGraphics) PURE;
	virtual void Release()					PURE;

public:
	virtual void Init_CreateObj() PURE;
	virtual void Init_InsertImg() PURE;
};