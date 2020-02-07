#include "RuntimeReflection/ReflectionGenerator.h"

#include "Core/IReflectionModule.h"

#include "fstream"
#include "queue"

namespace SteelEngine {

    void checkStructOrClassParents(ReflectionGenerator::ScopeInfo* info, std::stack<std::string>& parents)
    {
        if(!info)
        {
            return;
        }

        if(info->m_Parent->m_ScopeType == ReflectionGenerator::ScopeType::CLASS || info->m_Parent->m_ScopeType == ReflectionGenerator::ScopeType::STRUCT)
        {
            parents.push(info->m_Parent->m_Name);

            checkStructOrClassParents(info->m_Parent, parents);
        }
    }

    void searchFor(const ReflectionGenerator::ScopeInfo* info, const std::string& name, const ReflectionGenerator::ScopeInfo** res)
    {
        if(*res)
        {
            return;
        }

        for(auto it = info->m_Structure.begin(); it != info->m_Structure.end(); ++it)
        {
            if((*it)->m_Name == name)
            {
                *res = *it;

                break;
            }
            else
            {
                searchFor((*it), name, res);
            }
        }
    }

    void searchFor(const ReflectionGenerator::ScopeInfo* info, std::stack<std::string>& names, const ReflectionGenerator::ScopeInfo** res)
    {
        if(names.empty())
        {
            return;
        }

        for(auto it = info->m_Structure.begin(); it != info->m_Structure.end(); ++it)
        {
            if(names.empty())
            {
                return;
            }

            std::string name = names.top();

            if((*it)->m_Name == name)
            {
                names.pop();

                *res = *it;

                searchFor(*it, names, res);
            }
            else
            {
                searchFor(*it, names, res);
            }
        }
    }

    void searchForReverse(const ReflectionGenerator::ScopeInfo* startPoint, std::stack<std::string>& names, const ReflectionGenerator::ScopeInfo** res)
    {
        if(!startPoint)
        {
            return;
        }

        for(auto it = startPoint->m_Structure.begin(); it != startPoint->m_Structure.end(); ++it)
        {
            if(names.empty())
            {
                return;
            }

            std::string name = names.top();

            if((*it)->m_Name == name)
            {
                names.pop();

                *res = *it;

                searchForReverse(*it, names, res);
            }
        }

        searchForReverse(startPoint->m_Parent, names, res);
    }

    void constructNamespace(const ReflectionGenerator::ScopeInfo* startPoint, std::stack<std::string>& res)
    {
        if(startPoint->m_Parent->m_ScopeType == ReflectionGenerator::ScopeType::CLASS || startPoint->m_Parent->m_ScopeType == ReflectionGenerator::ScopeType::STRUCT)
        {
            res.push(startPoint->m_Parent->m_Name);

            constructNamespace(startPoint->m_Parent, res);
        }
    }

    ReflectionGenerator::ScopeInfo* getLastStructOrClass(ReflectionGenerator::ScopeInfo* startPoint)
    {
        if(startPoint->m_Parent->m_ScopeType == ReflectionGenerator::ScopeType::CLASS || startPoint->m_Parent->m_ScopeType == ReflectionGenerator::ScopeType::STRUCT)
        {
            return getLastStructOrClass(startPoint->m_Parent);
        }
        else
        {
            return startPoint;
        }
    }

    void ReflectionGenerator::ProcessArguments(std::vector<Argument>& args)
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

    void ReflectionGenerator::ProcessMetaData(std::vector<MetaData>& meta)
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

