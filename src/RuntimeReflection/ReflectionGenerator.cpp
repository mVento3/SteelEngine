#include "RuntimeReflection/ReflectionGenerator.h"

#include "Module/ModuleExport.h"

#include "RuntimeReflection/Reflection.h"
#include "RuntimeReflection/ReflectionAttributes.h"

#include "Utils/Utils.h"

SE_PLUGIN(SteelEngine::ReflectionGenerator, "ReflectionGenerator", SteelEngine::Module::Details::PluginType::CORE_PLUGIN, SteelEngine::Module::Details::PluginFlag::NONE)

namespace SteelEngine {

	void ReflectionGenerator::ParseHeader()
	{
		Event::GlobalEvent::Broadcast(PreHeaderProcessEvent{ &m_HeaderLines });

		Type::uint32 lineIndex = 0;

		for (Type::uint32 i = 0; i < m_HeaderLines.size(); i++)
		{
			const std::string line = m_HeaderLines[i];
			std::string current = line;
			Type::uint32 index = 0;
			char currentChar = current[index++];
			std::string text = "";

			while (currentChar != '\0')
			{
				if (currentChar == '#')
				{
					isHash = true;
				}
				else if (currentChar == '{')
				{
					isA_Open = true;
				}
				else if (currentChar == '}')
				{
					isA_Open = false;
				}
				else if (currentChar == '(')
				{
					if(isFunction)
					{
						isIn = true;
					}

					isFunction = true;

					if (m_ClassName != "" && text == m_ClassName)
					{
						std::string current = line;

						replaceAll(current, "    ", "");
						replaceAll(current, "\t", "");
						replaceAll(current, ";", "");

						current.erase(current.begin(), current.begin() + strlen(m_ClassName.c_str()) + 1);
						current.erase(current.end() - 1, current.end());

						ConstructorInfo cons;

						std::vector<std::string> splitted = split(current, ',');

						current = m_HeaderLines[lineIndex - 1];

						if (current.find("SE_CONSTRUCTOR") != std::string::npos)
						{
							replaceAll(current, "    ", "");
							replaceAll(current, "\t", "");

							current.erase(current.begin(), current.begin() + strlen("SE_CONSTRUCTOR("));
							current.erase(current.end() - 1, current.end());

							cons.m_MetaData = ParseMeta(current);
						}

						for (std::string c : splitted)
						{
							if (c[0] == ' ')
							{
								c[0] = '\t';
							}

							std::vector<std::string> s2 = split(c, ' ');

							if (s2.size() > 2)
							{
								std::string res = "";

								for (Type::uint32 i = 0; i < s2.size() - 1; i++)
								{
									res += s2[i];

									if (i < s2.size() - 2)
									{
										res += " ";
									}
								}

								cons.m_Arguments.push_back(ArgumentInfo{ res, s2[s2.size() - 1] });
							}
							else if (s2.size() == 2)
							{
								replaceAll(s2[0], "    ", "");

								cons.m_Arguments.push_back(ArgumentInfo{ s2[0], s2[1] });
							}
						}

						m_Constructors.push_back(cons);
					}
				}
				else if (currentChar == ')')
				{
					if (isFunction && !isIn)
					{
						break;
					}
				}
				else if(currentChar == '\t')
				{
					
				}
				else if (currentChar == '/')
				{
					if (isComment)
					{
						break;
					}

					isComment = true;
				}
				else if (currentChar == ' ')
				{
					// Here we should check the current syntax/text

					Event::GlobalEvent::Broadcast(CheckCurrentTextEvent{ text, lineIndex, line });

					if (text == "")
					{

					}
					else if (text == "namespace")
					{
						std::string current = line;

						replaceAll(current, "\t", "");

						std::vector<std::string> splitted = split(current, ' ');

						for (std::string nam : splitted)
						{
							if (nam != "{" && nam != "namespace")
							{
								m_NamespaceHierarchy.push_back(nam);
							}
						}

						break;
					}
					else if (text == "using")
					{
						break;
					}
					else if (text == "struct")
					{
						isStruct = true;

						break;
					}
					else if (text == "class")
					{
						isClass = true;

						break;
					}
					else if (text == "friend")
					{
						break;
					}
					else if (text == "static")
					{
						break;
					}
					else if (text == "template")
					{
						break;
					}
					else if (text == "return")
					{
						break;
					}
					else if (text == "const")
					{
						break;
					}
					else if (text == "void")
					{
						break;
					}
					else if (text == "inline")
					{
						break;
					}
					else if (text == "include")
					{
						if (line.find("/") == std::string::npos)
						{
							std::string current = line;

							current.erase(current.begin(), current.begin() + strlen("#include \""));
							current.erase(current.end() - 1, current.end());

							filesystem::path linePath = current;
							filesystem::path path = m_PathHeader;

							linePath.replace_extension("cpp");

							path.replace_filename(linePath);

							if (filesystem::exists(path))
							{
								m_AdditionalIncludeDependencies.push_back(path.string());
							}
						}
					}
					else
					{
						text += ' ';
					}
				}
				else if (currentChar == ';')
				{
					if (text != "" && m_SerializeAll && !isFunction)
					{
						m_PropertiesToSerialize.push_back(split(text, ' ')[1]);
					}
				}
				else
				{
					if (isA_Open && (isClass || isStruct))
					{
						break;
					}

					text += currentChar;

					if (text == "public")
					{
						m_LastProtectionFlag = ProtectionFlag::PUBLIC;

						break;
					}
					else if (text == "private")
					{
						m_LastProtectionFlag = ProtectionFlag::PRIVATE;

						break;
					}
					else if (text == "protected")
					{
						m_LastProtectionFlag = ProtectionFlag::PROTECTED;

						break;
					}

					if (text == "SE_CLASS")
					{
						m_Reflect = true;

						std::string current = line;

						replaceAll(current, "    ", "");
						replaceAll(current, "\t", "");
						replaceAll(current, " ", "");

						current.erase(current.begin(), current.begin() + strlen("SE_CLASS("));
						current.erase(current.end() - 1, current.end());

						if (current.find(SE_GET_TYPE_NAME(ReflectionAttribute::SE_RUNTIME_SERIALIZE)) != std::string::npos)
						{
							m_SerializeAll = true;
						}

						if (current.find(SE_GET_TYPE_NAME(ReflectionAttribute::SE_NO_SERIALIZE)) != std::string::npos)
						{
							m_GenerateSerializeFunction = false;

							MethodInfo methInfo;

							methInfo.m_Name = "Serialize";
							methInfo.m_ReturnType = "char*";

							ClassMethod clsMeth;

							clsMeth.m_MethodInfo = methInfo;
							clsMeth.m_ProtectionFlag = ProtectionFlag::PUBLIC;

							m_Methods.push_back(clsMeth);

							methInfo.m_Name = "Deserialize";
							methInfo.m_ReturnType = "char*";

							clsMeth.m_MethodInfo = methInfo;
							clsMeth.m_ProtectionFlag = ProtectionFlag::PUBLIC;

							m_Methods.push_back(clsMeth);
						}

						m_ClassMetaDataInfo = ParseMeta(current);

						std::string p = m_HeaderLines[lineIndex + 1];

						replaceAll(p, "    ", "");

						std::vector<std::string> splitted = split(p, ' ');

						m_ClassName = splitted[1];

						splitted.erase(splitted.begin(), splitted.begin() + 2);

						// Has some inheritance
						if (splitted.size() > 0 && splitted[0] == ":")
						{
							for (Type::uint32 i = 1; i < splitted.size(); i++)
							{
								if (splitted[i] != "public" &&
									splitted[i] != "protected" &&
									splitted[i] != "private")
								{
									m_Inheritance.push_back(splitted[i]);
								}
							}
						}
						else
						{
							m_GenerateSerializeFunction = false;
						}

						Event::GlobalEvent::Broadcast(SE_ClassMacroEvent
						{
							&m_ClassMetaDataInfo,
							m_ClassName,
							&m_Inheritance
						});
					}
					else if (text == "SE_STRUCT")
					{
						m_Reflect = true;

						std::string current = line;

						replaceAll(current, "    ", "");
						replaceAll(current, "\t", "");

						current.erase(current.begin(), current.begin() + strlen("SE_STRUCT("));
						current.erase(current.end() - 1, current.end());

						if (current.find(SE_GET_TYPE_NAME(ReflectionAttribute::SE_RUNTIME_SERIALIZE)) != std::string::npos)
						{
							m_SerializeAll = true;
						}

						if (current.find(SE_GET_TYPE_NAME(ReflectionAttribute::SE_NO_SERIALIZE)) != std::string::npos)
						{
							m_GenerateSerializeFunction = false;
						}

						m_ClassMetaDataInfo = ParseMeta(current);

						std::vector<std::string> splitted = split(m_HeaderLines[lineIndex + 1], ' ');

						m_ClassName = splitted[1];

						splitted.erase(splitted.begin(), splitted.begin() + 2);

						// Has some inheritance
						if (splitted.size() > 0 && splitted[0] == ":")
						{
							for (Type::uint32 i = 1; i < splitted.size(); i++)
							{
								if (splitted[i] != "public" &&
									splitted[i] != "protected" &&
									splitted[i] != "private")
								{
									m_Inheritance.push_back(splitted[i]);
								}
							}
						}
						else
						{
							m_GenerateSerializeFunction = false;
						}
					}
					else if (text == "SE_VALUE")
					{
						std::string current = line;
						bool serialize = false;

						replaceAll(current, "    ", "");
						replaceAll(current, "\t", "");

						current.erase(current.begin(), current.begin() + strlen("SE_VALUE("));
						current.erase(current.end() - 1, current.end());

						if (current.find(SE_GET_TYPE_NAME(ReflectionAttribute::SE_RUNTIME_SERIALIZE)) != std::string::npos)
						{
							serialize = true;
						}

						ClassProperty prop = {};

						prop.m_MetaData = ParseMeta(current);

						current = m_HeaderLines[lineIndex + 1];

						replaceAll(current, "    ", "");
						replaceAll(current, "\t", "");
						replaceAll(current, ";", "");

						size_t open = RuntimeDatabase::s_InvalidID;
						size_t closed = RuntimeDatabase::s_InvalidID;

						if((open = current.find("[")) != std::string::npos &&
							(closed = current.find("]")) != std::string::npos)
						{
							current.erase(current.begin() + open, current.end());
						}

						std::vector<std::string> splitted = split(current, ' ');

						prop.m_ArgumentInfo = ArgumentInfo{ splitted[0], splitted[1] };
						prop.m_ProtectionFlag = m_LastProtectionFlag;

						m_Properties.push_back(prop);

						Event::GlobalEvent::Broadcast(SE_ValueMacroEvent{ &prop });

						if (serialize)
						{
							m_PropertiesToSerialize.push_back(splitted[1]);
						}
					}
					else if (text == "SE_METHOD")
					{
						std::string current = line;

						replaceAll(current, "    ", "");
						replaceAll(current, "\t", "");

						current.erase(current.begin(), current.begin() + strlen("SE_METHOD("));
						current.erase(current.end() - 1, current.end());

						ClassMethod meth = {};

						meth.m_MetaData = ParseMeta(current);

						current = m_HeaderLines[lineIndex + 1];

						replaceAll(current, "    ", "");
						replaceAll(current, "\t", "");

						size_t pos1 = current.find("(");

						current.erase(current.begin() + pos1, current.end());

						std::vector<std::string> splitted = split(current, ' ');

						meth.m_MethodInfo = MethodInfo{ splitted[0], splitted[1] };
						meth.m_ProtectionFlag = m_LastProtectionFlag;

						m_Methods.push_back(meth);

						Event::GlobalEvent::Broadcast(SE_MethodMacroEvent{ &meth });
					}
					else if(text == "SE_ENUM")
					{
						std::string current = line;

						replaceAll(current, "    ", "");
						replaceAll(current, "\t", "");

						current.erase(current.begin(), current.begin() + strlen("SE_ENUM("));
						current.erase(current.end() - 1, current.end());

						current = m_HeaderLines[lineIndex + 1];

						replaceAll(current, "    ", "");
						replaceAll(current, "\t", "");

						std::vector<std::string> splitted = split(current, ' ');

						EnumInfo enum_;

						enum_.m_EnumName = splitted[1];

						for(Type::uint32 i = 1; current != "}"; i++)
						{
							current = m_HeaderLines[lineIndex + 1 + i];
							
							replaceAll(current, "    ", "");
							replaceAll(current, "\t", "");
							replaceAll(current, ";", "");

							if(current.find("SE_ELEMENT") != std::string::npos)
							{
								current = m_HeaderLines[lineIndex + 2 + i];

								replaceAll(current, "    ", "");
								replaceAll(current, "\t", "");

								if(current[current.size() - 1] == ',')
								{
									current.erase(current.size() - 1);
								}

								EnumElement ele;

								ele.m_ElementName = current;

								enum_.m_Elements.push_back(ele);
							}
						}

						m_Enums.push_back(enum_);
					}
				}

				currentChar = current[index++];
			}

			isFunction = false;
			isHash = false;
			isIn = false;

			if (isA_Open && isClass)
			{
				isClass = false;
			}

			if (isA_Open && isStruct)
			{
				isStruct = false;
			}

			lineIndex++;
		}

		std::vector<std::string> toFind;

		for (Type::uint32 i = 0; i < m_Inheritance.size(); i++)
		{
			std::string current = m_Inheritance[i];

			replaceAll(current, "::", " ");

			std::vector<std::string> splitted = split(current, ' ');

			toFind.push_back(splitted[splitted.size() - 1]);
		}

		// Seek for interface source
		for (const auto& entry : filesystem::directory_iterator("build/GeneratedReflection"))
		{
			filesystem::path path = entry.path();

			if (path.extension() == ".h")
			{
				continue;
			}

			for (Type::uint32 i = 0; i < toFind.size(); i++)
			{
				if (path.filename().string().find(toFind[i]) != std::string::npos)
				{
					m_AdditionalIncludeDependencies.push_back(path.string());

					break;
				}
			}
		}
	}

