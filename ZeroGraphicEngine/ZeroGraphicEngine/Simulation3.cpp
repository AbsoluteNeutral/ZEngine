#include "stdafx.h"
#include "EditorMB.h"
#include "Engine.h"
#include "Simulation.h"

zg::Vector3 gravityForce{ 0.0f, -1.8f, 0.0f };

// ______________________________________________________________________ RigidBody
struct RigidBody
{
	/**************************************************************************
		- Initalize Rigidbody
		- Pre-Computation
	**************************************************************************/
	RigidBody(GameObject* obj_, MeshRenderer* mesh_)
		:owner(obj_)
		, mesh(mesh_)
		, ListOfVertices	{}
		, ListOfmi			{}
		, ListOfRi			{}
		, ListOfVertexForce	{}
		, ListOfVertexTorque{}

		, mass(0.2f)
		, invMass(1.0f / mass)
		, linearDamping(0.2f)
		//, angularDamping(0.2f)
		, totalForce{}
		, totalTorque{}
		, linearVelocity{}
		, angularMomentum{}
		, angularVelocity{}
		, inertiaTensorInverse{}
	{
		if (mesh)
		{
			if (mesh->model)
			{
				//__________ Get all Vertices in the model
				const auto& getList = mesh->model->GetListOfVertices();
				float totalWeight = 0.0f;

				for (const auto& elem : getList)
				{
					//weed out duplicate vertices
					bool dup = false;
					for (const auto& elem2 : ListOfVertices)
					{
						if (elem2 == elem.position)
						{
							dup = true;
							break;
						}
					}

					if (dup) continue;

					ListOfVertices.push_back(elem.position);		//store each vertices
					ListOfVertexForce.push_back(zg::Vector3{});		//each vertices force vector
					ListOfVertexTorque.push_back(zg::Vector3{});	//each vertices torque

					ListOfRi.push_back(elem.position);				//store each ri vector, assume CenterOfMass at (0.0, 0.0, 0.0)
					totalWeight += elem.position.Length();			//get total weight to compute initial tensor
				}

				// __________ Generate mi - weight distribution
				for (const auto& elem : ListOfVertices)
				{
					ListOfmi.push_back( elem.Length() / totalWeight);
				}

				//float totalWeight2 = 0.0f;
				//for (size_t i = 0; i < ListOfRi.size(); ++i)
				//{
				//	//totalWeight2 += ListOfmi[i];
				//	centerOfMassLocal += ListOfmi[i] * ListOfVertices[i];
				//}
				////std::cout << totalWeight2;
				////std::cout << centerOfMassLocal;
				//centerOfMassLocal.Reset();

				//__________ Pre-Compute inertia tensor Inverse
				float ixx = 0.0f;
				float iyy = 0.0f;
				float izz = 0.0f;
				float ixy = 0.0f;
				float ixz = 0.0f;
				float iyz = 0.0f;
				for (size_t i = 0; i < ListOfRi.size(); ++i)
				{
					zg::Vector3& elem = ListOfRi[i];
					float& mi		  = ListOfmi[i];
					ixx += mi * (elem.y * elem.y + elem.z * elem.z);
					iyy += mi * (elem.x * elem.x + elem.z * elem.z);
					izz += mi * (elem.x * elem.x + elem.y * elem.y);
					ixy += mi * elem.x * elem.y;
					ixz += mi * elem.x * elem.z;
					iyz += mi * elem.y * elem.z;
				}
				//Use 4x4 matrix to compute inverse matrix
				zg::Matrix44 mmmminertiaTensorInverse = zg::GaussJordanInverse(zg::Matrix44
				{
					 ixx, -ixy, -ixz, 0.0f,
					-ixy,  iyy, -iyz, 0.0f,
					-ixz, -iyz,  izz, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				});
				//Store it back as a 3x3 matrix
				inertiaTensorInverse = zg::Matrix33
				{
					mmmminertiaTensorInverse.m00, mmmminertiaTensorInverse.m01, mmmminertiaTensorInverse.m02,
					mmmminertiaTensorInverse.m10, mmmminertiaTensorInverse.m11, mmmminertiaTensorInverse.m12,
					mmmminertiaTensorInverse.m20, mmmminertiaTensorInverse.m21, mmmminertiaTensorInverse.m22,
				};
			}
			Reset();
		}		
	}

