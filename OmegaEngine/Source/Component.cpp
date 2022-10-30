#include "Component.h"

Component::Component(GameObject* obj, Types type):GO(obj),type(type),active(true)
{

}
Component::~Component()
{

}
/*
void Component::()
{

}
*/

void Component::Enable()
{
	active = true;
}
void Component::Disable()
{
	active = false;
}

bool Component::IsEnable()
{
	return active;
}

void Component::Load()
{
}

void Component::Save()
{
}

Component::Types Component::GetType() const
{
	return type;
}

GameObject* Component::GetGO()
{
	return GO;
}

//Types Component::()
//{
//
//}