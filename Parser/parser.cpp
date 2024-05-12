#pragma once
#include "parser.hpp"
namespace Parser {
	namespace Type_Checker {
		bool isFloat(std::string data)
		{
			if (!data.length()) { return false; }
			int digit_count = 0;
			for (char c : data)
			{
				if (!isdigit(c))
				{
					if (c != ' ' && c != '.')
					{
						return false;
					}
					continue;
				}
				++digit_count;
			}
			return digit_count; //will return 0 if no digits
		}

		/* verify if at least one occurence of type is found in data */
		bool canParse(std::string downloaded_data, int type)
		{
			if (downloaded_data.find_first_of(get_price_type(type)) == std::string::npos)
			{
				return false;
			}
			return true;
		}

		bool isDate(std::string val) //locked in for this parser function
		{
			//std::cout << "isDate: " << val;
			int num_count=0, dash_count=0;
			for (char c : val)
			{
				if (isdigit(c)) { num_count++; continue; }
				else if (c == '-') { dash_count++; continue; }
			}

			//std::cout << ((num_count == 8) && (dash_count == 2)) << "\n";
			return ((num_count == 8) && (dash_count == 2));

		}

		void eraseNonNumeric(std::string& str, bool keep_dash)
		{
			std::string new_str = "";
			bool first_dec_passed = false; //first decimal is technically 5.
			if (str.find_first_of(".") == std::string::npos) {
				first_dec_passed = true; //NO DECIMAL EXISTS (MONTHLY DATA)
			}
			bool second_dec_passed = false;
			for (int i = 0; i < str.size(); i++)
			{
				char c = str.at(i);
				if (!first_dec_passed)
				{
					if (c == '.')
					{
						first_dec_passed = true;
					}
					continue;
				}

				if (isdigit(c))
				{
					new_str.push_back(c);
				}
				else if ((!second_dec_passed && c == '.'))
				{
					new_str.push_back(c);
					second_dec_passed = true;
				}
				else if (keep_dash && c == '-')
				{
					new_str.push_back(c);
				}
			}
			str = new_str;
		}
	}
	std::string create_url(std::string ticker, int function, int type, int interval)
	{
		//ticker, function, type, interval
		std::string url = "https://www.alphavantage.co/query?function=";
		url.append(get_function_type(function));

		url.append("&symbol=");
		url.append(ticker);

		if (type) //highkey fix this
		{
			url.append("&OHLC="); //or series_type
			url.append(get_price_type(type));
		}

		url.append("&interval=");
		url.append(get_interval_type(interval));

		url.append("&apikey=");
		url.append(get_key());

		std::cout << "URL: " << url << "\n";

		return url;
	}

	std::vector<std::string> parse_multi_type(int type)
	{
		switch (type)
		{
		case T_OPENCLOSE:
			return { "open","close" };
		case T_HIGHLOW:
			return { "high","low" };
		case T_ALL:
			return { "high","low","open","close","volume" };
		}
		Error_Scanner::push_error();
		return { "open" };
	}

	std::string download_data(Unparsed_Data uD, bool use_ohlc)
	{
		std::string downloaded_data = "";
		if (!use_ohlc) //likely more common, this case occurs when getting price data
		{
			downloaded_data = Internet::DownloadString(create_url(uD.ticker, uD.function, 0, uD.interval)); //skip OHLC clause
		}
		else {
			downloaded_data = Internet::DownloadString(create_url(uD.ticker, uD.function, uD.type, uD.interval));
		}

		if (downloaded_data.length() <= 1000) {
			Error_Scanner::push_error();
			return "";
		}

		std::string return_data = "";

		std::istringstream iss(downloaded_data);
		std::string line = "";
		size_t loc = 0;
		while (std::getline(iss, line))
		{
			loc = line.find("Time Series");
			if (loc != std::string::npos)
			{
				return_data = downloaded_data.substr(loc);
				break;
			}
		}

		if (return_data.length()) //good ending
		{
			return return_data;
		}

		Error_Scanner::push_warning();
		return downloaded_data;
	}

