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
	

	float x = rot.x * DEGTORAD;
	float y = rot.y * DEGTORAD;
	float z = rot.z * DEGTORAD;

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
	if (ImGui::CollapsingHeader("Transform"))
	{

		ImGui::Text("X	");
		ImGui::SameLine();
		ImGui::Text("Y	");
		ImGui::SameLine();
		ImGui::Text("Z	");
		ImGui::DragFloat3("Pos.",&pos);

		ImGui::Text("X	");
		ImGui::SameLine();
		ImGui::Text("Y	");
		ImGui::SameLine();
		ImGui::Text("Z	");
		ImGui::DragFloat3("Rotation.", &rot);
		
		ImGui::Text("X	");
		ImGui::SameLine();
		ImGui::Text("Y	");
		ImGui::SameLine();
		ImGui::Text("Z	");
		ImGui::DragFloat3("Scale.", &scl);



	}

	TransformMatrix();
}


