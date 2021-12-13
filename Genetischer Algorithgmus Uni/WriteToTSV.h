#pragma once
#include <vector>
#include <string>

class WriteToTSV
{
private:
	std::string m_dataLines{""};
public:
	WriteToTSV ( std::vector<std::string> head );
	void addDataLine ( std::vector<std::string> line );
	bool output ( const std::string& filepath );
};

