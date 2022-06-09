#include "logger.hpp"

void logger(std::string file_name, std::string log)
{
	std::ofstream ofs((std::string(LOGS_FOLDER) + std::string("/") + file_name).c_str(), std::ios_base::app);
	if (!ofs.is_open())
	{
		std::cerr << "Logging impossible" << std::endl;
		return;
	}
	ofs << std::endl
		<< "#----------------------------- NEW LOG ----------------------#\n"
		<< log << std::endl;
	ofs.close();
}

void one_file_logger_int(std::string src, std::string log)
{
	if (DEBUG)
		std::cout << log;
	std::ofstream ofs((std::string(LOGS_FOLDER) + std::string("/") + ONE_FILE_LOGS).c_str(), std::ios_base::app);
	if (!ofs.is_open())
	{
		std::cerr << "Logging impossible" << std::endl;
		return;
	}
	ofs << std::endl
		<< "#----------------------------- NEW LOG FROM " << src << " ----------------------#\n"
		<< log << std::endl;
	ofs.close();
}

void one_file_logger_raw(std::string log)
{
	if (DEBUG)
		std::cout << log;
	// std::ofstream	ofs((std::string(LOGS_FOLDER) + std::string("/") + ONE_FILE_LOGS).c_str(), std::ios_base::app);
	// if (!ofs.is_open())
	// {
	// 	std::cerr << "Logging impossible" << std::endl;
	// 	return;
	// }
	// ofs << log;
	// ofs.close();
}
