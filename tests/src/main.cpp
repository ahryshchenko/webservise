#include "pch.h"

int main(void)
{
	try
	{
		std::map<std::string>

		ShellExecute(NULL, L"open", L"WebService.exe", NULL, NULL, SW_SHOWDEFAULT);
	}
	catch (const std::exception&)
	{
	}

	return 0;
}