	Unparsed_Data craft_unparsed_data(std::string ticker, int function, int type, int interval) //ticker, function, pricetype, interval
	{
		Unparsed_Data uD;
		uD.ticker = ticker;
		uD.function = function;
		uD.type = type;
		uD.interval = interval;
		return uD;
	}


	Parsed_Data parse(Stock* stk, Unparsed_Data uD, PARSE_FUNCTION_TYPE ftype)
	{ //Stock ptr, type(open,high,low),



		//you could highkey do ftype = uD.function;
		if (!uD.ticker.length() || !uD.function || !uD.interval)
		{
			Error_Scanner::push_error();
			return *(new Parsed_Data);
		}

		Error_Scanner::scan_for_warnings(); //set static int
		std::string data = download_data(uD, (ftype == FTYPE_TECHNICAL)); //download price data, bool passed is TRUE if we are using technical

		if (Error_Scanner::scan_for_errors())
		{
			Error_Scanner::push_error(); //inc error count so static int throws again
			return *(new Parsed_Data);
		}
		else if (Error_Scanner::scan_for_warnings())
		{
			//fuck bruh we didnt clear metadata
		}

		if (!Type_Checker::canParse(data, uD.type))
		{
			Error_Scanner::push_error();
			return *(new Parsed_Data);
		}

		bool is_multi = multi_price_type(uD.type);

		std::vector<std::string> target_types{};

		if (is_multi) {
			target_types = parse_multi_type(uD.type); //get multiple types
			if (Error_Scanner::scan_for_errors())
			{
				//we didnt have a valid multi type.
				std::cout << "INVALID MUTLI TYPE";
			}
		}
		else
		{
			target_types.push_back(get_price_type(uD.type));
		}

		if (stk->use_dates)
		{
			target_types.push_back("DATE_SCANNER");
			is_multi = true; //override
		}

		if (!target_types.size())
		{
			Error_Scanner::push_error();
			return *(new Parsed_Data);
		}
		//std::string target_type = get_price_type(uD.type);

		Parsed_Data pD;

		if (!is_multi) { //single, no need to slow down w/ loop
			std::istringstream iss(data);
			std::string line = "";
			size_t loc = 0;
			while (std::getline(iss, line))
			{
				if (line.find(target_types[0]) != std::string::npos) //if we can find type "OPEN" (for example) in line
				{
					//ok
					Type_Checker::eraseNonNumeric(line, false); //get rid of chars that arent price
					if (Type_Checker::isFloat(line)) //check if float
					{
						float topush = 0; //holster var
						try {
							topush = std::stof(line); //stof of line
							pD.price_data.push_back(topush); //push back to price data vector
						}
						catch (std::exception& e)
						{
							Error_Scanner::push_error();
							return pD;
						}
					}
				}
			}
			if (!pD.price_data.size())
			{
				Error_Scanner::push_error();
			}
			return pD;
		}


		//multi
		std::istringstream iss(data);
		std::string line = "";
		size_t loc = 0;
		while (std::getline(iss, line))
		{
			for (std::string target_type : target_types) {
				if (target_type == "DATE_SCANNER")
				{
					//if(line[0] == '"' && )
					if (Type_Checker::isDate(line))
					{
						//std::cout << "Date: " << line << "\n";
						Type_Checker::eraseNonNumeric(line, true);//keep dashes
						//std::cout << "Newdata: " << line << "\n";
						pD.dates.push_back(line);
					}
					break;

				}
				else if (line.find(target_type) != std::string::npos) //if we can find type "OPEN" (for example) in line
				{
					//ok
					Type_Checker::eraseNonNumeric(line, false); //get rid of chars that arent price
					if (Type_Checker::isFloat(line)) //check if float
					{
						float topush = 0; //holster var
						try {
							topush = std::stof(line); //stof of line
							pD.price_data.push_back(topush); //push back to price data vector
						}
						catch (std::exception& e)
						{
							Error_Scanner::push_error();
							return pD;
						}
					}
					break; //end loop case early
				}
			}
		}
		if (!pD.price_data.size())
		{
			Error_Scanner::push_error();
		}
		return pD;

	}



}
