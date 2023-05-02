#pragma once
#include "Component.h"
#include "Particle.h"
#include "Emitter.h"
#include "P_Module.h"
#include "Mesh.h"

class Application;
class ResourceMesh;
class ResourceTexture;

class ParticleSystemComponent : public Component
{
public:
    ParticleSystemComponent(GameObject* gameObject);

    ParticleSystemComponent(GameObject* gameObject, ParticleSystemComponent& copy);

    ~ParticleSystemComponent();

    void CreateEmitterMesh(uint resourceUID);

    Mesh& GetEmitterMesh();

    void DestroyEmitterMesh();

    void ChangeEmitterMeshTexture(ResourceTexture* resource);

    void OnEnable() override;
    void OnDisable() override;
#ifdef STANDALONE
    void OnEditor() override;

    void MarkAsDead() override;
    void MarkAsAlive() override;
#endif  
    void ResetEmitterTimers();

    void DestroyEmitterMeshTexture();

    void StopEmitter();

    void Serialization(json& j) override;
    void DeSerialization(json& j) override;

    bool GetPlayOnScene() { return playOnScene; }

    bool GetPauseOnScene() { return pauseOnScene; }

    bool GetPlayOnGame() { return playOnGame; }

    bool GetStopEmitting() { return StopEmittingOnGame; }

    Emitter& GetParticleSystemEmitter() { return ParticleEmitter; }

    void SetPlayOnGame(bool playongame);

    void SetStopEmitting(bool stopemitting);

    ParticleProperties particleProps;

private:

    void SetPlayOnScene(bool playonscene);
    void SetPauseOnScene(bool pauseonscene);
    
private:
    //Only for testing with particles on Scene
    bool playOnScene = false;
    bool pauseOnScene = false;
    //Play Particles on Game
    bool playOnGame = false;
    bool StopEmittingOnGame = false;

    int size = 100;
    int sizeCpy = 100;
    
    ResourceMesh* _resource = nullptr;
    ResourceTexture* _resourceText = nullptr;
    //Mark As Alive Use
    uint _resourceUID;
    uint _resourceTextUID;
    Application* app;
    Emitter ParticleEmitter; 

    //Particle System Modules
    std::vector<P_Module*> ParticleModules;

    friend class Emitter;
    friend class ParticleManager;
    friend class P_Module;
    friend class P_MainModule;
    friend class P_EmissionModule;
    friend class ModuleRenderer3D;
};