	void ReflectionGenerator::ParseSource()
	{
		Type::uint32 lineIndex = 0;

		for (std::string line : m_SourceLines)
		{

		}
	}

	std::vector<ReflectionGenerator::MetaDataInfo> ReflectionGenerator::ParseMeta(const std::string& line)
	{
		std::vector<MetaDataInfo> res;

		std::string current = line;

		replaceAll(current, " ", "");

		if (current == "")
		{
			return res;
		}

		std::string value;
		std::string key;
		bool isOpen = false;
		bool isNextAttribute = true;
		bool wasComma = false;
		bool wasEqual = false;

		for (char c : current)
		{
			switch (c)
			{
			case '(':
				isOpen = true;
				value.push_back(c);
				break;
			case ')':
				isOpen = false;
				value.push_back(c);
				break;
			case ',':
				wasComma = true;

				if (!isOpen)
				{
					isNextAttribute = true;
				}
				else
				{
					isNextAttribute = false;
					value.push_back(c);
				}
				break;
			case '=':
				wasEqual = true;
				break;
			default:
				if (wasComma && isNextAttribute)
				{
					MetaDataInfo info;

					info.m_Key = key;
					info.m_Value = value;

					if (info.m_Value == "")
					{
						info.m_Value = "true";
					}

					res.push_back(info);

					key.clear();
					value.clear();

					wasComma = false;
					isNextAttribute = true;
					wasEqual = false;
				}

				if (!wasEqual)
				{				
					key.push_back(c);
				}
				else
				{
					value.push_back(c);
				}
				break;
			}
		}

		MetaDataInfo info;

		info.m_Key = key;
		info.m_Value = value;

		if (info.m_Value == "")
		{
			info.m_Value = "true";
		}

		res.push_back(info);

		return res;
	}

