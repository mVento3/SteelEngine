#pragma once

#include "RuntimeReflection/IReflectionGenerator.h"

#include "vector"

namespace SteelEngine {

	class ReflectionGenerator : public Interface::IReflectionGenerator
	{
	public:
		enum ProtectionFlag
		{
			PUBLIC,
			PROTECTED,
			PRIVATE
		};

		struct MetaDataInfo
		{
			std::string m_Key;
			std::string m_Value;
		};

		struct ArgumentInfo
		{
			std::string m_Key;
			std::string m_Name;
		};

		struct MethodInfo
		{
			std::string m_ReturnType;
			std::string m_Name;
			// Eventually add the arguments
		};

		struct ClassProperty
		{
			std::vector<MetaDataInfo>	m_MetaData;
			ArgumentInfo				m_ArgumentInfo;
			ProtectionFlag				m_ProtectionFlag;
		};

		struct ClassMethod
		{
			std::vector<MetaDataInfo>	m_MetaData;
			MethodInfo					m_MethodInfo;
			ProtectionFlag				m_ProtectionFlag;
		};

		struct ConstructorInfo
		{
			std::vector<MetaDataInfo> m_MetaData;
			std::vector<ArgumentInfo> m_Arguments;
		};

		struct EnumElement
		{
			std::string					m_ElementName;
			std::vector<MetaDataInfo>	m_MetaData;
		};

		struct EnumInfo
		{
			std::string					m_EnumName;
			std::vector<MetaDataInfo>	m_MetaData;
			std::vector<EnumElement>	m_Elements;
		};

	private:
		std::vector<std::string> m_HeaderLines;
		std::vector<std::string> m_SourceLines;

		filesystem::path m_PathSource;
		filesystem::path m_PathHeader;

		bool m_Reflect;
		ProtectionFlag m_LastProtectionFlag;
		bool m_SerializeAll;
		bool m_GenerateSerializeFunction = true;

		std::vector<MetaDataInfo>		m_ClassMetaDataInfo;
		std::vector<ConstructorInfo>	m_Constructors;
		std::vector<ClassProperty>		m_Properties;
		std::vector<ClassMethod>		m_Methods;
		std::vector<std::string>		m_PropertiesToSerialize;
		std::vector<EnumInfo>			m_Enums;
		std::vector<std::string>		m_NamespaceHierarchy;
		std::vector<std::string>		m_Inheritance;

		std::string m_ClassName;

		bool isComment = false;
		bool isA_Open = false;
		bool isFunction = false;
		bool isHash = false;
		bool isClass = false;
		bool isStruct = false;
		bool isIn = false;

		void ParseHeader();
		void ParseSource();
		std::vector<MetaDataInfo> ParseMeta(const std::string& line);
		void GenerateMetaDataInfo(std::ofstream& out, std::vector<MetaDataInfo> meta);

	public:
		ReflectionGenerator();
		~ReflectionGenerator();

		std::vector<std::string> m_AdditionalIncludeDependencies;

		Result Load(const filesystem::path& fileCpp, const filesystem::path& fileH) override;
		Result Parse() override;
		Result Generate(const filesystem::path& generatePath) override;
		void Clear() override;
	};

}