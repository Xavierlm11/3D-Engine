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

	void Enable();

	void Disable();

	bool IsEnable();
	Types GetType();

public:
	Types type =Types::NONE;
	GameObject* GO;
	bool active=true;
};

