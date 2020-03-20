#include "RuntimeReflection/Parser.h"

#include "RuntimeDatabase/RuntimeDatabase.h"

namespace SteelEngine {

    void Parser::ProcessMetaData(std::vector<MetaData>& meta)
    {
        Type::uint32 braces = 0;
        std::string word;
        MetaData metaData;
        bool wasEqual = false;

        while(1)
        {
            m_Lexer++;

            if(m_Lexer.GetToken() == "(")
            {
                if(braces != 0)
                {
                    word += m_Lexer.GetToken();

                    if(m_Lexer.Space())
                    {
                        word += " ";
                    }
                }

                braces++;
            }
            else if(m_Lexer.GetToken() == ")")
            {
                braces--;

                if(braces == 0)
                {
                    if(wasEqual && word != "")
                    {
                        metaData.m_Value = word;
                    }
                    else if(word != "")
                    {
                        metaData.m_Key = word;
                        metaData.m_Value = "true";
                    }
                    else
                    {
                        break;
                    }

                    word.clear();
                    meta.push_back(metaData);

                    break;
                }
                else
                {
                    word += m_Lexer.GetToken();

                    if(m_Lexer.Space())
                    {
                        word += " ";
                    }
                }
            }
            else if(m_Lexer.GetToken() == ",")
            {
                if(braces > 1)
                {
                    word += m_Lexer.GetToken();

                    if(m_Lexer.Space())
                    {
                        word += " ";
                    }
                }
                else
                {
                    if(wasEqual)
                    {
                        metaData.m_Value = word;
                    }
                    else
                    {
                        metaData.m_Key = word;
                        metaData.m_Value = "true";
                    }

                    wasEqual = false;

                    word.clear();
                    meta.push_back(metaData);
                }
            }
            else if(m_Lexer.GetToken() == "=")
            {
                metaData.m_Key = word;
                wasEqual = true;

                word.clear();
            }
            else
            {
                word += m_Lexer.GetToken();

                if(m_Lexer.Space())
                {
                    word += " ";
                }
            }
        }
    }

    void Parser::ProcessArguments(std::vector<Argument>& args)
    {
        Type::uint32 brackets = 0;
        std::string word;

        while(1)
        {
            if(m_Lexer.GetToken() == "(")
            {
                if(brackets > 0)
                {
                    word += m_Lexer.GetToken();
                }

                brackets++;
            }
            else if(m_Lexer.GetToken() == ")")
            {
                brackets--;

                if(brackets == 0)
                {
                    while(word[word.size() - 1] == ' ')
                    {
                        word.erase(word.size() - 1);
                    }

                    size_t found = word.find_last_of(" ");

                    if(found != RuntimeDatabase::s_InvalidID)
                    {
                        args.push_back(Argument{ word.substr(0, found), word.substr(found + 1) });
                        word.clear();
                    }

                    break;
                }
                else
                {
                    word += m_Lexer.GetToken();
                }
            }
            else if(m_Lexer.GetToken() == ",")
            {
                if(brackets > 1)
                {
                    word += m_Lexer.GetToken();
                }
                else
                {
                    size_t found = word.find_last_of(" ");

                    if(found != RuntimeDatabase::s_InvalidID)
                    {
                        args.push_back(Argument{ word.substr(0, found), word.substr(found + 1) });
                        word.clear();
                    }
                }
            }
            else if(m_Lexer.GetToken() == "=")
            {
                Type::uint32 braces = 1;

                while(1)
                {
                    m_Lexer++;

                    if(m_Lexer.GetToken() == "(")
                    {
                        braces++;
                    }
                    else if(m_Lexer.GetToken() == ")")
                    {
                        braces--;

                        if(braces == 0)
                        {
                            m_Lexer.SaveToken(m_Lexer.GetToken());

                            break;
                        }
                    }
                }
            }
            else
            {
                word += m_Lexer.GetToken();

                if(m_Lexer.Space())
                {
                    word += " ";
                }
            }

            m_Lexer++;
        }
    }

    Parser::Parser()
    {
        m_CurrentWorkingScope = 0;
        m_CurrentScopeToAddByMeta = 0;
        m_CurrentProtectionLevel = ProtectionLevel::NONE;
    }

    Parser::~Parser()
    {

    }

