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

void CTransform::TransformMatrix(float3 _pos, float3 _rot, float3 _scl)
{
	

	float x = _rot.x * DEGTORAD;
	float y = _rot.y * DEGTORAD;
	float z = _rot.z * DEGTORAD;

	matrix[0] = cos(y) * cos(z);
	matrix[1] = -cos(x) * sin(z) + sin(y) * cos(z) * sin(x);
	matrix[2] = sin(x) * sin(z) + sin(y) * cos(z) * cos(x);
	matrix[3] = _pos.x;

	matrix[4] = cos(y) * sin(z);
	matrix[5] = cos(x) * cos(z) + sin(y) * sin(z) * sin(z);
	matrix[6] = -sin(x) * cos(z) + sin(y) * sin(z) * cos(x);
	matrix[7] = _pos.y;

	matrix[8] = -sin(y);
	matrix[9] = cos(y) * sin(x);
	matrix[10] = cos(x) * cos(y);
	matrix[11] = _pos.z;

	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1;

	matrix[0] *= _scl.x;
	matrix[5] *= _scl.y;
	matrix[10] *= _scl.z;

	matrix = transpose(matrix);

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


