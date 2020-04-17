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
            DELETED,
            NONE
        };

        typedef std::function<void(const std::filesystem::path&, FileStatus)> ActionFunction;

    private:
        typedef std::function<void()> UpdateFunction;

        std::unordered_map<std::string, std::filesystem::file_time_type> m_Paths;
        bool m_Running;
        ActionFunction m_Action;

        UpdateFunction m_UpdateFunction;

        bool Contains(const std::string& key);

        void UpdateRecursive();
        void UpdateNonRecursive();

    public:
        FileWatcher(
            const std::filesystem::path& path,
            ActionFunction action,
            bool recursive = true
        );
        ~FileWatcher();

        std::filesystem::path m_Path;

        void Update();
    };

}