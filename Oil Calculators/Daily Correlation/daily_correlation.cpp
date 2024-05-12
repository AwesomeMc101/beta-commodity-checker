#include "daily_correlation.hpp"

namespace Oil_Daily_Correlation {

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

	std::vector<Oil_Daily_Correlation_Data> correlate_oil_equity(Stock* stock, bool setback_comdays)
	{
		//int oil_type = stock->commodity_associations; //kms
		std::vector<Oil_Daily_Correlation_Data> return_data{};
		for (COMMODITY_TYPE oil_type : stock->commodity_associations)
		{
			std::cout << "\nCommodity: " << get_commodity_name(oil_type) << "\n";
			Oil_Template* oil = Commodity_Main::Oils::grab_oilptr_by_name(get_commodity_name(oil_type)); //get a ptr to the right oil type by converting oil_type to its string and inputting it to grab_oilptr

			/* ensure oil commodity is SET and stock is SET */
			if (!oil->enabled || !stock->price_data.size()) {
				std::cout << "OIL ENABLED? " << oil->enabled << " || PRICE SIZE? " << stock->price_data.size() << "\n";
				printf("skip");
				continue;
			}

			/* ensure we dont change our permanent price data with auto_resize by creating temporary vectors. */
			std::vector<float> oil_scoped_prices = oil->daily_prices, stock_scoped_prices = stock->price_data; //we do not want to get rid of the ORIGINAL prices stored in the classes pointed to
			auto_resize(oil_scoped_prices, stock_scoped_prices); //ensure same size
			if (!oil_scoped_prices.size()) { Error_Scanner::push_error(); return {}; }
			if (setback_comdays) //remove the latest oil value so that we are predicting a day previous
			{
				oil_scoped_prices.erase(oil_scoped_prices.begin());
				stock_scoped_prices.pop_back(); //balance out for covariance.
				if (!oil_scoped_prices.size()) { Error_Scanner::push_error(); return {}; }
				if (!stock_scoped_prices.size()) { Error_Scanner::push_error(); return {}; }
			}
			/* set our class up for pushback */
			Oil_Daily_Correlation_Data od_cd;
			od_cd.correlation = Mathematics::correlation_coefficient(oil_scoped_prices, stock_scoped_prices, 0);
			od_cd.stock_ptr = stock;
			od_cd.oil_used = oil_type;
			return_data.push_back(od_cd);
			stock->oil_DC_values.push_back(od_cd.correlation);
		}

		if (return_data.size()) { return return_data; } //GOOD

		Error_Scanner::push_error();
		return {};
	}

}
