#include "ini_parser.hpp"
#include <fstream>

using namespace ema;
using namespace config;



IniParser::IniParser(const FileName& fileName) : m_fileName(fileName)
{
	reload();
}


void IniParser::reload()
{
	m_sections.clear();
	std::wfstream iniFile(m_fileName, std::fstream::in);
	detectEncoding(iniFile);
	parseStream(iniFile);
}




void IniParser::parseStream(std::wostream& iniFile)
{
	Parser parser(*this);
	while (!iniFile.eof())
	{
		std::wstring line = LoadString(iniFile);
		parser.parseLine(line);
	}
}


IniParser::Parser::Parser(IniParser& owner) : m_owner(owner)
{
}

void IniParser::Parser::truncate(ParseLine& pline)
{
	while (*pline && (*pline == L'\t' || *pline == L' '))
		++pline;
}

IniParser::Comment& IniParser::Parser::getCurrentComments()
{
	if (m_currentSection.m_name.empty())
		return m_owner.m_prefixComment;
	else
	{
		if (m_currentKey.m_name.empty())
			return m_currentSection.m_comment;
		else
			return m_currentKey.m_comment;
	}
}


void IniParser::Parser::pushSection()
{
	if (m_currentSection.m_name.empty())
		return;
	m_owner.m_sections.push_back(m_currentSection);
	m_currentSection.m_keys.clear();
	m_currentSection.m_name.clear();
	m_currentSection.m_comment.clear();
}

void IniParser::Parser::pushKey()
{
	if (m_currentKey.m_name.empty())
		return;

	if (m_currentSection.m_name.empty())
		throw ParsingError();

	m_currentSection.m_keys.push_back(m_currentKey);
	m_currentKey.m_name.clear();
	m_currentKey.m_value.clear();
	m_currentKey.m_comment.clear();
}

IniParser::Parser::~Parser()
{
	pushKey();
	pushSection();
}


void IniParser::Parser::parseLine(const std::wstring& line)
{
	ParseLine pline = line.c_str();
	truncate(pline);


	switch (*pline)
	{
	case 0:
	case L';':
		getCurrentComments().push_back(line); break;
	case L'[':
		{
			pushSection();
			ParseLine start_name = pline + 1;
			ParseLine end_name = start_name;
			while (*end_name != L']' && *end_name != 0)
				end_name++;
			if (*end_name == 0 || end_name == start_name)
				throw ParsingError();

			pline = end_name + 1;
			truncate(pline);
			if (*pline)
				throw ParsingError();

			m_currentSection.m_name = std::wstring(start_name, end_name);
		}; break;
	default:
		{
			pushKey();
			ParseLine start_name = pline;
			while (*pline && *pline != L'\t' && *pline != L' ')
				++pline;
			ParseLine end_name = pline;
			truncate(pline);
			if (*pline != L'=')
				throw ParsingError();
			pline++;
			truncate(pline);
			m_currentKey.m_name = std::wstring(start_name, end_name);
			m_currentKey.m_value = std::wstring(pline);
		}
	}
}
