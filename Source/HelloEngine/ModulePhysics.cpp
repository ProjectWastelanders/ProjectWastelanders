#include "Headers.h"
#include "ModulePhysics.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "LayerGame.h"
#include "ModuleLayers.h"
#include "PhysicsComponent.h"
#include "GameObject.h"

btDiscreteDynamicsWorld* ModulePhysics::world = nullptr;
std::vector <PhysBody3D*> ModulePhysics::physBodies;

ModulePhysics::ModulePhysics()
{
	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();

	hasToChangeGravity = false;

	gravityToChange = float3(0, -9.8, 0);

	hasToSetRenderBuffers = true;
}

ModulePhysics::~ModulePhysics()
{
	RELEASE(world);
	RELEASE(solver);
	RELEASE(broad_phase);
	RELEASE(dispatcher);
	RELEASE(collision_conf);
}

bool ModulePhysics::Init()
{
	return true;
}

bool ModulePhysics::Start()
{
	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	
	world->setGravity(btVector3(0.0f, -10.0f, 0.0f));

	// Testing------------------------------------------
	//PrimCube cube = PrimCube(2);
	///////testBody = CreatePhysBody(&cube);
	//testBody->SetVelocity(12, 10, 10);
	///////testBody->SetPos(10, 10, 10);
	//PrimCube cube = PrimCube(2);
	//testBody = CreatePhysBody(&cube);
	//testBody2 = CreatePhysBody(&cube);
	////testBody->SetVelocity(12, 10, 10);
	//testBody->SetPos(10, 0, 0);
	//testBody2->SetPos(-10, 0, 0);
	//testBody->SetVelocity(-0.05f, 0, 0);
	//testBody2->SetVelocity(0.05f, 0, 0);

	//testBody->body->setGravity(btVector3(0, 0, 0));
	//testBody2->body->setGravity(btVector3(0, 0, 0));
	// Testing------------------------------------------

	//for (int i = 0; i < physBodies.size(); i++) 
	//{
	//	if (ModuleLayers::gameObjects.count(physBodies[i]->gameObjectUID) != 0)
	//	{
	//		GameObject* go = ModuleLayers::gameObjects[physBodies[i]->gameObjectUID];
	//		go->GetComponent<PhysicsComponent>()->CheckRenderBuffers();
	//	}
	//}

	return true;
}

