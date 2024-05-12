#include "quartely_percent_comp.hpp"


namespace QPC_API {
	std::string get_secondary_type(QPC_API_TYPE qat_)
	{
		switch (qat_) {
		case QPC_API_GROSSPROFIT:
			return "grossProfit";
		case QPC_API_NETINCOME:
			return "netIncome";
		case QPC_API_EBITDA: //EARNINGS BEFORE TAX, INTEREST, DEPRECIATION, AND amortization
			return "ebitda";
		case QPC_API_TOTALREVENUE:
			return "totalRevenue";
		case QPC_SAPI_PRICE:
			return "SPEC_PRICE";
		}
		return "netIncome";
	}
}

namespace Oil_Quarterly /* use double here as we need more data siz*/
{
	namespace Utilities {
		std::string craft_qpc_url(std::string ticker)
		{
			std::string url = "https://www.alphavantage.co/query?function=INCOME_STATEMENT&symbol=";
			url.append(ticker);
			url.append("&apikey=");
			url.append(get_key());
			return url;
		}

		std::string parse_data(std::string bufferdata, size_t loc) //parse_date**
		{
			std::string substrd = bufferdata.substr(loc);
			std::string returnvalue = ""; //inefficient but fuck it
			for (char c : substrd)
			{
				if (isdigit(c) || c == '-')
				{
					returnvalue.push_back(c);
				}
			}
			if (returnvalue.size()) { return returnvalue; }
			
			std::cout << "(PD) (QPC.cpp) PARSE_DATA(E) FAILURE: " << bufferdata << "\n";

			if (bufferdata.find("None") != std::string::npos) {
				Error_Scanner::push_warning();
				std::cout << "(PD) (QPC.cpp) PARSE_DATE FIXTURE - 'NONE' LOCATED. PUSH 0?\n";
				return "0";
			}

			Error_Scanner::push_error();
			return "";
		}

		bool is_number(std::string buffer)
		{
			if (!buffer.length()) { return false; }
			for (char c : buffer)
			{
				if (!isdigit(c)) { return false; }
			}
			return true;
		}
	}
	OQ_DOUBLE calculate_quartely_correlation(std::vector<OQ_DOUBLE>, std::string); //stock income data, oil function name

