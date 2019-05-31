#pragma once

#include "RuntimeReflection/IReflectionGenerator.h"
#include "RuntimeReflection/Lexer.h"

#include "fstream"
#include "sstream"

namespace SteelEngine {

	class ReflectionGenerator : public Interface::IReflectionGenerator
	{
	public:
		enum ProtectionFlag
		{
			PUBLIC,
			PROTECTED,
			PRIVATE,
			NOT_SPECIFIED
		};

		enum ClassType
		{
			CLASS_TYPE,
			STRUCT_TYPE
		};

		struct MetaDataInfo
		{
			std::string m_Key;
			std::string m_Value;
		};

		struct ArgumentInfo
		{
			std::string m_Type;
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

		struct InheritanceInfo
		{
			std::string m_Name;
			ProtectionFlag m_Protection;
		};

		struct ClassData
		{
			std::string m_ClassName;
			ClassType m_Type;

			std::vector<InheritanceInfo> 	m_Inheritance;
			std::vector<MetaDataInfo>		m_ClassMetaDataInfo;
			std::vector<ConstructorInfo>	m_Constructors;
			std::vector<ClassProperty>		m_Properties;
			std::vector<ClassMethod>		m_Methods;
			std::vector<EnumInfo>			m_Enums;
			std::vector<std::string> 		m_Hierarchy;
			bool m_Reflect;

			std::vector<ClassData*> m_Others;
		};

	// Header Parsing Events:
		struct PreHeaderProcessEvent
		{
			const std::vector<std::string>* m_HeaderLines;
		};

		struct SE_ClassMacroEvent
		{
			const std::vector<ReflectionGenerator::MetaDataInfo>* m_MetaData;
			const std::string m_ClassName;
			const std::vector<InheritanceInfo>* m_Inheritance;
		};

		struct SE_ValueMacroEvent
		{
			const ReflectionGenerator::ClassProperty* m_Info;
			std::vector<ClassProperty>* m_Properties;
		};

		struct SE_MethodMacroEvent
		{
			const ReflectionGenerator::ClassMethod* m_Info;
			std::vector<ClassMethod>* m_Methods;
		};

		struct ClearValuesEvent
		{

		};

	// Source Parsing Events

	// Header Generate Events
		struct GenerateHeaderEvent
		{
			std::ofstream* m_Out;
			std::vector<std::string>* m_GeneratedBodyMacro;
		};

	// Source Generate Events
		struct GenerateSourceEvent
		{
			std::ofstream* m_Out;
			const std::string m_NamespacedClassName;
		};

	private:
		std::vector<std::string> m_HeaderLines;
		std::vector<std::string> m_SourceLines;

		filesystem::path m_PathSource;
		filesystem::path m_PathHeader;

		ProtectionFlag m_LastProtectionFlag;

		std::vector<std::string>	m_GeneratedBodyMacro;
		std::vector<ClassData*> 	m_Classes;

		void ParseHeader();
		void ParseSource();
		std::vector<MetaDataInfo> ParseMeta(const std::string& line);
		void GenerateMetaDataInfo(std::ofstream& out, std::vector<MetaDataInfo> meta);

	public:
		ReflectionGenerator();
		~ReflectionGenerator();

		Result Load(const filesystem::path& fileCpp, const filesystem::path& fileH) override;
		Result Parse() override;
		Result Generate(const filesystem::path& generatePath) override;
		void Clear() override;
	};

}