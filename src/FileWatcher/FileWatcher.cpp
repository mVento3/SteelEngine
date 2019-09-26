#include "FileWatcher/FileWatcher.h"

namespace SteelEngine {

    bool FileWatcher::Contains(const std::string& key)
    {
        return m_Paths.find(key) != m_Paths.end();
    }

    FileWatcher::FileWatcher(
        const std::filesystem::path& path,
        const std::function<void(const std::filesystem::path&, FileStatus)>& action) :
        m_Path(path),
        m_Action(action)
    {
        m_Running = true;

        for(auto &file : std::filesystem::recursive_directory_iterator(path))
        {
            m_Paths[file.path().string()] = std::filesystem::last_write_time(file);
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

        for(auto& file : std::filesystem::recursive_directory_iterator(m_Path))
        {
            auto currentFileLastWriteTime = std::filesystem::last_write_time(file);

            std::string file_ = file.path().string();

            if(!Contains(file_))
            {
                m_Paths[file_] = currentFileLastWriteTime;

                m_Action(file_, FileStatus::CREATED);
            }
            else
            {
                if(m_Paths[file_] != currentFileLastWriteTime)
                {
                    m_Paths[file_] = currentFileLastWriteTime;

                    m_Action(file_, FileStatus::MODIFIED);
                }
            }
        }
    }

}