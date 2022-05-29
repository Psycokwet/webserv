#include "logger.hpp"

void logger(std::string file_name, std::string log)
{
	std::ofstream	ofs((std::string(LOGS_FOLDER) + std::string("/") + file_name).c_str(), std::ios_base::app);
	if (!ofs.is_open())
	{
		std::cerr << "Logging impossible" << std::endl;
		return;
	}
	ofs << "#----------------------------- NEW LOG ----------------------#\n"<< log << std::endl;
	ofs.close();
}