	void ReflectionGenerator::GenerateMetaDataInfo(std::ofstream& out, std::vector<MetaDataInfo> meta)
	{
		out << "(\n";

		for (Type::uint32 i = 0; i < meta.size(); i++)
		{
			out << "SteelEngine::Reflection::MetaData(" <<
				meta[i].m_Key << ", " <<
				meta[i].m_Value << ")";

			if (i < meta.size() - 1)
			{
				out << ",\n";
			}
			else
			{
				out << "\n";
			}
		}

		out << ")\n";
	}

	ReflectionGenerator::ReflectionGenerator()
	{
		m_Reflect = false;
		m_SerializeAll = false;
	}

	ReflectionGenerator::~ReflectionGenerator()
	{

	}

	Result ReflectionGenerator::Load(const filesystem::path& fileCpp, const filesystem::path& fileH)
	{
		m_PathSource = fileCpp;
		m_PathHeader = fileH;

		std::string line;

		std::ifstream headerFile(fileH);

		while (std::getline(headerFile, line))
		{
			m_HeaderLines.push_back(line);
		}

		headerFile.close();

		std::ifstream sourceFile(fileCpp);

		while (std::getline(sourceFile, line))
		{
			m_SourceLines.push_back(line);
		}

		sourceFile.close();

		return SE_TRUE;
	}

