#pragma once

#include "RuntimeReflection/IReflectionGenerator.h"
#include "RuntimeReflection/Lexer.h"

#include "fstream"
#include "sstream"

namespace SteelEngine {

	struct IReflectionData;

	class ReflectionGenerator : public IReflectionGenerator
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

		struct IMetaDataPiece
		{
			virtual size_t GetTypeID() const = 0;
		};

		template <typename A>
		struct MetaDataPiece : public IMetaDataPiece
		{
			MetaDataPiece(const A& value) :
				m_Value(value)
			{
				m_TypeID = typeid(A).hash_code();
			}

			size_t m_TypeID = RuntimeDatabase::s_InvalidID;
			A m_Value;

			size_t GetTypeID() const override
			{
				return m_TypeID;
			}
		};

		struct MetaDataInfoOther
		{
			IMetaDataPiece* m_Key;
			IMetaDataPiece* m_Value;

			template <typename KeyType, typename ValueType>
			MetaDataInfoOther(const KeyType& key, const ValueType& value)
			{
				m_Key = new MetaDataPiece<KeyType>(key);
				m_Value = new MetaDataPiece<ValueType>(value);
			}

			template <typename A>
			bool Compare(const A& key)
			{
				size_t keyTypeID = typeid(A).hash_code();

				if(keyTypeID == m_Key->GetTypeID())
				{
					MetaDataPiece<A>* value = (MetaDataPiece<A>*)m_Value;

					if(value->m_Value == key)
					{
						return true;
					}
				}

				return false;
			}

			bool Compare(const char* key)
			{
				size_t keyTypeID = typeid(const char*).hash_code();

				if(keyTypeID == m_Key->GetTypeID())
				{
					MetaDataPiece<const char*>* value = (MetaDataPiece<const char*>*)m_Value;

					if(strcmp(key, value->m_Value) == 0)
					{
						return true;
					}
				}

				return false;
			}
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
			std::vector<ArgumentInfo> m_Arguments;
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
			EnumElement(const std::string& name, const std::vector<MetaDataInfo>& meta = {}) :
				m_ElementName(name),
				m_MetaData(meta)
			{

			}

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
			std::vector<MetaDataInfo> m_MetaData;
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
			IReflectionData*				m_Data;
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
			IReflectionData* m_Data;
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

		struct SE_InheritanceMacroEvent
		{
			const ReflectionGenerator::InheritanceInfo* m_Info;
			std::vector<InheritanceInfo>* m_Inheritances;
		};

		struct SE_ConstructorMacroEvent
		{
			std::vector<ArgumentInfo> m_Args;
		};

		struct ClearValuesEvent
		{

		};

		struct GenerateMethodReflection
		{
			std::vector<ClassMethod> m_Methods;
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
			const std::string m_ClassName;
		};

	private:
		std::vector<std::string> m_HeaderLines;

		std::filesystem::path m_PathHeader;

		ProtectionFlag m_LastProtectionFlag;

		std::vector<std::string>	m_GeneratedBodyMacro;
		std::vector<ClassData*> 	m_Classes;

		void ParseHeader();
		std::vector<MetaDataInfo> ParseMeta(const std::string& line);
		void GenerateMetaDataInfo(std::ofstream& out, std::vector<MetaDataInfo> meta);
		void ProcessMetaData(Lexer& lexer, std::vector<MetaDataInfo>& res, std::string& word, ProtectionFlag* flag = 0);

	public:
		ReflectionGenerator();
		~ReflectionGenerator();

		Result Load(const std::filesystem::path& fileH) override;
		Result Parse() override;
		Result Generate(const std::filesystem::path& cwd, const std::filesystem::path& generatePath) override;
		void Clear() override;
	};

}