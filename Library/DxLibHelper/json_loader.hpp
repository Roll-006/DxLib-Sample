#pragma once
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

namespace json_loader
{
    inline bool Save(const std::string_view& file_path, const nlohmann::json& data)
    {
        std::ofstream ofs(std::string(file_path), std::ios::out);

        if (!ofs) return false;

        try
        {
            ofs << data.dump(4);
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    [[nodiscard]] inline bool Load(const std::string_view& file_path, nlohmann::json& data)
    {
        std::string path = std::string(file_path);
        std::ifstream ifs(path);

        if (!ifs) return false;

        try
        {
            ifs >> data;
        }
        catch (...)
        {
            return false;
        }

        return true;
    }
}
