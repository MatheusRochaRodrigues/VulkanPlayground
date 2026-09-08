#include "scene.h"  
   
void Scene::init() { 
    
} 

void Scene::Update(float deltaTime) {  
    for (auto& system : systems) system->Update(deltaTime);
}

Entity Scene::CreateEntity() { return entityManager.CreateEntity(); }  

template<typename T, typename... Args>
T* Scene::RegisterSystem(Args&&... args) {
    auto system = std::make_unique<T>(std::forward<Args>(args)...);
    T* ptr = system.get();
    systems.emplace_back(std::move(system));

    std::cerr << "[INFO - Sys] "
        << typeid(T).name()
        << "\n";


    return ptr;
}

void Scene::SelectEntity(Entity entity) { selectedEntity = entity; }

Entity Scene::GetSelectedEntity() const { return selectedEntity; }
 
template<typename T, typename... Args>
T& Scene::AddComponent(Entity entity, Args&&... args) {
    if (!componentManager) throw std::runtime_error("componentManager is nullptr");

    try { 
        //return componentManager->AddComponent<T>(entity, std::forward<Args>(args)...); 
        T& comp = componentManager->AddComponent<T>(entity, std::forward<Args>(args)...); 
        std::cout << "+ Registrado " << typeid(T).name() << "\n";
        return comp;
    } catch (const std::exception& e) {
        std::cerr << "[ERRO] AddComponent em Scene falhou para tipo " << typeid(T).name() << ": " << e.what() << "\n";
        throw; // repropaga para pegar mais acima se quiser
    }
}

template<typename T>
void Scene::RemoveComponent(Entity entity) {
    componentManager->RemoveComponent<T>(entity);
}

/*
void Scene::RemoveComponent(Entity entity, const TypeInfo& type) {
    const auto& storages = componentManager->GetAllStorages();
    auto it = storages.find(type.typeIndex);
    if (it != storages.end()) {
        it->second->Remove(entity);
        return;
    }
    
    std::cout << "[Error] nao foi possivel remover o Componente (n�o existe)" << std::endl;
    //assert(false && "TransformComponent cannot be removed");
}
*/
 
template<typename T>
bool Scene::HasComponent(Entity entity) const {
    return componentManager->HasComponent<T>(entity);
}

template<typename T>
T& Scene::GetComponent(Entity entity) {
    assert(HasComponent<T>(entity));
    return componentManager->GetComponent<T>(entity);           // scene.GetComponent<Transform>(e)     // assert no debug
}               

template<typename T>
T* Scene::TryGetComponent(Entity entity) {                           
    return componentManager->TryGetComponent<T>(entity);        //  if (auto* t = scene.TryGetComponent<Transform>(e))     // seguro        
} 
 
template<typename... Components>
std::vector<Entity> Scene::GetEntitiesWith() {
    return componentManager->GetEntitiesWith<Components...>();
}; 
  
std::vector<Entity> Scene::GetAllEntities() const {
    return entityManager.GetAllEntities();
} 

bool Scene::HasComponentType(Entity e, std::type_index type) const {
    return componentManager->HasComponentType(e, type);
}
std::vector<std::type_index> Scene::GetComponentTypes(Entity e) const {
    return componentManager->GetComponentTypes(e);
}   

template<typename T>
Entity Scene::GetFirstEntityOfType() const {
    return componentManager->GetFirstEntityOfType<T>();
}

bool Scene::EntityExists(Entity e) const {
    return entityManager.Exists(e); // delega para o EntityManager
} 

template<typename T, typename... Args>
std::shared_ptr<T> Scene::EmplaceResource(Args&&... args)
{
    auto resource = std::make_shared<T>(std::forward<Args>(args)...);
    resources[typeid(T)] = resource;
    return resource;

    //auto bridge = scene.EmplaceResource<ChunkStreamingBridge>();   way use example
} 

template<typename T>
std::shared_ptr<T> Scene::GetResource()
{
    auto it = resources.find(typeid(T));
    if (it == resources.end())
        throw std::runtime_error("Resource not found");

    return std::static_pointer_cast<T>(it->second);
}

template<typename T>
std::shared_ptr<T> Scene::TryGetResource()
{
    auto it = resources.find(typeid(T));
    if (it == resources.end())
        return nullptr;

    return std::static_pointer_cast<T>(it->second);
}

template<typename T>
T* Scene::TryGetResourceRaw()      //if you want to avoid the cost of the shared_ptr, Pointer Raw is lighter.
{
    auto it = resources.find(typeid(T));
    if (it == resources.end())
        return nullptr;

    return static_cast<T*>(it->second.get());
}

void Scene::DestroyGameObject(Entity e) {
    toDestroy.push_back(e);
}



 