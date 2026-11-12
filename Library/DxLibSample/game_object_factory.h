#pragma once
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>

namespace DxLibSample
{
    class Scene;
    class GameObject;
}

namespace DxLibSample
{
    class GameObjectFactory final
    {
    public:
        using CreateFunction = std::function<std::shared_ptr<GameObject>(Scene&)>;
        
        static void Register(const std::string& type, CreateFunction function)
        {
            GetMap()[type] = function;
        }
        
        [[nodiscard]] static std::shared_ptr<GameObject> Create(const std::string& type, Scene& scene)
        {
            auto& map = GetMap();
            auto itr = map.find(type);

            if (itr == map.end())
            {
                return nullptr;
            }

            return itr->second(scene);
        }

    private:
        [[nodiscard]] static std::unordered_map<std::string, CreateFunction>& GetMap()
        {
            static std::unordered_map<std::string, CreateFunction> map;
            return map;
        }
    };
}