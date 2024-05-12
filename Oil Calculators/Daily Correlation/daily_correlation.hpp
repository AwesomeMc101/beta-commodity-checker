#pragma once
#include "classes.hpp"
#include "math_lib.hpp"
#include "commodity_main.hpp"

using Commodity_Main::Oils::Oil_Template;

#include <numeric>
#include <iostream>

// MOVED TO CLASSES.HPP
//#define OIL_TYPE_WTI 10
//#define OIL_TYPE_BRENT 50
//#define OIL_TYPE_NATURALGAS 90
//
//#define OIL_MTYPE_WB 60
//#define OIL_MTYPE_WBN 150 

typedef short OIL_TYPE;

struct Oil_Daily_Correlation_Data { //what a long name!
	float correlation;
	Stock* stock_ptr;
	OIL_TYPE oil_used;
};

namespace Oil_Daily_Correlation {
	//namespace Mathematics { //hand coded
	//	float average(std::vector<float> data);
	//	float covariance(std::vector<float> data_a, std::vector<float> data_b);
	//	float sum(std::vector<float> data);
	//	float standard_deviation(std::vector<float> data);
	//	float correlation_coefficient(std::vector<float> data_a, std::vector<float> data_b, int n);
	//}

	void auto_resize(std::vector<float>&, std::vector<float>&);
	std::vector<Oil_Daily_Correlation_Data> correlate_oil_equity(Stock* stock, bool); //bool is just for if you'd like to set back the days so its 1 day ahead

}
