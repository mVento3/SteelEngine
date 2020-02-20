#pragma once

#include "vector"
#include "string"
#include "stack"

#include "Core/Type.h"

namespace SteelEngine {

    struct Lexer
    {
    private:
        std::vector<std::string> m_Lines;
        std::string m_Token;
        std::string m_CurrentLine;
        Type::uint32 m_LineIndex;
        Type::uint32 m_CurrentLineSize;
        Type::uint32 m_CharacterIndex;
        char m_CurrentCharacter;
        bool m_End;
        bool m_Space;
        char m_PendingSymbol;
        bool m_NewLine;
        bool m_WasNewLine;
        std::stack<std::string> m_SavedToken;

        Lexer& Increment()
        {
            m_WasNewLine = m_NewLine;

            m_Token.clear();
            m_Space = false;
            m_NewLine = false;

            if(m_PendingSymbol != '\0')
            {
                m_Token += m_PendingSymbol;

                if(m_CurrentLine[m_CharacterIndex] == ' ')
                {
                    m_Space = true;
                }

                if((m_PendingSymbol == ':' &&
                    m_CurrentLine[m_CharacterIndex] == ':') ||
                    (m_PendingSymbol == '-' &&
                    m_CurrentLine[m_CharacterIndex] == '>'))
                {
                    m_Token += m_CurrentLine[m_CharacterIndex++];
                }

                m_PendingSymbol = '\0';

                return *this;
            }

            while(1)
            {
                m_CurrentCharacter = m_CurrentLine[m_CharacterIndex++];

                if((m_CurrentCharacter == ' ' && m_Token == "") || m_CurrentCharacter == '\t')
                {
                    continue;
                }

                if(m_CurrentCharacter == ' ')
                {
                    m_Space = true;
                }

                if(m_CurrentCharacter == '{' ||
                    m_CurrentCharacter == '}' ||
                    m_CurrentCharacter == '[' ||
                    m_CurrentCharacter == ']' ||
                    m_CurrentCharacter == '(' ||
                    m_CurrentCharacter == ')' ||
                    m_CurrentCharacter == '<' ||
                    m_CurrentCharacter == '>' ||
                    m_CurrentCharacter == ',' ||
                    m_CurrentCharacter == ';' ||
                    m_CurrentCharacter == ':' ||
                    m_CurrentCharacter == '-' ||
                    m_CurrentCharacter == '&' ||
                    m_CurrentCharacter == '*')
                {
                    m_PendingSymbol = m_CurrentCharacter;

                    break;
                }
            // Comment
                else if(m_CurrentCharacter == '/' &&
                    m_CurrentLine[m_CharacterIndex] == '/')
                {
                    m_CurrentLine = m_Lines[m_LineIndex++];
                    m_CurrentLineSize = m_CurrentLine.size();
                    m_CharacterIndex = 0;

                    break;
                }

                if(m_CurrentCharacter != ' ' && m_CurrentCharacter != '\0')
                {
                    m_PendingSymbol = '\0';

                    m_Token += m_CurrentCharacter;
                }
                else if(m_CharacterIndex >= m_CurrentLineSize || m_CurrentCharacter == '\n')
                {
                    if(m_Lines.size() == m_LineIndex)
                    {
                        m_End = true;
                    }
                    else
                    {
                        m_CurrentLine = m_Lines[m_LineIndex++];
                        m_CurrentLineSize = m_CurrentLine.size();
                        m_CharacterIndex = 0;
                        m_NewLine = true;
                    }

                    break;
                }
                else
                {
                    break;
                }
            }

            if(m_Token == "")
            {
                if(m_End)
                {
                    return *this;
                }

                return Increment();
            }

            return *this;
        }

    public:
        Lexer()
        {
            m_LineIndex = 0; // +1
            m_CharacterIndex = 0;

            m_End = false;
            m_NewLine = false;

            m_PendingSymbol = '\0';
        }

        Lexer(std::vector<std::string>& lines) :
            Lexer()
        {
            Load(lines);
        }

        void Load(const std::vector<std::string>& lines)
        {
            m_LineIndex = 0; // +1
            m_CharacterIndex = 0;

            m_End = false;

            m_PendingSymbol = '\0';

            m_Lines = lines;

            m_CurrentLine = m_Lines[m_LineIndex++];
            m_CurrentLineSize = m_CurrentLine.size();

            // Increment();
        }

        inline bool End()
        {
            return m_End;
        }

        inline bool Space()
        {
            return m_Space;
        }

        inline bool NewLine()
        {
            return m_NewLine;
        }

        inline bool WasNewLine()
        {
            return m_WasNewLine;
        }

        inline std::string GetToken()
        {
            if(!m_SavedToken.empty())
            {
                m_Token = m_SavedToken.top();
            }

            return m_Token;
        }

        inline std::string GetCurrentLine()
        {
            return m_CurrentLine;
        }

        inline void SaveToken(const std::string& token)
        {
            m_SavedToken.push(token);
        }

        inline void SkipLine()
        {
            m_LineIndex++;

            if(m_LineIndex >= m_Lines.size())
            {
                m_End = true;

                return;
            }

            m_CurrentLine = m_Lines.at(m_LineIndex - 1);
            m_CharacterIndex = 0;
            m_CurrentLineSize = m_CurrentLine.size();
        }

        Lexer operator++(int v)
        {
            if(m_SavedToken.empty())
            {
                return Increment();
            }
            else
            {
                m_SavedToken.pop();

                return *this;
            }
        }
    };

}