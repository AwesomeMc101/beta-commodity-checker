#include "download_string.hpp"

namespace Internet
{
	HINTERNET interweb_handle; //this may not work as its in the obj file

	int initialize_webaccess()
	{
		interweb_handle = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
		if (interweb_handle == INVALID_HANDLE_VALUE)
		{
			Error_Scanner::push_error();
			return 0;
		}
		return 1;
	}
	void destroy_webaccess()
	{
		InternetCloseHandle(interweb_handle);
	}
	string replaceAll(string subject, const string& search,
		const string& replace) {
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != string::npos) {
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return subject;
	}

	string DownloadString(string URL) {
		//interweb_handle = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
		bool re_entered = false;
	entry:
		HINTERNET urlFile;
		string rtn;
		if (interweb_handle) {
			urlFile = InternetOpenUrlA(interweb_handle, URL.c_str(), NULL, NULL, NULL, NULL);
			if (urlFile) {
				char buffer[3500];
				DWORD bytesRead;
				do {
					InternetReadFile(urlFile, buffer, 3500, &bytesRead);
					rtn.append(buffer, bytesRead);
					memset(buffer, 0, 3500);
				} while (bytesRead);
				InternetCloseHandle(urlFile);
				string p = replaceAll(rtn, "|n", "\r\n");
				return p;
			}
		}
		else
		{
			printf("webfail\n");
			if (!re_entered)
			{
				printf("reentering due to webinit failure\n");
				Error_Scanner::push_warning();
				initialize_webaccess();
				re_entered = true;
				goto entry;
			}
			else {
				Error_Scanner::push_error();
			}
		}
		string p = replaceAll(rtn, "|n", "\r\n");
		return p;
	}
}
