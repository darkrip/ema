#include "custom_packs_ini_configurator.hpp"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/bind.hpp>

using namespace ema;
using namespace config;


CustomPackerIniConfigurator::CustomPackerIniConfigurator(const FileName& inifilename):
	m_iniParser(inifilename)
{
	for(size_t index = 0; index < m_iniParser.sectionCount(); ++index)
		createPacker(index);
}


void CustomPackerIniConfigurator::createPacker(size_t index)
{
	std::wstring name = m_iniParser.getSectionName(index);
	construct(name);

	setPackArchiverEncoding(m_iniParser.getString(index, L"Archiver"));

	std::wstring sID=m_iniParser.getString(index, L"ID");
	std::wstring sIDPos=m_iniParser.getString(index, L"IDPos");
	std::wstring sIDSeekRange=m_iniParser.getString(index, L"IDSeekRange");
	std::wstring sExcludeIDs=m_iniParser.getString(index, L"ExcludeIDs");
	
	parsePackIdList(sID, sIDPos, sIDSeekRange, sExcludeIDs);

	setPackDescription(m_iniParser.getString(index, L"Description"));

	parseExtensions(m_iniParser.getString(index, L"Extension", 
		m_iniParser.getString(index, L"Extensions")));

	setArchiver(m_iniParser.getString(index, L"Description"));
	setPackIconPath(m_iniParser.getString(index, L"Icon"));

	setPackListCommandHandler(console::ListCommandHandler(
		m_iniParser.getStringList(index, L"Format"), 
		m_iniParser.getString(index, L"Start"), 
		m_iniParser.getString(index, L"End"),
		m_iniParser.getStringList(index, L"IgnoreString")));

	setPackIsArchiveCommand(console::ConsoleCommand(m_iniParser.getString(index, L"IsArchive")));
	setPackListCommand(console::ConsoleCommand(m_iniParser.getString(index, L"List")));
	setPackExtractCommand(console::ConsoleCommand(m_iniParser.getString(index, L"Extract")));
	setPackExtractWithPathCommand(console::ConsoleCommand(m_iniParser.getString(index, L"ExtractWithPath")));

	setPackTestCommand(console::ConsoleCommand(m_iniParser.getString(index, L"Test")));
	setPackDeleteCommand(console::ConsoleCommand(m_iniParser.getString(index, L"Delete")));
	setPackAddCommand(console::ConsoleCommand(m_iniParser.getString(index, L"Add")));
	setPackMoveCommand(console::ConsoleCommand(m_iniParser.getString(index, L"Move")));

	setPackInputString(m_iniParser.getString(index, L"InputString"));

	setPackBatchUnpack(m_iniParser.getBool(index, L"BatchUnpack"));
	setPackSkipEmpty(m_iniParser.getBool(index, L"SkipEmpty"));
	setPackSkipDirs(m_iniParser.getBool(index, L"SkipDirsInFileList"));
	setPackSearchForUglyDirs(m_iniParser.getBool(index, L"SearchForUglyDirs"));
	setPackUnixPath(m_iniParser.getBool(index, L"UnixPath"));
	setPackDebug(m_iniParser.getBool(index, L"Debug"));

	parseCustomVariables(m_iniParser.getStringList(index, L"CustomVariable"));

	parseLanguageFile(m_iniParser.getString(index, L"LanguageFile"));
}


void CustomPackerIniConfigurator::parsePackIdList(StringRef sID, StringRef sIDPos, StringRef sIDSeekRange, StringRef sExcludeIDs)
{
	pack::PackerIdList pil;
	//TODO:

	setPackIdList(pil);
}


void CustomPackerIniConfigurator::parseExtensions(StringRef extensions)
{
	StringList exts;
	boost::split(exts, extensions, boost::is_any_of(L","));
	std::for_each(exts.begin(), exts.end(), boost::bind(&boost::trim<String>,
		_1, std::locale()));

	std::for_each(exts.begin(), exts.end(), [](String& str){ 
		if (str[0] == L'.')str.erase(0, 1);	});

	setPackExtensions(exts);
}


void CustomPackerIniConfigurator::parseCustomVariables(StringListRef strings)
{
	//TODO:
}

void CustomPackerIniConfigurator::parseLanguageFile(StringRef)
{
	//TODO:
}