#include "CTransform.h"
#include "Globals.h"

CTransform::CTransform(GameObject* obj):Component( obj, Types::TRANSFORM )
{
	this->GO = obj;
}

CTransform::~CTransform()
{

}

void CTransform::Update()
{

	
}

void CTransform::TransformMatrix()
{
	//pos = _pos;
	//rot = _rot;
	//scl = _scl;

	float x = rot.x * DEGTORAD;
	float y = rot.y * DEGTORAD;
	float z = rot.z * DEGTORAD;

	/*Quat _rot = Quat::FromEulerXYZ(x, y, z);
	mat4x4::
	matrix = float4x4::FromTRS(pos, _rot, scl).Transposed();
	*/
	matrix[0] = cos(y) * cos(z);
	matrix[1] = -cos(x) * sin(z) + sin(y) * cos(z) * sin(x);
	matrix[2] = sin(x) * sin(z) + sin(y) * cos(z) * cos(x);
	matrix[3] = pos.x;

	matrix[4] = cos(y) * sin(z);
	matrix[5] = cos(x) * cos(z) + sin(y) * sin(z) * sin(z);
	matrix[6] = -sin(x) * cos(z) + sin(y) * sin(z) * cos(x);
	matrix[7] = pos.y;

	matrix[8] = -sin(y);
	matrix[9] = cos(y) * sin(x);
	matrix[10] = cos(x) * cos(y);
	matrix[11] = pos.z;

	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1;

	matrix[0] *= scl.x;
	matrix[5] *= scl.y;
	matrix[10] *= scl.z;

	matrix = transpose(matrix);

	


	

}

void CTransform::OnInspector()
{
		float3 newPos = pos;
		float3 newScl = scl;
		float3 newrot = rot;
	if (ImGui::CollapsingHeader("Transform"))
	{

		ImGui::Text("X	");
		ImGui::SameLine();
		ImGui::Text("Y	");
		ImGui::SameLine();
		ImGui::Text("Z	");
		if(ImGui::DragFloat3("Pos.", newPos.ptr(), 0.1))SetPos(newPos);
		
		ImGui::Text("X	");
		ImGui::SameLine();
		ImGui::Text("Y	");
		ImGui::SameLine();
		ImGui::Text("Z	");
		if(ImGui::DragFloat3("Rotation.", newrot.ptr(),0.1)) SetRot(newrot);
		
		ImGui::Text("X	");
		ImGui::SameLine();
		ImGui::Text("Y	");
		ImGui::SameLine();
		ImGui::Text("Z	");
		if (ImGui::DragFloat3("Scale.", newScl.ptr(), 0.1)) SetScale(newScl);



	}

	//TransformMatrix(pos, rot, scl);

	//if (GO->children.size() != 0)
	//{
	//	for (uint i = 0; i < GO->children.size(); ++i)
	//	{
	//		//GO->children[i]->GOtrans->TransformMatrix(GO->children[i]->GOtrans->GetPos() + pos, GO->children[i]->GOtrans->GetRot() + rot, GO->children[i]->GOtrans->GetScale() + scl);
	//		/*GO->children[i]->GOtrans->SetPos(GO->children[i]->GOtrans->GetPos() + newPos);
	//		GO->children[i]->GOtrans->SetRot(GO->children[i]->GOtrans->GetRot() + newPos);
	//		GO->children[i]->GOtrans->SetScale({ GO->children[i]->GOtrans->GetScale().x * newScl.x,
	//			GO->children[i]->GOtrans->GetScale().y * newScl.y,
	//			GO->children[i]->GOtrans->GetScale().z * newScl.z });*/
	//		//GO->children[i]->GOtrans->SetRot(GO->children[i]->GOtrans->GetRot() + rot), nrot = rot;
	//		/*if (pos.x != GO->children[i]->GOtrans->npos.x || pos.y != GO->children[i]->GOtrans->npos.y || pos.z != GO->children[i]->GOtrans->npos.z)
	//		{
	//			if ((pos.x < GO->children[i]->GOtrans->npos.x || pos.y < GO->children[i]->GOtrans->npos.y || pos.z < GO->children[i]->GOtrans->npos.z)&&(pos.x >= 0 || pos.y >=0|| pos.z >=0))
	//				GO->children[i]->GOtrans->SetPos(GO->children[i]->GOtrans->GetPos() + pos), GO->children[i]->GOtrans->npos=pos;

	//			else if ((pos.x < GO->children[i]->GOtrans->npos.x || pos.y < GO->children[i]->GOtrans->npos.y || pos.z < GO->children[i]->GOtrans->npos.z) && (pos.x <= 0 || pos.y <= 0 || pos.z <= 0))
	//				GO->children[i]->GOtrans->SetPos(GO->children[i]->GOtrans->GetPos() - pos), GO->children[i]->GOtrans->npos = pos;

	//			else if ((pos.x > GO->children[i]->GOtrans->npos.x || pos.y > GO->children[i]->GOtrans->npos.y || pos.z > GO->children[i]->GOtrans->npos.z) && (pos.x <= 0 || pos.y <= 0 || pos.z <= 0))
	//				GO->children[i]->GOtrans->SetPos(GO->children[i]->GOtrans->GetPos() + pos), GO->children[i]->GOtrans->npos = pos;

	//			else if ((pos.x > GO->children[i]->GOtrans->npos.x || pos.y > GO->children[i]->GOtrans->npos.y || pos.z > GO->children[i]->GOtrans->npos.z) && (pos.x >= 0 || pos.y >= 0 || pos.z >= 0))
	//				GO->children[i]->GOtrans->SetPos(GO->children[i]->GOtrans->GetPos() - pos), GO->children[i]->GOtrans->npos = pos;

	//		}*/

	//		if (rot.x != GO->children[i]->GOtrans->nrot.x || rot.y != GO->children[i]->GOtrans->nrot.y || rot.z != GO->children[i]->GOtrans->nrot.z)
	//			GO->children[i]->GOtrans->SetRot(GO->children[i]->GOtrans->GetRot() + rot), GO->children[i]->GOtrans->nrot=rot;

	//		if (scl.x != GO->children[i]->GOtrans->nscl.x || scl.y != GO->children[i]->GOtrans->nscl.y || scl.z != GO->children[i]->GOtrans->nscl.z)
	//			GO->children[i]->GOtrans->SetScale({ 
	//			GO->children[i]->GOtrans->GetScale().x * scl.x,
	//			GO->children[i]->GOtrans->GetScale().y * scl.y,
	//			GO->children[i]->GOtrans->GetScale().z * scl.z }), GO->children[i]->GOtrans->nscl=scl;
	//	}
	//}


	
}