	std::vector<std::pair<std::string, OQ_DOUBLE>> parse_stock_data(Stock* stk, QPC_REPORTTYPE reptype, QPC_API_TYPE qat_type)//download string parser
	{
		std::string url = Utilities::craft_qpc_url(stk->ticker);
		std::string Downloaded_Data = Internet::DownloadString(Utilities::craft_qpc_url(stk->ticker));
		std::string return_data = "";
		Error_Scanner::scan_for_errors();
		switch (reptype)
		{
		case QPC_REPORT_ANNUAL:
		{
			size_t loc = Downloaded_Data.find("quarterlyReports");
			if (loc != std::string::npos)
			{
				return_data = Downloaded_Data.substr(0, loc);
			}
			else
			{
				Error_Scanner::push_error();
				std::cout << "(PSD) (QPC.cpp) FAILED TO FIND QUARTERLY REPORTS (ANNUAL)\n";
				std::cout << "(PSD) URL: " << url << "\n";
				//return_data = "nil";
			}
			break;
		}
		case QPC_REPORT_QUARTERLY:
		{
			size_t loc = Downloaded_Data.find("quarterlyReports");
			if (loc != std::string::npos)
			{
				return_data = Downloaded_Data.substr(loc);
			}
			else
			{
				std::cout << "(PSD) (QPC.cpp) FAILED TO FIND QUARTERLY REPORTS\n";
				std::cout << "(PSD) URL: " << url << "\n";
				Error_Scanner::push_error();
				//return_data = "nil";
			}
		}
		}

		if (Error_Scanner::scan_for_errors() || !return_data.size()) { printf("OGreturn"); return {}; }

		std::istringstream iss(return_data);
		std::string buffer;

		std::string datebuffer;

		std::vector<std::pair<std::string, OQ_DOUBLE>> ret_Vector{};

		if (qat_type == QPC_SAPI_PRICE)
		{
			std::vector<std::string> t_dates;
			while (std::getline(iss, buffer)) //holy fuck
			{
				//std::cout << buffer;
				size_t fiscal = buffer.find("fiscalDateEnding");
				if (fiscal != std::string::npos)
				{
					//printf("\n\nfiscal found\n\n");
					std::string temp = Utilities::parse_data(buffer, fiscal);
					if (!Error_Scanner::scan_for_errors())
					{
						//ret_Vector.push_back(std::make_pair(temp, 0));
						t_dates.push_back(temp);
					}
				}
			}
			if (!t_dates.size()) {
				std::cout << "(PSD) (QPC.cpp) T-DATES<> SIZECALL\n";
				Error_Scanner::push_error();
				return{};
			}

			int size_diff = 1;

			if (stk->price_data.size() > stk->dates.size())
			{
				//likely used more than 1 type 
				if (!stk->dates.size()) { Error_Scanner::push_error(); return{}; }
				size_diff = ceil(stk->price_data.size() / stk->dates.size());
			}
			else if (stk->dates.size() > stk->price_data.size()) //actual problem here
			{
				std::cout << "(PSD) (QPC.cpp) STK.DATES > STK.PRICES\n";
				Error_Scanner::push_error();
				return{};
			}

			for (std::string t_date : t_dates)
			{
				for (int i = 0; i < stk->dates.size(); i++)
				{
					//std::string s_date = stk->dates[i];
					if (stk->dates[i] == t_date)
					{
						//correct date
						int price_idx = i * size_diff; //if 2 prices for every date, 
						//date idx = 1,2,3,4,5
						//price idx = 2,4,6,8,10
						if (price_idx >= stk->price_data.size()) { continue; }
						ret_Vector.push_back(std::make_pair(t_date, stk->price_data[price_idx]));
					}
				}
			}


			return ret_Vector;
			
		}

		std::string secondary_type = QPC_API::get_secondary_type(qat_type);

		

		while (std::getline(iss, buffer)) //holy fuck
		{
			//std::cout << buffer;
			size_t fiscal = buffer.find("fiscalDateEnding");
			if (fiscal != std::string::npos)
			{
				//printf("\n\nfiscal found\n\n");
				std::string temp = Utilities::parse_data(buffer, fiscal); //parse DATE
				if (!Error_Scanner::scan_for_errors())
				{
					datebuffer = temp;
				}
			}
			else
			{
				size_t netincome = buffer.find(secondary_type);
				if (netincome != std::string::npos)
				{
					//printf("\nnetincome found\n");
					if (secondary_type == "netIncome")
					{
						size_t nettest = buffer.find("netIncomeFromContinuingOperations");
						if (nettest != std::string::npos)
						{
							continue;
						}
					}
					std::string temp = Utilities::parse_data(buffer, netincome); //parse DATE
					if (!Error_Scanner::scan_for_errors() && Utilities::is_number(temp))
					{
						if (datebuffer.size())
						{
							try {
							//	std::cout << "PB: " << datebuffer << " && " << temp << "\n";
								ret_Vector.push_back(std::make_pair(datebuffer, std::stold(temp)));
							}
							catch (std::exception& e)
							{
								std::cout << "(PSD) (QPC.cpp) FAILED TO STOLD/PUSHBACK REGULAR QUARTERLY DATA\n";
								Error_Scanner::push_error();
							}
							datebuffer.clear();
						}
					}
				}
			}
		}

		if (ret_Vector.size()) { return ret_Vector; } //GOOD
		printf("bad return");
		Error_Scanner::push_error();
		return {};

	}

	std::vector<std::pair<OQ_DOUBLE, OQ_DOUBLE>> find_required_oil_prices(QPC_OIL_TYPE oil_type, std::vector<std::string> dates, std::vector<OQ_DOUBLE> eq_prices)
	{
		std::string oil_function = "";
		Oil_Template* oil = Commodity_Main::Oils::grab_oilptr_by_name(get_commodity_name(oil_type)); //get a ptr to the right oil type by converting oil_type to its string and inputting it to grab_oilptr

			/* ensure oil commodity is SET and stock is SET */
		if (!oil->enabled || !oil->dated_enabled) { //we need dated_enabled too
			std::cout << "REQ FAILURE;";
			Error_Scanner::push_error();
			return {};
		}

		if (eq_prices.size() < dates.size())
		{
			std::cout << "FROP EQ < DATES (QPC)\n";
			Error_Scanner::push_error();
			return {};
		}

		/* Do we make oil auto parse prices too now... or just redownload oil. CRY ASF WE NEED TO MAKE OG OIL AUTOPARSE DATES AAAAAAAA FUCKER */
		//make another vector tbh dont break old code. just make prices write to 2 diff vectors lmao, more data == less time spent :) 
		//^^ fuckit we ball i did it
		std::vector<std::pair<OQ_DOUBLE, OQ_DOUBLE>> required_prices{};
		for (int i = 0; i < dates.size(); i++)
		{
			std::string quarter_date = dates[i];
			for (auto pair : oil->dated_prices)
			{
				if (pair.first == quarter_date)
				{
					//std::cout << "[" << get_commodity_name(oil_type) << "]: " << pair.first << " || " << pair.second << "\n";
					required_prices.push_back(std::make_pair(eq_prices[i], pair.second));
					continue;
				}
			}
		}

		if (!required_prices.size())
		{
			//bad sizecall
			std::cout << "FROP Sizecall (QPC)\n";
			Error_Scanner::push_error();
			return {};
		}

		return required_prices;
	}

