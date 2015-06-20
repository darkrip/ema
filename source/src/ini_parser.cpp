#include "ini_parser.hpp"
#include <fstream>
#include <vector>
#include <boost/locale.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "windows.h"

using namespace ema;
using namespace config;


const std::string utf16_encoding = "UTF-16";
const std::string utf8_encoding = "UTF-8";




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

bool IniParser::detectEncodingByBOM(std::istream& iniFile)
{
	typedef std::vector<unsigned char> BOM;
	struct BOMRecord
	{
		BOM      				   m_bom;
		std::string                m_encoding;
	};
	typedef std::vector<BOMRecord> BOMRecords;
	BOMRecords records = {
		{ { 0xEF, 0xBB, 0xBF }, utf8_encoding },
		{ { 0xFE, 0xFF }, utf16_encoding },
	};

	size_t maxBOMSize = std::max_element(records.begin(), records.end(),
		[](const BOMRecord& l, const BOMRecord& r){ return l.m_bom.size() < r.m_bom.size(); })
		->m_bom.size();

	BOM bom(maxBOMSize);
	for (auto it = bom.begin(); it != bom.end(); ++it)
	{
		char ch;
		iniFile.get(ch);
		*it = ch;
	}
		

	for (auto it = records.begin(); it != records.end(); ++it)
		if (it->m_bom == bom)
		{
			m_iniFileLocale = it->m_encoding;
			iniFile.seekg(it->m_bom.size());
			return true;
		}

	iniFile.seekg(0);
	return false;
}


void IniParser::detectEncoding(std::istream& iniFile)
{
	if (!detectEncodingByBOM(iniFile))
	{

		std::vector<wchar_t> expectedChars = { L' ', L'\n', L'\r', L'\t', L';', L'[' };

		wchar_t test;

		iniFile.read((char*)&test, sizeof(test));
		iniFile.seekg(0);

		auto it = std::find(expectedChars.begin(), expectedChars.end(), test);
		if (it != expectedChars.end())
		{
			m_iniFileLocale = utf16_encoding;
		}
		else
		{
			//TODO Dirty
			UINT langid = GetACP();
			m_iniFileLocale = "cp";
			m_iniFileLocale += std::to_string(langid);
		}
	}

	if (m_iniFileLocale == utf16_encoding)
	{
		char buf[sizeof(wchar_t)];
		memcpy(buf, L"\n", sizeof(buf));
		m_sn = std::string(buf, buf + sizeof(buf));
		memcpy(buf, L"\r", sizeof(buf));
		m_sr = std::string(buf, buf + sizeof(buf));
	}
	else
	{
		m_sn = boost::locale::conv::from_utf("\n", m_iniFileLocale);
		m_sr = boost::locale::conv::from_utf("\r", m_iniFileLocale);
	}
}


std::wstring IniParser::loadString(std::istream& iniStream)
{
	std::string buf;
	char ch=0;

	while (!iniStream.eof())
	{
		iniStream.get(ch);
		buf += ch;

		if (buf.size() >= m_sn.size() && buf.substr(buf.length() - m_sn.size()) == m_sn)
		{
			size_t epos = buf.length() - m_sn.size();
			if (buf.size() >= m_sn.size() + m_sr.size() &&
				buf.substr(buf.length() - m_sr.size() - m_sn.size(), buf.length() - epos) == m_sr)
				epos = buf.length() - m_sr.size() - m_sn.size();
			
			buf.erase(epos);
			break;
		}
	}
	
	std::wstring result;
	
	if (m_iniFileLocale == utf16_encoding)
	{
		buf += (char)0;
		result = (const wchar_t*)buf.c_str();
	}
	else
	{
		result = boost::locale::conv::to_utf<wchar_t>(buf, m_iniFileLocale);
	}
	

	return result;
}

bool IniParser::isKeyExist(SectionIndex index, KeyNameRef key_name)
{
	if (index == wrongIndex || index >= sectionCount())
		return false;

	return m_sections.get<0>()[index].m_keys.get<1>().find(key_name) != m_sections.get<0>()[index].m_keys.get<1>().end();
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

String IniParser::getString(SectionNameRef section_name, KeyNameRef key_name, const std::wstring &defValue)
{
	auto sectionIt = m_sections.get<1>().find(section_name);
	if(sectionIt==m_sections.get<1>().end())
		return defValue;
	else
		return getString(m_sections.get<0>().iterator_to(*sectionIt) - m_sections.get<0>().begin(), key_name, defValue);
}

bool IniParser::getBool(SectionIndex sectionIndex, KeyNameRef key_name, bool defValue)
{
	String str = getString(sectionIndex, key_name);
	boost::trim(str);
	if(str.empty())
		return defValue;
	if(str==L"1" || str==L"true" || str==L"t" || str==L"y" )
		return true;
	else if(str==L"0" || str==L"false" || str==L"f" || str==L"n" )
		return false;
	else 
		return defValue;
}

bool IniParser::getBool(SectionNameRef section_name, KeyNameRef key_name, bool defValue)
{
	auto sectionIt = m_sections.get<1>().find(section_name);
	if(sectionIt==m_sections.get<1>().end())
		return defValue;
	else
		return getBool(m_sections.get<0>().iterator_to(*sectionIt) - m_sections.get<0>().begin(), key_name, defValue);

	
}


StringList IniParser::getStringList(SectionIndex section_index, KeyNameRef key_name)
{
	StringList result;
	if (isKeyExist(section_index, key_name))
	{
		result.push_back(getString(section_index, key_name));
	} 
	else
		for (size_t i = 0;; ++i)
		{
			KeyName key_name_l = key_name + std::to_wstring(i);
			if (!isKeyExist(section_index, key_name_l))
				break;
			result.push_back(getString(section_index, key_name_l));
		};
	return result;
}

StringList IniParser::getStringList(SectionNameRef section_name, KeyNameRef key_name)
{
	auto sectionIt = m_sections.get<1>().find(section_name);
	if (sectionIt == m_sections.get<1>().end())
		return StringList();
	else
		return getStringList(m_sections.get<0>().iterator_to(*sectionIt) - m_sections.get<0>().begin(), key_name);
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
	pushKey();
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
	{
		m_currentKey.m_name.clear();
		throw ParsingError();
	}

	m_currentSection.m_keys.push_back(m_currentKey);
	m_currentKey.m_name.clear();
	m_currentKey.m_value.clear();
	m_currentKey.m_comment.clear();
}

IniParser::Parser::~Parser()
{
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
			while (*pline && *pline != L'\t' && *pline != L' ' && *pline != L'=')
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
