#pragma once

#include "string"
#include "chrono"
#include "unordered_map"
#include "filesystem"
#include "functional"
#include "thread"

namespace SteelEngine {

    class FileWatcher
    {
    public:
        enum FileStatus
        {
            CREATED,
            MODIFIED,
            DELETED
        };

    private:
        std::unordered_map<std::string, std::filesystem::file_time_type> m_Paths;
        bool m_Running;
        std::function<void(const std::filesystem::path&, FileStatus)> m_Action;

        bool Contains(const std::string& key);

    public:
        FileWatcher(
            const std::filesystem::path& path,
            const std::function<void(const std::filesystem::path&, FileStatus)>& action
        );
        ~FileWatcher();

        std::filesystem::path m_Path;

        void Update();
    };

}