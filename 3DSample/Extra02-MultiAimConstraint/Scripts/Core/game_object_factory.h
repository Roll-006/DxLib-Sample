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
    /// �^���Ɛ����֐���o�^
    /// </summary>
    /// <param name="type">�^��</param>
    /// <param name="function">�����֐�</param>
    static void Register(const std::string& type, CreateFunction function)
    {
        GetMap()[type] = function;
    }

    /// <summary>
    /// �^������Q�[���I�u�W�F�N�g�𐶐�
    /// </summary>
    /// <param name="type">�^��</param>
    /// <param name="gameObject">�Q�[���I�u�W�F�N�g���������Ă���V�[��</param>
    /// <returns>���������Q�[���I�u�W�F�N�g</returns>
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
    /// �^���Ɛ����֐���ێ������}�b�v���擾
    /// ��������
    /// </summary>
    /// <returns>�^���Ɛ����֐���ێ������}�b�v</returns>
    static std::unordered_map<std::string, CreateFunction>& GetMap()
    {
        static std::unordered_map<std::string, CreateFunction> map;
        return map;
    }
};