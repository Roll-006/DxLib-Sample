#pragma once
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>

class Component;
class GameObject;

class ComponentFactory final
{
public:
    using CreateFunction = std::function<std::shared_ptr<Component>(GameObject&)>;

    /// <summary>
    /// 型名と生成関数を登録
    /// </summary>
    /// <param name="type">型名</param>
    /// <param name="function">生成関数</param>
    static void Register(const std::string& type, CreateFunction function)
    {
        GetMap()[type] = function;
    }

    /// <summary>
    /// 型名からコンポーネントを生成
    /// </summary>
    /// <param name="type">型名</param>
    /// <param name="gameObject">コンポーネントを所持しているオブジェクト</param>
    /// <returns></returns>
    static std::shared_ptr<Component> Create(const std::string& type, GameObject& gameObject)
    {
        auto& map = GetMap();
        auto itr = map.find(type);

        if (itr == map.end())
        {
            return nullptr;
        }

        return itr->second(gameObject);
    }

private:
    /// <summary>
    /// 型名と生成関数を保持したマップを取得
    /// 初期化順
    /// </summary>
    /// <returns>型名と生成関数を保持したマップ</returns>
    static std::unordered_map<std::string, CreateFunction>& GetMap()
    {
        static std::unordered_map<std::string, CreateFunction> map;
        return map;
    }

    //static std::unordered_map<std::string, CreateFunction> _functionMap;
};
