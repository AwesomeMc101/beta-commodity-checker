#pragma once
#include "classes.hpp"
#include "math_lib.hpp"
#include "commodity_main.hpp"

using Commodity_Main::Minerals::Mineral_Template;

#include <numeric>
#include <iostream>

namespace Mineral_Daily_Correlation {

	void auto_resize(std::vector<float>&, std::vector<float>&);

	float get_mineral_daily_correlation(Stock*, Mineral_Template*);

	void do_mineral_daily(Stock*);

}
