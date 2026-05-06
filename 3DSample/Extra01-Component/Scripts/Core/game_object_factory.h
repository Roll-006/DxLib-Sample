#pragma once
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>

class Scene;
class GameObject;

class GameObjectFactory final
{
public:
    using CreateFunction = std::function<std::shared_ptr<GameObject>(Scene&)>;

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
    /// 型名からゲームオブジェクトを生成
    /// </summary>
    /// <param name="type">型名</param>
    /// <param name="gameObject">ゲームオブジェクトを所持しているシーン</param>
    /// <returns>生成したゲームオブジェクト</returns>
    static std::shared_ptr<GameObject> Create(const std::string& type, Scene& scene)
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
};
