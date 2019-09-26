#include "Utils/Options.h"

namespace SteelEngine {

    Options::Options(char* argv[], Type::uint32 argc, std::vector<Descriptor>& desc) :
        m_Descriptor(&desc)
    {
        // Skip program name
        argc -= (argc > 0);
        argv += (argc > 0);

        m_Arguments = "";
 
        for(Type::uint32 i = 0; i < argc; i++)
        {
            m_Arguments.append(argv[i]);

            if(i < argc - 1)
            {
                m_Arguments.append(" ");
            }
        }

        std::vector<std::string> splitted = split(m_Arguments, '-');

        for(Type::uint32 i = 0; i < splitted.size(); i++)
        {
            std::string* sp = &splitted[i];

            if(sp->size() > 0)
            {
                if(sp->at(sp->size() - 1) == ' ')
                {
                    sp->erase(sp->end() - 1);
                }

                std::vector<std::string> splitted2 = split(*sp, ' ');

                for(Type::uint32 j = 0; j < desc.size(); j++)
                {
                    Descriptor* desc_ = &desc[j];

                    if(desc_->m_Option == splitted2[0])
                    {
                        desc_->m_IsSet = true;

                        if(splitted2.size() == 2)
                        {
                            desc_->m_Result = splitted2[1];
                        }

                        break;
                    }
                }
            }
        }
    }

    Options::~Options()
    {

    }

    Options::Descriptor Options::operator[](Type::uint32 index)
    {
        return m_Descriptor->at(index);
    }

}