#include "FileWatcher/FileWatcher.h"

namespace SteelEngine {

    bool FileWatcher::Contains(const std::string& key)
    {
        return m_Paths.find(key) != m_Paths.end();
    }

    void FileWatcher::UpdateRecursive()
    {
        for(auto& file : std::filesystem::recursive_directory_iterator(m_Path))
        {
            auto currentFileLastWriteTime = std::filesystem::last_write_time(file);

            std::string file_ = file.path().string();

            if(!Contains(file_))
            {
                m_Paths[file_] = currentFileLastWriteTime;

                m_Action(file_, FileStatus::CREATED);
            }
            else if(m_Paths[file_] != currentFileLastWriteTime)
            {
                m_Paths[file_] = currentFileLastWriteTime;

                m_Action(file_, FileStatus::MODIFIED);
            }
        }
    }

    void FileWatcher::UpdateNonRecursive()
    {
        for(auto& file : std::filesystem::directory_iterator(m_Path))
        {
            auto currentFileLastWriteTime = std::filesystem::last_write_time(file);

            std::string file_ = file.path().string();

            if(!Contains(file_))
            {
                m_Paths[file_] = currentFileLastWriteTime;

                m_Action(file_, FileStatus::CREATED);
            }
            else if(m_Paths[file_] != currentFileLastWriteTime)
            {
                m_Paths[file_] = currentFileLastWriteTime;

                m_Action(file_, FileStatus::MODIFIED);
            }
        }
    }

    FileWatcher::FileWatcher(
        const std::filesystem::path& path,
        ActionFunction action,
        bool recursive) :
        m_Path(path),
        m_Action(action)
    {
        m_Running = true;

        for(auto &file : std::filesystem::recursive_directory_iterator(path))
        {
            m_Paths[file.path().string()] = std::filesystem::last_write_time(file);
        }

        if(recursive)
        {
            m_UpdateFunction = std::bind(&FileWatcher::UpdateRecursive, this);
        }
        else
        {
            m_UpdateFunction = std::bind(&FileWatcher::UpdateNonRecursive, this);
        }
    }

    FileWatcher::~FileWatcher()
    {

    }

    void FileWatcher::Update()
    {
        auto it = m_Paths.begin();

        while(it != m_Paths.end())
        {
            if(!std::filesystem::exists(it->first))
            {
                m_Action(it->first, FileStatus::DELETED);

                it = m_Paths.erase(it);
            }
            else
            {
                it++;
            }
        }

        m_UpdateFunction();
    }

}