    std::string ReflectionGenerator::WriteArguments(const std::vector<ReflectionGenerator::Argument>& args, ReflectionGenerator::ScopeInfo* info)
    {
        std::string processedArgs;

        for(Type::uint32 i = 0; i < args.size(); i++)
        {
            ReflectionGenerator::Argument arg = args[i];

            std::vector<std::string> splitted = split(arg.m_Type, ' ');

            for(Type::uint32 j = 0; j < splitted.size(); j++)
            {
                if(splitted[j] != "const")
                {
                    size_t found = RuntimeDatabase::s_InvalidID;
                    std::string res = splitted[j];

                    while((found = res.find('*')) != RuntimeDatabase::s_InvalidID || (found = res.find('&')) != RuntimeDatabase::s_InvalidID)
                    {
                        res.erase(found);
                    }

                    std::string res2 = res;

                    replaceAll(res2, "::", " ");

                    std::vector<std::string> splitted2 = split(res2, ' ');
                    std::stack<std::string> names;

                    for(auto it = splitted2.rbegin(); it != splitted2.rend(); ++it)
                    {
                        names.push(*it);
                    }

                    const ReflectionGenerator::ScopeInfo* foundScope = 0;

                    searchForReverse(info, names, &foundScope);

                    if(foundScope)
                    {
                        std::stack<std::string> constructedNamespace;

                        constructNamespace(foundScope, constructedNamespace);

                        while(!constructedNamespace.empty())
                        {
                            processedArgs.append(constructedNamespace.top()).append("::");

                            constructedNamespace.pop();
                        }

                        processedArgs.append(splitted2[splitted2.size() - 1]);
                    }
                    else
                    {
                        processedArgs.append(splitted[j]);
                    }
                }
                else
                {
                    processedArgs.append(splitted[j]).append(" ");
                }
            }

            if(i < args.size() - 1)
            {
                processedArgs.append(", ");
            }
        }

        return processedArgs;
    }

    std::string ReflectionGenerator::WriteMetaData(const ScopeInfo* info)
    {
        std::string result;

        if(!info->m_MetaData.empty())
        {
            result.append("(\n");

            for(Type::uint32 i = 0; i < info->m_MetaData.size(); i++)
            {
                MetaData v = info->m_MetaData[i];

                result.append("SteelEngine::Reflection::MetaData(").append(v.m_Key).append(", ").append(v.m_Value).append(")");

                if(i < info->m_MetaData.size() - 1)
                {
                    result.append(",\n");
                }
                else
                {
                    result.append("\n");
                }
            }

            result.append(")\n");
        }

        return result;
    }

