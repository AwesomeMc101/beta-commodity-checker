#pragma once
#include <string>
#include <Windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#include "error_scanner.hpp"

namespace Internet
{

	extern HINTERNET interweb_handle;

	using namespace std;

	int initialize_webaccess();
	void destroy_webaccess();
	string replaceAll(string subject, const string& search,
		const string& replace);

	string DownloadString(string URL);
	
}
