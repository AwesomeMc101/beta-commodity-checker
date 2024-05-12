#pragma once

#include <string>
#include <iostream>
namespace Error_Scanner
{
	struct {
		int error_count = 0;
		int warning_count = 0;
	} Errors;

	bool scan_for_errors(); //returns true on error
	bool scan_for_warnings(); //returns true on warning

	void push_error();
	void push_warning();
	void clear_errors();
	void clear_warnings();
}
