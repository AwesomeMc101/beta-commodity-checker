#pragma once

#include <vector>
#include <map>
#include <string>

#define COMMODITY_OIL_WTI 1
#define COMMODITY_OIL_BRENT 2
#define COMMODITY_OIL_NATURAL_GAS 3

#define COMMODITY_COPPER 5
#define COMMODITY_ALUMINUM 9

#define COMMODITY_SUGAR 4
#define COMMODITY_CORN 6
#define COMMODITY_WHEAT 7
#define COMMODITY_COFFEE 8
#define COMMODITY_COTTON 10

/* Oil type used by Oil Calculators*/
#define OIL_TYPE_WTI 10
#define OIL_TYPE_BRENT 50
#define OIL_TYPE_NATURALGAS 90

#define OIL_MTYPE_WB 60
#define OIL_MTYPE_WBN 150 


typedef unsigned short COMMODITY_TYPE;

#define COM_OILT 900
#define COM_CROPT 901
#define COM_METALT 902

const std::vector<COMMODITY_TYPE> metals{COMMODITY_COPPER, COMMODITY_ALUMINUM};
const std::vector<COMMODITY_TYPE> crops{ COMMODITY_WHEAT, COMMODITY_CORN, COMMODITY_COTTON, COMMODITY_SUGAR, COMMODITY_COFFEE };
const std::vector<COMMODITY_TYPE> oil{ COMMODITY_OIL_WTI, COMMODITY_OIL_BRENT, COMMODITY_OIL_NATURAL_GAS};

typedef std::pair<std::string, float> PRICE_PAIR;

struct Stock {
	std::string ticker;

	std::vector<unsigned short> commodity_associations;
	bool is_multi = false; //is multiple commodity associations

	bool use_dates = false;
	std::vector<float> price_data;
	std::vector<std::string> dates;

	std::vector<PRICE_PAIR> monthly_price_data;

	std::vector<double> oil_QPC_values; //QUARTERLY_PERCENT_CORRELATIOM
	std::vector<double> oil_DC_values; //DAILY_CORRELATION
	std::vector<float> oil_MAC_values; //monthly average correlation

	std::vector<float> mineral_DC_values;

};

void calculate_multi(Stock* stk);

void set_multi_group(Stock* stk, int type);

void set_single_type(Stock* stk, int type);

Stock* init_stock(std::string ticker);

std::string get_commodity_name(COMMODITY_TYPE);