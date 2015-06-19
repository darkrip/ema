#include "custom_packs_ini_configurator.hpp"



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

	setPackListProcessPattern(ListProcessPattern(m_iniParser.getStringList(index, L"Format"), m_iniParser.getString(index, L"Start"), m_iniParser.getString(index, L"End")));

	setPackIsArchiveCommand(ConsoleCommand(m_iniParser.getString(index, L"IsArchive")));
	setPackListCommand(ConsoleCommand(m_iniParser.getString(index, L"List")));
	setPackExtractCommand(ConsoleCommand(m_iniParser.getString(index, L"Extract")));
	setPackExtractWithPathCommand(ConsoleCommand(m_iniParser.getString(index, L"ExtractWithPath")));

	setPackTestCommand(ConsoleCommand(m_iniParser.getString(index, L"Test")));
	setPackDeleteCommand(ConsoleCommand(m_iniParser.getString(index, L"Delete")));
	setPackAddCommand(ConsoleCommand(m_iniParser.getString(index, L"Add")));
	setPackMoveCommand(ConsoleCommand(m_iniParser.getString(index, L"Move")));

	setPackInputString(ConsoleCommand(m_iniParser.getString(index, L"InputString")));

	setPackBatchUnpack(m_iniParser.getBool(index, L"BatchUnpack"));
	setPackSkipEmpty(m_iniParser.getBool(index, L"SkipEmpty"));
	setPackSkipDirs(m_iniParser.getBool(index, L"SkipDirsInFileList"));
	setPackSearchForUglyDirs(m_iniParser.getBool(index, L"SearchForUglyDirs"));
	setPackUnixPath(m_iniParser.getBool(index, L"UnixPath"));
	setPackDebug(m_iniParser.getBool(index, L"Debug"));

	parseCustomVariables(m_iniParser.getStringList(index, L"CustomVariable"));

	setPackLandFile(m_iniParser.getString(index, L"LandFile"));

	//PackerIdList pil;

}