#include "RuntimeReflection/ReflectionGenerator.h"

#include "Core/IReflectionModule.h"

#include "fstream"
#include "queue"

namespace SteelEngine {

    void checkStructOrClassParents(Parser::ScopeInfo* info, std::stack<std::string>& parents)
    {
        if(!info)
        {
            return;
        }

        if(info->m_Parent->m_ScopeType == Parser::ScopeType::CLASS || info->m_Parent->m_ScopeType == Parser::ScopeType::STRUCT)
        {
            parents.push(info->m_Parent->m_Name);

            checkStructOrClassParents(info->m_Parent, parents);
        }
    }

    void checkIfAnyStructOrClass(const Parser::ScopeInfo* info, bool& res)
    {
        if(res)
        {
            return;
        }

        for(auto it = info->m_Structure.begin(); it != info->m_Structure.end(); ++it)
        {
            if(((*it)->m_ScopeType == Parser::ScopeType::CLASS || (*it)->m_ScopeType == Parser::ScopeType::STRUCT) && (*it)->m_IsReflectionLabelSet)
            {
                res = true;

                break;
            }
            else
            {
                checkIfAnyStructOrClass((*it), res);
            }
        }
    }

    void searchForReverse(const Parser::ScopeInfo* startPoint, std::stack<std::string>& names, const Parser::ScopeInfo** res)
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

    void constructNamespace(const Parser::ScopeInfo* startPoint, std::stack<std::string>& res)
    {
        if(startPoint->m_Parent->m_ScopeType == Parser::ScopeType::CLASS || startPoint->m_Parent->m_ScopeType == Parser::ScopeType::STRUCT)
        {
            res.push(startPoint->m_Parent->m_Name);

            constructNamespace(startPoint->m_Parent, res);
        }
    }

    Parser::ScopeInfo* getLastStructOrClass(Parser::ScopeInfo* startPoint)
    {
        if(startPoint->m_Parent->m_ScopeType == Parser::ScopeType::CLASS || startPoint->m_Parent->m_ScopeType == Parser::ScopeType::STRUCT)
        {
            return getLastStructOrClass(startPoint->m_Parent);
        }
        else
        {
            return startPoint;
        }
    }

