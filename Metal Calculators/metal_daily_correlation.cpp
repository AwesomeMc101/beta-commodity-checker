#include "metal_daily_correlation.hpp"

namespace Mineral_Daily_Correlation {

	void auto_resize(std::vector<float>& v_a, std::vector<float>& v_b)
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

	float get_mineral_daily_correlation(Stock* stk_ptr, Mineral_Template* min_ptr)
	{
		//define vectors that can be deleted because we dont wanna edit our main price data by resizing it!

		std::vector<float> mineral_scoped_prices = min_ptr->daily_prices, 
			stock_scoped_prices = stk_ptr->price_data; //we do not want to get rid of the ORIGINAL prices stored in the classes pointed to

		//resize it
		auto_resize(mineral_scoped_prices, stock_scoped_prices); //ensure same size

		if (!stock_scoped_prices.size()) { Error_Scanner::push_error(); return {}; } //since sizes are same, no need to check both.

		//erase first/last values so mineral values are associated with TOMORROWS data (Predictive Analysis)
		mineral_scoped_prices.erase(mineral_scoped_prices.begin());
		stock_scoped_prices.pop_back(); //balance out for covariance.
		if (!mineral_scoped_prices.size()) { Error_Scanner::push_error(); return 0; }
		if (!stock_scoped_prices.size()) { Error_Scanner::push_error(); return 0; }
		
		float correlation_coefficient = Mathematics::correlation_coefficient(mineral_scoped_prices, stock_scoped_prices, 0);
		return correlation_coefficient;
	}

	void do_mineral_daily(Stock* stk)
	{
		for (COMMODITY_TYPE mineral_type : stk->commodity_associations)
		{
			//Commodity_Main::Minerals::
			Error_Scanner::scan_for_errors();
			Mineral_Template* min_ptr = Commodity_Main::Minerals::grab_mineralptr_by_type(mineral_type);

			if (Error_Scanner::scan_for_errors())
			{
				//mineral_ptr failure
				Error_Scanner::push_error();
				continue;
			}
			if (!min_ptr->enabled)//disabled
			{
				Error_Scanner::push_error();
				continue;
			}

			float correlation = get_mineral_daily_correlation(stk, min_ptr);
			if (!Error_Scanner::scan_for_errors())
			{
				//successful (no errors, !)
				stk->mineral_DC_values.push_back(correlation);
			}

			delete min_ptr; //memory management....

		}

	}

}
