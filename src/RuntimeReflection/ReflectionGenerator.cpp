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

		ClassData* currentData = 0;
		ConstructorInfo cons;
		ClassProperty prop;
		ClassMethod meth;

		std::stack<ClassData*> classStack;

		Type::uint32 bracesSize = 0;
		std::vector<Type::uint32> okBraces;
		std::vector<Type::uint32> notOkBraces;

		std::vector<std::string> namespaces;

		bool wasOkBrace = false;
		bool wasNotOkBrace = false;

		bool seClass = false;
		bool seValue = false;
		bool seMethod = false;

		m_LastProtectionFlag = ProtectionFlag::NOT_SPECIFIED;

		Lexer lexer(m_HeaderLines);

		while(1)
		{
			lexer++;

			if(lexer.End())
			{
				break;
			}

			if(lexer.GetToken() == "namespace")
			{
				wasOkBrace = true;

				lexer++;

				namespaces.push_back(lexer.GetToken());
			}
			else if(lexer.GetToken() == "struct" ||
				lexer.GetToken() == "class")
			{
				wasOkBrace = true;

				if(currentData == 0)
				{
					currentData = new ClassData();

					classStack.push(currentData);
				}
				else if(!seClass)
				{
					ClassData* data = new ClassData();
					currentData->m_Others.push_back(data);
					currentData = data;
					classStack.push(data);
				}

				if(lexer.GetToken() == "class")
				{
					currentData->m_Type = ClassType::CLASS_TYPE;
				}
				else if(lexer.GetToken() == "struct")
				{
					currentData->m_Type = ClassType::STRUCT_TYPE;
				}

				lexer++;

				currentData->m_ClassName = lexer.GetToken();
				currentData->m_Hierarchy = namespaces;
				namespaces.push_back(lexer.GetToken());

				lexer++;

				// Have some inheritance
				if(lexer.GetToken() == ":")
				{
					lexer++; // or public or name

					while(1)
					{
						if(lexer.GetToken() == "{")
						{
							lexer.SaveToken(lexer.GetToken());

							break;
						}
						else if(lexer.GetToken() == ",")
						{
							lexer++;
							continue;
						}

						if(lexer.GetToken() == "public")
						{
							std::string word = "";

							while(1)
							{
								lexer++;

								if(lexer.GetToken() == "{" ||
									lexer.GetToken() == ",")
								{
									lexer.SaveToken(lexer.GetToken());

									break;
								}

								word += lexer.GetToken();
							}

							currentData->m_Inheritance.push_back(InheritanceInfo{ word, ProtectionFlag::PUBLIC });
						}
						else if(lexer.GetToken() == "private")
						{
							std::string word = "";

							while(1)
							{
								lexer++;

								if(lexer.GetToken() == "{" ||
									lexer.GetToken() == ",")
								{
									lexer.SaveToken(lexer.GetToken());

									break;
								}

								word += lexer.GetToken();
							}

							currentData->m_Inheritance.push_back(InheritanceInfo{ word, ProtectionFlag::PRIVATE });
						}
						else if(lexer.GetToken() == "protected")
						{
							std::string word = "";

							while(1)
							{
								lexer++;

								if(lexer.GetToken() == "{" ||
									lexer.GetToken() == ",")
								{
									lexer.SaveToken(lexer.GetToken());

									break;
								}

								word += lexer.GetToken();
							}

							currentData->m_Inheritance.push_back(InheritanceInfo{ word, ProtectionFlag::PROTECTED });
						}
						else
						{
							// Pure inheritance name
						}

						lexer++;
					}
				}
				else
				{
					// Need to get back, or save the token
					lexer.SaveToken(lexer.GetToken());
				}

				Event::GlobalEvent::Broadcast(SE_ClassMacroEvent
					{
						&currentData->m_ClassMetaDataInfo,
						currentData->m_ClassName,
						&currentData->m_Inheritance
					});
			}
			else if(lexer.GetToken() == "public")
			{
				m_LastProtectionFlag = ProtectionFlag::PUBLIC;
			}
			else if(lexer.GetToken() == "protected")
			{
				m_LastProtectionFlag = ProtectionFlag::PROTECTED;
			}
			else if(lexer.GetToken() == "private")
			{
				m_LastProtectionFlag = ProtectionFlag::PRIVATE;
			}
			else if(lexer.GetToken() == "{")
			{
				if(wasOkBrace)
				{
					wasOkBrace = false;

					okBraces.push_back(bracesSize);
				}
				else
				{
					wasNotOkBrace = false;

					notOkBraces.push_back(bracesSize);
				}

				bracesSize++;
			}
			else if(lexer.GetToken() == "}")
			{
				bracesSize--;

				bool found = false;

				for(Type::uint32 i = 0; i < okBraces.size(); i++)
				{
					if(okBraces[i] == bracesSize)
					{
						okBraces.erase(okBraces.begin() + i);
						found = true;

						namespaces.pop_back();

						if(!classStack.empty())
						{
							classStack.pop();
						}

						if(!classStack.empty())
						{
							currentData = classStack.top();
						}
						else if(currentData)
						{
							m_Classes.push_back(currentData);
							currentData = 0;
						}

						break;
					}
				}

				if(!found)
				{
					for(Type::uint32 i = 0; i < notOkBraces.size(); i++)
					{
						if(notOkBraces[i] == bracesSize)
						{
							notOkBraces.erase(notOkBraces.begin() + i);
							found = true;

							break;
						}
					}
				}
			}
			else if(currentData && lexer.GetToken() == currentData->m_ClassName)
			{
				// Constructor

				wasNotOkBrace = true;

				lexer++;

				if(lexer.GetToken() == "*" ||
					lexer.GetToken() == "&" ||
					lexer.GetToken() != "(" ||
					lexer.GetToken() == ">")
				{
					continue;
				}

				if(!notOkBraces.empty())
				{
					continue;
				}

				std::string word = "";
				ArgumentInfo arg;
				std::string name = "";
				std::vector<std::string> splitted;

				while(1)
				{
					if(lexer.GetToken() == "(")
					{
						lexer++;

						continue;
					}
					else if(lexer.GetToken() == ")")
					{
						if(splitted.size() == 0)
						{
							splitted = split(word, ' ');

							if(splitted.size() == 2)
							{
								arg.m_Type = splitted[0];
								arg.m_Name = splitted[1];

								cons.m_Arguments.push_back(arg);
							}
							else if(splitted.size() == 3)
							{
								arg.m_Type = splitted[0] + " " + splitted[1];
								arg.m_Name = splitted[2];

								cons.m_Arguments.push_back(arg);
							}
						}
						else
						{
							if(splitted.size() == 2)
							{
								arg.m_Type = splitted[0];
								arg.m_Name = splitted[1];

								cons.m_Arguments.push_back(arg);
							}
							else if(splitted.size() == 3)
							{
								arg.m_Type = splitted[0] + " " + splitted[1];
								arg.m_Name = splitted[2];

								cons.m_Arguments.push_back(arg);
							}
						}

						word.clear();

						break;
					}
					else if(lexer.GetToken() == ",")
					{
						lexer++;

						if(splitted.size() == 0)
						{
							splitted = split(word, ' ');

							if(splitted.size() == 2)
							{
								arg.m_Type = splitted[0];
								arg.m_Name = splitted[1];

								cons.m_Arguments.push_back(arg);
							}
							else if(splitted.size() == 3)
							{
								arg.m_Type = splitted[0] + " " + splitted[1];
								arg.m_Name = splitted[2];

								cons.m_Arguments.push_back(arg);
							}
						}
						else
						{
							if(splitted.size() == 2)
							{
								arg.m_Type = splitted[0];
								arg.m_Name = splitted[1];

								cons.m_Arguments.push_back(arg);
							}
							else if(splitted.size() == 3)
							{
								arg.m_Type = splitted[0] + " " + splitted[1];
								arg.m_Name = splitted[2];

								cons.m_Arguments.push_back(arg);
							}
						}

						word.clear();
					}
					else if(lexer.GetToken() == "=")
					{
						lexer++;

						word.erase(word.end() - 1);

						splitted = split(word, ' ');

						word.clear();
					}
					else
					{
						word += lexer.GetToken();

						if(lexer.Space())
						{
							word += " ";
						}

						lexer++;
					}
				}

				currentData->m_Constructors.push_back(cons);
			}
			else if(lexer.GetToken() == "SE_CLASS" ||
				lexer.GetToken() == "SE_STRUCT")
			{
				seClass = true;

				if(currentData == 0)
				{
					currentData = new ClassData();
				}

				currentData->m_Reflect = true;

				if(lexer.GetToken() == "SE_CLASS")
				{
					currentData->m_Type = ClassType::CLASS_TYPE;
				}
				else if(lexer.GetToken() == "SE_STRUCT")
				{
					currentData->m_Type = ClassType::STRUCT_TYPE;
				}

				lexer++; // (
				
				std::string word = "";
				bool wasEqual = false;
				MetaDataInfo meta;
				std::stack<bool> roundBraces;

				roundBraces.push(true);

				while(1)
				{
					lexer++;

					if(lexer.GetToken() == ")" && roundBraces.size() == 1)
					{
						if(wasEqual && word != "")
						{
							meta.m_Value = word;
						}
						else if(word != "")
						{
							meta.m_Key = word;
							meta.m_Value = "true";
						}
						else
						{
							break;
						}
						
						word.clear();
						currentData->m_ClassMetaDataInfo.push_back(meta);

						break;
					}
					else if(lexer.GetToken() == ")")
					{
						word += lexer.GetToken();
						roundBraces.pop();
					}
					else if(lexer.GetToken() == "(")
					{
						word += lexer.GetToken();
						roundBraces.push(true);
					}
					else if(lexer.GetToken() == "," && roundBraces.size() == 1)
					{
						if(wasEqual)
						{
							meta.m_Value = word;
						}
						else
						{
							meta.m_Key = word;
							meta.m_Value = "true";
						}

						wasEqual = false;
						word.clear();
						currentData->m_ClassMetaDataInfo.push_back(meta);
					}
					else if(lexer.GetToken() == "=")
					{
						wasEqual = true;
						meta.m_Key = word;
						word.clear();
					}
					else
					{
						word += lexer.GetToken();
					}
				}
			}
			else if(lexer.GetToken() == "SE_VALUE")
			{
				seValue = true;

				lexer++;

				std::string word = "";
				bool wasEqual = false;
				MetaDataInfo meta;

				while(1)
				{
					lexer++;

					if(lexer.GetToken() == ")")
					{
						if(wasEqual && word != "")
						{
							meta.m_Value = word;
						}
						else if(word != "")
						{
							meta.m_Key = word;
							meta.m_Value = "true";
						}
						else
						{
							break;
						}
						
						word.clear();
						prop.m_MetaData.push_back(meta);

						break;
					}
					else if(lexer.GetToken() == ",")
					{
						if(wasEqual)
						{
							meta.m_Value = word;
						}
						else
						{
							meta.m_Key = word;
							meta.m_Value = "true";
						}

						wasEqual = false;
						word.clear();
						prop.m_MetaData.push_back(meta);
					}
					else if(lexer.GetToken() == "=")
					{
						wasEqual = true;
						meta.m_Key = word;
						word.clear();
					}
					else
					{
						word += lexer.GetToken();
					}
				}
			}
			else if(lexer.GetToken() == "SE_METHOD")
			{
				seMethod = true;

				lexer++;

				std::string word = "";
				bool wasEqual = false;
				MetaDataInfo meta;

				while(1)
				{
					lexer++;

					if(lexer.GetToken() == ")")
					{
						if(wasEqual && word != "")
						{
							meta.m_Value = word;
						}
						else if(word != "")
						{
							meta.m_Key = word;
							meta.m_Value = "true";
						}
						else
						{
							break;
						}
						
						word.clear();
						meth.m_MetaData.push_back(meta);
						meth.m_ProtectionFlag = m_LastProtectionFlag;

						break;
					}
					else if(lexer.GetToken() == ",")
					{
						if(wasEqual && word != "")
						{
							meta.m_Value = word;
						}
						else if(word != "")
						{
							meta.m_Key = word;
							meta.m_Value = "true";
						}

						wasEqual = false;
						word.clear();
						meth.m_MetaData.push_back(meta);
						meth.m_ProtectionFlag = m_LastProtectionFlag;
					}
					else if(lexer.GetToken() == "=")
					{
						wasEqual = true;
						meta.m_Key = word;
						word.clear();
					}
					else
					{
						word += lexer.GetToken();
					}
				}

				lexer++;

				while(1)
				{
					if(lexer.GetToken() == "const")
					{
						word += lexer.GetToken();

						if(lexer.Space())
						{
							word += " ";
						}

						lexer++;
					}

					word += lexer.GetToken();

					if(lexer.Space())
					{
						lexer++;

						break;
					}

					lexer++;
				}

				meth.m_MethodInfo.m_ReturnType = word;
				meth.m_MethodInfo.m_Name = lexer.GetToken();

				currentData->m_Methods.push_back(meth);

				Event::GlobalEvent::Broadcast(SE_MethodMacroEvent{ &meth, &currentData->m_Methods });

				meth.m_MetaData.clear();
			}
			else if(lexer.GetToken() == "GENERATED_BODY")
			{
				
			}
			else if(lexer.GetToken() == "#pragma")
			{
				lexer++;
			}
			else if(lexer.GetToken() == "#include")
			{
				lexer++;
			}
			else if(lexer.GetToken() == ";")
			{
				if(!notOkBraces.empty())
				{
					continue;
				}

				std::string curr = lexer.GetCurrentLine();

				replaceAll(curr, "    ", "");
				replaceAll(curr, "\t", "");
				replaceAll(curr, ";", "");

				std::vector<std::string> splittedEqual =
					split(curr, '=');
				
				std::vector<std::string> splitted =
					split(splittedEqual[0], ' ');
				
				if(splitted.size() == 2 &&
					splitted[1].find("(") == std::string::npos &&
					splitted[1].find(")") == std::string::npos)
				{
					std::string& sec = splitted[1];

					size_t pos1 = sec.find("[");
					size_t pos2 = sec.find("]");

					if(pos1 != std::string::npos &&
						pos2 != std::string::npos)
					{
						sec.erase(sec.begin() + pos1, sec.end());
					}

					if(splitted[0] == "const")
					{
						prop.m_ArgumentInfo.m_Type =
							splitted[0] + " " + splitted[1];

						prop.m_ArgumentInfo.m_Name =
							splitted[2];
					}
					else
					{
						prop.m_ArgumentInfo.m_Type =
							splitted[0];

						prop.m_ArgumentInfo.m_Name =
							splitted[1];
					}

					if(m_LastProtectionFlag == ProtectionFlag::NOT_SPECIFIED &&
						currentData->m_Type == ClassType::CLASS_TYPE)
					{
						m_LastProtectionFlag = ProtectionFlag::PROTECTED;
					}

					prop.m_ProtectionFlag = m_LastProtectionFlag;

					currentData->m_Properties.push_back(prop);

					Event::GlobalEvent::Broadcast(SE_ValueMacroEvent{ &prop, &currentData->m_Properties });

					prop.m_MetaData.clear();
				}
			}
		}
	}

	void ReflectionGenerator::ParseSource()
	{
		// Type::uint32 lineIndex = 0;

		// for (std::string line : m_SourceLines)
		// {

		// }
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
		ClassData* data = 0;

		for(Type::uint32 i = 0; i < m_Classes.size(); i++)
		{
			data = m_Classes[i];

			if(data->m_Reflect)
			{
				//return Result(SE_TRUE, "Nothing to reflect!");
				break;
			}
		}

		filesystem::path path(m_PathHeader);

		std::string rawFilename = path.filename().string();

		rawFilename = split(rawFilename, '.')[0];

		std::ofstream headerFile(generatePath.string() + "/" + rawFilename + ".Generated.h");

		headerFile << "#include \"RuntimeReflection/ReflectionGeneratorMacros.h\"\n";
		headerFile << "\n";
		headerFile << "#define GENERATED_BODY";

		Event::GlobalEvent::Broadcast(GenerateHeaderEvent{ &headerFile, &m_GeneratedBodyMacro });

		if(m_GeneratedBodyMacro.size() > 0)
		{
			headerFile << " \\\n";
		}

		for(Type::uint32 i = 0; i < m_GeneratedBodyMacro.size(); i++)
		{
			headerFile << m_GeneratedBodyMacro[i];

			if(i < m_GeneratedBodyMacro.size() - 1)
			{
				headerFile << "\\";
			}

			headerFile << "\n";
		}

		m_GeneratedBodyMacro.clear();

		headerFile.close();

		std::ofstream sourceFile(generatePath.string() + "/" + rawFilename + ".Generated.cpp");

		std::string namespace_ = "";
		std::string namespacedClassName = "";

		for (Type::uint32 i = 0; i < data->m_Hierarchy.size(); i++)
		{
			namespace_ += data->m_Hierarchy[i];

			if(i < data->m_Hierarchy.size() - 1)
			{
				namespace_ += "::";
			}
		}

		namespacedClassName = namespace_ + "::" + data->m_ClassName;

		data->m_ClassMetaDataInfo.push_back(MetaDataInfo{ "\"sizeof\"", "sizeof(" + namespacedClassName + ")" });

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
			sourceFile << ">(\"" << data->m_ClassName << "\")\n";

			GenerateMetaDataInfo(sourceFile, data->m_ClassMetaDataInfo);

			for(InheritanceInfo inh : data->m_Inheritance)
			{
				sourceFile << ".Inheritance";
				sourceFile << "(\"" << inh.m_Name << "\")\n";
			}

			for (ConstructorInfo consInfo : data->m_Constructors)
			{
				sourceFile << ".Constructor<";

				for (Type::uint32 i = 0; i < consInfo.m_Arguments.size(); i++)
				{
					sourceFile << consInfo.m_Arguments[i].m_Type;

					if (i < consInfo.m_Arguments.size() - 1)
					{
						sourceFile << ", ";
					}
				}

				sourceFile << ">()\n";

				GenerateMetaDataInfo(sourceFile, consInfo.m_MetaData);
			}

			for (ClassProperty clsProp : data->m_Properties)
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

			for (ClassMethod clsMeth : data->m_Methods)
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

			for (EnumInfo enum_ : data->m_Enums)
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

		Event::GlobalEvent::Broadcast(GenerateSourceEvent{ &sourceFile, namespacedClassName });

		// Here we are generating info for the runtime compilator

		if (data->m_Constructors.size() > 0)
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

					for (ConstructorInfo consInfo : data->m_Constructors)
					{
						if(consInfo.m_Arguments.size() > 0)
						{
							sourceFile << "COMPARE_CONSTRUCTOR(";

							for (Type::uint32 i = 0; i < consInfo.m_Arguments.size(); i++)
							{
								sourceFile << consInfo.m_Arguments[i].m_Type;

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

		sourceFile.close();

		return SE_TRUE;
	}

	void ReflectionGenerator::Clear()
	{
		m_HeaderLines.clear();
		m_SourceLines.clear();

		for(Type::uint32 i = 0; i < m_Classes.size(); i++)
		{
			delete m_Classes[i];
			m_Classes[i] = 0;
		}

		m_Classes.clear();

		Event::GlobalEvent::Broadcast(ClearValuesEvent{});
	}

}