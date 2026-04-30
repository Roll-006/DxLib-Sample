#pragma once
#include <memory>
#include "game_object_concept.h"

class Transform;

class Scene abstract : public std::enable_shared_from_this<Scene>
{
public:
	Scene(const std::string& jsonPath);
	virtual ~Scene() = default;

	void Initialize();
	void Update();
	void LateUpdate();
	void Render() const;

	/// <summary>
	/// ゲームオブジェクトを追加
	/// </summary>
	/// <typeparam name="T">ゲームオブジェクトの型</typeparam>
	/// <returns>登録したゲームオブジェクト</returns>
	template<GameObjectT T>
	std::shared_ptr<T> AddGameObject()
	{
		const auto object = std::make_shared<T>();
		_objects.emplace_back(object);
		return object;
	}

	/// <summary>
	/// 同一シーン上のオブジェクトを検索
	/// </summary>
	/// <param name="name">オブジェクト名</param>
	/// <returns>見つかったオブジェクトのTransform。見つからなかった場合はnullptr</returns>
	std::shared_ptr<Transform> Find(const std::string& name);

private:
	/// <summary>
	/// オブジェクトをJSONから読み込んで生成 (再帰関数)
	/// </summary>
	/// <param name="childrenJson">オブジェクトのJSONデータ</param>
	/// <returns>その階層で読み込んだオブジェクトのトランスフォーム</returns>
	std::vector<std::shared_ptr<Transform>> LoadGameObjects(const nlohmann::json& childrenJson);

public:
	std::string _name;
	std::string _jsonPath;
	std::vector<std::shared_ptr<GameObject>> _objects;

	friend void from_json	(const nlohmann::json& json, Scene& scene);
	friend void to_json		(nlohmann::json& json, const Scene& scene);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, Scene& scene)
{
	json.at("name"). get_to(scene._name);
}

inline void to_json(nlohmann::json& json, const Scene& scene)
{
	json =
	{
		{ "name", scene._name },
	};
}
#pragma endregion
