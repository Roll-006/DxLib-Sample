#pragma once
#include <memory>

class GameObject;
class Transform;

class Scene abstract : public std::enable_shared_from_this<Scene>
{
public:
	Scene(/*const std::string& jsonPath*/);
	virtual ~Scene() = default;

	void Initialize();
	void Update();
	void LateUpdate();
	void Render() const;

	/// <summary>
	/// 同一シーン上のオブジェクトを検索
	/// </summary>
	/// <param name="name">オブジェクト名</param>
	/// <returns>見つかったオブジェクトのTransform。見つからなかった場合はnullptr</returns>
	std::shared_ptr<Transform> Find(const std::string& name);

public:
	std::string _name;
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
