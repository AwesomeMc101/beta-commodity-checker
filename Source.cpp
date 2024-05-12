#include <iostream>
#include "classes.hpp"
#include "parser.hpp"

#include "commodity_main.hpp"
#include "daily_correlation.hpp"
#include "create_stock.hpp"
#include "quartely_percent_comp.hpp"
#include "monthly_avg_comp.hpp"
#include "decide_correlations.hpp"

int main(){
  //just an example!!!
   std::vector<std::string> tickers = { "XOM", "CVX", "SHEL","TTE","COP","BP","EQNR","SLB"};
    std::vector<Stock*> oil_set = EasyLib::create_stock_set(tickers, COM_FULLOIL, FUNC_TIM_SER_DAILY, T_CLOSE);
    Commodity_Main::initialize_all();
    Commodity_Main::set_all_prices(COM_INTERVAL_DAILY);

    std::cout << "Done!\nErrors: " << Error_Scanner::Errors.error_count << "\n";

    std::cout << "Disabled: " << Commodity_Main::disabled_commodities.size();

    std::vector<QPC_API_TYPE> qpc_apis{QPC_API_EBITDA, QPC_API_GROSSPROFIT, QPC_API_NETINCOME, QPC_API_TOTALREVENUE, QPC_SAPI_PRICE};
    for (Stock* stk_ptr : oil_set)
    {
        std::vector<Oil_Daily_Correlation_Data> odcd_vec = Oil_Daily_Correlation::correlate_oil_equity(stk_ptr, true);
        Oil_Quarterly::do_qac_vector(stk_ptr, qpc_apis);
        Quarterly_Avg_Comp::do_qavg_comp(stk_ptr); //actually monthly ngl
    
        Correlated_Stock* c_stk = Decide_Correlations::form_correlated_stock(stk_ptr);

        for (int i = 0; i < c_stk->correlations.size(); i++)
        {
            COMMODITY_TYPE ctype = c_stk->correlations[i];
            short eq_type = c_stk->correlations_equations[i];

            std::cout << "[" << eq_type << "] " << c_stk->stk_ptr->ticker << " | " << eq_type << "\n";
        }

    }

  EasyLib::free_stockset_memory(oil_set);

}