	/**************************************************************************
		Gizmo
	**************************************************************************/
	void DrawGizmo()
	{
		auto m = owner->transform.GetMatrix();

		zg::Vector3 world = owner->transform.position;
		Gizmo::Sphere(world, 0.04f, CC_BLUE);

		for (const auto& elem : ListOfVertices)
		{
			Gizmo::Sphere(zg::TransformPoint(m, elem), 0.04f, CC_BLUE);
			Gizmo::Ray(world, zg::TransformVector(m, elem), 1.0f, 0.01f, CC_BLUE);
		}
		//zg::Vector3 world = zg::TransformPoint(m, centerOfMassLocal);
		//for (const auto& elem : ListOfRi)
		//{
		//	Gizmo::Ray(world, zg::TransformVector(m, elem), 1.0f, 0.01f, CC_BLUE);
		//}
	}

	/**************************************************************************
		Get the World-Coordinates of each vertices
	**************************************************************************/
	std::vector<zg::Vector3> GetListOfWorldPositionVertices()
	{
		std::vector<zg::Vector3> tmp;
		auto m = owner->transform.GetMatrix();
		for (const auto& elem : ListOfVertices)
		{
			zg::Vector3 pt = zg::TransformPoint(m, elem);
			tmp.push_back(pt);
			//if (pt.IsNan())
			//	pt.Reset();
		}
		return tmp;
	}

	void SetMass(float mass_) 
	{
		mass = mass_;
		invMass = 1.0f / mass_;
	}

	/**************************************************************************
		Main Physics Engine Update
	**************************************************************************/
	void Update()
	{
		//temp variable to get delta-time
		float getddt = FrameRateController::dttimescale;

		// __________ Part 1: Generate Total Linear/Torque Force 
		totalForce  = zg::SumOfVector(ListOfVertexForce);
		totalTorque = zg::SumOfVector(ListOfVertexTorque);

		//__________ Part 2: Generate Linear/Angular Velocity using law of Momentum
		//__________ Calculate Linear Velocity, integrate;
		linearVelocity += invMass * totalForce * getddt;

		//Universal/non-physcial based Linear Damping computation
		//linearVelocity *= static_cast<float>(1.0) - (linearDamping * getddt);	

		//Translate the object
		owner->transform.Translate(linearVelocity);

		//__________ Compute InertiaTensor Inverse
		zg::Matrix33 rotationMatrix = owner->transform.rotation.GetMatrix33();
		zg::Matrix33 inertiaTensor_Update = rotationMatrix * inertiaTensorInverse * rotationMatrix.Transposed();

		//__________ Calculate Angular Velocity, integrate;
		angularMomentum += totalTorque * getddt;
		//Note: AngularVelocity is in Radian
		angularVelocity = inertiaTensor_Update * angularMomentum;

		//Universal/non-physcial based Angular Damping computation
		//angularVelocity *= static_cast<float>(1.0) - (angularDamping * getddt);
		//Gizmo::Ray(owner->transform.position, (angularMomentum), 1.0f, 0.01f, CC_RED);
		//Gizmo::Ray(owner->transform.position, (angularVelocity), 1.0f, 0.01f, CC_BLUE);

		//Rotate the object
		owner->transform.SetRotation(zg::ToQuaternion(angularVelocity.x, angularVelocity.y, angularVelocity.z) );

		Reset(); //reset all forces
	}

	/**************************************************************************
		Reset All Force per frame update
	**************************************************************************/
	void Reset()
	{
		//Reset all Force/Torque
		//__________ Pre-Generate Each vertices' mass force
		for (auto& elem2 : ListOfVertexForce)
			elem2 = (mass * 0.5f) * gravityForce;
		for (auto& elem2 : ListOfVertexTorque)
			elem2 = zg::Vector3{};
	}

