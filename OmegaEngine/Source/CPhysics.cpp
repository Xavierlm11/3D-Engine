#include "CPhysics.h"
#include "ImGui/imgui.h"
#include "Primitive.h"
#include "ModulePhysics.h"

#include "Module.h"

CPhysics::CPhysics(GameObject* obj) :Component(obj, Types::PHYSICS)
{

	phys = nullptr;

	shapeSelected = ColliderShape::NONE;

	isShapeSelectedTrigger[0] = false;
	isShapeSelectedTrigger[1] = false;
	isShapeSelectedTrigger[2] = false;

	isShapeSelected[0] = false;
	isShapeSelected[1] = false;
	isShapeSelected[2] = false;

	isShapeCreated[0] = false;
	isShapeCreated[1] = false;
	isShapeCreated[2] = false;

	isStatic = false;
	collider = nullptr;
}

CPhysics::~CPhysics()
{
	
}

void CPhysics::Update()
{
	
}

void CPhysics::CheckShapes() {

	for (int h = 0; h < static_cast<int>(ColliderShape::Count); h++) {

		//if (isShapeSelectedTrigger[h] == true) {
			///isShapeSelected[h] = true;
			///isShapeSelectedTrigger[h] = false;
		if (isShapeSelected[h] == true) {
			if (isShapeCreated[h] == false)
			{
				if (shapeSelected != ColliderShape::NONE && shapeSelected != static_cast<ColliderShape>(h)) {

				}
				shapeSelected = static_cast<ColliderShape>(h);
				isShapeCreated[h] = true;
			}
		}
		else {
			if (isShapeCreated[h] == true) {
				//isShapeCreated[h] = false;
				/*for (int i = 0; i < static_cast<int>(ColliderShape::Count); i++) {
					if (i != h) {
						isShapeCreated[i] = false;
					}
				}*/
			}
		}

	}

	int a = 0;
	for (int i = 0; i < static_cast<int>(ColliderShape::Count); i++) {
		if (isShapeSelected[i] == true) 
		{
			a++;
		}
	}

	if (a == 0) {
		shapeSelected = ColliderShape::NONE;
		for (int i = 0; i < static_cast<int>(ColliderShape::Count); i++) {
			if (isShapeCreated[i] == true)
			{
				isShapeCreated[i] = false;
			}
		}
	}

	if (shapeSelected == ColliderShape::NONE) 
	{
		for (int i = 0; i < static_cast<int>(ColliderShape::Count); i++) {
			if (isShapeSelected[i] == true) {
				isShapeSelected[i] = false;
				if (isShapeCreated[i] == true) {
					isShapeCreated[i] = false;
				}
			}
		}
	}
	else 
	{
		for (int i = 0; i < static_cast<int>(ColliderShape::Count); i++) {

			if (i != static_cast<int>(shapeSelected)) {
				if (isShapeSelected[i] == true) {
					isShapeSelected[i] = false;
					if (isShapeCreated[i] == true) {
						isShapeCreated[i] = false;
					}
				}
			}

		}
	}
}

void CPhysics::OnInspector()
{
	if (ImGui::CollapsingHeader("Physics"))
	{
		if (ImGui::CollapsingHeader("Physic Body"))
		{
			ImGui::Checkbox("Static", &isStatic);
		}

		if (ImGui::CollapsingHeader("Collider"))
		{
			if (collider == nullptr) 
			{
				ImGui::Text("This object has no collider attached to it.");
				ImGui::Text("Select one shape and create one.");

				ImGui::Checkbox("Box", &isShapeSelected[0]);

				ImGui::Checkbox("Sphere", &isShapeSelected[1]);

				ImGui::Checkbox("Cylinder", &isShapeSelected[2]);

				CheckShapes();

				if (shapeSelected != ColliderShape::NONE) {
					if (ImGui::Button("Create Collider"))
					{
						switch (shapeSelected)
						{
						case CPhysics::ColliderShape::BOX: 
							{
								PrimCube cube;
								collider = phys->AddBody(cube,3.f);
							}
							break;
						case CPhysics::ColliderShape::SPHERE:
							{
								PrimSphere sphere;
								collider = phys->AddBody(sphere, 3.f);
							}
							break;
						case CPhysics::ColliderShape::CYLINDER:
							{
								PrimCylinder cylinder;
								collider = phys->AddBody(cylinder, 3.f);
							}
							break;
						default:
							break;
						}
					}
				}
				
			}
			else {
				const char* colliderType = "";
				switch (shapeSelected)
				{
				case CPhysics::ColliderShape::BOX:
				{
					colliderType = "Box";
				}
				break;
				case CPhysics::ColliderShape::SPHERE:
				{
					colliderType = "Sphere";
				}
				break;
				case CPhysics::ColliderShape::CYLINDER:
				{
					colliderType = "Cylinder";
				}
				break;
				default:
					break;
				}
				std::string colName = "Collider Attached: ";
				colName  += colliderType;
				ImGui::Text(colName.c_str());
				
				if (ImGui::Button("Remove Collider"))
				{
					phys->RemoveBody(collider);
					collider->~PhysBody3D();
					collider = nullptr;
				}
			}
			
		}

		ImGui::Text("");

		if (ImGui::Button("Remove Component", ImVec2(140, 20))) 
		{

		}

	}
}
