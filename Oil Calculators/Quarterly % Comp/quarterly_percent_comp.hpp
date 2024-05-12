#pragma once
#include "download_string.hpp"
#include "classes.hpp"
#include "error_scanner.hpp"
#include "api.hpp"
#include "commodity_main.hpp"

#include "math_lib.hpp"

#include <sstream>

typedef float OQ_DOUBLE; //we need hella storage space for these huge numbers
typedef short QPC_OIL_TYPE; //ok

#define QPC_REPORT_ANNUAL 30
#define QPC_REPORT_QUARTERLY 50
typedef short QPC_REPORTTYPE;

#define QPC_API_NETINCOME 1
#define QPC_API_GROSSPROFIT 2
#define QPC_API_EBITDA 3 //EARNINGS BEFORE TAX, INTEREST, DEPRECIATION, AND amortization
#define QPC_API_TOTALREVENUE 4
#define QPC_SAPI_PRICE 5
typedef short QPC_API_TYPE;
namespace QPC_API {
	std::string get_secondary_type(QPC_API_TYPE);
}

namespace Oil_Quarterly /* use double here as we need more data siz*/
{
	using Commodity_Main::Oils::Oil_Template;
	namespace Utilities {
		std::string craft_qpc_url(std::string);
		std::string parse_data(std::string bufferdata, size_t loc);
		bool is_number(std::string buffer);
	}
	OQ_DOUBLE calculate_quartely_correlation(std::vector<OQ_DOUBLE>, std::string); //stock income data, oil function name
	std::vector<std::pair<std::string, OQ_DOUBLE>> parse_stock_data(Stock* stk, QPC_REPORTTYPE, QPC_API_TYPE); //download string parser
	void auto_resize(std::vector<OQ_DOUBLE>& v_a, std::vector<OQ_DOUBLE>& v_b);
	std::vector<std::pair<OQ_DOUBLE, OQ_DOUBLE>> find_required_oil_prices(QPC_OIL_TYPE oil_type, std::vector<std::string> dates, std::vector<OQ_DOUBLE> eq_prices);
	std::vector<OQ_DOUBLE> do_quarterly_pcomp(Stock* stk, QPC_API_TYPE); //main function
	void do_qac_vector(Stock*, std::vector<QPC_API_TYPE>);
}