	//init
	GameObject* owner;
	MeshRenderer* mesh;
	std::vector<zg::Vector3> ListOfVertices;	//list of vertices
	std::vector<float> ListOfmi;				//weight of vertices
	std::vector<zg::Vector3> ListOfRi;			//vector from center of mass
	std::vector<zg::Vector3> ListOfVertexForce;	//Vertices' Forces
	std::vector<zg::Vector3> ListOfVertexTorque;//Vertices' Torque

	//values
	zg::Vector3 centerOfMassLocal;
	float mass;
	float invMass;
	float linearDamping;
	//float angularDamping;
	zg::Vector3 totalForce;
	zg::Vector3 totalTorque;
	zg::Vector3 linearVelocity;
	zg::Vector3 angularMomentum;
	zg::Vector3 angularVelocity;
	zg::Matrix33 inertiaTensorInverse;
};
// ______________________________________________________________________ RigidBody End

zg::Vector3 anchor1{ -5.0f, 20.0f, -15.0f };
zg::Vector3 anchor2{  5.0f, 15.0f, 15.0f };
static std::vector<PickingCircle*> listofnodeA;
static std::vector<RigidBody> listofrigidbodyA;
static std::vector<float> listOfKConstant;
static int numOfObj = 5;
int index1 = 0;
int index2 = 2;
bool objectgizmo_3 = false;
int options = 0;

/**************************************************************************
	Spring Mass Damper Generator
**************************************************************************/
void GenerateSpringMassDamperForce(const std::vector<zg::Vector3>& listofq, int ind1_, int ind2_)
{
	//Assume two point of contact on a single rigidbody
	size_t sz = listofq.size();		//The entire spring system of points
	int counter = 0;				//current rigidbody to update

	for (size_t i = 1; i < sz - 1; i += 2)
	{
		//__________ 1st point a on the rigid body - Linear Force
		// spring = (-K constant of Line) * (currentPosition - AnchorPoint)
		// damper = linearDamping * velocity
		zg::Vector3 spring1 = -listOfKConstant[counter] * (listofq[i] - listofq[i - 1]);
		zg::Vector3 damper1 = -listofrigidbodyA[counter].linearDamping * listofrigidbodyA[counter].linearVelocity;
		
		//if (spring1.IsNan())
		//	spring1.Reset();
		//if (damper1.IsNan())
		//	damper1.Reset();

		//__________ 2nd point a on the rigid body - Linear Force
		zg::Vector3 spring2 = -listOfKConstant[counter + 1] * (listofq[i + 1] - listofq[i + 2]);
		zg::Vector3 damper2 = -listofrigidbodyA[counter].linearDamping * listofrigidbodyA[counter].linearVelocity;

		//if (spring2.IsNan())
		//	spring2.Reset();
		//if (damper2.IsNan())
		//	damper2.Reset();

		//__________ Set the Linear Force back to it's vertices
		listofrigidbodyA[counter].ListOfVertexForce[ind1_] += spring1 + damper1;
		listofrigidbodyA[counter].ListOfVertexForce[ind2_] += spring2 + damper2;

		//Gizmo::Ray(listofrigidbodyA[counter].owner->transform.position, listofrigidbodyA[counter].ListOfVertexForce[ind1_], 1.0f, 0.01f, CC_PURPLE);
		//Gizmo::Ray(listofrigidbodyA[counter].owner->transform.position, listofrigidbodyA[counter].ListOfVertexForce[ind2_], 1.0f, 0.01f, CC_PINK);

		//__________ Torque
		//set the Torque back to it's vertex 
		listofrigidbodyA[counter].ListOfVertexTorque[ind1_] += (listofq[i]	  - listofrigidbodyA[counter].owner->transform.position).Cross(listofrigidbodyA[counter].ListOfVertexForce[ind1_]);
		listofrigidbodyA[counter].ListOfVertexTorque[ind2_] += (listofq[i + 1] - listofrigidbodyA[counter].owner->transform.position).Cross(listofrigidbodyA[counter].ListOfVertexForce[ind2_]);

		//Gizmo::Sphere(listofq[i], 0.03f, CC_GREEN);
		//Gizmo::Sphere(listofq[i + 1], 0.03f, CC_BLUE);
		//Gizmo::Ray(listofrigidbodyA[counter].owner->transform.position, (listofq[i] - listofrigidbodyA[counter].owner->transform.position), 1.0f, 0.01f, CC_GREEN);
		//Gizmo::Ray(listofrigidbodyA[counter].owner->transform.position, listofrigidbodyA[counter].ListOfVertexTorque[ind1_], 1.0f, 0.01f, CC_GREEN_3);
		//Gizmo::Ray(listofrigidbodyA[counter].owner->transform.position, (listofq[i + 1] - listofrigidbodyA[counter].owner->transform.position), 1.0f, 0.01f, CC_BLUE);
		//Gizmo::Ray(listofrigidbodyA[counter].owner->transform.position, listofrigidbodyA[counter].ListOfVertexTorque[ind2_], 1.0f, 0.01f, CC_BLUE_2);
		//Gizmo::Ray(listofrigidbodyA[counter].owner->transform.position, listofrigidbodyA[counter].ListOfVertexTorque[ind2_] + listofrigidbodyA[counter].ListOfVertexTorque[ind1_], 1.0f, 0.01f, CC_RED);

		//Set next rigidbody to updated
		++counter;
	}
}

