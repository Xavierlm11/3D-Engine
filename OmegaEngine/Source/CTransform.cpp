#include "CTransform.h"
#include "Globals.h"
//#include "External/MathGeoLib/include/Math/float4x4.h"

CTransform::CTransform(GameObject* obj):Component( obj, Types::TRANSFORM )
{
	this->GO = obj;
	setIdentity(matrix);
	//rmatrix = float4x4::identity;
	//rmatrix[0][0] = 1;
	if (GO->parent != nullptr)
	{
		/*
			gpos = ppos + pos;
			grot = prot + rot;
			gscl.x = pscl.x * scl.x;
			gscl.y = pscl.y * scl.y;
			gscl.z = pscl.z * scl.z;
			TransformMatrix(gpos, grot, gscl);*/

		
			ppos = GO->parent->GOtrans->GetPos();
			prot = GO->parent->GOtrans->GetRot();
			pscl = GO->parent->GOtrans->GetScale();
			
	}

	//if (GO->GOcam != nullptr)
	//{
	//	pos = GO->GOcam->cameraFrustum.pos ;
	//	//GO->GOcam->Reference  ;
	//}
}


CTransform::~CTransform()
{

}

void CTransform::Update()
{

	
}

void CTransform::setIdentity(mat4x4 mat)
{
	for (int i = 0; i<15; ++i)
	{
		if (i == 0 || i == 5 || i == 10 || i == 15) mat[i] = 1;
		else mat[i] = 0;
	}
}
void CTransform::TransformMatrix(float3 _pos, float3 _rot, float3 _scl)
{
	//pos = _pos;
	//rot = _rot;
	//scl = _scl;

	float x = _rot.x * DEGTORAD;
	float y = _rot.y * DEGTORAD;
	float z = _rot.z * DEGTORAD;

	/*float x = _rot.x ;
	float y = _rot.y ;
	float z = _rot.z ;*/

	Quat qrot = Quat::FromEulerXYZ(x, y, z);
	
	
	rmatrix = float4x4::FromTRS(_pos, qrot, _scl).Transposed();


	matrix.translate(_pos.x, _pos.y, _pos.z);
	matrix.scale(_scl.x, _scl.y, _scl.z);
	
	/*matrix[0] = (cos(y) * cos(z))  *_scl.x;
	matrix[1] = -cos(x) * sin(z) + sin(y) * cos(z) * sin(x);
	matrix[2] = sin(x) * sin(z) + sin(y) * cos(z) * cos(x);
	matrix[3] = _pos.x;

	matrix[4] = cos(y) * sin(z);
	matrix[5] = (cos(x) * cos(z) + sin(y) * sin(z) * sin(z)) * _scl.y;
	matrix[6] = -sin(x) * cos(z) + sin(y) * sin(z) * cos(x);
	matrix[7] = _pos.y;

	matrix[8] = -sin(y);
	matrix[9] = cos(y) * sin(x);
	matrix[10] = (cos(x) * cos(y)) * _scl.z;
	matrix[11] = _pos.z;

	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1;*/

	//matrix[0] *= _scl.x;
	//matrix[5] *= _scl.y;
	//matrix[10] *= _scl.z;

	if (GO->GOcam != nullptr)
	{
		GO->GOcam->cameraFrustum.pos = _pos;
		GO->GOcam->Reference = _pos;

		GO->GOcam->cameraFrustum.up = rmatrix.RotatePart().Col(1).Normalized();
		GO->GOcam->cameraFrustum.front = rmatrix.RotatePart().Col(2).Normalized();

		/*Quat dir;
		GO->GOcam->cameraFrustum.WorldMatrix().Decompose(float3(), dir, float3());

		Quat X = { 0, 0, 0, 1 };
		X.SetFromAxisAngle(float3(1.0f, 0.0f, 0.0f), _rot.x * DEGTORAD);
		dir = X * dir;
		float AngleY = (float)dy * Sensitivity;

		Quat Y = { 0, 0, 0, 1 };
		Y.SetFromAxisAngle(float3(0.0f, 1.0f, 0.0f), _rot.y * DEGTORAD);
		dir = dir * Y;

		Quat Z = { 0, 0, 0, 1 };
		Z.SetFromAxisAngle(float3(0.0f, 0.0f, 1.0f), _rot.z * DEGTORAD);
		dir = Z * dir;


		float AngleX = (float)dx * Sensitivity;

		float4x4 matrix = GO->GOcam->cameraFrustum.WorldMatrix();
		matrix.SetRotatePart(dir.Normalized());
		GO->GOcam->cameraFrustum.SetWorldMatrix(matrix.Float3x4Part());*/

	}

	//matrix = transpose(matrix);

	
	
}

