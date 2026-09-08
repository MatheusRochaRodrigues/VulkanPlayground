#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
// #include <type_traits>
#include <cassert>

#include <scene/ecs/component.h>
#include <scene/ecs/entity.h>
#include <scene/ecs/system.h>

class Scene {
public:
    void init();
 
    Entity CreateEntity();
 
    template<typename T, typename... Args>
    T& AddComponent(Entity entity, Args&&... args);

    template<typename T>
    void RemoveComponent(Entity entity);

    template<typename T>
    bool HasComponent(Entity entity) const;

    template<typename T>
    T& GetComponent(Entity entity);

    template<typename T>
    T* TryGetComponent(Entity entity);

    template<typename... Components>
    std::vector<Entity> GetEntitiesWith();
 
    template<typename T, typename... Args>
    T* RegisterSystem(Args&&... args);

    void Update(float deltaTime);
 
    void SelectEntity(Entity entity);
    Entity GetSelectedEntity() const;
 
    std::vector<Entity> GetAllEntities() const;
    bool EntityExists(Entity e) const;
 
    bool HasComponentType(Entity e, std::type_index type) const;
    std::vector<std::type_index> GetComponentTypes(Entity e) const;

    template<typename T>
    Entity GetFirstEntityOfType() const;

    // Resources
    template<typename T, typename... Args>
    std::shared_ptr<T> EmplaceResource(Args&&... args);

    template<typename T>
    std::shared_ptr<T> GetResource();

    template<typename T>
    std::shared_ptr<T> TryGetResource();

    template<typename T>
    T* TryGetResourceRaw();
 

    void DestroyGameObject(Entity e);

private:
    std::unique_ptr<ComponentManager> componentManager = std::make_unique<ComponentManager>();

    std::vector<std::unique_ptr<System>> systems;

    EntityManager entityManager;

    std::unordered_map<std::type_index, std::shared_ptr<void>> resources;

    Entity selectedEntity = INVALID_ENTITY;

    std::vector<Entity> toDestroy;
};