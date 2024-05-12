#pragma once
#define KEY "SNIPPED :("
const char* get_key();

#define T_NIL 0
#define T_OPEN 1
#define T_CLOSE 2
#define T_HIGH 3
#define T_LOW 4
#define T_VOLUME 5

#define T_OPENCLOSE 6
#define T_HIGHLOW 7
#define T_ALL 8

#define T_DEFAULT "open"

const float EX_min_correlation = 0.0975; //minimum value that a correlation coefficient can be to be valid

#define ENTITLEMENT_DELAYED "&entitlement=delayed"
#define ENTITLEMENT_REALTIME "&entitlement=realtime"

const char* get_price_type(int type);
bool multi_price_type(int type);

#define IN_NIL 0
#define IN_1MIN 1
#define IN_5MIN 2
#define IN_15MIN 3
#define IN_30MIN 4
#define IN_60MIN 5
#define IN_1HR 5 //1 hour = 60 min :)
#define IN_DAILY 6
#define IN_WEEKLY 7
#define IN_MONTHLY 8

const char* get_interval_type(int type);

#define FUNC_NIL 0
#define FUNC_TIM_SER_INTRADAY 1
#define FUNC_TIM_SER_DAILY 2
#define FUNC_TIM_SER_MONTHLY 3

const char* get_function_type(int type);

#define PARSE_FUNCTION_TYPE int

#define FTYPE_PRICES 1
#define FTYPE_TECHNICAL 2
#define FTYPE_OTHER 3

typedef unsigned short OUTPUT_SIZE;

#define OUTPUT_FULL 100
#define OUTPUT_COMPACT 200

extern OUTPUT_SIZE g_outputsize;
void set_global_output();
const char* get_global_output();