void CTransform::SetGlobalTrans()
{
	if (GO->parent->parent == nullptr)
	{

		TransformMatrix(pos,rot,scl);
	}
	else
	{
		gpos = ppos + pos;
		grot = prot + rot;
		gscl.x = pscl.x * scl.x;
		gscl.y = pscl.y * scl.y;
		gscl.z = pscl.z * scl.z;
		TransformMatrix(gpos, grot, gscl );

	}

	if (GO->GOphys != nullptr) {
		if (GO->GOphys->collider != nullptr) {
			//GO->GOphys->SaveOffsetMatrix();
			GO->GOphys->colPos.x = matrix[12] + GO->GOphys->offsetMatrix[12];
			GO->GOphys->colPos.y = matrix[13] + GO->GOphys->offsetMatrix[13];
			GO->GOphys->colPos.z = matrix[14] + GO->GOphys->offsetMatrix[14];
			GO->GOphys->CallUpdateShape();

		}
	}
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
		if (ImGui::DragFloat3("Pos.", newPos.ptr(), 0.1))
		{
			if (GO->GOphys != nullptr) {

				if (GO->GOphys->collider != nullptr) {
					GO->GOphys ->SaveOffsetMatrix();
				}
			}
				
			SetPos(newPos);
		}
			
		
		ImGui::Text("X	");
		ImGui::SameLine();
		ImGui::Text("Y	");
		ImGui::SameLine();
		ImGui::Text("Z	");
		if (ImGui::DragFloat3("Rotation.", newrot.ptr(), 0.1)) {
			if (GO->GOphys != nullptr) {

				if (GO->GOphys->collider != nullptr) {
					GO->GOphys->SaveOffsetMatrix();
				}
			}
			SetRot(newrot);
		}
			
		
		ImGui::Text("X	");
		ImGui::SameLine();
		ImGui::Text("Y	");
		ImGui::SameLine();
		ImGui::Text("Z	");
		if (ImGui::DragFloat3("Scale.", newScl.ptr(), 0.1))
		{
			if (GO->GOphys != nullptr) {

				if (GO->GOphys->collider != nullptr) {
					GO->GOphys->SaveOffsetMatrix();
				}
			}
			SetScale(newScl);
		}



	}
	
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
		//float3 cpos = GO->Get + pos;

	UpdatePos();
}

void CTransform::UpdatePos()
{

	float3 cpos = ppos + pos;
	if (GO->children.size() != 0)
	{

		for (uint i = 0; i < GO->children.size(); ++i)
		{
			GO->children[i]->GOtrans->SetGPos(cpos);
			//GO->children[i]->GOtrans->SetPos(cpos);
		}
	}

	

	SetGlobalTrans();
	//TransformMatrix();
}

void CTransform::SetGPos(float3 _pos)
{

	ppos = _pos;

	UpdatePos();
}



void CTransform::SetRot(float3 _rot) {
	rot = _rot;

	if (rot.x >= 361)
	{
		rot.x = 0;
	}
	if (rot.y >= 361)
	{
		rot.y = 0;
	}
	if (rot.z >= 361)
	{
		rot.z = 0;
	}

	if (rot.x < 0)
	{
		rot.x = 360;
	}
	if (rot.y < 0)
	{
		rot.y = 360;
	}
	if (rot.z < 0)
	{
		rot.z = 360;
	}

	UpdateRot();
	
	//TransformMatrix();
}

void CTransform::UpdateRot()
{

	float3 crot = prot + rot;
	if (GO->children.size() != 0)
	{

		for (uint i = 0; i < GO->children.size(); ++i)
		{
			GO->children[i]->GOtrans->SetGRot(crot);
			//GO->children[i]->GOtrans->SetPos(cpos);
		}
	}

	

	SetGlobalTrans();
	//TransformMatrix();
}

void CTransform::SetGRot(float3 _rot)
{

	prot = _rot;

	UpdateRot();
}



void CTransform::SetScale(float3 _scl) {
	scl = _scl;

	UpdateScl();
	//TransformMatrix();
}

void CTransform::UpdateScl()
{

	float3 cscl ;

	cscl.x = GO->GetParent()->GOtrans->GetScale().x* scl.x ;
	cscl.y = GO->GetParent()->GOtrans->GetScale().y* scl.y;
	cscl.z = GO->GetParent()->GOtrans->GetScale().z* scl.z;

	if (GO->children.size() != 0)
	{
		for (uint i = 0; i < GO->children.size(); ++i)
		{
			GO->children[i]->GOtrans->SetGScl(cscl);
		}
	}

	SetGlobalTrans();
}

void CTransform::SetGScl(float3 _scl)
{

	pscl = _scl;

	UpdateScl();
}
	

