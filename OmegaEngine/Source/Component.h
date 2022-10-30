#pragma once

class GameObject;

class Component
{
public:
	enum class Types
	{
		NONE,
		TRANSFORM,
		MESH,
		MATERIAL,
		LIGHT
	};
	Component(GameObject* obj, Types type);
	virtual ~Component();

	virtual void Enable();

	virtual void Disable();

	virtual bool IsEnable();

	virtual void Load();

	virtual void Save();

	Types GetType() const;
	GameObject* GetGO() ;
	virtual void Update(){}

public:
	Types type =Types::NONE;
	GameObject* GO;
	bool active=true;
};