    Result Parser::Load(const std::vector<std::string>& lines)
    {
        m_Lexer.Load(lines);

        return SE_TRUE;
    }

    Result Parser::Load(const std::filesystem::path& filename)
    {
        return SE_TRUE;
    }

    Result Parser::Parse()
    {
        std::string word;

        while(!m_Lexer.End())
        {
            m_Lexer++;

            if(m_Lexer.GetToken() == "#pragma")
            {
                m_Lexer++;
            }
            else if(m_Lexer.GetToken() == "#include")
            {
                m_Lexer++;
            }
            else if(m_Lexer.GetToken() == "#define")
            {
                m_Lexer.SkipLine();
            }
            else if(m_Lexer.GetToken() == "{")
            {
                if(m_Scopes.empty())
                {
                    m_Scopes.push(m_CurrentScopeToAdd);
                    m_Structure.push_back(m_CurrentScopeToAdd);

                    m_CurrentWorkingScope = m_CurrentScopeToAdd;
                }
                else
                {
                    if(!m_CurrentWorkingScope)
                    {
                        m_CurrentWorkingScope = m_Scopes.top();
                    }

                    m_CurrentScopeToAdd->m_Parent = m_CurrentWorkingScope;
                    m_CurrentScopeToAdd->m_Protection = m_CurrentProtectionLevel;

                    m_CurrentWorkingScope->m_Scopes.push(m_CurrentScopeToAdd);
                    m_CurrentWorkingScope->m_Structure.push_back(m_CurrentScopeToAdd);

                    m_CurrentWorkingScope = m_CurrentScopeToAdd;
                    m_CurrentScopeToAdd = 0;
                }
            }
            else if(m_Lexer.GetToken() == "}")
            {
                m_CurrentWorkingScope = m_CurrentWorkingScope->m_Parent;

                if(m_CurrentWorkingScope)
                {
                    m_CurrentProtectionLevel = m_CurrentWorkingScope->m_Protection;
                }
            }
            else if(m_Lexer.GetToken() == "class" || m_Lexer.GetToken() == "struct")
            {
                if(m_Lexer.GetToken() == "class")
                {
                    if(!m_CurrentScopeToAddByMeta)
                    {
                        m_CurrentScopeToAdd = new ClassScope(m_Lexer++.GetToken());
                    }
                    else
                    {
                        m_CurrentScopeToAdd = m_CurrentScopeToAddByMeta;
                        m_CurrentScopeToAdd->m_Name = m_Lexer++.GetToken();
                        m_CurrentScopeToAddByMeta = 0;
                    }
                }
                else
                {
                    if(!m_CurrentScopeToAddByMeta)
                    {
                        m_CurrentScopeToAdd = new StructScope(m_Lexer++.GetToken());
                    }
                    else
                    {
                        m_CurrentScopeToAdd = m_CurrentScopeToAddByMeta;
                        m_CurrentScopeToAdd->m_Name = m_Lexer++.GetToken();
                        m_CurrentScopeToAddByMeta = 0;
                    }
                }

                if(m_Lexer++.GetToken() == ":")
                {
                    std::string word;
                    ProtectionLevel protection;
                    StructScope* str = (StructScope*)m_CurrentScopeToAdd;

                    while(1)
                    {
                        m_Lexer++;

                        if(m_Lexer.GetToken() == "virtual")
                        {
                            m_Lexer++;
                        }

                        if(m_Lexer.GetToken() == "public")
                        {
                            protection = ProtectionLevel::PUBLIC;
                        }
                        else if(m_Lexer.GetToken() == "protected")
                        {
                            protection = ProtectionLevel::PROTECTED;
                        }
                        else if(m_Lexer.GetToken() == "private")
                        {
                            protection = ProtectionLevel::PRIVATE;
                        }
                        else if(m_Lexer.GetToken() == "SE_INHERITANCE")
                        {
                            m_CurrentScopeToAddByMeta = new InheritanceScope();

                            m_CurrentScopeToAddByMeta->m_IsReflectionLabelSet = true;

                            ProcessMetaData(m_CurrentScopeToAddByMeta->m_MetaData);
                        }
                        else if(m_Lexer.GetToken() == ",")
                        {
                            if(!m_CurrentScopeToAddByMeta)
                            {
                                m_CurrentScopeToAdd = new InheritanceScope(word);
                            }
                            else
                            {
                                m_CurrentScopeToAdd = m_CurrentScopeToAddByMeta;
                                m_CurrentScopeToAdd->m_Name = m_Lexer++.GetToken();
                                m_CurrentScopeToAddByMeta = 0;
                            }

                            InheritanceScope* inherit = (InheritanceScope*)m_CurrentScopeToAdd;

                            inherit->m_Protection = protection;

                            str->m_Inheritance.push_back(inherit);

                            word.clear();
                        }
                        else if(m_Lexer.GetToken() == "{")
                        {
                            if(!m_CurrentScopeToAddByMeta)
                            {
                                m_CurrentScopeToAdd = new InheritanceScope(word);
                            }
                            else
                            {
                                m_CurrentScopeToAdd = m_CurrentScopeToAddByMeta;
                                m_CurrentScopeToAdd->m_Name = m_Lexer++.GetToken();
                                m_CurrentScopeToAddByMeta = 0;
                            }

                            InheritanceScope* inherit = (InheritanceScope*)m_CurrentScopeToAdd;

                            inherit->m_Protection = protection;

                            str->m_Inheritance.push_back(inherit);

                            word.clear();

                            m_Lexer.SaveToken(m_Lexer.GetToken());

                            break;
                        }
                        else
                        {
                            word += m_Lexer.GetToken();
                        }
                    }

                    m_CurrentScopeToAdd = str;
                }
                else
                {
                    m_Lexer.SaveToken(m_Lexer.GetToken());
                }
            }
            else if(m_Lexer.GetToken() == "namespace")
            {
                if(!m_CurrentScopeToAddByMeta)
                {
                    m_CurrentScopeToAdd = new NamespaceScope(m_Lexer++.GetToken());
                }
                else
                {
                    m_CurrentScopeToAdd = m_CurrentScopeToAddByMeta;
                    m_CurrentScopeToAdd->m_Name = m_Lexer++.GetToken();
                    m_CurrentScopeToAddByMeta = 0;
                }
            }
            else if(m_Lexer.GetToken() == "enum")
            {
                if(!m_CurrentScopeToAddByMeta)
                {
                    m_CurrentScopeToAdd = new EnumScope(m_Lexer++.GetToken());
                }
                else
                {
                    m_CurrentScopeToAdd = m_CurrentScopeToAddByMeta;
                    m_CurrentScopeToAdd->m_Name = m_Lexer++.GetToken();
                    m_CurrentScopeToAddByMeta = 0;
                }

                EnumScope* eS = (EnumScope*)m_CurrentScopeToAdd;
                StructScope* structScope = (StructScope*)m_CurrentWorkingScope;

                bool waitForCloseBrace = false;
                std::string word;

                while(1)
                {
                    m_Lexer++;

                    if(m_Lexer.GetToken() == ";" && !waitForCloseBrace)
                    {
                        break;
                    }
                    else if(m_Lexer.GetToken() == "{")
                    {
                        waitForCloseBrace = true;
                    }
                    else if(m_Lexer.GetToken() == "}" && waitForCloseBrace)
                    {
                        if(!m_CurrentScopeToAddByMeta)
                        {
                            m_CurrentScopeToAdd = new EnumElementScope(word);
                        }
                        else
                        {
                            m_CurrentScopeToAdd = m_CurrentScopeToAddByMeta;
                            m_CurrentScopeToAdd->m_Name = word;
                            m_CurrentScopeToAddByMeta = 0;
                        }

                        EnumElementScope* ele = (EnumElementScope*)m_CurrentScopeToAdd;

                        ele->m_Parent = eS;

                        eS->m_Elements.push_back(ele);

                        word.clear();

                        break;
                    }
                    else if(m_Lexer.GetToken() == "SE_ELEMENT")
                    {
                        m_CurrentScopeToAddByMeta = new EnumElementScope();

                        m_CurrentScopeToAddByMeta->m_IsReflectionLabelSet = true;

                        ProcessMetaData(m_CurrentScopeToAddByMeta->m_MetaData);
                    }
                    else if(m_Lexer.GetToken() == ",")
                    {
                        if(!m_CurrentScopeToAddByMeta)
                        {
                            m_CurrentScopeToAdd = new EnumElementScope(word);
                        }
                        else
                        {
                            m_CurrentScopeToAdd = m_CurrentScopeToAddByMeta;
                            m_CurrentScopeToAdd->m_Name = word;
                            m_CurrentScopeToAddByMeta = 0;
                        }

                        EnumElementScope* ele = (EnumElementScope*)m_CurrentScopeToAdd;

                        ele->m_Parent = eS;

                        eS->m_Elements.push_back(ele);

                        word.clear();
                    }
                    else
                    {
                        word += m_Lexer.GetToken();
                    }
                }

                eS->m_Parent = structScope;

                structScope->m_Enums.push_back(eS);

                m_CurrentWorkingScope->m_Scopes.push(eS);
                m_CurrentWorkingScope->m_Structure.push_back(eS);
            }
            else if(m_Lexer.GetToken() == "public")
            {
                m_Lexer++; // :

                if(m_CurrentWorkingScope)
                {
                    m_CurrentWorkingScope->m_Protection = ProtectionLevel::PUBLIC;
                }

                m_CurrentProtectionLevel = ProtectionLevel::PUBLIC;
            }
            else if(m_Lexer.GetToken() == "protected")
            {
                m_Lexer++; // :

                if(m_CurrentWorkingScope)
                {
                    m_CurrentWorkingScope->m_Protection = ProtectionLevel::PROTECTED;
                }

                m_CurrentProtectionLevel = ProtectionLevel::PROTECTED;
            }
            else if(m_Lexer.GetToken() == "private")
            {
                m_Lexer++; // :

                if(m_CurrentWorkingScope)
                {
                    m_CurrentWorkingScope->m_Protection = ProtectionLevel::PRIVATE;
                }

                m_CurrentProtectionLevel = ProtectionLevel::PRIVATE;
            }
            else if(m_Lexer.GetToken() == "SE_CLASS")
            {
                m_CurrentScopeToAddByMeta = new ClassScope();

                m_CurrentScopeToAddByMeta->m_IsReflectionLabelSet = true;

                ProcessMetaData(m_CurrentScopeToAddByMeta->m_MetaData);
            }
            else if(m_Lexer.GetToken() == "SE_STRUCT")
            {
                m_CurrentScopeToAddByMeta = new StructScope();

                m_CurrentScopeToAddByMeta->m_IsReflectionLabelSet = true;

                ProcessMetaData(m_CurrentScopeToAddByMeta->m_MetaData);
            }
            else if(m_Lexer.GetToken() == "SE_ENUM")
            {
                m_CurrentScopeToAddByMeta = new EnumScope();

                m_CurrentScopeToAddByMeta->m_IsReflectionLabelSet = true;

                ProcessMetaData(m_CurrentScopeToAddByMeta->m_MetaData);
            }
            else if(m_Lexer.GetToken() == "SE_VALUE")
            {
                m_CurrentScopeToAddByMeta = new PropertyScope();

                m_CurrentScopeToAddByMeta->m_IsReflectionLabelSet = true;

                ProcessMetaData(m_CurrentScopeToAddByMeta->m_MetaData);
            }
            else if(m_Lexer.GetToken() == "SE_METHOD")
            {
                m_CurrentScopeToAddByMeta = new FunctionScope();

                m_CurrentScopeToAddByMeta->m_IsReflectionLabelSet = true;

                ProcessMetaData(m_CurrentScopeToAddByMeta->m_MetaData);
            }
            else if(m_Lexer.GetToken() == "SE_NAMESPACE")
            {
                m_CurrentScopeToAddByMeta = new NamespaceScope();

                m_CurrentScopeToAddByMeta->m_IsReflectionLabelSet = true;

                ProcessMetaData(m_CurrentScopeToAddByMeta->m_MetaData);
            }
            else if(m_Lexer.GetToken() == "GENERATED_BODY")
            {

            }
            else if(m_Lexer.GetToken() == "override")
            {
                // m_Lexer++;
            }
            else if(m_Lexer.GetToken() == "using" || m_Lexer.GetToken() == "friend" || m_Lexer.GetToken() == "typedef")
            {
                while(m_Lexer++.GetToken() != ";")
                {

                }
            }
            else if(m_Lexer.GetToken() == "template")
            {
                Type::uint32 braces = 0;

                while(1)
                {
                    m_Lexer++;

                    if(m_Lexer.GetToken() == "<")
                    {
                        braces++;
                    }
                    else if(m_Lexer.GetToken() == ">")
                    {
                        braces--;

                        if(braces == 0)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                if(m_Lexer.GetToken() == "[")
                {
                    while(m_Lexer.GetToken() != "]")
                    {
                        m_Lexer++;
                    }

                    m_Lexer++;
                }

                if(m_Lexer.GetToken() == "=")
                {
                    while(m_Lexer.GetToken() != ";")
                    {
                        m_Lexer++;
                    }
                }

                if(m_Lexer.GetToken() == "static")
                {
                    m_Lexer++;
                }

                if(m_Lexer.GetToken() == "inline")
                {
                    m_Lexer++;
                }

                if(m_Lexer.GetToken() == "virtual")
                {
                    m_Lexer++;
                }

                std::string saved;
                bool success = false;

                if(!m_Scopes.empty())
                {
                    // Constructor
                    if(m_CurrentWorkingScope && m_Lexer.GetToken() == m_CurrentWorkingScope->m_Name)
                    {
                        std::string token = m_Lexer.GetToken();

                        if(m_Lexer++.GetToken() == "(")
                        {
                            success = true;

                            if(!m_CurrentScopeToAddByMeta)
                            {
                                m_CurrentScopeToAdd = new ConstructorScope(m_CurrentWorkingScope->m_Name);
                            }
                            else
                            {
                                m_CurrentScopeToAdd = m_CurrentScopeToAddByMeta;
                                m_CurrentScopeToAdd->m_Name = m_CurrentWorkingScope->m_Name;
                                m_CurrentScopeToAddByMeta = 0;
                            }

                            ConstructorScope* cons = (ConstructorScope*)m_CurrentScopeToAdd;

                            ProcessArguments(cons->m_Arguments);

                            Type::uint32 braces = 0;

                            while(1)
                            {
                                m_Lexer++;

                                if(m_Lexer.GetToken() == ";" && braces == 0)
                                {
                                    break;
                                }
                                else if(m_Lexer.GetToken() == "{")
                                {
                                    braces++;
                                }
                                else if(m_Lexer.GetToken() == "}")
                                {
                                    braces--;

                                    if(braces == 0)
                                    {
                                        break;
                                    }
                                }
                            }

                            StructScope* structScope = (StructScope*)m_CurrentWorkingScope;

                            cons->m_Parent = structScope;

                            structScope->m_Constructors.push_back(cons);
                        }
                        else
                        {
                            saved = token;
                        }
                    }
                    else if(m_CurrentWorkingScope && m_Lexer.GetToken() == "~" + m_CurrentWorkingScope->m_Name)
                    {
                        std::string token = m_Lexer.GetToken();

                        if(m_Lexer++.GetToken() == "(")
                        {
                            success = true;
                            m_Lexer++;

                            bool waitForCloseBrace = false;

                            while(1)
                            {
                                m_Lexer++;

                                if(m_Lexer.GetToken() == ";" && !waitForCloseBrace)
                                {
                                    break;
                                }
                                else if(m_Lexer.GetToken() == "{")
                                {
                                    waitForCloseBrace = true;
                                }
                                else if(m_Lexer.GetToken() == "}" && waitForCloseBrace)
                                {
                                    break;
                                }
                            }
                        }
                        else
                        {
                            saved = token;
                        }
                    }
                }

                if(!success && m_CurrentWorkingScope)
                {
                    word += saved;
                    word += m_Lexer.GetToken();

                    if(m_Lexer.Space())
                    {
                        word += " ";
                    }

                    Lexer lexer;
                    std::vector<std::string> line;
                    Type::uint32 braces = 0;
                    bool wasBracket = false;

                    line.push_back(word);
                    lexer.Load(line);

                    while(!lexer.End())
                    {
                        lexer++;

                        if(lexer.GetToken() == "<")
                        {
                            braces++;

                            wasBracket = true;
                        }
                        else if(lexer.GetToken() == ">")
                        {
                            braces--;

                            if(braces == 0)
                            {
                                wasBracket = false;
                            }
                        }
                    }

                    if(m_Lexer.GetToken() == "(" && word[0] != ':' && !wasBracket)
                    {
                        size_t found = word.find_last_of(" ");

                        if(found != RuntimeDatabase::s_InvalidID)
                        {
                            if(!m_CurrentScopeToAddByMeta)
                            {
                                m_CurrentScopeToAdd = new FunctionScope(word.substr(found + 1, word.size() - 2 - found));
                            }
                            else
                            {
                                m_CurrentScopeToAdd = m_CurrentScopeToAddByMeta;
                                m_CurrentScopeToAdd->m_Name = word.substr(found + 1, word.size() - 2 - found);
                                m_CurrentScopeToAddByMeta = 0;
                            }
                        }

                        FunctionScope* func = (FunctionScope*)m_CurrentScopeToAdd;

                        func->m_ReturnType = word.substr(0, found);
                        func->m_Protection = m_CurrentProtectionLevel;
                        func->m_Parent = m_CurrentWorkingScope;

                        ProcessArguments(func->m_Arguments);

                        Type::uint32 braces = 0;

                        while(1)
                        {
                            m_Lexer++;

                            if(m_Lexer.GetToken() == ";" && braces == 0)
                            {
                                break;
                            }
                            else if(m_Lexer.GetToken() == "{")
                            {
                                braces++;
                            }
                            else if(m_Lexer.GetToken() == "}")
                            {
                                braces--;

                                if(braces == 0)
                                {
                                    break;
                                }
                            }
                        }

                        if(m_CurrentWorkingScope->m_ScopeType == ScopeType::CLASS || m_CurrentWorkingScope->m_ScopeType == ScopeType::STRUCT)
                        {
                            StructScope* structScope = (StructScope*)m_CurrentWorkingScope;

                            structScope->m_Functions.push_back(func);
                        }
                        else if(m_CurrentWorkingScope && m_CurrentWorkingScope->m_ScopeType == ScopeType::NAMESPACE)
                        {
                            m_CurrentWorkingScope->m_Scopes.push(func);
                            m_CurrentWorkingScope->m_Structure.push_back(func);
                        }

                        word.clear();
                    }
                    else if(m_Lexer.GetToken() == ";")
                    {
                        while(word[word.size() - 2] == ' ')
                        {
                            word.erase(word.size() - 2, 1);
                        }

                        Lexer lexer;
                        bool wasConst = false;
                        std::vector<std::string> lines;
                        Type::uint32 braces = 0;

                        wasBracket = false;

                        lines.push_back(word);
                        lexer.Load(lines);
                        word.clear();

                        while(!lexer.End())
                        {
                            lexer++;

                            if(lexer.GetToken() == "<")
                            {
                                braces++;

                                wasBracket = true;
                            }
                            else if(lexer.GetToken() == ">")
                            {
                                braces--;

                                if(braces == 0)
                                {
                                    wasBracket = false;
                                }
                            }

                            if(lexer.GetToken() == "const" && !wasBracket)
                            {
                                wasConst = true;

                                break;
                            }
                            else
                            {
                                word += lexer.GetToken();

                                if(lexer.Space())
                                {
                                    word += " ";
                                }
                            }
                        }

                        if(wasConst)
                        {
                            continue;
                        }

                        size_t found = word.find_last_of(" ");

                        if(found != RuntimeDatabase::s_InvalidID)
                        {
                            StructScope* structScope = (StructScope*)m_CurrentWorkingScope;

                            if(!m_CurrentScopeToAddByMeta)
                            {
                                m_CurrentScopeToAdd = new PropertyScope(word.substr(found + 1, word.size() - 2 - found));
                            }
                            else
                            {
                                m_CurrentScopeToAdd = m_CurrentScopeToAddByMeta;
                                m_CurrentScopeToAdd->m_Name = word.substr(found + 1, word.size() - 2 - found);
                                m_CurrentScopeToAddByMeta = 0;
                            }

                            PropertyScope* prop = (PropertyScope*)m_CurrentScopeToAdd;

                            prop->m_Type = word.substr(0, found);
                            prop->m_Parent = m_CurrentWorkingScope;
                            prop->m_Protection = m_CurrentProtectionLevel;

                            structScope->m_Properties.push_back(prop);
                        }

                        word.clear();
                    }
                }
            }
        }

        return SE_TRUE;
    }

    void Parser::Clear()
    {
        for(Type::uint32 i = 0; i < m_Structure.size(); i++)
        {
            m_Structure[i]->Clear();

            delete m_Structure[i];
            m_Structure[i] = 0;
        }

        m_Structure.clear();

        while(!m_Scopes.empty())
        {
            m_Scopes.pop();
        }
    }

}