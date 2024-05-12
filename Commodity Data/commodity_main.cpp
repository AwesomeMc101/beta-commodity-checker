#pragma once
#include "commodity_main.hpp"

namespace Commodity_Main
{
	std::vector<std::string> disabled_commodities{};
	namespace Oils
	{

		Oil_Template* brent = new Oil_Template;
		Oil_Template* wti = new Oil_Template;
		Oil_Template* naturalgas = new Oil_Template;

		void initialize_oil_data()
		{
			brent->function_name = "BRENT";
			wti->function_name = "WTI";
			naturalgas->function_name = "NATURAL_GAS";
		}

		void set_prices(Oil_Template* commodity, COM_INTERVAL_TYPE interval)
		{
			Error_Scanner::scan_for_errors();
			std::string downloaded_data = Parse_Commodity::download_commodity_data(commodity->function_name, interval);
			if (Error_Scanner::scan_for_errors()) { 
				Error_Scanner::push_error();
				disabled_commodities.push_back(commodity->function_name);
				commodity->enabled = false;
				return; 
			}

			commodity->daily_prices = Parse_Commodity::parse_commodity(commodity->function_name, interval, downloaded_data);
			commodity->enabled = true;

			if (Error_Scanner::scan_for_errors())
			{
				std::cout << "ERROR ON DATED OIL COMMODTY.";
				Error_Scanner::push_error();
				disabled_commodities.push_back(commodity->function_name);
				commodity->enabled = false;
			}

			commodity->dated_prices = Parse_Commodity::date_parse_commodity(commodity->function_name, interval, downloaded_data);
			commodity->dated_enabled = true;
			if (Error_Scanner::scan_for_errors())
			{
				commodity->dated_enabled = false;
				Error_Scanner::push_error();
				//disabled_commodities.push_back(commodity->function_name);
			}
		}

		Oil_Template* grab_oilptr_by_name(std::string commodity_name)
		{
			if (commodity_name == brent->function_name) {
				return brent; 
			}
			if (commodity_name == wti->function_name) { return wti; }
			if (commodity_name == naturalgas->function_name) { return naturalgas; }

			Error_Scanner::push_error();
			return wti; //

		}

	}
	namespace Minerals 
	{

		Mineral_Template* copper = new Mineral_Template;
		Mineral_Template* aluminum = new Mineral_Template;

		void initialize_mineral_data()
		{
			aluminum->function_name = "ALUMINUM";
			copper->function_name = "COPPER";
		}

		Mineral_Template* grab_mineralptr_by_type(COMMODITY_TYPE ctype)
		{
			switch (ctype)
			{
			case COMMODITY_ALUMINUM:
				return aluminum;
			case COMMODITY_COPPER:
				return copper;
			}

			Error_Scanner::push_error();
			return aluminum;
		}

		void set_prices(Mineral_Template* commodity, COM_INTERVAL_TYPE interval)
		{
			Error_Scanner::scan_for_errors();
			//commodity->daily_prices = Parse_Commodity::parse_commodity(commodity->function_name, interval);
			std::string downloaded_data = Parse_Commodity::download_commodity_data(commodity->function_name, interval);
			if (Error_Scanner::scan_for_errors()) {
				Error_Scanner::push_error();
				disabled_commodities.push_back(commodity->function_name);
				commodity->enabled = false;
				return;
			}

			commodity->daily_prices = Parse_Commodity::parse_commodity(commodity->function_name, interval, downloaded_data);
			commodity->enabled = true;
			if (Error_Scanner::scan_for_errors())
			{
				Error_Scanner::push_error();
				disabled_commodities.push_back(commodity->function_name);
			}

			commodity->dated_prices = Parse_Commodity::date_parse_commodity(commodity->function_name, interval, downloaded_data);
			commodity->dated_enabled = true;
			if (Error_Scanner::scan_for_errors())
			{
				commodity->dated_enabled = false;
				Error_Scanner::push_error();
				//disabled_commodities.push_back(commodity->function_name);
			}
		}

	}
	namespace Crops
	{

		Crop_Template* corn = new Crop_Template;
		Crop_Template* wheat = new Crop_Template;
		Crop_Template* cotton = new Crop_Template;
		Crop_Template* sugar = new Crop_Template;
		Crop_Template* coffee = new Crop_Template;

		void initialize_crop_data()
		{
			wheat->function_name = "WHEAT";
			corn->function_name = "CORN";
			cotton->function_name = "COTTON";
			sugar->function_name = "SUGAR";
			coffee->function_name = "COFFEE";
		}

		void set_prices(Crop_Template* commodity, COM_INTERVAL_TYPE interval)
		{
			Error_Scanner::scan_for_errors();
			//commodity->daily_prices = Parse_Commodity::parse_commodity(commodity->function_name, interval);
			std::string downloaded_data = Parse_Commodity::download_commodity_data(commodity->function_name, interval);
			if (Error_Scanner::scan_for_errors()) {
				Error_Scanner::push_error();
				disabled_commodities.push_back(commodity->function_name);
				commodity->enabled = false;
				return;
			}

			commodity->daily_prices = Parse_Commodity::parse_commodity(commodity->function_name, interval, downloaded_data);
			commodity->enabled = true;

			if (Error_Scanner::scan_for_errors())
			{

				std::cout << "Error: " << commodity->function_name << " || Error Size: " << Error_Scanner::Errors.error_count << "\n";
				Error_Scanner::push_error();
				disabled_commodities.push_back(commodity->function_name);
			}
		}
	}


	void initialize_all()
	{
		Oils::initialize_oil_data();
		Minerals::initialize_mineral_data();
		Crops::initialize_crop_data();
	}

	void set_all_prices(COM_INTERVAL_TYPE interval) //TODO: PERFORM INTERVAL CHECKS AS CROPS CANT DO DAILY || DONE
	{
		COM_INTERVAL_TYPE holster = -1;

		/* Oils */
		if (interval >= COM_INTERVAL_QUARTERLY)
		{
			holster = interval;
			interval = COM_INTERVAL_MONTHLY;
		}

		Oils::set_prices(Oils::brent, interval);
		Oils::set_prices(Oils::wti, interval);
		Oils::set_prices(Oils::naturalgas, interval);

		/* Minerals */
		if (interval <= COM_INTERVAL_WEEKLY) {
			if (holster >= COM_INTERVAL_MONTHLY) {
				interval = holster;
			}
			else {
				interval = COM_INTERVAL_MONTHLY;
			}
		}

		Minerals::set_prices(Minerals::copper, interval);
		Minerals::set_prices(Minerals::aluminum, interval);

		/* Crops */
		Crops::set_prices(Crops::coffee, interval);
		Crops::set_prices(Crops::wheat, interval);
		Crops::set_prices(Crops::sugar, interval);
		Crops::set_prices(Crops::corn, interval);
		Crops::set_prices(Crops::cotton, interval);
		Crops::set_prices(Crops::wheat, interval);
	}
	bool is_disabled(std::string f_type)
	{
		for (std::string disabled : disabled_commodities)
		{
			if (disabled == f_type) { return true; }
		}
		return false;
	}

}