UpdateStatus ModulePhysics::PreUpdate()
{
	/*if (hasToSetRenderBuffers == true) 
	{
		for (int i = 0; i < physBodies.size(); i++) 
		{
			if (ModuleLayers::gameObjects.count(physBodies[i]->gameObjectUID) != 0)
			{
				GameObject* go = ModuleLayers::gameObjects[physBodies[i]->gameObjectUID];
				go->GetComponent<PhysicsComponent>()->CheckRenderBuffers();
			}
		}
		hasToSetRenderBuffers = false;
	}*/
	//world->stepSimulation(Application::Instance()->GetDeltaTime(), 15);

	////testBody->Push(1, 1, 1);
	//testBody->body->activate(true);
	////std::cout << "\ndt:" << EngineTime::GameDeltaTime()<<std::endl;
	////std::cout << "\n-------------------------\nx" << testBody->GetVelocity().x << "\ny" << testBody->GetVelocity().y << "\nz" << testBody->GetVelocity().z;
	//std::cout <<"\n-------------------------\nx" << testBody->GetPos().x << "\ny" << testBody->GetPos().y << "\nz" << testBody->GetPos().z;
#ifdef _DEBUG
	if (LayerGame::S_IsPlaying() && !LayerGame::S_IsPause())
	{
		world->stepSimulation(EngineTime::GameDeltaTime(), 15);
	}
	else
	{
		world->stepSimulation(0);
	}
#elif STANDALONE
	if (LayerGame::S_IsPlaying() && !LayerGame::S_IsPause())
	{
		world->stepSimulation(EngineTime::GameDeltaTime(), 15);
}
	else
	{
		world->stepSimulation(0);
	}
#else
	world->stepSimulation(EngineTime::GameDeltaTime(), 15);
#endif	

	////////////OPCION 1 
	// (Adaptacion de como estaba. Usa 2 mapas, el del frame anterior y el del actual, y compara. 
	// Primero actualiza los mapas para este frame. 
	// Luego, actualiza los mapas de los objetos que colisionan.
	// Finalmente, actualiza los mapas de los objetos que no han colisionado y que en el anterior frame si lo hacian)


	//if (LayerGame::S_IsPlaying()) {
	//	for (int i = 0; i < physBodies.size(); i++)
	//	{
	//		PhysBody3D* pbody = physBodies.at(i);
	//		pbody->lastFrameCollidingBodies.clear();
	//		pbody->lastFrameCollidingBodies.insert(pbody->thisFrameCollidingBodies.begin(), pbody->thisFrameCollidingBodies.end());
	//		pbody->thisFrameCollidingBodies.clear();
	//	}

	//	int numManifolds = world->getDispatcher()->getNumManifolds();

	//	for (int i = 0; i < numManifolds; i++)
	//	{
	//		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
	//		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
	//		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());


	//		int numContacts = contactManifold->getNumContacts();
	//		if (numContacts > 0)
	//		{
	//			//Console::S_Log("CONTACT");

	//			PhysBody3D* pBodyA = (PhysBody3D*)obA->getUserPointer();
	//			PhysBody3D* pBodyB = (PhysBody3D*)obB->getUserPointer();

	//			if (pBodyA && pBodyB)
	//			{
	//				if (ModuleLayers::gameObjects.count(pBodyA->gameObjectUID) != 0 && ModuleLayers::gameObjects[pBodyA->gameObjectUID] != nullptr)
	//				{
	//					GameObject* objectA = ModuleLayers::gameObjects[pBodyA->gameObjectUID];
	//					GameObject* objectB = ModuleLayers::gameObjects[pBodyB->gameObjectUID];

	//					if (!objectA->IsActive() || !objectB->IsActive())
	//						continue;

	//					//Prov
	//					/*objectA->OnCollisionEnter(pBodyB);*/

	//					if (pBodyA->lastFrameCollidingBodies.count(pBodyB->gameObjectUID) != 0 && pBodyA->lastFrameCollidingBodies[pBodyB->gameObjectUID] != nullptr)
	//					{
	//						//stay
	//						objectA->OnCollisionStay(pBodyB);
	//						pBodyA->thisFrameCollidingBodies.insert(std::pair<uint, PhysBody3D*>(pBodyB->gameObjectUID, pBodyB));
	//					}
	//					else
	//					{
	//						//enter
	//						objectA->OnCollisionEnter(pBodyB);
	//						pBodyA->thisFrameCollidingBodies.insert(std::pair<uint, PhysBody3D*>(pBodyB->gameObjectUID, pBodyB));
	//					}
	//				}

	//				if (ModuleLayers::gameObjects.count(pBodyB->gameObjectUID) != 0 && ModuleLayers::gameObjects[pBodyB->gameObjectUID] != nullptr)
	//				{
	//					GameObject* objectA = ModuleLayers::gameObjects[pBodyA->gameObjectUID];
	//					GameObject* objectB = ModuleLayers::gameObjects[pBodyB->gameObjectUID];

	//					if (!objectB->IsActive())
	//						continue;

	//					//Prov
	//					/*objectB->OnCollisionEnter(pBodyA);*/

	//					if (pBodyB->lastFrameCollidingBodies.count(pBodyA->gameObjectUID) != 0 && pBodyB->lastFrameCollidingBodies[pBodyA->gameObjectUID] != nullptr)
	//					{
	//						//stay
	//						objectB->OnCollisionStay(pBodyA);
	//						pBodyB->thisFrameCollidingBodies.insert(std::pair<uint, PhysBody3D*>(pBodyA->gameObjectUID, pBodyA));
	//					}
	//					else
	//					{
	//						//enter
	//						objectB->OnCollisionEnter(pBodyA);
	//						pBodyB->thisFrameCollidingBodies.insert(std::pair<uint, PhysBody3D*>(pBodyA->gameObjectUID, pBodyA));
	//					}
	//				}
	//			}
	//		}
	//	}

	//	for (int i = 0; i < physBodies.size(); i++)
	//	{
	//		PhysBody3D* pbody = physBodies.at(i);

	//		for (auto it1 = pbody->lastFrameCollidingBodies.begin(); it1 != pbody->lastFrameCollidingBodies.end(); it1++) {

	//			if (pbody->thisFrameCollidingBodies.count(it1->first) == 0 || pbody->thisFrameCollidingBodies[it1->first] == nullptr)
	//			{
	//				//exit
	//				GameObject* obj = ModuleLayers::gameObjects[pbody->gameObjectUID];

	//				obj->OnCollisionExit(pbody->lastFrameCollidingBodies[it1->first]);
	//			}
	//		}
	//	}
	//}

	////OPCION 1.5 (Adaptacion de la 1 que quiza esta un poco mas optimizado)
	

	if (LayerGame::S_IsPlaying()) {
		for (int i = 0; i < physBodies.size(); i++)
		{
			PhysBody3D* pbody = physBodies.at(i);
			pbody->lastFrameCollidingBodies.clear();
			pbody->lastFrameCollidingBodies.insert(pbody->thisFrameCollidingBodies.begin(), pbody->thisFrameCollidingBodies.end());
			pbody->thisFrameCollidingBodies.clear();
		}

		int numManifolds = world->getDispatcher()->getNumManifolds();

		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
			btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());


			int numContacts = contactManifold->getNumContacts();
			if (numContacts > 0)
			{
				//Console::S_Log("CONTACT");

				PhysBody3D* pBodyA = (PhysBody3D*)obA->getUserPointer();
				PhysBody3D* pBodyB = (PhysBody3D*)obB->getUserPointer();

				if (pBodyA && pBodyB)
				{
					if ((ModuleLayers::gameObjects.count(pBodyA->gameObjectUID) != 0 && ModuleLayers::gameObjects[pBodyA->gameObjectUID] != nullptr)
					&& (ModuleLayers::gameObjects.count(pBodyB->gameObjectUID) != 0 && ModuleLayers::gameObjects[pBodyB->gameObjectUID] != nullptr))
					{
						GameObject* objectA = ModuleLayers::gameObjects[pBodyA->gameObjectUID];
						GameObject* objectB = ModuleLayers::gameObjects[pBodyB->gameObjectUID];

						if (!objectA->IsActive() || !objectB->IsActive())
							continue;

						if (pBodyA->lastFrameCollidingBodies.count(pBodyB->gameObjectUID) != 0 && pBodyA->lastFrameCollidingBodies[pBodyB->gameObjectUID] != nullptr)
						{
							//stay
							objectA->OnCollisionStay(pBodyB);
							pBodyA->thisFrameCollidingBodies.insert(std::pair<uint, PhysBody3D*>(pBodyB->gameObjectUID, pBodyB));
						}
						else
						{
							//enter
							objectA->OnCollisionEnter(pBodyB);
							pBodyA->thisFrameCollidingBodies.insert(std::pair<uint, PhysBody3D*>(pBodyB->gameObjectUID, pBodyB));
						}

						if (pBodyB->lastFrameCollidingBodies.count(pBodyA->gameObjectUID) != 0 && pBodyB->lastFrameCollidingBodies[pBodyA->gameObjectUID] != nullptr)
						{
							//stay
							objectB->OnCollisionStay(pBodyA);
							pBodyB->thisFrameCollidingBodies.insert(std::pair<uint, PhysBody3D*>(pBodyA->gameObjectUID, pBodyA));
						}
						else
						{
							//enter
							objectB->OnCollisionEnter(pBodyA);
							pBodyB->thisFrameCollidingBodies.insert(std::pair<uint, PhysBody3D*>(pBodyA->gameObjectUID, pBodyA));
						}
					}
					
				}
			}
		}

		for (int i = 0; i < physBodies.size(); i++)
		{
			PhysBody3D* pbody = physBodies.at(i);

			for (auto it1 = pbody->lastFrameCollidingBodies.begin(); it1 != pbody->lastFrameCollidingBodies.end(); it1++) {

				if (pbody->thisFrameCollidingBodies.count(it1->first) == 0 || pbody->thisFrameCollidingBodies[it1->first] == nullptr)
				{
					//exit
					GameObject* obj = ModuleLayers::gameObjects[pbody->gameObjectUID];

					obj->OnCollisionExit(pbody->lastFrameCollidingBodies[it1->first]);
				}
			}
		}
	}

	///////////OPCION 2 
	// (Itera cada gameobject con colisiones. Por cada uno, itera otra vez para encontrar posibles colisiones.
	// Luego, itera todas las colisiones que se están produciendo.
	// Si una de esas colisiones coincide con los gameobject que se estan iterando, actualiza su mapa.
	// Si ninguna colision coincide con los gameobject que se estan iterando, actualiza su mapa.

	
	//if (LayerGame::S_IsPlaying()) {

	//	for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	//	{
	//		btCollisionObject* obj1 = world->getCollisionObjectArray()[i];

	//		for (int j = world->getNumCollisionObjects() - 1; j >= 0; j--)
	//		{
	//			if (j != i)
	//			{
	//				btCollisionObject* obj2 = world->getCollisionObjectArray()[j];

	//				PhysBody3D* pBody1 = (PhysBody3D*)obj1->getUserPointer();
	//				PhysBody3D* pBody2 = (PhysBody3D*)obj2->getUserPointer();

	//				GameObject* object1 = ModuleLayers::gameObjects[pBody1->gameObjectUID];
	//				GameObject* object2 = ModuleLayers::gameObjects[pBody2->gameObjectUID];


	//				bool obj1ContactsObj2 = false;

	//				int numManifolds = world->getDispatcher()->getNumManifolds();
	//				for (int k = 0; k < numManifolds; k++)
	//				{
	//					btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(k);
	//					btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
	//					btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

	//					int numContacts = contactManifold->getNumContacts();
	//					if (numContacts > 0)
	//					{
	//						//Console::S_Log("CONTACT");

	//						PhysBody3D* pBodyA = (PhysBody3D*)obA->getUserPointer();
	//						PhysBody3D* pBodyB = (PhysBody3D*)obB->getUserPointer();

	//						if (pBodyA && pBodyB)
	//						{
	//							if ((ModuleLayers::gameObjects.count(pBodyA->gameObjectUID) != 0 && ModuleLayers::gameObjects[pBodyA->gameObjectUID] != nullptr)
	//							&& (ModuleLayers::gameObjects.count(pBodyB->gameObjectUID) != 0 && ModuleLayers::gameObjects[pBodyB->gameObjectUID] != nullptr))
	//							{
	//								GameObject* objectA = ModuleLayers::gameObjects[pBodyA->gameObjectUID];
	//								GameObject* objectB = ModuleLayers::gameObjects[pBodyB->gameObjectUID];

	//								if (!objectA->IsActive() || !objectB->IsActive())
	//									continue;

	//								if (object1 == objectA && object2 == objectB)
	//								{
	//									obj1ContactsObj2 = true;
	//									if (pBody1->thisFrameCollidingBodies.count(pBody2->gameObjectUID) != 0 && pBody1->thisFrameCollidingBodies[pBody2->gameObjectUID] != nullptr)
	//									{
	//										//stay
	//										object1->OnCollisionStay(pBody2);
	//									}
	//									else
	//									{
	//										//enter
	//										object1->OnCollisionEnter(pBody2);
	//										pBody1->thisFrameCollidingBodies.insert(std::pair<uint, PhysBody3D*>(pBody2->gameObjectUID, pBody2));
	//									}
	//									//Console::S_Log("TRUE: " + object1->GetName() + " with " + object2->GetName());
	//								}
	//								else if (object1 == objectB && object2 == objectA)
	//								{
	//									obj1ContactsObj2 = true;
	//									if (pBody1->thisFrameCollidingBodies.count(pBody2->gameObjectUID) != 0 && pBody1->thisFrameCollidingBodies[pBody2->gameObjectUID] != nullptr)
	//									{
	//										//stay
	//										object1->OnCollisionStay(pBody2);
	//									}
	//									else
	//									{
	//										//enter
	//										object1->OnCollisionEnter(pBody2);
	//										pBody1->thisFrameCollidingBodies.insert(std::pair<uint, PhysBody3D*>(pBody2->gameObjectUID, pBody2));
	//									}
	//									//Console::S_Log("TRUE: " + object1->GetName() + " with " + object2->GetName());
	//								}
	//							}				
	//						}
	//					}
	//				}
	//				
	//				if (obj1ContactsObj2 == false) 
	//				{
	//					if (pBody1->thisFrameCollidingBodies.count(pBody2->gameObjectUID) != 0 && pBody1->thisFrameCollidingBodies[pBody2->gameObjectUID] != nullptr)
	//					{
	//						//exit
	//						object1->OnCollisionExit(pBody2);
	//						pBody1->thisFrameCollidingBodies.erase(pBody2->gameObjectUID);
	//					}
	//					//Console::S_Log("FALSE: " + object1->GetName() + " does not collide with " + object2->GetName());
	//				}				
	//			}
	//		}
	//	}
	//}

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModulePhysics::Update()
{
	world->updateAabbs();

	for (int i = 0; i < physBodies.size(); i++) 
	{
		if (LayerGame::S_IsPlaying())
			physBodies[i]->Update();
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModulePhysics::PostUpdate()
{
	if (hasToChangeGravity == true)
	{
		SetNewGravityAtLast();
		hasToChangeGravity = false;
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

bool ModulePhysics::CleanUp()
{
	for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
	}

	for (int i = 0; i < constraints.size(); i++)
	{
		world->removeConstraint(constraints[i]);
		RELEASE(constraints[i]);
	}

	constraints.clear();

	for (int i = 0; i < motions.size(); i++)
	{
		RELEASE(motions[i]);
	}

	motions.clear();

	for (int i = 0; i < shapes.size(); i++)
	{
		RELEASE(shapes[i]);
	}

	shapes.clear();

	//for (int i = 0; i < bodies.size(); i++)
	//{
	//	RELEASE(bodies[i]);
	//}

	//bodies.clear();

	return true;
}

PhysBody3D* ModulePhysics::CreatePhysBody(const Primitive* primitive, float mass)
{
	btCollisionShape* colShape = nullptr;
	btTransform setUpTransform;

	switch (primitive->GetType())
	{
	case PrimitiveTypes::Primitive_Cube:
	{
		PrimCube* cube = (PrimCube*)primitive;
		colShape = new btBoxShape(btVector3(cube->size.x * 0.5f, cube->size.y * 0.5f, cube->size.z * 0.5f));
		setUpTransform.setFromOpenGLMatrix(cube->transform.ptr());
	}
	break;
	case PrimitiveTypes::Primitive_Sphere:
	{
		PrimSphere* sphere = (PrimSphere*)primitive;
		colShape = new btSphereShape(sphere->radius);
		setUpTransform.setFromOpenGLMatrix(sphere->transform.ptr());
	}
	break;
	case PrimitiveTypes::Primitive_Cylinder:
	{
		PrimCylinder* cylinder = (PrimCylinder*)primitive;
		colShape = new btCylinderShapeX(btVector3(cylinder->height * 0.5f, cylinder->radius, 0.0f));
		setUpTransform.setFromOpenGLMatrix(cylinder->transform.ptr());
	}
	break;
	default:
		return nullptr;
	}

	shapes.push_back(colShape);

	btVector3 localInertia(0, 0, 0);

	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(setUpTransform);
	motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);

	body->setActivationState(DISABLE_DEACTIVATION);

	PhysBody3D* pbody = new PhysBody3D(body);

	switch (primitive->GetType())
	{
		case PrimitiveTypes::Primitive_Cube:
		{
			pbody->SetShape(ColliderShape::BOX);
		}
		break;
		case PrimitiveTypes::Primitive_Sphere:
		{
			pbody->SetShape(ColliderShape::SPHERE);
		}
		break;
		case PrimitiveTypes::Primitive_Cylinder:
		{
			pbody->SetShape(ColliderShape::CYLINDER);
		}
		break;
	}

	body->setUserPointer(pbody);

	world->addRigidBody(body);

	physBodies.push_back(pbody);

	return pbody;
}

void ModulePhysics::S_RemovePhysBody(PhysBody3D* physBody)
{
	world->removeRigidBody(physBody->body);
	RELEASE(physBody->body);
	for (int i = 0; i < physBodies.size(); ++i)
	{
		if (physBodies[i] == physBody)
		{
			RELEASE(physBodies[i]);
			physBodies.erase(physBodies.begin() + i);
		}
	}
}

void ModulePhysics::UpdatePhysBodyPos(PhysBody3D* physBody)
{
	if (ModuleLayers::gameObjects.count(physBody->gameObjectUID) == 0)
		return;
	float3 currentTransformPos = ModuleLayers::gameObjects[physBody->gameObjectUID]->transform->GetGlobalPosition();
	physBody->SetPos(physBody->colPos.x + currentTransformPos.x, physBody->colPos.y + currentTransformPos.y, physBody->colPos.z + currentTransformPos.z);
}

void ModulePhysics::UpdatePhysBodyRotation(PhysBody3D* physBody)
{
	if (ModuleLayers::gameObjects.count(physBody->gameObjectUID) == 0)
		return;

	physBody->SetRotation(physBody->colRot.x, physBody->colRot.y, physBody->colRot.z);
}

void ModulePhysics::UpdatePhysBodyScaleBox(PhysBody3D* physBody)
{
	physBody->SetScale(physBody->colScl.x, physBody->colScl.y, physBody->colScl.z);
}

void ModulePhysics::UpdatePhysBodyScaleSphere(PhysBody3D* physBody, float radius)
{
	physBody->SetScale(radius, radius, radius);
}

void ModulePhysics::UpdatePhysBodyScaleCylinder(PhysBody3D* physBody, float radius, float height)
{
	physBody->SetScale(radius, height, radius);
}

void ModulePhysics::SetGlobalGravity(float3 grav)
{
	world->setGravity(btVector3(grav[0], grav[1], grav[2]));
}

void ModulePhysics::SetGlobalGravityAtFirst(float3 grav)
{
	world->setGravity(btVector3(grav[0], grav[1], grav[2]));
}

float3 ModulePhysics::GetGlobalGravity()
{
	float3 grav = { world->getGravity()[0], world->getGravity()[1], world->getGravity()[2] };
	return grav;
}

void ModulePhysics::SetNewGravityAtLast()
{
	SetGlobalGravity(gravityToChange);
}

void ModulePhysics::PrepareNewGravityAtLast(float3 grav)
{
	hasToChangeGravity = true;
	gravityToChange = grav;
}