#include "WriteToTSV.h"

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

WriteToTSV::WriteToTSV ( std::vector<std::string> head )
{
	addDataLine ( head );
}

void WriteToTSV::addDataLine ( std::vector<std::string> line )
{
	std::stringstream ss;
	for (int i = 0; i < line.size ( ); ++i)
	{
		ss << line[i] << '\t';
	}
	ss << std::endl;
	m_dataLines += ss.str ( );
}

bool WriteToTSV::output ( const std::string& filepath )
{
	std::replace ( m_dataLines.begin ( ), m_dataLines.end ( ), '.', ',' );

	std::ofstream file ( filepath, std::ios::out | std::ios::trunc );
	if (!file.is_open())
	{
		return false;
	}
	for (auto& line : m_dataLines)
		file << line;
	file.close ( );
	return true;
}