    void ReflectionGenerator::Process(ScopeInfo* info, std::ofstream& file)
    {
        if(!info)
        {
            return;
        }

        static std::vector<std::string> namespaces;
        static std::stack<std::string> parents;
        static std::queue<std::string> parents2;

        if(info->m_ScopeType == ScopeType::NAMESPACE)
        {
            file << "namespace " << info->m_Name << " {\n";

            namespaces.push_back(info->m_Name);
        }
        else if(info->m_ScopeType == ScopeType::STRUCT || info->m_ScopeType == ScopeType::CLASS)
        {
            if(!m_BeginRecordingOnce)
            {
                if(info->m_IsReflectionLabelSet)
                {
                    file << "REGISTER_REFLECTION\n";
                    file << "{\n";

                    m_BeginRecordingOnce = true;
                }
            }

            StructScope* str = (StructScope*)info;

            IReflectionData const* const* types = Reflection::GetTypes();

            for(Type::uint32 i = 0; i < Reflection::GetTypesSize(); i++)
            {
                const IReflectionData* type = types[i];

                if(type->GetMetaData(Reflection::ReflectionAttribute::REFLECTION_MODULE)->Convert<bool>())
                {
                    str->m_ReflectionModules.push_back((IReflectionModule*)type->Create());
                }
            }

            if(str->m_IsReflectionLabelSet)
            {
                if(str->m_IsReflectionLabelSet)
                {
                    for(IReflectionModule* module : str->m_ReflectionModules)
                    {
                        module->ProcessStructure(str);
                    }
                }

                checkStructOrClassParents(str, parents);

                str->m_MetaData.push_back(MetaData{ "\"sizeof\"", "sizeof(" + str->m_Name + ")" });

                file << "SteelEngine::ReflectionRecorder::Register<";

                std::string parentsStr;

                while(!parents.empty())
                {
                    parentsStr += parents.top() + "::";
                    parents2.push(parents.top());

                    parents.pop();
                }

                file << parentsStr << str->m_Name << ">(\"";
                file << str->m_Name << "\"";

                if(!namespaces.empty())
                {
                    file << ",";
                    file << "{\n";

                    for(Type::uint32 i = 0; i < namespaces.size(); i++)
                    {
                        file << "\"" << namespaces[i] << "\"";

                        if(i < namespaces.size() - 1 || !parents2.empty())
                        {
                            file << ",\n";
                        }
                        else
                        {
                            file << "\n";
                        }
                    }

                    size_t s = parents2.size();
                    Type::uint32 i = 0;

                    while(!parents2.empty())
                    {
                        file << "\"" << parents2.front() << "\"";

                        parents2.pop();

                        if(i < s - 1)
                        {
                            file << ",\n";
                        }
                        else
                        {
                            file << "\n";
                        }
                    }

                    file << "}\n";
                }

                file << ")\n";
                file << WriteMetaData(str);

                for(ConstructorScope* v : str->m_Constructors)
                {
                    file << ".Constructor<";
                    file << WriteArguments(v->m_Arguments, str);
                    file << ">()\n";
                    file << WriteMetaData(v);
                }

                for(InheritanceScope* v : str->m_Inheritance)
                {
                    file << ".Inheritance<" << v->m_Name << ">(\"" << v->m_Name << "\")\n";
                }

                for(PropertyScope* v : str->m_Properties)
                {
                    for(IReflectionModule* module : str->m_ReflectionModules)
                    {
                        module->ProcessProperty(v);
                    }

                    if((v->m_Protection == ProtectionLevel::PUBLIC || (v->m_ScopeType == ScopeType::STRUCT && v->m_Protection == ProtectionLevel::NONE)) && v->m_IsReflectionLabelSet)
                    {
                        file << ".Property(\"" << v->m_Name << "\", &" << parentsStr << str->m_Name << "::" << v->m_Name << ")\n";
                        file << WriteMetaData(v);
                    }
                }

                for(FunctionScope* v : str->m_Functions)
                {
                    if((v->m_Protection == ProtectionLevel::PUBLIC || (v->m_ScopeType == ScopeType::STRUCT && v->m_Protection == ProtectionLevel::NONE)) && v->m_IsReflectionLabelSet)
                    {
                        file << ".Method(\"" << v->m_Name << "\", &" << parentsStr << str->m_Name << "::" << v->m_Name << ")\n";
                        file << WriteMetaData(v);
                    }
                }

                for(EnumScope* v : str->m_Enums)
                {
                    if(v->m_IsReflectionLabelSet)
                    {
                        file << ".Enum<" << parentsStr << str->m_Name << "::" << v->m_Name << ">(\"" << v->m_Name << "\")\n";
                        file << ".Values({\n";

                        for(Type::uint32 i = 0; i < v->m_Elements.size(); i++)
                        {
                            EnumElementScope* e = v->m_Elements[i];

                            file << "SteelEngine::ReflectionEnumElement(\"" << e->m_Name << "\", " << parentsStr << str->m_Name << "::" << v->m_Name << "::" << e->m_Name << ")";
                            file << WriteMetaData(e);

                            if(i < v->m_Elements.size() - 1)
                            {
                                file << ",\n";
                            }
                            else
                            {
                                file << "\n";
                            }
                        }

                        file << "})\n";
                        file << WriteMetaData(v);
                    }
                }

                file << ";\n";
            }
        }

        while(!info->m_Scopes.empty())
        {
            Process(info->m_Scopes.top(), file);

            info->m_Scopes.pop();
        }

        if(info->m_Parent && info->m_Parent->m_ScopeType == ScopeType::NAMESPACE)
        {
            if(!m_EndRecordingOnce)
            {
                if(info->m_IsReflectionLabelSet)
                {
                    file << "}\n";

                    if(info->m_Parent == 0 || info->m_Parent->m_ScopeType == ScopeType::NAMESPACE)
                    {
                        if(info->m_ScopeType == ScopeType::STRUCT || info->m_ScopeType == ScopeType::CLASS)
                        {
                            StructScope* str = (StructScope*)info;

                            for(IReflectionModule* module : str->m_ReflectionModules)
                            {
                                module->GenerateSource(file);
                            }

                            for(IReflectionModule* module : str->m_ReflectionModules)
                            {
                                module->GenerateHeader(m_GeneratedHeaderLinesByModules);
                            }

                            if(!str->m_Constructors.empty())
                            {
                                file << "#ifdef RUNTIME_COMPILE\n";
                                file << "extern \"C\" __declspec(dllexport) TypeInfo* allocateRuntimeObject(SteelEngine::RuntimeDatabase::ConstructedObjectsVector* typeInfo)\n";
                                file << "{\n";
                                {
                                    file << "DECLARE_TYPE_INFO(" << str->m_Name << ")\n";
                                    file << "{\n";
                                    {
                                        file << "FIND_THE_RIGHT_OBJECT\n";
                                        file << "\n";

                                        for(ConstructorScope* cons : str->m_Constructors)
                                        {
                                            if(cons->m_Arguments.size() > 0)
                                            {
                                                file << "COMPARE_CONSTRUCTOR(";

                                                for(Type::uint32 i = 0; i < cons->m_Arguments.size(); i++)
                                                {
                                                    file << cons->m_Arguments[i].m_Type;

                                                    if(i < cons->m_Arguments.size() - 1)
                                                    {
                                                        file << ", ";
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                file << "COMPARE_CONSTRUCTOR_(";
                                            }

                                            file << ")\n";
                                        }
                                    }
                                    file << "};\n";
                                    file << "\n";
                                    file << "return result;\n";
                                }
                                file << "}\n";
                                file << "#endif\n";
                            }
                        }
                    }

                    m_EndRecordingOnce = true;
                }
            }
        }

        if(info->m_ScopeType == ScopeType::NAMESPACE)
        {
            file << "}\n";

            namespaces.pop_back();
        }
    }

    ReflectionGenerator::ReflectionGenerator()
    {
        m_CurrentWorkingScope = 0;
        m_CurrentScopeToAddByMeta = 0;
        m_CurrentProtectionLevel = ProtectionLevel::NONE;
        m_BeginRecordingOnce = false;
        m_EndRecordingOnce = false;
    }

    ReflectionGenerator::~ReflectionGenerator()
    {

    }

    Result ReflectionGenerator::Load(const std::filesystem::path& filename)
    {
        std::ifstream file(filename);
        std::string line;

        while(std::getline(file, line))
        {
            m_LoadedLines.push_back(line);
        }

        if(file.is_open())
        {
            file.close();
        }

        m_Lexer.Load(m_LoadedLines);

        m_Filename = filename;

        return SE_TRUE;
    }

    Result ReflectionGenerator::Parse()
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
            else if(m_Lexer.GetToken() == "{")
            {
                if(m_Scopes.empty())
                {
                    m_Scopes.push(m_CurrentScopeToAdd);
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
                        else if(m_Lexer.GetToken() == ",")
                        {
                            InheritanceScope* inherit = new InheritanceScope();

                            inherit->m_Name = word;
                            inherit->m_Protection = protection;

                            str->m_Inheritance.push_back(inherit);

                            word.clear();
                        }
                        else if(m_Lexer.GetToken() == "{")
                        {
                            InheritanceScope* inherit = new InheritanceScope();

                            inherit->m_Name = word;
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

                    // while(m_Lexer.GetToken() != "{")
                    // {
                    //     InheritanceScope* inherit = new InheritanceScope();
                    //     std::string word = m_Lexer++.GetToken(); // public

                    //     if(word == "public")
                    //     {
                    //         inherit->m_Protection == ProtectionLevel::PUBLIC;
                    //     }
                    //     else if(word == "protected")
                    //     {
                    //         inherit->m_Protection == ProtectionLevel::PROTECTED;
                    //     }
                    //     else if(word == "private")
                    //     {
                    //         inherit->m_Protection == ProtectionLevel::PRIVATE;
                    //     }

                    //     inherit->m_Name = m_Lexer++.GetToken(); // name

                    //     StructScope* str = (StructScope*)m_CurrentScopeToAdd;

                    //     str->m_Inheritance.push_back(inherit);

                    //     m_Lexer++;
                    // }
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
                        break;
                    }
                    else if(m_Lexer.GetToken() == "SE_ELEMENT")
                    {
                        m_CurrentScopeToAddByMeta = new StructScope();

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
            else if(m_Lexer.GetToken() == "using" || m_Lexer.GetToken() == "friend")
            {
                while(m_Lexer++.GetToken() != ";")
                {
                    
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

                if(m_Lexer.GetToken() == "const")
                {
                    while(m_Lexer.GetToken() != ";")
                    {
                        m_Lexer++;
                    }

                    continue;
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

                if(!success)
                {
                    word += saved;
                    word += m_Lexer.GetToken();

                    if(m_Lexer.Space())
                    {
                        word += " ";
                    }

                    if(m_Lexer.GetToken() == "(" && word[0] != ':')
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
                        StructScope* structScope = (StructScope*)m_CurrentWorkingScope;

                        func->m_ReturnType = word.substr(0, found);
                        func->m_Protection = m_CurrentProtectionLevel;
                        func->m_Parent = m_CurrentWorkingScope;

                        ProcessArguments(func->m_Arguments);

                        structScope->m_Functions.push_back(func);

                        word.clear();
                    }
                    else if(m_Lexer.GetToken() == ";")
                    {
                        while(word[word.size() - 2] == ' ')
                        {
                            word.erase(word.size() - 2, 1);
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

            // m_Lexer++;
        }

        return SE_TRUE;
    }

    Result ReflectionGenerator::Generate(const std::filesystem::path& cwd, const std::filesystem::path& generatePath)
    {
        std::string p_ = m_Filename.string();

		replaceAll(p_, "\\", "/");

		std::vector<std::string> splitted_ = split(p_, '/');
		std::string finalPath = generatePath.string() + "/";
		std::string includePath = "";

        std::string p2 = cwd.string();

		replaceAll(p2, "\\", "/");

		for(Type::uint32 i = split(p2, '/').size() + 1; i < splitted_.size() - 1; i++)
		{
			finalPath.append(splitted_[i]).append("/");
			includePath.append(splitted_[i]).append("/");

			try
			{
				std::filesystem::create_directory(finalPath);
			}
			catch(const std::exception& e)
			{
				printf("%s\n", e.what());
			}
		}

        includePath.append(splitted_[splitted_.size() - 1]);

        std::string lol = "D:/Projects/C++/SteelEngine/Test";

        std::ofstream sourceFile(finalPath + m_Filename.stem().string() + ".Generated.cpp");

		sourceFile << "#include \"HotReloader/IRuntimeObject.h\"\n";
		sourceFile << "#include \"RuntimeReflection/ReflectionRecorder.h\"\n";
        sourceFile << "#include \"" << includePath << "\"\n";
        sourceFile << "#include \"" << finalPath << m_Filename.stem().string() << ".Generated.h" << "\"\n";
		sourceFile << "\n";

        while(!m_Scopes.empty())
        {
            ScopeInfo* scope = m_Scopes.top();

            Process(scope, sourceFile);

            m_Scopes.pop();
        }

        if(sourceFile.is_open())
        {
            sourceFile.close();
        }

        std::ofstream headerFile(finalPath + m_Filename.stem().string() + ".Generated.h");

        headerFile << "#include \"RuntimeReflection/ReflectionGeneratorMacros.h\"\n";
		headerFile << "\n";
		headerFile << "#define GENERATED_BODY";

        if(!m_GeneratedHeaderLinesByModules.empty())
        {
            headerFile << " \\\n";

            for(Type::uint32 i = 0; i < m_GeneratedHeaderLinesByModules.size(); i++)
            {
                headerFile << m_GeneratedHeaderLinesByModules[i];

                if(i < m_GeneratedHeaderLinesByModules.size() - 1)
                {
                    headerFile << "\\\n";
                }
                else
                {
                    headerFile << "\n";
                }
            }
        }

        if(headerFile.is_open())
        {
            headerFile.close();
        }

        return SE_TRUE;
    }

    void ReflectionGenerator::Clear()
    {
        m_GeneratedHeaderLinesByModules.clear();
        m_LoadedLines.clear();

        m_BeginRecordingOnce = false;
        m_EndRecordingOnce = false;
    }

}