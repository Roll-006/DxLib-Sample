#pragma once
#include "component_concept.h"
#include "game_object.h"

class Component abstract
{
public:
	Component();
	virtual ~Component() = default;

    virtual void Initialize(const std::shared_ptr<GameObject>& gameObject) abstract;
    virtual void Update()       abstract;
    virtual void LateUpdate()   abstract;
    virtual void Render() const abstract;

    virtual void Deserialize(const nlohmann::json& json) abstract;

    /// <summary>
    /// コンポーネントを取得する
    /// </summary>
    /// <typeparam name="T">コンポーネントの型</typeparam>
    /// <returns>取得するコンポーネント。対象が見つからなかった場合はnullptr</returns>
    template<ComponentT T>
    std::shared_ptr<T> GetComponent()
    {
        return _gameObject.lock()->GetComponent<T>();
    }

    /// <summary>
    /// 同一コンポーネントを取得する
    /// </summary>
    /// <typeparam name="T">コンポーネントの型</typeparam>
    /// <returns>取得するコンポーネント。対象が一つも見つからなかった場合は空vector</returns>
    template<ComponentT T>
    std::vector<std::shared_ptr<T>> GetComponents()
    {
        return _gameObject.lock()->GetComponents<T>();
    }

    /// <summary>
    /// コンポーネントを有効にするかを設定
    /// </summary>
    /// <param name="enable">true : 有効にする, false : 無効にする</param>
    void SetEnable(const bool enable) { _enabled = enable; }

protected:
	std::weak_ptr<GameObject> _gameObject;
    bool _enabled;
};