	void auto_resize(std::vector<OQ_DOUBLE>& v_a, std::vector<OQ_DOUBLE>& v_b)
	{
		if (v_a.size() == v_b.size()) { return; }
		bool a_larger = (v_a.size() > v_b.size());

		switch (a_larger)
		{
		case true:
			//a is larger
			v_a.resize(v_b.size()); //make a smaller (same size as B)
			break;
		case false:
			v_b.resize(v_a.size()); //make b smaller (same size as A)
			break;
		}
		return;
	}

	std::vector<OQ_DOUBLE> do_quarterly_pcomp(Stock* stk, QPC_API_TYPE qat_Data)  //main function
	{
		std::vector<std::pair<std::string, OQ_DOUBLE>> all_quarterly_data = parse_stock_data(stk, QPC_REPORT_QUARTERLY, qat_Data); //{(date, netincome)}
		if (Error_Scanner::scan_for_errors()) { Error_Scanner::push_error(); return {}; } //check for errors in getting quarterly data

		//now we need to check for oil prices for each of the dates we have in the vector
		//vector.first

		std::vector<std::string> dates{};
		std::vector<OQ_DOUBLE> price_points{};
		for (auto pair : all_quarterly_data) //auto is my bbg
		{
			dates.push_back(pair.first);
			price_points.push_back(pair.second);
		}

		if (!dates.size() || !price_points.size()) { Error_Scanner::push_error(); return{}; }
		

		for (auto commodity : stk->commodity_associations)
		{
			//std::vector<OQ_DOUBLE> oil_prices = find_required_oil_prices(commodity, dates);
			std::vector<std::pair<OQ_DOUBLE, OQ_DOUBLE>> price_pairs = find_required_oil_prices(commodity, dates, price_points);
			if (Error_Scanner::scan_for_errors()) { Error_Scanner::push_error();  continue; } //dont necessarily return...

			/*
				we now have the following data
			 - pricepoints{} = stock prices [[deprecated??]]
			 - oil_prices{} = PAIR OF OIL AND EQUITY PRICES
			 */

			

			//if (price_points.size() != oil_prices.size())
			//{
			//	Error_Scanner::push_warning();
			//	auto_resize(price_points, oil_prices); //no reason to waste time doing this if they're the same size.
			//}

			/*
			* equity_changes = % changes for all equities over course of time (GROSS PROoilIT)
			* oil_changes = % changes for all oil prices over course of time
			*/

			std::vector<float> equity_prices, oil_prices;

			for (auto pair : price_pairs)
			{
				equity_prices.push_back(pair.first);
				oil_prices.push_back(pair.second);
			}

			std::vector<OQ_DOUBLE> equity_changes = Mathematics::get_all_percent_changes(equity_prices);
			std::vector<OQ_DOUBLE> oil_changes = Mathematics::get_all_percent_changes(oil_prices);

			/* 
			* reinterpret vectors as floats to work with math lib
			* OMG WONT THIS DESTROY THE WHOLE POINT OMGOMG
			*  ^^ No, all the values are rounded the same way :)
			*/
			std::vector<float> equity_float_data(equity_changes.begin(), equity_changes.end());
			std::vector<float> oil_float_data(oil_changes.begin(), oil_changes.end());

			float correlation_coefficient = Mathematics::correlation_coefficient(equity_float_data, oil_float_data, 0);
			
			stk->oil_QPC_values.push_back(correlation_coefficient);

		}

		if (stk->oil_QPC_values.size())
		{
			std::vector<float> conv(stk->oil_QPC_values.begin(), stk->oil_QPC_values.end()); //ts makes me want to quit C++
			return (conv);
		}

		std::cout << "(QPC) NO QPC VALUES\n";

		Error_Scanner::push_error();
		return{};

	}

	void do_qac_vector(Stock* stk, std::vector<QPC_API_TYPE> qac_vector)
	{
		for (QPC_API_TYPE qat_type : qac_vector) {
			Oil_Quarterly::do_quarterly_pcomp(stk, qat_type);
		}

	}

}
