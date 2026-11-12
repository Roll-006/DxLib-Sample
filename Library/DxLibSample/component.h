#pragma once
#include "game_object.h"

namespace DxLibSample
{
    class Component abstract : public std::enable_shared_from_this<Component>
    {
    public:
        Component();
        virtual ~Component() = default;

        virtual void Initialize(const std::shared_ptr<GameObject>& gameObject) abstract;
        virtual void Start()        abstract;
        virtual void Update()       abstract;
        virtual void LateUpdate()   abstract;
        virtual void Render() const abstract;

        virtual void Deserialize(const nlohmann::json& json) abstract;

        /// <summary>
        /// ゲームオブジェクトを取得する
        /// </summary>
        /// <returns>ゲームオブジェクト</returns>
        std::shared_ptr<GameObject> GetGameObject() const { return _gameObject.lock(); }
        
        /// <summary>
        /// コンポーネントを取得する
        /// </summary>
        /// <typeparam name="T">コンポーネントの型</typeparam>
        /// <returns>取得するコンポーネント。対象が見つからなかった場合はnullptr</returns>
        template<ComponentT T>
        std::shared_ptr<T> GetComponent() const
        {
            return _gameObject.lock()->GetComponent<T>();
        }

        /// <summary>
        /// 同一コンポーネントを取得する
        /// </summary>
        /// <typeparam name="T">コンポーネントの型</typeparam>
        /// <returns>取得するコンポーネント。対象が一つも見つからなかった場合は空vector</returns>
        template<ComponentT T>
        std::vector<std::shared_ptr<T>> GetComponents() const
        {
            return _gameObject.lock()->GetComponents<T>();
        }
        
        template<ComponentT T>
        std::shared_ptr<T> GetComponentInChildren() const
        {
            return _gameObject.lock()->GetComponentInChildren<T>();
        }
		
        template<ComponentT T>
        std::vector<std::shared_ptr<T>> GetComponentsInChildren() const
        {
            return _gameObject.lock()->GetComponentsInChildren<T>();
        }

        std::shared_ptr<Transform> GetTransform() const
        {
            return _gameObject.lock()->GetTransform();
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
}