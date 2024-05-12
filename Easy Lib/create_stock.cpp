#include "create_stock.hpp"
namespace EasyLib {

	int g_interval = IN_DAILY;

	void initialize_global_types(int interval)
	{
		if (interval > IN_MONTHLY) { std::cout << "global interval failure.\n"; interval = IN_DAILY; }
		g_interval = interval;
	}

	COMMODITY_TYPE parse_full_set(COM_FULL cf_data)
	{
		switch (cf_data)
		{
		case COM_FULLOIL:
			return COM_OILT;
		case COM_FULLCROP:
			return COM_CROPT;
		case COM_FULLMINERAL:
			return COM_METALT;
		}

		/* not a full type */
		return { (COMMODITY_TYPE)cf_data }; //cast cf_data to comtype

	}
	Stock* create_stock(std::string ticker, COM_FULL cf_data, FUNCTION_TYPE f_t, PRICE_TYPE p_t, bool use_dates) //replace COM_FULL with a single type if not using all of one
	{
		COMMODITY_TYPE type_data = parse_full_set(cf_data);
		Stock* ret_stock = init_stock(ticker);
	

		if (type_data >= COM_OILT) //set commodity types
		{
			set_multi_group(ret_stock, type_data);
		}
		else
		{
			set_single_type(ret_stock, type_data);
		}

		ret_stock->use_dates = use_dates;

		Unparsed_Data uD = Parser::craft_unparsed_data(ticker, f_t, p_t, g_interval); //create an unparsed class

		Parsed_Data pD = Parser::parse(ret_stock, uD, f_t); //parse data

		ret_stock->price_data = pD.price_data;
		if (use_dates)
		{
			ret_stock->dates = pD.dates;
		}

		uD = Parser::craft_unparsed_data(ticker, FUNC_TIM_SER_MONTHLY, p_t, g_interval);
		ret_stock->use_dates = true;
		pD = Parser::parse(ret_stock, uD, FUNC_TIM_SER_MONTHLY);
		
		for (int i = 0; i < pD.price_data.size(); i++)
		{
			//std::cout << "\nMONTHLY PAIR: " << pD.dates[i] << " && " << pD.price_data[i] << "\n";
			ret_stock->monthly_price_data.push_back(std::make_pair(pD.dates[i], pD.price_data[i]));
		}

		calculate_multi(ret_stock); //see if multi type... ik but safe

		std::cout << "\n\n----" << ticker << " created. " << ret_stock->price_data.size() << " | " << ret_stock->monthly_price_data.size() << "--\n\n\n";

		return ret_stock;

	}
	std::vector<Stock*> create_stock_set(std::vector<std::string> ticker_set, COM_FULL c_f, FUNCTION_TYPE f_t, PRICE_TYPE p_t)
	{
		std::vector<Stock*> ptr_vec; //don't automatically allocate memory, we need this data forever. free the stock set when we stop using it fr.
		for (std::string ticker : ticker_set)
		{
			ptr_vec.push_back(create_stock(ticker, c_f, f_t, p_t, 1)); //do not allocate
		}
		return ptr_vec;
	}
	void free_stockset_memory(std::vector<Stock*>& set)
	{
		while (set.size())
		{
			auto* ptr = set.back();
			set.pop_back();
			delete ptr;
		}
		set.clear();
		//deleted 
	}
}
