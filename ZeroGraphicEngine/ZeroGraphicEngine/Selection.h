#ifndef SELECTION_H
#define SELECTION_H

#include "ZGEngine.h"
#include "PickingCircle.h"
#include <vector>

enum SELECTION_TYPE
{
	SEL_NONE = 0,
	SEL_SINGLE,
	SEL_MULTIPLE
};

class Selection
{
public:
	Selection		() noexcept;
	~Selection		();
	void Set		(GameObject* rhs);
	void Set		(PickingCircle* rhs); // Setter. Selected object will now be pointee of 'rhs'
	void SetEmpty();
	void ForceUpdate();
	bool isEmpty(); // Checks if the selection is empty	

	bool operator==(const Selection& rhs) const;

	//zg::Matrix44 worldMatrix;
	//zg::Matrix44 localMatrix;
	zg::Vector3 eularAngle;
	zg::Vector3 localEularAngle;

	bool meshgeneratebonesss;

	PickingCircle*		pickingCircle;
	GameObject*			gameObject;
	Camera*				camera;
	MeshRenderer*		meshRenderer;
	//Sprite2D*			sprite2D;
	//ParticleEmitter*	particleEmitter;
	//AABB*				aabb;
	//BC*					bc;
	//OBB*				obb;
	//CapsuleCollider*	capsuleCollider;
	//RigidBody*			rigidBody;
	//std::vector<Script*>script;
	//Sound*				sound;
	//TextRenderer*		textRenderer;
	//Light*				light;
	//UISprite*			uisprite;
	//VideoCompo*			videocompo;
};

extern std::vector<Selection>	ListOfSelection;
SELECTION_TYPE CheckSelectionType();
void SelectInverse();

#endif
