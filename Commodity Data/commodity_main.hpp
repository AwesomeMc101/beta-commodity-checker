#pragma once
#include <vector>
#include <string>

#include "parse_commodity.hpp"
#include "classes.hpp"


namespace Commodity_Main 
{

	typedef std::pair<std::string, long double> DATE_VECTOR;
	extern std::vector<std::string> disabled_commodities; //push back upon price setting error.

	namespace Oils 
	{
		struct Oil_Template{
			std::string function_name = "";
			std::vector<float> daily_prices{};
			bool enabled = false;

			std::vector<DATE_VECTOR> dated_prices{};
			bool dated_enabled = false;
		};

		extern Oil_Template* brent;
		extern Oil_Template* wti;
		extern Oil_Template* naturalgas;

		void initialize_oil_data(); //this needs to be run.
		void set_prices(Oil_Template*, COM_INTERVAL_TYPE);
		Oil_Template* grab_oilptr_by_name(std::string);

	}

	namespace Minerals
	{
		struct Mineral_Template {
			std::string function_name = "";
			std::vector<float> daily_prices{};
			bool enabled = false;

			std::vector<DATE_VECTOR> dated_prices{};
			bool dated_enabled = false;

		};

		extern Mineral_Template* copper;
		extern Mineral_Template* aluminum;

		void initialize_mineral_data();
		void set_prices(Mineral_Template*, COM_INTERVAL_TYPE);
		Mineral_Template* grab_mineralptr_by_type(COMMODITY_TYPE);

	}

	namespace Crops
	{
		struct Crop_Template {
			std::string function_name = "";
			std::vector<float> daily_prices{};
			bool enabled = false;

			std::vector<DATE_VECTOR> dated_prices{};
			bool dated_enabled = false;
		};

		extern Crop_Template* corn;
		extern Crop_Template* wheat;
		extern Crop_Template* cotton;
		extern Crop_Template* sugar;
		extern Crop_Template* coffee;

		void initialize_crop_data();
		void set_prices(Crop_Template*, COM_INTERVAL_TYPE);

	}

	void initialize_all(); //ease of access
	void set_all_prices(COM_INTERVAL_TYPE);
	bool is_disabled(std::string f_type);
}
