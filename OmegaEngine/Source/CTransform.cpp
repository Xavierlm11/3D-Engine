#include "CTransform.h"
#include "Globals.h"

CTransform::CTransform(GameObject* obj):Component( obj, Types::TRANSFORM )
{
	this->GO = obj;
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
			ppos = GO->parent->GOtrans->GetRot();
			ppos = GO->parent->GOtrans->GetScale();
			
	}
}

CTransform::~CTransform()
{

}

void CTransform::Update()
{

	
}

void CTransform::TransformMatrix(float3 _pos, float3 _rot, float3 _scl)
{
	//pos = _pos;
	//rot = _rot;
	//scl = _scl;

	float x = _rot.x * DEGTORAD;
	float y = _rot.y * DEGTORAD;
	float z = _rot.z * DEGTORAD;

	/*Quat _rot = Quat::FromEulerXYZ(x, y, z);
	mat4x4::
	matrix = float4x4::FromTRS(pos, _rot, scl).Transposed();
	*/
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
		if(ImGui::DragFloat3("Pos.", newPos.ptr(), 0.1))
			SetPos(newPos);
		
		ImGui::Text("X	");
		ImGui::SameLine();
		ImGui::Text("Y	");
		ImGui::SameLine();
		ImGui::Text("Z	");
		if(ImGui::DragFloat3("Rotation.", newrot.ptr(),0.1)) 
			SetRot(newrot);
		
		ImGui::Text("X	");
		ImGui::SameLine();
		ImGui::Text("Y	");
		ImGui::SameLine();
		ImGui::Text("Z	");
		if (ImGui::DragFloat3("Scale.", newScl.ptr(), 0.1)) 
			SetScale(newScl);



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

	if (GO->GOcam != nullptr)
	{
		GO->GOcam->cameraFrustum.pos = cpos;
		GO->GOcam->Reference = cpos;
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

	if (GO->GOcam != nullptr)
	{
		
		Quat dir;
		GO->GOcam->cameraFrustum.WorldMatrix().Decompose(float3(), dir, float3());
		
		Quat X = { 0, 0, 0, 1 };
		X.SetFromAxisAngle(float3(1.0f, 0.0f, 0.0f), crot.x * DEGTORAD);
		dir = X * dir;
		//float AngleY = (float)dy * Sensitivity;

		Quat Y = { 0, 0, 0, 1 };
		Y.SetFromAxisAngle(float3(0.0f, 1.0f, 0.0f), crot.y * DEGTORAD);
		dir = dir * Y;
		
		Quat Z = { 0, 0, 0, 1 };
		Z.SetFromAxisAngle(float3(0.0f, 0.0f, 1.0f), crot.z * DEGTORAD);
		dir = Z * dir;


		//float AngleX = (float)dx * Sensitivity;

		float4x4 matrix = GO->GOcam->cameraFrustum.WorldMatrix();
		matrix.SetRotatePart(dir.Normalized());
		GO->GOcam->cameraFrustum.SetWorldMatrix(matrix.Float3x4Part());
		
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
	