/**************************************************************************
	Init simulation
**************************************************************************/
void Init_Simulation3()
{
	//Set Camera location for this simulation
	MainCam->owner->transform.SetPosition(0.0f, 12.5f, 42.0f);
	MainCam->owner->transform.SetRotation(0.0f, 0.0f, 0.0f);

	//Create Objects
	listofnodeA.clear();

	int num = 10;
	float zoffset = 1.3f;
	for (int i = 0; i < num; ++i)
	{
		PickingCircle* picker1 = nullptr;
		picker1 = CreateGameObject(2, "Points" + std::to_string(i));
		picker1->owner->transform.SetPosition( -2.0f + (zoffset * i), 6.0f, 0.0f );
		listofnodeA.push_back(picker1);

		SelectionUpdate_UponGameObjectAddAndDeleted(picker1);
		selectedObj.meshRenderer->color.a = 1.0f;
		listofrigidbodyA.push_back(RigidBody{ selectedObj.gameObject, selectedObj.meshRenderer });
	}

	//Create Spring Constant K
	for (int i = 0; i < num + 1; ++i)
		listOfKConstant.push_back(0.5f);
}

void Update_Simulation3()
{

	//Two anchor points controls
	const float speed = 0.5f;
	if (KeyUp(MOUSE_RIGHT))
	{
		if (KeyDown(KEY_W))	anchor1.y += speed;
		if (KeyDown(KEY_S))	anchor1.y -= speed;
		if (KeyDown(KEY_A))	anchor1.x -= speed;
		if (KeyDown(KEY_D))	anchor1.x += speed;
		if (KeyDown(KEY_Q))
		{
			anchor1.z -= speed;
			anchor2.z += speed;
		}
		if (KeyDown(KEY_E))
		{
			anchor1.z += speed;
			anchor2.z -= speed;
		}
		if (KeyDown(KEY_UP))	anchor2.y += speed;
		if (KeyDown(KEY_DOWN))	anchor2.y -= speed;
		if (KeyDown(KEY_LEFT))	anchor2.x -= speed;
		if (KeyDown(KEY_RIGHT))	anchor2.x += speed;
	}

	//__________ Set the entire anchor points and qi to generate SpringMassDamper Force
	std::vector<zg::Vector3> listOfQi;
	if (options == 0)
	{
		listOfQi.push_back(anchor1);
		for (int i = 0; i < numOfObj; ++i)
		{
			auto tmp = listofrigidbodyA[i].GetListOfWorldPositionVertices();
			listOfQi.push_back(tmp[index1]);
			listOfQi.push_back(tmp[index2]);
		}
		listOfQi.push_back(anchor2);

		//__________ Generate Spring Mass Damper Force
		GenerateSpringMassDamperForce(listOfQi, index1, index2);

		//__________ Update RigidBody
		for (auto& elem : listofrigidbodyA)
		{
			if (elem.owner->transform.position.x < -5000.0f || elem.owner->transform.position.x > 5000.0f) elem.owner->transform.SetPosition(0.0f, 0.0f, 0.0f);
			if (elem.owner->transform.position.y < -5000.0f || elem.owner->transform.position.y > 5000.0f) elem.owner->transform.SetPosition(0.0f, 0.0f, 0.0f);
			if (elem.owner->transform.position.z < -5000.0f || elem.owner->transform.position.z > 5000.0f) elem.owner->transform.SetPosition(0.0f, 0.0f, 0.0f);

			elem.Update();
		}
	}
	else if (options == 1)
	{

	}


	//Draw Gizmo
	if (options == 0)
	{
		for (auto& elem : listOfQi)
			Gizmo::Sphere(elem, 0.1f, CC_YELLOWGREEN);
		size_t sz = listOfQi.size();
		for (size_t i = 1; i < sz; ++i)
			Gizmo::Line(listOfQi[i], listOfQi[i - 1]);
	}
	
	Gizmo::Sphere(anchor1, 0.1f, CC_RED);
	Gizmo::Sphere(anchor2, 0.1f, CC_RED);

	if(objectgizmo_3)
		for (auto& elem : listofrigidbodyA)
			elem.DrawGizmo();
}