    std::string ReflectionGenerator::WriteArguments(const std::vector<Parser::Argument>& args, Parser::ScopeInfo* info)
    {
        std::string processedArgs;

        for(Type::uint32 i = 0; i < args.size(); i++)
        {
            Parser::Argument arg = args[i];

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

                    const Parser::ScopeInfo* foundScope = 0;

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

    std::string ReflectionGenerator::WriteMetaData(const Parser::ScopeInfo* info)
    {
        std::string result;

        if(!info->m_MetaData.empty())
        {
            result.append("(\n");

            for(Type::uint32 i = 0; i < info->m_MetaData.size(); i++)
            {
                Parser::MetaData v = info->m_MetaData[i];

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

    void ReflectionGenerator::Subprocess(Parser::ScopeInfo* info, std::ofstream& file)
    {
        if(!info)
        {
            return;
        }

        static std::vector<std::string> namespaces;
        static std::stack<std::string> parents;
        static std::queue<std::string> parents2;

        if(info->m_ScopeType == Parser::ScopeType::NAMESPACE)
        {
            file << "namespace " << info->m_Name << " {\n";

            namespaces.push_back(info->m_Name);
        }
        else if(info->m_ScopeType == Parser::ScopeType::STRUCT || info->m_ScopeType == Parser::ScopeType::CLASS)
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

            Parser::StructScope* str = (Parser::StructScope*)info;

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

                str->m_MetaData.push_back(Parser::MetaData{ "\"sizeof\"", "sizeof(" + str->m_Name + ")" });

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

                for(Parser::ConstructorScope* v : str->m_Constructors)
                {
                    file << ".RegisterConstructor<";
                    file << WriteArguments(v->m_Arguments, str);
                    file << ">()\n";
                    file << WriteMetaData(v);
                }

                for(Parser::InheritanceScope* v : str->m_Inheritance)
                {
                    file << ".RegisterInheritance<" << v->m_Name + v->m_TemplateName << ">(\"" << v->m_Name << "\")\n";
                }

                for(Parser::PropertyScope* v : str->m_Properties)
                {
                    for(IReflectionModule* module : str->m_ReflectionModules)
                    {
                        module->ProcessProperty(v);
                    }

                    if((v->m_Protection == Parser::ProtectionLevel::PUBLIC || (v->m_ScopeType == Parser::ScopeType::STRUCT && v->m_Protection == Parser::ProtectionLevel::NONE)) && v->m_IsReflectionLabelSet)
                    {
                        file << ".RegisterProperty(\"" << v->m_Name << "\", &" << parentsStr << str->m_Name << "::" << v->m_Name << ")\n";
                        file << WriteMetaData(v);
                    }
                }

                for(Parser::FunctionScope* v : str->m_Functions)
                {
                    bool found = false;

                    for(Parser::FunctionScope* v2 : str->m_Functions)
                    {
                        if(v != v2 && v->m_Name == v2->m_Name)
                        {
                            found = true;
                        }
                    }

                    if(!found)
                    {
                        if((v->m_Protection == Parser::ProtectionLevel::PUBLIC || (v->m_Parent->m_ScopeType == Parser::ScopeType::STRUCT && v->m_Protection == Parser::ProtectionLevel::NONE)) && v->m_IsReflectionLabelSet)
                        {
                            file << ".RegisterMethod(\"" << v->m_Name << "\", ";

                            if(!v->m_Arguments.empty())
                            {
                                file << "{ ";

                                for(Type::uint32 i = 0; i < v->m_Arguments.size(); i++)
                                {
                                    Parser::Argument arg = v->m_Arguments[i];
                                    const Parser::ScopeInfo* res = 0;
                                    std::stack<std::string> names;

                                    names.push(arg.m_Type);

                                    searchForReverse(info, names, &res);

                                    if(res)
                                    {
                                        constructNamespace(res, names);

                                        std::string constructed;

                                        while(!names.empty())
                                        {
                                            constructed += names.top() + "::";

                                            names.pop();
                                        }

                                        file << "SteelEngine::Reflection::FucntionArgument<" << constructed << arg.m_Type << ">(\"" << arg.m_Name << "\")";
                                    }
                                    else
                                    {
                                        file << "SteelEngine::Reflection::FucntionArgument<" << parentsStr << arg.m_Type << ">(\"" << arg.m_Name << "\")";
                                    }

                                    if(i < v->m_Arguments.size() - 1)
                                    {
                                        file << ",\n";
                                    }
                                }

                                file << " }, ";
                            }

                            file << "&" << parentsStr << str->m_Name << "::" << v->m_Name << ")\n";
                            file << WriteMetaData(v);
                        }
                    }
                    else
                    {
                        if((v->m_Protection == Parser::ProtectionLevel::PUBLIC || (v->m_ScopeType == Parser::ScopeType::STRUCT && v->m_Protection == Parser::ProtectionLevel::NONE)) && v->m_IsReflectionLabelSet)
                        {
                            file << ".RegisterMethod<";

                            if(!v->m_Arguments.empty())
                            {
                                file << v->m_ReturnType << ", ";
                            }
                            else
                            {
                                file << v->m_ReturnType;
                            }

                            for(Type::uint32 i = 0; i < v->m_Arguments.size(); i++)
                            {
                                file << v->m_Arguments[i].m_Type;

                                if(i < v->m_Arguments.size() - 1)
                                {
                                    file << ", ";
                                }
                            }

                            file << ">(\"" << v->m_Name << "\", ";

                            if(!v->m_Arguments.empty())
                            {
                                file << "{ ";

                                for(Type::uint32 i = 0; i < v->m_Arguments.size(); i++)
                                {
                                    Parser::Argument arg = v->m_Arguments[i];
                                    const Parser::ScopeInfo* res = 0;
                                    std::stack<std::string> names;

                                    names.push(arg.m_Type);

                                    searchForReverse(info, names, &res);

                                    if(res)
                                    {
                                        constructNamespace(res, names);

                                        std::string constructed;

                                        while(!names.empty())
                                        {
                                            constructed += names.top() + "::";

                                            names.pop();
                                        }

                                        file << "SteelEngine::Reflection::FucntionArgument<" << constructed << arg.m_Type << ">(\"" << arg.m_Name << "\")";
                                    }
                                    else
                                    {
                                        file << "SteelEngine::Reflection::FucntionArgument<" << parentsStr << arg.m_Type << ">(\"" << arg.m_Name << "\")";
                                    }

                                    if(i < v->m_Arguments.size() - 1)
                                    {
                                        file << ",\n";
                                    }
                                }

                                file << " }, ";
                            }

                            file << "&" << parentsStr << str->m_Name << "::" << v->m_Name << ")\n";
                            file << WriteMetaData(v);
                        }
                    }
                }

                for(Parser::EnumScope* v : str->m_Enums)
                {
                    if(v->m_IsReflectionLabelSet)
                    {
                        file << ".RegisterEnum<" << parentsStr << str->m_Name << "::" << v->m_Name << ">(\"" << v->m_Name << "\")\n";
                        file << ".Values({\n";

                        for(Type::uint32 i = 0; i < v->m_Elements.size(); i++)
                        {
                            Parser::EnumElementScope* e = v->m_Elements[i];

                            file << "SteelEngine::Reflection::EnumElement(\"" << e->m_Name << "\", " << parentsStr << str->m_Name << "::" << v->m_Name << "::" << e->m_Name << ")";
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
            Subprocess(info->m_Scopes.top(), file);

            info->m_Scopes.pop();
        }

        if(info->m_Parent && info->m_Parent->m_ScopeType == Parser::ScopeType::NAMESPACE)
        {
            if(!m_EndRecordingOnce)
            {
                if(info->m_IsReflectionLabelSet)
                {
                    file << "}\n";

                    if(info->m_Parent == 0 || info->m_Parent->m_ScopeType == Parser::ScopeType::NAMESPACE)
                    {
                        if(info->m_ScopeType == Parser::ScopeType::STRUCT || info->m_ScopeType == Parser::ScopeType::CLASS)
                        {
                            Parser::StructScope* str = (Parser::StructScope*)info;

                            for(IReflectionModule* module : str->m_ReflectionModules)
                            {
                                module->GenerateSource(file);
                            }

                            for(IReflectionModule* module : str->m_ReflectionModules)
                            {
                                module->GenerateHeader(m_GeneratedHeaderLinesByModules);
                            }
                        }
                    }

                    m_EndRecordingOnce = true;
                }
            }
        }

        if(info->m_ScopeType == Parser::ScopeType::NAMESPACE)
        {
            file << "}\n";

            namespaces.pop_back();
        }
    }

    ReflectionGenerator::ReflectionGenerator()
    {
        m_BeginRecordingOnce = false;
        m_EndRecordingOnce = false;
        m_GenerateReflection = true;
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

        m_Parser.Load(m_LoadedLines);

        m_Filename = filename;

        return SE_TRUE;
    }

    Result ReflectionGenerator::Parse()
    {
        Result res = m_Parser.Parse();

        if(!m_Parser.ShouldParse())
        {
            m_GenerateReflection = false;

            return SE_TRUE;
        }

        m_ParsedStructure = &m_Parser.GetParsed();

        if(!m_ParsedStructure->empty())
        {
            for(Type::uint32 i = 0; i < m_ParsedStructure->size(); i++)
            {
                bool res = false;

                checkIfAnyStructOrClass(m_ParsedStructure->at(i), res);

                if(!res)
                {
                    m_GenerateReflection = false;
                }
            }
        }
        else
        {
            m_GenerateReflection = false;
        }

        return res;
    }

    Result ReflectionGenerator::Generate(const std::filesystem::path& cwd, const std::filesystem::path& generatePath)
    {
        if(!m_GenerateReflection)
        {
            return SE_TRUE;
        }

        std::string p_ = m_Filename.string();

		replaceAll(p_, "\\", "/");

		std::vector<std::string> splitted_ = split(p_, '/');
		std::string finalPath = generatePath.string() + "/";
		std::string includePath = "";

        std::string p2 = cwd.string();

		replaceAll(p2, "\\", "/");

		for(Type::uint32 i = split(p2, '/').size() + 1; i < splitted_.size() - 1; i++)
		{
			includePath.append(splitted_[i]).append("/");
            finalPath.append(splitted_[i]).append("/");

            try
            {
                std::filesystem::create_directory(finalPath);
            }
            catch(const std::exception& e)
            {
                printf("%s\n", e.what());
            }
		}

        std::ofstream sourceFile(finalPath + m_Filename.stem().string() + ".Generated.cpp");

		sourceFile << "#include \"HotReloader/IRuntimeObject.h\"\n";
		sourceFile << "#include \"RuntimeReflection/ReflectionRecorder.h\"\n";
        sourceFile << "#include \"" << includePath << splitted_[splitted_.size() - 1] << "\"\n";
        sourceFile << "#include \"" << includePath << m_Filename.stem().string() << ".Generated.h" << "\"\n";
		sourceFile << "\n";

        for(Type::uint32 i = 0; i < m_ParsedStructure->size(); i++)
        {
            Parser::ScopeInfo* scope = m_ParsedStructure->at(i);

            Subprocess(scope, sourceFile);
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
        m_GenerateReflection = true;

        m_Parser.Clear();
    }

}