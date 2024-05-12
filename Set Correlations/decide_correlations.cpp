#include "decide_correlations.hpp"

namespace Decide_Correlations
{

	bool check(float correl_value)
	{
		return (correl_value >= EX_min_correlation);
	}

	void set_global_type(Correlated_Stock* c_stk)
	{
		//if(c_stk->stk_ptr->commodity_associations)

		for (short int com_type : c_stk->stk_ptr->commodity_associations)
		{
			//if(x == COMMODITY_COPPER || x )
			switch (com_type)
			{
			case COMMODITY_COPPER:
			case COMMODITY_ALUMINUM:
				c_stk->cor_global_type = COR_GLOBAL_METAL;
				return;
			case COMMODITY_COFFEE:
			case COMMODITY_CORN:
			case COMMODITY_COTTON:
			case COMMODITY_SUGAR:
			case COMMODITY_WHEAT:
				c_stk->cor_global_type = COR_GLOBAL_CROP;
				return;

			case COMMODITY_OIL_BRENT:
			case COMMODITY_OIL_NATURAL_GAS:
			case COMMODITY_OIL_WTI:
				c_stk->cor_global_type = COR_GLOBAL_OIL;
				return;
			}
		}
	}

	const short loop_type_DC = 0;
	const short loop_type_QPC = 1;
	const short loop_type_MAC = 2;


	/* this function checks each value in the given oil vector and sees if it is greater than the required correlation value (found in: api.hpp i think)
	used instead of fucking writing 3+ loops lol.
	
	*/
	void OIL_FORLOOP_CHECK(Correlated_Stock* c_stk, short type)
	{

		std::vector<double> values{};
		switch (type)
		{
		case loop_type_DC:
			values = c_stk->stk_ptr->oil_DC_values;
			break;
		case loop_type_QPC:
			values = c_stk->stk_ptr->oil_QPC_values;
			break;
		case loop_type_MAC: {
			values.clear();
			for (float value : c_stk->stk_ptr->oil_MAC_values)
			{
				values.push_back((double)value);
			}
			break;
			//values = c_stk->stk_ptr->oil_MAC_values;
		}
		}

		if (!values.size()) {
			Error_Scanner::push_error();
			return;
		}

		for (int i = 0; i < values.size(); i++)
		{
			if (i > c_stk->stk_ptr->commodity_associations.size()) { //ensure com_type wont crash us
				Error_Scanner::push_error();
				break;
			}

			float value = values[i];
			COMMODITY_TYPE com_type = c_stk->stk_ptr->commodity_associations[i];
			if (check(value))
			{

				c_stk->correlations.push_back(com_type);
				switch (type)
				{
				case loop_type_DC:
					c_stk->correlations_equations.push_back(CORRELATED_EQUATION::OIL_DC);
					break;
				case loop_type_QPC:
					c_stk->correlations_equations.push_back(CORRELATED_EQUATION::OIL_QPC);
					break;
				case loop_type_MAC:
					c_stk->correlations_equations.push_back(CORRELATED_EQUATION::OIL_MAC);
					break;
				}
				//c_stk->correlations_equations.push_back(CORRELATED_EQUATION::OIL_QPC);
			}
		}
	}

	void check_oil(Correlated_Stock* c_stk)
	{
		//i dont think we need to pass stock here since its address is alrdy in c_stk...
		OIL_FORLOOP_CHECK(c_stk, loop_type_DC);
		OIL_FORLOOP_CHECK(c_stk, loop_type_QPC);
		OIL_FORLOOP_CHECK(c_stk, loop_type_MAC);

	}
	void check_metals(Correlated_Stock*, Stock*);
	void check_crops(Correlated_Stock*, Stock*);

	Correlated_Stock* form_correlated_stock(Stock* stk_ptr)
	{
		Correlated_Stock* c_stk = new Correlated_Stock;
		c_stk->stk_ptr = stk_ptr;
		set_global_type(c_stk);

		switch (c_stk->cor_global_type)
		{
		case COR_GLOBAL_OIL:
			check_oil(c_stk);
			break;
		}

		return c_stk;
	}


}
