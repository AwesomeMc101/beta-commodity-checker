#pragma once
#include "math_lib.hpp"

namespace Mathematics {

	float sum(std::vector<float> data)
	{
		return (std::accumulate(data.begin(), data.end(), decltype(data)::value_type(0)));
	}

	float average(std::vector<float> data)
	{
		if (!data.size()) { return 0; }
		//double total = std::accumulate(data.begin(), data.end(), decltype(data)::value_type(0)); //add all values together, use decltype to get FLOAT and ensure we use decimal points.
		return (sum(data) / data.size());
	}
	float covariance(std::vector<float> data_a, std::vector<float> data_b)
	{
		size_t data_size = data_a.size();
		if (data_size != data_b.size())
		{
			std::cout << "\nCovariance sizecall: " << data_a.size() << " vs. " << data_b.size() << "\n";
			return 0;
		}

		float mean_a = average(data_a);
		float mean_b = average(data_b);

		float total = 0;
		for (int i = 0; i < data_size; i++) //both vectors confirmed to be same size.
		{
			float dmean_a = (data_a[i] - mean_a);
			float dmean_b = (data_b[i] - mean_b);
			total += (dmean_a * dmean_b);
		}

		return (total / data_size);

	}

	float standard_deviation(std::vector<float> data)
	{
		if (!data.size()) { return 0; }

		float avg = average(data);
		float total = 0;

		for (float val : data)
		{
			total += (powf((val - avg), 2));
		}
		return (total / data.size());
	}

	float correlation_coefficient(std::vector<float> data_a, std::vector<float> data_b, int n)
	{
		float covariance_value = covariance(data_a, data_b);
		return(covariance_value / (standard_deviation(data_a) * standard_deviation(data_b)));
	}

	float percent_change(float old_price, float new_price)
	{
		//OQ_DOUBLE result = (new_price - old_price);
		new_price -= old_price;
		new_price /= old_price;
		return (new_price * 100);
	}

	std::vector <float > get_all_percent_changes(std::vector<float> values)
	{
		std::vector<float> return_vector{};
		for (int i = 0; i < (values.size() - 1); i++)
		{
			//i+1 = new value
			return_vector.push_back(percent_change(values[i], values[i + 1]));
		}
		return return_vector;
	}

	bool verify_correlation(float data)
	{
		return (data >= EX_min_correlation);
	}

}
