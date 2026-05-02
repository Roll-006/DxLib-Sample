#pragma once
#include "../Core/component.h"

class GameObject;

/// <summary>
/// オブジェクトの座標、回転、スケールを保持している。
/// また、親子関係も保持している。
/// </summary>
class Transform final : public Component
{
public:
	Transform();

	void Initialize(const std::shared_ptr<GameObject>& gameObject) override;
	void Update()       override;
	void LateUpdate()   override;
	void Render() const	override;

	void Deserialize(const nlohmann::json& json) override;

	/// <summary>
	/// 特定のオブジェクトの子オブジェクトを検索
	/// </summary>
	/// <param name="name">オブジェクト名</param>
	/// <returns>見つかったオブジェクトのTransform。見つからなかった場合はnullptr</returns>
	std::shared_ptr<Transform> Find(const std::string& name) const;

	/// <summary>
	/// 全ての子オブジェクトを切り離す
	/// </summary>
	void DetachChildren();

	/// <summary>
	/// 対象の方向を見る
	/// </summary>
	/// <param name="target">注視する対象</param>
	/// <param name="up">upベクトル</param>
	void LookAt(const Vector3& target, const Vector3& up = Vector3::GetUp());

	#pragma region Getter
	Vector3		GetLocalPosition()	const { return _localPosition; }
	Vector3		GetLocalRotation()	const { return _localRotation; }
	Vector3		GetLocalScale()		const { return _localScale; }
	Matrix4x4	GetLocalMatrix()	const { return Matrix4x4::CreateTRS(_localPosition, _localRotation, _localScale); }

	Vector3		GetWorldPosition();
	//Vector3		GetWorldRotation();
	Vector3		GetWorldScale();
	Matrix4x4	GetWorldMatrix();

	Vector3		GetRight();
	Vector3		GetUp();
	Vector3		GetForward();

	/// <summary>
	/// 親トランスフォームを取得
	/// </summary>
	/// <returns>親トランスフォーム。見つからなかった場合はnullptr</returns>
	std::shared_ptr<Transform> GetParent() const;

	/// <summary>
	/// 子トランスフォームを取得
	/// </summary>
	/// <param name="index">インデックス</param>
	/// <returns>子トランスフォーム。見つからなかった場合はnullptr</returns>
	std::shared_ptr<Transform> GetChild(const int index) const;

	/// <summary>
	/// 全ての子トランスフォームを取得
	/// </summary>
	/// <returns>全ての子トランスフォーム</returns>
	std::vector<std::shared_ptr<Transform>> GetChildren() const { return _children; }

	/// <summary>
	/// 子トランスフォームの数を取得
	/// </summary>
	/// <returns>子トランスフォームの数</returns>
	int GetChildCount() const { return static_cast<int>(_children.size()); }

	/// <summary>
	/// 階層最上位のトランスフォームを取得
	/// </summary>
	/// <returns>階層最上位のトランスフォーム</returns>
	std::shared_ptr<Transform> GetRoot();

	/// <summary>
	/// 兄弟内(同一階層)でのインデックスを取得する
	/// </summary>
	/// <returns>兄弟内でのインデックス</returns>
	int GetSiblingIndex();
	#pragma endregion

	#pragma region Setter
	void SetLocalPosition	(const Vector3& position);
	void SetLocalRotation	(const Vector3& rotation);
	void SetLocalScale		(const Vector3& scale);
	void SetLocalScale		(const float scale);

	/// <summary>
	/// 親トランスフォームを設定する
	/// </summary>
	/// <param name="parent">親トランスフォーム</param>
	void SetParent(const std::shared_ptr<Transform>& parent);

	/// <summary>
	/// 兄弟内(同一階層)でインデックスを指定しトランスフォームを移動させる
	/// </summary>
	/// <param name="index">兄弟内でのインデックス</param>
	//void SetSiblingIndex(const int index);
	#pragma endregion
	
private:
	/// <summary>
	/// 座標、回転、スケールに更新があった場合に行列を更新する
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// 子トランスフォームを追加する
	/// </summary>
	/// <param name="child">子トランスフォーム</param>
	void AddChild(const std::shared_ptr<Transform>& child);

	/// <summary>
	/// 子トランスフォームを解除する
	/// </summary>
	/// <param name="index">子のインデックス</param>
	void RemoveChild(const int index);

	/// <summary>
	/// 座標、回転、スケール情報が汚された
	/// </summary>
	void OnDirty();

private:
	Vector3		_localPosition;
	Vector3		_localRotation;
	Vector3		_localScale;
	Matrix4x4	_worldMatrix;
	bool		_isDirty;
	std::weak_ptr<Transform> _parent;
	std::vector<std::shared_ptr<Transform>> _children;

	friend void from_json	(const nlohmann::json& json, Transform& transform);
	friend void to_json		(nlohmann::json& json, const Transform& transform);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& json, Transform& transform)
{
	json.at("enabled").			get_to(transform._enabled);
	json.at("localPosition").	get_to(transform._localPosition);
	json.at("localRotation").	get_to(transform._localRotation);
	json.at("localScale").		get_to(transform._localScale);

	transform._localRotation *= math::kDeg2Rad;
}

inline void to_json(nlohmann::json& json, const Transform& transform)
{
	json =
	{
		{ "enabled",		transform._enabled },
		{ "localPosition",	transform._localPosition },
		{ "localRotation",	transform._localRotation },
		{ "localScale",		transform._localScale },
	};
}
#pragma endregion
