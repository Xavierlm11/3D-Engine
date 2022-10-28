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

//Types Component::()
//{
//
//}