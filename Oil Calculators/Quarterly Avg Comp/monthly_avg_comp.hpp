#include "quartely_percent_comp.hpp"

#include "math_lib.hpp"
/* CHANGED TO MONTHLY */

using Commodity_Main::Oils::Oil_Template;

namespace Quarterly_Avg_Comp {

	/* create the oil price vector between a set of dates. */

	typedef std::pair<std::string, float> QAC_PAIR;

	std::vector<float> get_oil_price_vector(std::string min_date, std::string max_date, unsigned short);
	std::vector<QAC_PAIR> get_equity_reports(Stock*, QPC_REPORTTYPE); //stock, report_type
	void do_qavg_comp(Stock*);
}
