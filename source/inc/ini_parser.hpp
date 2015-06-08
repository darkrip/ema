#ifndef __INI_PARSER_HPP__
#define __INI_PARSER_HPP__

#include "defs.hpp"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <iostream>
#include <exception>
#include <list>

namespace ema
{
namespace config
{

/*
Parser of ini files, with supporting UTF16, UTF8 and currrent system Locale

Encoding detection:
	1) In exists BOM, using BOM encoding
	2) Try to find in stream first common symbols \s, \n, \r, \t, ';', '['
		if this symbols is multibyte - use UFT16 encoding,
		else use current system encoding
*/

class IniParser
{
public:
	class ParsingError : public std::exception
	{};

	typedef std::wstring SectionName;
	typedef const SectionName& SectionNameRef;
	typedef std::wstring KeyName;
	typedef const KeyName& KeyNameRef;
	typedef size_t SectionIndex;
	static const SectionIndex wrongIndex = (SectionIndex)-1;

	IniParser(const FileName&);

	SectionIndex getSectionIndex(SectionNameRef name)const;
	SectionIndex sectionCount()const{ return m_sections.size(); }
	SectionName getSectionName(SectionIndex index)const;
	void createSection(SectionNameRef, SectionIndex index = wrongIndex);
	void deleteSection(SectionIndex);


	bool isKeyExist(SectionIndex, KeyNameRef);
	void deleteKey(SectionIndex, KeyNameRef);

	std::wstring getString(SectionIndex, KeyNameRef, const std::wstring &defValue = L"");
	std::wstring getString(SectionNameRef, KeyNameRef, const std::wstring &defValue = L"");

	void setString(SectionIndex, KeyNameRef, const std::wstring &value);
	void setString(SectionNameRef, KeyNameRef, const std::wstring &value);

	void reload();
	void commit();

private:
	typedef std::list<std::wstring> Comment;

	struct Key
	{
		KeyName m_name;
		std::wstring m_value;
		Comment m_comment;
	};

	typedef boost::multi_index_container<Key,
		boost::multi_index::indexed_by<
			boost::multi_index::random_access<>,
			boost::multi_index::hashed_unique<
				boost::multi_index::member<
					Key, KeyName, &Key::m_name
				>
			>
		>
	> Keys;


	struct Section
	{
		SectionName m_name;
		Comment     m_comment;
		Keys        m_keys;
	};

	typedef boost::multi_index_container<Section,
		boost::multi_index::indexed_by<
			boost::multi_index::random_access<>,
			boost::multi_index::hashed_unique<
				boost::multi_index::member<
					Section, SectionName, &Section::m_name
				>
			>
		>
	> Sections;


	class Parser
	{
	public:
		Parser(IniParser&);
		~Parser();
		void parseLine(const std::wstring& line);
	private:
		typedef const wchar_t* ParseLine;
		void truncate(ParseLine&);
		void pushSection();
		void pushKey();
		Comment& getCurrentComments();
		IniParser& m_owner;
		Section m_currentSection;
		Key     m_currentKey;
	};
	friend class Parser;


	void detectEncoding(std::istream& iniFile);
	void parseStream(std::istream& iniFile);
	std::wstring loadString(std::istream&);


	std::locale m_iniFileLocale;
	Sections    m_sections;
	FileName    m_fileName;
	Comment		m_prefixComment;



};
}//config
}//ema








#endif//__INI_PARSER_HPP__