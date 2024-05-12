#pragma once
#include "parse_commodity.hpp"

namespace Parse_Commodity
{
	std::string get_interval(COM_INTERVAL_TYPE interval)
	{
		//interval type ^^
		switch (interval)
		{
		case COM_INTERVAL_DAILY:
			return "daily";
		case COM_INTERVAL_WEEKLY:
			return "weekly";
		case COM_INTERVAL_MONTHLY:
			return "monthly";
		case COM_INTERVAL_QUARTERLY:
			return "quarterly";
		case COM_INTERVAL_ANNUALLY:
			return "annual";
		}

		Error_Scanner::push_warning();

		return "daily";//shouldnt happen
	}

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

		void eraseNonNumeric(std::string& str)
		{
			std::string new_str = "";
			bool first_dec_passed = true; //first decimal is technically 5.
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
			}
			str = new_str;
		}
		void eraseNonNumericDate(std::string& str)
		{
			std::string new_str = "";
			for (int i = 0; i < str.size(); i++)
			{
				char c = str.at(i);
				

				if (isdigit(c))
				{
					new_str.push_back(c);
				}
				else if (c == '-')
				{
					new_str.push_back(c);
				}
			}
			str = new_str;
		}
	}

	std::string generate_url(std::string function_type, COM_INTERVAL_TYPE interval) //remember... function type is just like BRENT, WTI, COPPER etc
	{
		std::string url = "https://www.alphavantage.co/query?function=";
		
		url.append(function_type);

		url.append(ENTITLEMENT_DELAYED);
		
		url.append("&interval=");
		url.append(get_interval(interval));

		url.append("&apikey=");
		url.append(get_key());
		return url;
	}

	void clear_metadata(std::string& data)
	{
		size_t loc = data.find_first_of("data");
		data = data.substr(loc);//lol
	}

	bool ensure_safe_data(std::string data) { //downloaded data //CALL BEFORE CLEARMETADATA
		if (data.length() < 800)
		{
			return false;
		}
		size_t loc = data.find_first_of("data");
		if (loc == std::string::npos)
		{
			return false;
		}

		return true;
	}

	std::string download_commodity_data(std::string function_type, COM_INTERVAL_TYPE interval)
	{
		std::string url = generate_url(function_type, interval);
		if (!url.length()) //check we have a URL at all
		{
			//printf("NO URL LENGTH\n");
			std::cout << "[" << function_type << "] | NO URL LENGTH.\n";
			Error_Scanner::push_error();
			return "";
		}

		std::string data = Internet::DownloadString(url);
		if (!ensure_safe_data(data)) //check that data is safe
		{
			std::cout << "[" << function_type << "] | UNSAFE DATA.\n";
			Error_Scanner::push_error();
			return "";
		}

		clear_metadata(data); //clear metadata
		return data;
	}
	std::vector<std::pair<std::string, long double>> date_parse_commodity(std::string function_type, COM_INTERVAL_TYPE interval, std::string downloaded_data)
	{
		std::istringstream iss(downloaded_data);
		std::string buffer = "";
		std::string date_buffer = "";
		std::vector<std::pair<std::string, long double>> return_vector;
		while (std::getline(iss, buffer))
		{
			size_t loc = buffer.find("date");
			if (loc != std::string::npos)
			{
				Type_Checker::eraseNonNumericDate(buffer);
				date_buffer = buffer;
			}
			else if (((loc = buffer.find("value")) != std::string::npos) && date_buffer.length())
			{
				Type_Checker::eraseNonNumeric(buffer);
				if (Type_Checker::isFloat(buffer)) {
					//std::cout << "\n" << function_type << "PB: " << date_buffer << " && " << buffer;
					return_vector.push_back(std::make_pair(date_buffer, std::stold(buffer)));
				}
				date_buffer.clear();
			}
		}
		if (return_vector.size()) {
			return return_vector;
		}
		Error_Scanner::push_error();
		return {};
	}
	std::vector<float> parse_commodity(std::string function_type, COM_INTERVAL_TYPE interval, std::string downloaded_data)
	{
		//std::string url = generate_url(function_type, interval);
		//if (!url.length()) //check we have a URL at all
		//{
		//	//printf("NO URL LENGTH\n");
		//	std::cout << "[" << function_type << "] | NO URL LENGTH.\n";
		//	Error_Scanner::push_error();
		//	return { 0 };
		//}

		//std::string data = Internet::DownloadString(url);
		//if (!ensure_safe_data(data)) //check that data is safe
		//{
		//	std::cout << "[" << function_type << "] | UNSAFE DATA.\n";
		//	Error_Scanner::push_error();
		//	return { 0 };
		//}

		//clear_metadata(data); //clear metadata

		std::istringstream datastrean(downloaded_data); //string stream to read from
		std::string line = ""; //line for getline() to write to
		std::vector<float> price_data{}; //finally, initialize our price vector which is pushed back into when a price is good.


		while (std::getline(datastrean, line))
		{
			if (line.find("value") != std::string::npos)
			{
				//std::cout << "original Line: " << line << "\n";
				Type_Checker::eraseNonNumeric(line);
				if (Type_Checker::isFloat(line))
				{
					try {
						price_data.push_back(std::stof(line));
					}
					catch (std::exception& e)
					{
						Error_Scanner::push_error();
					}
				}
				else
				{
					//printf("NOT FLOAT\n");
					//std::cout << "NF Line: " << line << "\n";
					//Error_Scanner::push_error();
				}
			}
		}

		if (price_data.size()) { //what we want
			return price_data;
		}

		Error_Scanner::push_error();
		return { 0 };

	}

}
