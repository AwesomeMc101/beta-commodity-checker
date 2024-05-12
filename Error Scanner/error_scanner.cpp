#pragma once
#include "error_scanner.hpp"

bool Error_Scanner::scan_for_errors()
{
	static int static_error_count = 0;
	if (Errors.error_count > static_error_count)
	{
		std::cout <<"\n[Error] " << Errors.error_count << " > " << static_error_count << "\n";
		static_error_count = Errors.error_count;
		return true;
	}
	return false;
}
bool Error_Scanner::scan_for_warnings()
{
	static int static_warning_count = 0;
	if (Errors.warning_count > static_warning_count)
	{
		static_warning_count = Errors.warning_count;
		return true;
	}
	return false;
}

void Error_Scanner::push_error()
{
	Errors.error_count++;
}
void Error_Scanner::push_warning()
{
	Errors.warning_count++;
}
void Error_Scanner::clear_errors()
{
	Errors.error_count = 0;
}
void Error_Scanner::clear_warnings()
{
	Errors.warning_count = 0;
}
