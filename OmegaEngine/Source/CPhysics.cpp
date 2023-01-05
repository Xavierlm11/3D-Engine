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
	hasInit = false;
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
								cube.size.x = GO->GOtrans->GetScale().x;
								cube.size.y = GO->GOtrans->GetScale().y;
								cube.size.z = GO->GOtrans->GetScale().z;
								float3 pos = GO->GOtrans->GetPos();
								//cube.transform = GO->GOtrans->matrix;
								cube.SetPos(pos.x, pos.y, pos.z);
								//cube.SetRotation();
								
								collider = phys->AddBody(cube,3.f);
								
							}
							break;
						case CPhysics::ColliderShape::SPHERE:
							{
								PrimSphere sphere;
								sphere.radius = 1.0f;
								sphere.transform = GO->GOtrans->matrix;
								collider = phys->AddBody(sphere, 3.f);
							}
							break;
						case CPhysics::ColliderShape::CYLINDER:
							{
								PrimCylinder cylinder;
								cylinder.radius = 1.0f;
								cylinder.height = 1.0f;
								cylinder.transform = GO->GOtrans->matrix;
								collider = phys->AddBody(cylinder, 3.f);
								//collider->SetTransform(&GO->GOtrans->matrix);
								
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

				float3 boxScale;
				float sphereRadius;
				float2 cyliRadiusHeight;

				switch (shapeSelected)
				{
				case CPhysics::ColliderShape::BOX:
				{
					
					ImGui::Text("X	");
					ImGui::SameLine();
					ImGui::Text("Y	");
					ImGui::SameLine();
					ImGui::Text("Z	");
					if (ImGui::DragFloat3("Scale: ", boxScale.ptr(), 0.1)) {
						
					}
				}
				break;
				case CPhysics::ColliderShape::SPHERE:
				{
					if (ImGui::DragFloat("Radius: ", &sphereRadius, 0.1)) {

					}
				}
				break;
				case CPhysics::ColliderShape::CYLINDER:
				{
					if (ImGui::DragFloat2("Radius & Height: ", cyliRadiusHeight.ptr(), 0.1)) {

					}
				}
				break;
				default:
					break;
				}

				if (ImGui::Button("Update Collider Size"))
				{
					phys->RemoveBody(collider);
					collider->~PhysBody3D();
					collider = nullptr;

					switch (shapeSelected)
					{
					case CPhysics::ColliderShape::BOX:
					{
						PrimCube cube;
						cube.size.x = boxScale.x;
						cube.size.y = boxScale.y;
						cube.size.z = boxScale.z;
						//float3 pos = GO->GOtrans->GetPos();
						cube.transform = GO->GOtrans->matrix;
						//cube.SetPos(pos.x, pos.y, pos.z);

						collider = phys->AddBody(cube, 3.f);

					}
					break;
					case CPhysics::ColliderShape::SPHERE:
					{
						PrimSphere sphere;
						sphere.radius = sphereRadius;
						sphere.transform = GO->GOtrans->matrix;
						collider = phys->AddBody(sphere, 3.f);
					}
					break;
					case CPhysics::ColliderShape::CYLINDER:
					{
						PrimCylinder cylinder;
						cylinder.radius = cyliRadiusHeight.x;
						cylinder.height = cyliRadiusHeight.y;
						cylinder.transform = GO->GOtrans->matrix;
						collider = phys->AddBody(cylinder, 3.f);
						//collider->SetTransform(&GO->GOtrans->matrix);

					}
					break;
					default:
						break;
					}
				}

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
