#pragma once
#include "download_string.hpp"
#include "api.hpp"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#define COM_INTERVAL_DAILY 50
#define COM_INTERVAL_WEEKLY 51
#define COM_INTERVAL_MONTHLY 52
#define COM_INTERVAL_QUARTERLY 53
#define COM_INTERVAL_ANNUALLY 54

typedef unsigned int COM_INTERVAL_TYPE;

namespace Parse_Commodity {


	std::string get_interval(COM_INTERVAL_TYPE); //interval type ^^

	namespace Type_Checker {
		bool isFloat(std::string); //number
		void eraseNonNumeric(std::string& str);
		void eraseNonNumericDate(std::string& str); //for date
	}

	

	std::string generate_url(std::string, COM_INTERVAL_TYPE); //function name, interval

	void clear_metadata(std::string&);
	bool ensure_safe_data(std::string); //downloaded data

	std::vector<std::pair<std::string, long double>> date_parse_commodity(std::string, COM_INTERVAL_TYPE, std::string);
	std::string download_commodity_data(std::string, COM_INTERVAL_TYPE);
	std::vector<float> parse_commodity(std::string, COM_INTERVAL_TYPE, std::string); //function name, interval, downloaded_Data

}
