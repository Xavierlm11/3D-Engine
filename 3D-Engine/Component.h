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
		MATERIAL
	};
	Component(GameObject* obj, Types type);
	~Component();

	void Enable();

	void Disable();

	bool IsEnable();
	Types GetType();

public:
	Types type =Types::NONE;
	GameObject* GO;
	bool active=true;
};

