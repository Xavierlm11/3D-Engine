#include "CTransform.h"
#include "Globals.h"
CTransform::CTransform(GameObject* obj):Component( obj, Types::TRANSFORM )
{
	//this->GO = obj;
	
}

CTransform::~CTransform()
{

}

void CTransform::Update()
{
	
	
}

void CTransform::TransformMatrix(float3 _pos, float3 _rot, float3 _scl)
{
	

	float x = _rot.x * DEGTORAD;
	float y = _rot.y * DEGTORAD;
	float z = _rot.z * DEGTORAD;

	matrix[0][0] = cos(y) * cos(z);
	matrix[1][0] = -cos(x) * sin(z) + sin(y) * cos(z) * sin(x);
	matrix[2][0] = sin(x) * sin(z) + sin(y) * cos(z) * cos(x);
	matrix[3][0] = _pos.x;

	matrix[0][1] = cos(y) * sin(z);
	matrix[1][1] = cos(x) * cos(z) + sin(y) * sin(z) * sin(z);
	matrix[2][1] = -sin(x) * cos(z) + sin(y) * sin(z) * cos(x);
	matrix[3][1] = _pos.y;

	matrix[0][2] = -sin(y);
	matrix[1][2] = cos(y) * sin(x);
	matrix[2][2] = cos(x) * cos(y);
	matrix[3][2] = _pos.z;

	matrix[0][3] = 0;
	matrix[1][3] = 0;
	matrix[2][3] = 0;
	matrix[3][3] = 1;

	matrix[0][0] *= _scl.x;
	matrix[1][1] *= _scl.y;
	matrix[2][2] *= _scl.z;

	if (GO->GetParent()->GOtrans != nullptr)
	{
		glmatr = GO->parent->GOtrans->glmatr * matrix;
	}
	else
	{
		glmatr = matrix;
	}
	
	for (int i = 0; i < GO->children.size(); ++i)
	{
		CTransform* child = GO->children[i]->GOtrans;
		child->TransformMatrix(_pos,_rot,_scl);
	}
	//matrix = transpose(matrix);

}

void CTransform::OnInspector()
{
	if (ImGui::CollapsingHeader("Transform"))
	{

		ImGui::Text("X	");
		ImGui::SameLine();
		ImGui::Text("Y	");
		ImGui::SameLine();
		ImGui::Text("Z	");
		ImGui::DragFloat3("Pos.",pos.ptr());

		ImGui::Text("X	");
		ImGui::SameLine();
		ImGui::Text("Y	");
		ImGui::SameLine();
		ImGui::Text("Z	");
		ImGui::DragFloat3("Rotation.", rot.ptr());
		
		ImGui::Text("X	");
		ImGui::SameLine();
		ImGui::Text("Y	");
		ImGui::SameLine();
		ImGui::Text("Z	");
		ImGui::DragFloat3("Scale.", scl.ptr());



	}
	
	TransformMatrix(pos, rot, scl);
}

float4x4 CTransform::GetGlobal()
{
	
	if (GO->GetParent() == nullptr)
	{
		return GetMatrix();
	}
	return GO->GetParent()->GOtrans->GetGlobal() * matrix;
	
}


