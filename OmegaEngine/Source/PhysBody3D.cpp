#include "PhysBody3D.h"
#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "GameObject.h"

// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body) : body(body)
{}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	delete body;
	body = nullptr;
}

// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if (body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

float4x4 PhysBody3D::Mat4x4ToFloat4x4(mat4x4 matrix)
{
	float4x4 newFloat;

	//int counter;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			newFloat[i][j] = matrix[i*4+j];
		}
	}

	return newFloat;
}

void PhysBody3D::SetColTransform(float4x4 matrix, GameObject* go) {
	if (body != NULL) {
		//globalTrans = matrix;

		//float colTrans[16];
		//go->GOphys->collider->body->getWorldTransform().getOpenGLMatrix(colTrans);

		//mat4x4 colTransMat4x4;
		//for (int i = 0; i < 16; i++) {

		//	colTransMat4x4[i] = colTrans[i];

		//}
		//float4x4 colTransFloat4x4 = Mat4x4ToFloat4x4(colTransMat4x4);

		//float4x4 newtrans = globalTrans * colTransFloat4x4;
		//float3 pos;
		//Quat rot;
		//float3 scl;
		//
		//newtrans.Decompose(pos, rot, scl);
		//scl.Set(1, 1, 1);
		//newtrans = float4x4::FromTRS(pos, rot, scl);
		//btTransform trans;
		//trans.setFromOpenGLMatrix(newtrans.Transposed().ptr());
		//body->setWorldTransform(trans);
		////body->activate();


		globalTrans = matrix;
		float4x4 newtrans = globalTrans * transformMat;
		float3 pos;
		Quat rot;
		float3 scl;
		
		newtrans.Decompose(pos, rot, scl);
		scl.Set(1, 1, 1);
		newtrans = float4x4::FromTRS(pos, rot, scl);
		btTransform trans;
		trans.setFromOpenGLMatrix(newtrans.Transposed().ptr());
		body->setWorldTransform(trans);
		body->activate();
	}

		//go->GOtrans->matrix;

		//float4x4 parentTrans;
		//if (go->parent->parent == nullptr) {
		//	parentTrans = float4x4::identity;
		//}
		//else {
		//	parentTrans = Mat4x4ToFloat4x4(go->parent->parent->GOtrans->matrix);
		//}

		//float colTrans[16];
		//go->GOphys->collider->body->getWorldTransform().getOpenGLMatrix(colTrans);

		//mat4x4 colTransMat4x4;
		//for (int i = 0; i < 16; i++) {

		//	colTransMat4x4[i] = colTrans[i];

		//}
		//float4x4 colTransFloat4x4 = Mat4x4ToFloat4x4(colTransMat4x4);


		//float4x4 glbMat = parentTrans * colTransFloat4x4;
		//SetColTransform(glbMat, go);


		//float4x4 matrixInFloat;



		//float4x4 new_trans = matrixInFloat*; //= globalTransform * TransformMatrix;

	//s}
}

float4x4 PhysBody3D::GetColTransform(float4x4 matrix, GameObject* go)
{
	if (body != NULL)
	{
		float3 pos;
		Quat rot;
		float3 scl;

		matrix.Decompose(pos, rot, scl);

		float4x4 mat = globalTrans * transformMat;

		float3 new_pos;
		Quat new_rot;
		float3 new_scl;
		
		mat.Decompose(new_pos, new_rot, new_scl);
		matrix = float4x4::FromTRS(pos, rot, new_scl);

		return matrix;


	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if (body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}
