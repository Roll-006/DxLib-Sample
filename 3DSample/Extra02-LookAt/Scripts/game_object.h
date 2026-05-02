#pragma once
#include <memory>
#include "component_concept.h"

class Scene;
class Transform;

class GameObject abstract : public std::enable_shared_from_this<GameObject>
{
public:
	GameObject(const std::string& jsonPath);
    virtual ~GameObject() = default;

    void Initialize(const std::shared_ptr<Scene>& scene);
    void Update();
    void LateUpdate();
    void Render() const;

    /// <summary>
    /// コンポーネントを追加
    /// </summary>
    /// <typeparam name="T">コンポーネントの型</typeparam>
    /// <param name="json">読み込むJSONデータ</param>
    /// <returns>登録したコンポーネント</returns>
    template<ComponentT T>
    std::shared_ptr<T> AddComponent()
    {
        const auto component = std::make_shared<T>();
        _components.emplace_back(component);
        return component;
    }

    /// <summary>
    /// コンポーネントを取得する
    /// </summary>
    /// <typeparam name="T">コンポーネントの型</typeparam>
    /// <returns>取得するコンポーネント。対象が見つからなかった場合はnullptr</returns>
    template<ComponentT T>
    std::shared_ptr<T> GetComponent()
    {
        for (const auto& component : _components)
        {
            if (const auto casted = std::dynamic_pointer_cast<T>(component)) { return casted; }
        }

        return nullptr;
    }

    /// <summary>
    /// 同一コンポーネントを取得する
    /// </summary>
    /// <typeparam name="T">コンポーネントの型</typeparam>
    /// <returns>取得するコンポーネント。対象が一つも見つからなかった場合は空vector</returns>
    template<ComponentT T>
    std::vector<std::shared_ptr<T>> GetComponents()
    {
        std::vector<std::shared_ptr<T>> components = {};

        for (const auto& component : _components)
        {
            if (const auto casted = std::dynamic_pointer_cast<T>(component)) { components.emplace_back(casted); }
        }

        return components;
    }

	std::string GetName()   const { return _name; }
	std::string GetTag()    const { return _tag; }

    /// <summary>
    /// 同一シーン上のオブジェクトを検索
    /// </summary>
    /// <param name="name">オブジェクト名</param>
    /// <returns>見つかったオブジェクトのTransform。見つからなかった場合はnullptr</returns>
    std::shared_ptr<Transform> Find(const std::string& name);

    /// <summary>
    /// アクティブ状態を設定する
    /// </summary>
    /// <param name="isActive">true : アクティブ化する, false : 非アクティブ化する</param>
    void SetActive(const bool isActive) { _isActive = isActive; }

protected:
	std::string _name;
	std::string _tag;
    bool _isActive;
	std::vector<std::shared_ptr<Component>> _components;
    std::weak_ptr<Scene> _owenerScene;

	friend void from_json	(const nlohmann::json& json, GameObject& gameObject);
	friend void to_json		(nlohmann::json& json, const GameObject& gameObject);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, GameObject& gameObject)
{
	json.at("name").       get_to(gameObject._name);
	json.at("tag").        get_to(gameObject._tag);
	json.at("isActive").   get_to(gameObject._isActive);
}

inline void to_json(nlohmann::json& json, const GameObject& gameObject)
{
	json =
	{
		{ "name",       gameObject._name },
		{ "tag",        gameObject._tag },
		{ "isActive",   gameObject._isActive },
	};
}
#pragma endregion
