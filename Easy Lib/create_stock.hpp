#include "api.hpp"
#include "classes.hpp"
#include "parser.hpp"
#include "parse_commodity.hpp"

#include <iostream>

#define COM_FULLOIL 800
#define COM_FULLMINERAL 900
#define COM_FULLCROP 1000

typedef unsigned int COM_FULL;
typedef unsigned int FUNCTION_TYPE;
typedef unsigned int PRICE_TYPE;

namespace EasyLib {

	extern int g_interval;
	void initialize_global_types(int interval);

	COMMODITY_TYPE parse_full_set(COM_FULL);

	/* create_stock("XOM", COM_FULLOIL, FUNC_TIM_SER_DAILY, T_CLOSE);*/
	Stock* create_stock(std::string, COM_FULL, FUNCTION_TYPE, PRICE_TYPE, bool); //replace COM_FULL with a single type if not using all of one

	std::vector<Stock*> create_stock_set(std::vector<std::string>, COM_FULL, FUNCTION_TYPE, PRICE_TYPE);
	void free_stockset_memory(std::vector<Stock*>&);

}