void UpdateImgui_Simulation3()
{
	ImGui::Separator();
	ImGui::Text("W A S D E Q Key - Move Anchor 1");
	ImGui::Text("Up Arrow, Down Arrow Key - Move Anchor 2 ");

	ImGui::Separator();
	
	ImGui::DragFloat3("Gravity Force", &gravityForce.x, 0.1f);
	ImGui::SliderInt("Number Of Object", &numOfObj, 1, 10);
	ImGui::Checkbox("Toggle objectgizmo", &objectgizmo_3);

	if (ImGui::Button("Reset"))
	{
		numOfObj = 10;
		float zoffset = 1.3f;
		for (int i = 0; i < numOfObj; ++i)
		{
			listofrigidbodyA[i].owner->transform.Reset();
			listofrigidbodyA[i].owner->transform.SetPosition(-2.0f + (zoffset * i), 6.0f, 0.0f);
			listofrigidbodyA[i].linearVelocity.Reset();
			listofrigidbodyA[i].angularMomentum.Reset();
		}
	}

	if(ImGui::SliderFloat("K Constant", &listOfKConstant[0], 0.1f, 1.0f))
	{
		for (size_t i = 1; i < listOfKConstant.size(); ++i)
		{
			listOfKConstant[i] = listOfKConstant[0];
		}
	}

	if (ImGui::SliderFloat("Mass", &listofrigidbodyA[0].mass, 0.1f, 1.0f))
	{
		listofrigidbodyA[0].invMass = 1.0f / listofrigidbodyA[0].mass;
		for (size_t i = 1; i < listofrigidbodyA.size(); ++i)
		{
			listofrigidbodyA[i].mass = listofrigidbodyA[0].mass;
			listofrigidbodyA[i].invMass = 1.0f / listofrigidbodyA[0].mass;
		}
	}

	if (ImGui::SliderFloat("Linear Damping", &listofrigidbodyA[0].linearDamping, 0.1f, 2.0f))
	{
		for (size_t i = 1; i < listofrigidbodyA.size(); ++i)
		{
			listofrigidbodyA[i].linearDamping = listofrigidbodyA[0].linearDamping;
		}
	}

	if (ImGui::RadioButton("Option A", options == 0))
	{
		options = 0;
		for (auto& elem : listofnodeA)
		{
			SelectionUpdate_UponGameObjectAddAndDeleted(elem);
			selectedObj.meshRenderer->color.a = 0.7f;
		}

	}
	//if (ImGui::RadioButton("Option B", options == 1))
	//{
	//	options = 1;
	//
	//	for (auto& elem : listofnodeA)
	//	{
	//		SelectionUpdate_UponGameObjectAddAndDeleted(elem);
	//		selectedObj.meshRenderer->color.a = 0.0f;
	//	}
	//}
}
