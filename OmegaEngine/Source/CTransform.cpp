#include "CTransform.h"
#include "Globals.h"
#include "Component.h"
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

void CTransform::TransformMatrix1(float3 _pos, float3 _rot, float3 _scl)
{


	float x = _rot.x * DEGTORAD;
	float y = _rot.y * DEGTORAD;
	float z = _rot.z * DEGTORAD;

	

	matrix1[0][0] = cos(z) * cos(y) * _scl.x;
	matrix1[1][0] = -sin(z) * cos(x) + cos(z) * sin(y) * sin(x);
	matrix1[2][0] = sin(z) * sin(x) + cos(z) * sin(y) * cos(x);
	matrix1[0][1] = sin(z) * cos(y);
	matrix1[1][1] = (cos(z) * cos(x) + sin(z) * sin(y) * sin(x)) * _scl.y;
	matrix1[2][1] = -cos(z) * sin(x) + sin(z) * sin(y) * cos(x);
	matrix1[0][2] = -sin(y);
	matrix1[1][2] = cos(y) * sin(x);
	matrix1[2][2] = cos(y) * cos(x) * _scl.z;


	matrix1[3][0] = _pos.x;
	matrix1[3][1] = _pos.y;
	matrix1[3][2] = _pos.z;
	matrix1[0][3] = 0;
	matrix1[1][3] = 0;
	matrix1[2][3] = 0;
	matrix1[3][3] = 1;

/*matrix1[0] = cos(y) * cos(z);
	matrix1[1] = -cos(x) * sin(z) + sin(y) * cos(z) * sin(x);
	matrix1[2] = sin(x) * sin(z) + sin(y) * cos(z) * cos(x);
	matrix1[3] = _pos.x;

	matrix1[4] = cos(y) * sin(z);
	matrix1[5] = cos(x) * cos(z) + sin(y) * sin(z) * sin(z);
	matrix1[6] = -sin(x) * cos(z) + sin(y) * sin(z) * cos(x);
	matrix1[7] = _pos.y;

	matrix1[8] = -sin(y);
	matrix1[9] = cos(y) * sin(x);
	matrix1[10] = cos(x) * cos(y);
	matrix1[11] = _pos.z;

	matrix1[12] = 0;
	matrix1[13] = 0;
	matrix1[14] = 0;
	matrix1[15] = 1;

	matrix1[0] *= _scl.x;
	matrix1[5] *= _scl.y;
	matrix1[10] *= _scl.z;

	matrix1 = transpose(matrix1);*/
	/*float rx = rotation.x * DEGTORAD;
	float ry = rotation.y * DEGTORAD;
	float rz = rotation.z * DEGTORAD;*/

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

	TransformMatrix1(pos, rot, scl);
}


float4x4 CTransform::GetGlobal()
{

	if (GO->GetParent() == nullptr)
	{
		return GetMatrix();
	}
	return GO->GetParent()->GOtrans->GetGlobal() * matrix1;

}