#include "CPhysics.h"
#include "ImGui/imgui.h"
#include "Primitive.h"
#include "ModulePhysics.h"

#include "Module.h"

CPhysics::CPhysics(GameObject* obj) :Component(obj, Types::PHYSICS)
{

	phys = nullptr;

	shapeSelected = ColliderShape::NONE;

	isShapeSelected[0] = false;
	isShapeSelected[1] = false;
	isShapeSelected[2] = false;

	isShapeCreated[0] = false;
	isShapeCreated[1] = false;
	isShapeCreated[2] = false;

	isStatic = false;
	collider = nullptr;
	hasInit = false;

	sphereRadius = 1.f;
	cylRadiusHeight = {1.f, 1.f};
}

CPhysics::~CPhysics()
{

	if (collider!=nullptr) {
		phys->RemoveBody(collider);
		collider->~PhysBody3D();
		collider = nullptr;
	}

	phys = nullptr;
	delete phys;

}

void CPhysics::Update()
{
	if (phys->isWorldOn == true) {
		if (collider != nullptr) {
			float glMat[16];
			
			
			//collider->body->getpos
			collider->body->getWorldTransform().getOpenGLMatrix(glMat);
			//float3 newPos = {,0.f,0.f};
			glMat4x4[0] = glMat[0];
			glMat4x4[1] = glMat[1];
			glMat4x4[2] = glMat[2];
			glMat4x4[3] = glMat[3];

			glMat4x4[4] = glMat[4];
			glMat4x4[5] = glMat[5];
			glMat4x4[6] = glMat[6];
			glMat4x4[7] = glMat[7];

			glMat4x4[8] = glMat[8];
			glMat4x4[9] = glMat[9];
			glMat4x4[10] = glMat[10];
			glMat4x4[11] = glMat[11];

			glMat4x4[12] = glMat[12];
			glMat4x4[13] = glMat[13];
			glMat4x4[14] = glMat[14];
			glMat4x4[15] = glMat[15];

			GO->GOtrans->matrix = glMat4x4;
			
			GO->GOtrans->UpdateRot();
			//GO->GOtrans->UpdateScl();
			GO->GOtrans->matrix[12] = glMat4x4[12];
			GO->GOtrans->matrix[13] = glMat4x4[13];
			GO->GOtrans->matrix[14] = glMat4x4[14];
			
			//GO->GOtrans->matrix[12] += matrixBeforePhys[12];
			//GO->GOtrans->matrix[13] += matrixBeforePhys[13];
			//GO->GOtrans->matrix[14] += matrixBeforePhys[14];
			
			//GO->GOtrans->SetPos(GO->GOtrans->GetPos());
			//GO->GOtrans->UpdatePos();
			

			for each (GameObject* child in GO->children)
			{
				
					child->GOtrans->matrix = glMat4x4;
					child->GOtrans->matrix[12] = matrixBeforePhys[12];
					child->GOtrans->matrix[13] = matrixBeforePhys[13];
					child->GOtrans->matrix[14] = matrixBeforePhys[14];
			
				
				//child->GOtrans->UpdatePos();
			}
			
		}
		
	}
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

void CPhysics::CallUpdateShape()
{
	float mass;

	if (isStatic == true) {
		mass = 0.f;
	}
	else {
		mass = 1.f;
	}

	switch (shapeSelected)
	{
	case CPhysics::ColliderShape::BOX:
		phys->UpdateBoxColliderSize(collider, colPos, colRot, colScl, mass);
		break;
	case CPhysics::ColliderShape::SPHERE:
		phys->UpdateSphereColliderSize(collider, colPos, colRot, sphereRadius, mass);
		break;
	case CPhysics::ColliderShape::CYLINDER:
		phys->UpdateCylinderColliderSize(collider, colPos, colRot, cylRadiusHeight, mass);
		break;
	default:
		break;
	}
	
}

void CPhysics::CreateCollider()
{

	float rx ;
	float ry ;
	float rz ;

	float3 _rot = GO->GOtrans->GetRot();
	float3 _grot = GO->GOtrans->GetGRot();

	if (GO->parent->parent == nullptr)
	{
		
		//TransformMatrix(pos, rot, scl);
		 rx = _rot.x * DEGTORAD;
		 ry = _rot.y * DEGTORAD;
		 rz = _rot.z * DEGTORAD;
	}
	else
	{
		rx = _grot.x * DEGTORAD;
		ry = _grot.y * DEGTORAD;
		rz = _grot.z * DEGTORAD;
		//gpos = ppos + pos;
		//grot = prot + rot;
		//gscl.x = pscl.x * scl.x;
		//gscl.y = pscl.y * scl.y;
		//gscl.z = pscl.z * scl.z;
		//TransformMatrix(gpos, grot, gscl);

	}
	Quat qrot = Quat::FromEulerXYZ(rx, ry, rz);

	//rotation vector
	vec3 rvec(qrot.Axis().x, qrot.Axis().y, qrot.Axis().z);

	//rotation angle
	float ra = qrot.Angle(); 

	float mass;
	if (isStatic == true) {
		mass = 0.f;
	}
	else {
		mass = 1.f;
	}

	switch (shapeSelected)
	{
	case CPhysics::ColliderShape::BOX:
	{
		PrimCube cube;
		GO->GOtrans->setIdentity(cube.transform);
		
		cube.SetPos(colPos.x, colPos.y, colPos.z);
		//cube.SetRotation();
		cube.size.x = GO->GOtrans->GetScale().x;
		cube.size.y = GO->GOtrans->GetScale().y;
		cube.size.z = GO->GOtrans->GetScale().z;
		
		//cube.SetRotation(ra,rvec);
		//cube.SetRotation(ra, (rx, ry, rz));
		//cube.transform = GO->GOtrans->matrix;
		//cube.transform
		cube.color = Green;


		collider = phys->AddBody(cube, mass);

	}
	break;
	case CPhysics::ColliderShape::SPHERE:
	{
		PrimSphere sphere;
	
		sphere.SetPos(colPos.x, colPos.y, colPos.z);
		sphere.SetRotation(ra, rvec);
		sphere.radius = 1.0f;

		sphere.color = Green;

		collider = phys->AddBody(sphere, mass);
	}
	break;
	case CPhysics::ColliderShape::CYLINDER:
	{
		PrimCylinder cylinder;
		
		cylinder.SetPos(colPos.x, colPos.y, colPos.z);
		cylinder.SetRotation(ra, rvec);
		cylinder.radius = 1.0f;
		cylinder.height = 1.0f;
		cylinder.color = Green;

		collider = phys->AddBody(cylinder, mass);
		//collider->SetTransform(&GO->GOtrans->matrix);

	}
	break;
	default:
		break;
	}
}

void CPhysics::OnInspector()
{
	if (ImGui::CollapsingHeader("Physics"))
	{
		//if (ImGui::CollapsingHeader("Physic Body"))
		{
			if (ImGui::Checkbox("Static", &isStatic)) {
				if (collider != nullptr) {
					CallUpdateShape();
				}
				
			}
		}

		//if (ImGui::CollapsingHeader("Collider"))
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
						colPos = GO->GOtrans->GetPos();
						CreateCollider();
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

				switch (shapeSelected)
				{
				case CPhysics::ColliderShape::BOX:
				{
					ImGui::Text("X	");
					ImGui::SameLine();
					ImGui::Text("Y	");
					ImGui::SameLine();
					ImGui::Text("Z	");
					if (ImGui::DragFloat3("Position: ", colPos.ptr(), 0.1)) {
						CallUpdateShape();
					}
					ImGui::Text("X	");
					ImGui::SameLine();
					ImGui::Text("Y	");
					ImGui::SameLine();
					ImGui::Text("Z	");
					if (ImGui::DragFloat3("Rotation: ", colRot.ptr(), 0.1)) {
						CallUpdateShape();
					}
					ImGui::Text("X	");
					ImGui::SameLine();
					ImGui::Text("Y	");
					ImGui::SameLine();
					ImGui::Text("Z	");
					if (ImGui::DragFloat3("Scale: ", colScl.ptr(), 0.1)) {
						CallUpdateShape();
					}
				}
				break;
				case CPhysics::ColliderShape::SPHERE:
				{
					ImGui::Text("X	");
					ImGui::SameLine();
					ImGui::Text("Y	");
					ImGui::SameLine();
					ImGui::Text("Z	");
					if (ImGui::DragFloat3("Position: ", colPos.ptr(), 0.1)) {
						CallUpdateShape();
					}
					ImGui::Text("X	");
					ImGui::SameLine();
					ImGui::Text("Y	");
					ImGui::SameLine();
					ImGui::Text("Z	");
					if (ImGui::DragFloat3("Rotation: ", colRot.ptr(), 0.1)) {
						CallUpdateShape();
					}
					if (ImGui::DragFloat("Radius: ", &sphereRadius, 0.1)) {
						CallUpdateShape();
					}
				}
				break;
				case CPhysics::ColliderShape::CYLINDER:
				{
					ImGui::Text("X	");
					ImGui::SameLine();
					ImGui::Text("Y	");
					ImGui::SameLine();
					ImGui::Text("Z	");
					if (ImGui::DragFloat3("Position: ", colPos.ptr(), 0.1)) {
						CallUpdateShape();
					}
					ImGui::Text("X	");
					ImGui::SameLine();
					ImGui::Text("Y	");
					ImGui::SameLine();
					ImGui::Text("Z	");
					if (ImGui::DragFloat3("Rotation: ", colRot.ptr(), 0.1)) {
						CallUpdateShape();
					}
					if (ImGui::DragFloat2("Radius & Height: ", cylRadiusHeight.ptr(), 0.1)) {
						CallUpdateShape();
					}
				}
				break;
				default:
					break;
				}

				
					/*if (ImGui::Button("Update Collider Size"))
					{
						switch (shapeSelected)
						{
						case CPhysics::ColliderShape::BOX:
							phys->UpdateBoxColliderSize(collider, colPos, colRot, colScl, 1.0f);
							break;
						case CPhysics::ColliderShape::SPHERE:
							phys->UpdateSphereColliderSize(collider, colPos, colRot, sphereRadius, 1.0f);
							break;
						case CPhysics::ColliderShape::CYLINDER:
							phys->UpdateCylinderColliderSize(collider, colPos, colRot, cylRadiusHeight, 1.0f);
							break;
						default:
							break;
						}

					}*/

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
