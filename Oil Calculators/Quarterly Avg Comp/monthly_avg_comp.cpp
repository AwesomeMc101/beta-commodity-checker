#include "monthly_avg_comp.hpp"

namespace Quarterly_Avg_Comp {

	/* create the oil price vector between a set of dates. */
	static int greatest_iterator = 0;
	std::vector<float> get_oil_price_vector(std::string min_date, std::string max_date, unsigned short commodity_type)
	{
		Oil_Template* commodity = Commodity_Main::Oils::grab_oilptr_by_name(get_commodity_name(commodity_type));
		bool write = false;
		
		//order doesn't matter as we're just using to calc avg.
		std::vector<float> return_vector{};
		//static int greatest_iterator = 0;
		for (int i = 0; i < commodity->dated_prices.size(); i++)
		{
			//std::cout << max_date << " or " << min_date << " vs " << commodity->dated_prices[i].first << "\n";
			if (commodity->dated_prices[i].first == max_date || commodity->dated_prices[i].first == min_date)
			{
				//printf("\n\nswithc\n\n");
				//abort();
				write = !write; //switch write value
				greatest_iterator = i;
			}

			switch (write)
			{
			case true:
				//cast to float because dated_prices uses long double.
				return_vector.push_back((float)commodity->dated_prices[i].second);
				break;
			}
		}
		
		if (return_vector.size()) { return return_vector; }
		printf("\n\n\nOIL VEC ERROR\n\n\n");
		Error_Scanner::push_error();
		return{};

	}
	std::vector<QAC_PAIR> get_equity_reports(Stock* stk, QPC_REPORTTYPE rep_type) //stock, report_type
	{
		std::vector<std::pair<std::string, OQ_DOUBLE>> all_quarterly_data = Oil_Quarterly::parse_stock_data(stk, QPC_REPORT_QUARTERLY, rep_type); //{(date, netincome)}
		if (!all_quarterly_data.size()) {
			Error_Scanner::push_error();
			return{};
		}
		std::vector<QAC_PAIR> return_vector(all_quarterly_data.begin(), all_quarterly_data.end());
		return return_vector;
	}
	void do_qavg_comp(Stock* stk_ptr)
	{
		////std::vector<QAC_PAIR> equity_report_prices = get_equity_reports(stk_ptr, QPC_REPORT_QUARTERLY);
		//if (Error_Scanner::scan_for_errors()) {
		//	Error_Scanner::push_error();
		//	return 0.0;
		//}
		if (!stk_ptr->monthly_price_data.size())
		{
			Error_Scanner::push_error();
			return;
		}

		for (auto commodity : stk_ptr->commodity_associations) {
			//std::vector<std::vector<float>> aligned_oil_prices{};
			//std::vector<std::pair<float, float>> price_and_oil_averages{}; //PRICE, AVG GOING INTO THAT PRICE
			std::vector<float> equity_prices, avg_oil_prices;
			// 
			// 
			//std::string commodity_name = get_commodity_name(commodity);
			for (int iterator = 0; iterator < stk_ptr->monthly_price_data.size(); iterator++)
			{
				if (iterator >= (stk_ptr->monthly_price_data.size() - 1)) { continue; }
				/*
				idx.0 = latest
				idx.1 = earlier
				*/
				
				std::string min_date = stk_ptr->monthly_price_data[iterator + 1].first; //beginning date
				std::string max_date = stk_ptr->monthly_price_data[iterator].first; //later date

				float avg = Mathematics::average(get_oil_price_vector(min_date, max_date, commodity));
				//price_and_oil_averages.push_back(std::make_pair(stk_ptr->monthly_price_data[iterator].second, avg));
				equity_prices.push_back(stk_ptr->monthly_price_data[iterator].second);
				avg_oil_prices.push_back(avg);

				//the avg is the price goinginto the max date
				// 
				//aligned_oil_prices.push_back(get_oil_price_vector(min_date, max_date, commodity));
			}

			greatest_iterator = 0;

			float correlation = Mathematics::correlation_coefficient(equity_prices, avg_oil_prices, 0);
			std::cout << "\nCorrelation: " << correlation << "\n";
			stk_ptr->oil_MAC_values.push_back(correlation);
		}

	}
}
