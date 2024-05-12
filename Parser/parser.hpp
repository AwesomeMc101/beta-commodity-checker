#pragma once
#include "classes.hpp"
#include "api.hpp"
#include "error_scanner.hpp"
#include "download_string.hpp"

#include <string.h>
#include <sstream>


#include <iostream>

struct Parsed_Data {
	int type = T_NIL;
	int interval = IN_NIL;
	int function = FUNC_NIL;

	bool record_dates = false;

	std::vector<float> price_data{};
	std::vector<std::string> dates{};
};

struct Unparsed_Data {
	std::string ticker = "AAPL";
	int type = T_NIL;
	int interval = IN_NIL;
	int function = FUNC_NIL;
};

#define SPLIT_URL_TOK "&";

namespace Parser
{

	namespace Type_Checker {
		bool isFloat(std::string); //check if string can work in std::stof
		bool isDate(std::string);

		/* verify if at least one occurence of type is found in data */
		bool canParse(std::string, int); //fully downloaded data, type(open,high,low)
		void eraseNonNumeric(std::string&, bool); //string, keep_dashes(for dates)
	}

	std::string create_url(std::string, int, int, int); //ticker, function, type, interval
	
	std::vector<std::string> parse_multi_type(int);

	std::string download_data(Unparsed_Data, bool); //stock ticker

	/* ticker, function, pricetype, interval*/
	Unparsed_Data craft_unparsed_data(std::string, int, int, int); //ticker, function, pricetype, interval

	Parsed_Data parse(Stock*, Unparsed_Data uD, PARSE_FUNCTION_TYPE ftype); //Stock ptr, type(open,high,low),
	//ex call: parse(stkptr, T_OPEN)

}
