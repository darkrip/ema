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
	std::fstream iniFile(m_fileName, std::fstream::in);
	detectEncoding(iniFile);
	parseStream(iniFile);
}




void IniParser::parseStream(std::istream& iniFile)
{
	Parser parser(*this);
	while (!iniFile.eof())
	{
		std::wstring line = loadString(iniFile);
		parser.parseLine(line);
	}
}


void IniParser::detectEncoding(std::istream& iniFile)
{
	unsigned char bomUTF8[] ={0xEF, 0xBB, 0xBF};
	unsigned char bomUTF16[]={0xFE, 0xFF};
	unsigned char bomBuffer[3];


	iniFile>>bomBuffer;
}


std::wstring IniParser::loadString(std::istream&)
{
	return L"";
}


std::wstring IniParser::getString(SectionIndex index, KeyNameRef name, const std::wstring &defValue)
{
	if(index==wrongIndex || index>=sectionCount())
		return defValue;
	else
	{
		auto keyIt = m_sections.get<0>()[index].m_keys.get<1>().find(name);
		if(keyIt!=m_sections.get<0>()[index].m_keys.get<1>().end())
			return keyIt->m_value;
		else
			return defValue;
	}
}

std::wstring IniParser::getString(SectionNameRef section_name, KeyNameRef key_name, const std::wstring &defValue)
{
	auto sectionIt = m_sections.get<1>().find(section_name);
	if(sectionIt==m_sections.get<1>().end())
		return defValue;
	else
	{
		auto keyIt = sectionIt->m_keys.get<1>().find(key_name);
		if(keyIt!=sectionIt->m_keys.get<1>().end())
			return keyIt->m_value;
		else
			return defValue;
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
