#pragma once

#include <math.h>
#include <vector>
#include <string>
#include <numeric>
#include <iostream>

#include "api.hpp" //for correlation

namespace Mathematics { //hand coded

	typedef long double ML_LDOUBLE;

	float average(std::vector<float> data);
	float covariance(std::vector<float> data_a, std::vector<float> data_b);
	float sum(std::vector<float> data);
	float standard_deviation(std::vector<float> data);
	float correlation_coefficient(std::vector<float> data_a, std::vector<float> data_b, int n);

	float percent_change(float old_price, float new_price);
	std::vector<float> get_all_percent_changes(std::vector<float> values);
	bool verify_correlation(float);
}
