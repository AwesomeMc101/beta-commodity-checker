#pragma once
#include "api.hpp"

const char* get_key()
{
	return KEY;
}



const char* get_price_type(int type)
{
	switch (type)
	{
	case T_NIL:
		return T_DEFAULT;
	case T_OPEN:
		return "open";
	case T_CLOSE:
		return "close";
	case T_HIGH:
		return "high";
	case T_LOW:
		return "low";
	case T_VOLUME:
		return "volume";
	}
	return T_DEFAULT;
}

bool multi_price_type(int type)
{
	return (type >= T_OPENCLOSE);
}



const char* get_interval_type(int type)
{
	switch (type)
	{
	case IN_NIL:
		return "daily";
	case IN_1MIN:
		return "1min";
	case IN_5MIN:
		return "5min";
	case IN_15MIN:
		return "15min";
	case IN_30MIN:
		return "30min";
	case IN_60MIN: //includes IN_1HR
		return "60min";
	case IN_DAILY:
		return "daily";
	case IN_WEEKLY:
		return "weekly";
	case IN_MONTHLY:
		return "monthly";
	}
	return "daily";
}

const char* get_function_type(int type)
{
	switch (type)
	{
	case FUNC_NIL:
		//BAD BAD BAD

	case FUNC_TIM_SER_INTRADAY:
		return "TIME_SERIES_INTRADAY";
	case FUNC_TIM_SER_DAILY:
		return "TIME_SERIES_DAILY";
	case FUNC_TIM_SER_MONTHLY:
		return "TIME_SERIES_MONTHLY";
	}
}

OUTPUT_SIZE g_outputsize = OUTPUT_COMPACT;

void set_global_output(OUTPUT_SIZE size)
{
	if (size != OUTPUT_FULL && size != OUTPUT_COMPACT) { return; } //value is alrdy set by default

	g_outputsize = size;

}
const char* get_global_output()
{
	switch (g_outputsize)
	{
	case OUTPUT_COMPACT:
		return "compact";
	case OUTPUT_FULL:
		return "full";
	}
	return "compact"; //bad asf, shouldnt get here
}