mat4x4 CTransform::GlobalMatrix()
{
	if (GO->parent != nullptr)
	{
		return matrix;
	}

	return matrix * GO->parent->GOtrans->GlobalMatrix();
}

void CTransform::SetPos(float3 _pos)
{

	pos = _pos;

	//if(GO->parent->GOtrans!=nullptr)
		float3 cpos = GO->GetParent()->GOtrans->GetPos() + pos;
		//float3 cpos = GO->Get + pos;

	if (GO->children.size() != 0)
	{
		for (uint i = 0; i < GO->children.size(); ++i)
		{
			GO->children[i]->GOtrans->SetPos(cpos);
			//GO->children[i]->GOtrans->SetPos(cpos);
		}
	}

	if (GO->GOcam != nullptr)
	{
		GO->GOcam->cameraFrustum.pos = pos;
		GO->GOcam->Reference = pos;
	}

	TransformMatrix();
}
void CTransform::SetRot(float3 _rot) {
	rot = _rot;

	float3 crot = GO->GetParent()->GOtrans->GetRot() + rot;

	if (GO->children.size() != 0)
	{
		for (uint i = 0; i < GO->children.size(); ++i)
		{
			GO->children[i]->GOtrans->SetRot(crot);
		}
	}

	TransformMatrix();
}

void CTransform::SetScale(float3 _scl) {
	scl = _scl;

	float3 cscl ;

	cscl.x = GO->GetParent()->GOtrans->GetScale().x* scl.x ;
	cscl.y = GO->GetParent()->GOtrans->GetScale().y* scl.y;
	cscl.z = GO->GetParent()->GOtrans->GetScale().z* scl.z;

	if (GO->children.size() != 0)
	{
		for (uint i = 0; i < GO->children.size(); ++i)
		{
			GO->children[i]->GOtrans->SetPos(cscl);
		}
	}
	TransformMatrix();
}