	Result ReflectionGenerator::Parse()
	{
		ParseHeader();
		ParseSource();

		return SE_TRUE;
	}

	Result ReflectionGenerator::Generate(const filesystem::path& generatePath)
	{
		filesystem::path path(m_PathHeader);

		std::string rawFilename = path.filename().string();

		rawFilename = split(rawFilename, '.')[0];

		std::ofstream headerFile(generatePath.string() + "/" + rawFilename + ".Generated.h");

		Event::GlobalEvent::Broadcast(GenerateHeaderEvent{ &headerFile });

		if (m_Reflect)
		{
			if (m_GenerateSerializeFunction)
			{
				headerFile << "#include \"RuntimeReflection/ReflectionGeneratorMacros.h\"\n";
				headerFile << "\n";

				headerFile << "#define GENERATED_BODY \\\n";
				headerFile << "public:\\\n";
				headerFile << "void Serialize(SteelEngine::Interface::ISerializer* serializer) override;\\\n";
				headerFile << "void operator()(const SteelEngine::RecompiledEvent& event_) override;\n";
			}
		}

		headerFile.close();

		std::ofstream sourceFile(generatePath.string() + "/" + rawFilename + ".Generated.cpp");

		Event::GlobalEvent::Broadcast(GenerateSourceEvent{ &sourceFile });

		if (m_Reflect)
		{
			std::string namespacedClassName = "";

			for (Type::uint32 i = 0; i < m_NamespaceHierarchy.size(); i++)
			{
				namespacedClassName += m_NamespaceHierarchy[i] += "::";
			}

			namespacedClassName += m_ClassName;

			m_ClassMetaDataInfo.push_back(MetaDataInfo{ "\"sizeof\"", "sizeof(" + namespacedClassName + ")" });

			path.replace_extension(".h");

			sourceFile << "#include \"" << rawFilename + ".Generated.h" << "\"\n";

			sourceFile << "#include \"../" << m_PathHeader.string() << "\"\n";
			sourceFile << "#include \"RuntimeCompiler/IRuntimeObject.h\"\n";
			sourceFile << "#include \"RuntimeReflection/Reflection.h\"\n";
			sourceFile << "\n";

			// Here we are generating the reflection info

			sourceFile << "REGISTER_REFLECTION\n";
			sourceFile << "{\n";
			{
				sourceFile << "SteelEngine::Reflection::Register<";
				sourceFile << namespacedClassName;
				sourceFile << ">(\"" << m_ClassName << "\")\n";

				GenerateMetaDataInfo(sourceFile, m_ClassMetaDataInfo);

				for (ConstructorInfo consInfo : m_Constructors)
				{
					sourceFile << ".Constructor<";

					for (Type::uint32 i = 0; i < consInfo.m_Arguments.size(); i++)
					{
						sourceFile << consInfo.m_Arguments[i].m_Key;

						if (i < consInfo.m_Arguments.size() - 1)
						{
							sourceFile << ", ";
						}
					}

					sourceFile << ">()\n";

					GenerateMetaDataInfo(sourceFile, consInfo.m_MetaData);
				}

				for (ClassProperty clsProp : m_Properties)
				{
					if (clsProp.m_ProtectionFlag == ProtectionFlag::PRIVATE ||
						clsProp.m_ProtectionFlag == ProtectionFlag::PROTECTED)
					{
						continue;
					}

					sourceFile << ".Property(";
					sourceFile << "\"" << clsProp.m_ArgumentInfo.m_Name << "\", ";
					sourceFile << "&" << namespacedClassName << "::" << clsProp.m_ArgumentInfo.m_Name;
					sourceFile << ")\n";

					GenerateMetaDataInfo(sourceFile, clsProp.m_MetaData);
				}

				for (ClassMethod clsMeth : m_Methods)
				{
					if (clsMeth.m_ProtectionFlag == ProtectionFlag::PRIVATE ||
						clsMeth.m_ProtectionFlag == ProtectionFlag::PROTECTED)
					{
						continue;
					}

					sourceFile << ".Method(";
					sourceFile << "\"" << clsMeth.m_MethodInfo.m_Name << "\", ";
					sourceFile << "&" << namespacedClassName << "::" << clsMeth.m_MethodInfo.m_Name;
					sourceFile << ")\n";

					GenerateMetaDataInfo(sourceFile, clsMeth.m_MetaData);
				}

				for (EnumInfo enum_ : m_Enums)
				{
					sourceFile << ".Enum<" + namespacedClassName + "::" + enum_.m_EnumName + ">(\"" + enum_.m_EnumName + "\")\n";

					GenerateMetaDataInfo(sourceFile, enum_.m_MetaData);

					if (enum_.m_Elements.size() > 0)
					{
						sourceFile << ".Values\n";
						sourceFile << "(\n";

						for (Type::uint32 i = 0; i < enum_.m_Elements.size(); i++)
						{
							EnumElement ele = enum_.m_Elements[i];

							sourceFile << "SteelEngine::ReflectionValue(\"" + ele.m_ElementName;
							sourceFile << "\", " + namespacedClassName + "::" + enum_.m_EnumName + "::" + ele.m_ElementName + ")\n";

							GenerateMetaDataInfo(sourceFile, ele.m_MetaData);

							if (i < enum_.m_Elements.size() - 1)
							{
								sourceFile << ",\n";
							}
							else
							{
								sourceFile << "\n";
							}
						}

						sourceFile << ")\n";
					}
				}

				sourceFile << ";\n";
			}
			sourceFile << "}\n\n";

			if (m_GenerateSerializeFunction)
			{
				sourceFile << "void " + namespacedClassName + "::Serialize(SteelEngine::Interface::ISerializer* serializer)\n";
				sourceFile << "{\n";
				{
					for (Type::uint32 i = 0; i < m_PropertiesToSerialize.size(); i++)
					{					
						sourceFile << "SERIALIZE(" << namespacedClassName << "::" << m_PropertiesToSerialize[i] << ")\n";
					}

					for (std::string inh : m_Inheritance)
					{
						std::string curr = inh;

						replaceAll(curr, "::", " ");

						std::vector<std::string> splitted = split(curr, ' ');

						IReflectionData* data = Reflection::GetType(splitted[splitted.size() - 1]);

						if (data)
						{
							sourceFile << inh << "::Serialize(serializer);\n";
						}
					}
				}
				sourceFile << "}\n\n";
			}

			// Here we are generating info for the runtime compilator

			if (m_Constructors.size() > 0)
			{
				sourceFile << "#ifdef RUNTIME_COMPILE\n";
				sourceFile << "extern \"C\" __declspec(dllexport) TypeInfo* GetPerModuleInterface(void* typeInfo)\n";
				sourceFile << "{\n";
				{
					sourceFile << "DECLARE_TYPE_INFO(" << namespacedClassName << ")\n";
					sourceFile << "{\n";
					{
						sourceFile << "FIND_THE_RIGHT_OBJECT\n";
						sourceFile << "\n";

						for (ConstructorInfo consInfo : m_Constructors)
						{
							if(consInfo.m_Arguments.size() > 0)
							{
								sourceFile << "COMPARE_CONSTRUCTOR(";

								for (Type::uint32 i = 0; i < consInfo.m_Arguments.size(); i++)
								{
									sourceFile << consInfo.m_Arguments[i].m_Key;

									if (i < consInfo.m_Arguments.size() - 1)
									{
										sourceFile << ", ";
									}
								}
							}
							else
							{
								sourceFile << "COMPARE_CONSTRUCTOR_(";
							}

							sourceFile << ")\n";
						}
					}
					sourceFile << "};\n";
					sourceFile << "\n";
					sourceFile << "return result;\n";
				}
				sourceFile << "}\n";
				sourceFile << "#endif\n";
			}
		}
		else
		{
			sourceFile << "";
		}

		sourceFile.close();

		return SE_TRUE;
	}

	void ReflectionGenerator::Clear()
	{
		m_HeaderLines.clear();
		m_SourceLines.clear();
		m_Constructors.clear();
		m_Properties.clear();
		m_Methods.clear();
		m_PropertiesToSerialize.clear();
		m_Enums.clear();
		m_NamespaceHierarchy.clear();
		m_AdditionalIncludeDependencies.clear();
		m_Inheritance.clear();
		m_ClassName = "";
		m_Reflect = false;
	}

}