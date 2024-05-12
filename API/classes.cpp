#include "classes.hpp"
void calculate_multi(Stock* stk)
{
	if (stk->commodity_associations.size() > 1)
	{
		stk->is_multi = true;
	}
	stk->is_multi = false;
}

void set_multi_group(Stock* stk, int type)
{
	switch (type)
	{
	case COM_OILT:
		stk->commodity_associations = oil;
		break;
	case COM_CROPT:
		stk->commodity_associations = crops;
		break;
	case COM_METALT:
		stk->commodity_associations = metals;
		break;
	}
}

void set_single_type(Stock* stk, int type)
{
	stk->commodity_associations.push_back(abs(type));
}

Stock* init_stock(std::string ticker)
{
	Stock* stk = new Stock;
	stk->ticker = ticker;
	return stk;
}

std::string get_commodity_name(COMMODITY_TYPE ct)
{
	switch (ct)
	{
	case COMMODITY_OIL_BRENT:
		return "BRENT";
	case COMMODITY_OIL_NATURAL_GAS:
		return "NATURAL_GAS";
	case COMMODITY_OIL_WTI:
		return "WTI";
	}
}
