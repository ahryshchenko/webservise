#include "pch.h"

struct ParametersType
{
	//! Conversion type.
	std::wstring type_;
	//! Test url.
	std::wstring url_;
	//! Output file.
	std::wstring outputFile_;
};

int main(void)
{
	try
	{
		//! 3 and 4 contains invalid input.
		std::vector<ParametersType> params{
			{ L"XML_JSON", L"file://test1.xml", L"output1.json" }
			, { L"JSON_XML", L"file://test2.json", L"output2.xml" }
			,{ L"XML_JSON", L"file://test3.xml", L"output3.json" }
			,{ L"JSON_XML", L"file://test4.json", L"output4.xml" }
		};

		for (const auto& param : params)
		{
			ShellExecute(
				NULL
				, L"open"
				, L"WebService.exe"
				, std::wstring(
					param.url_ + L' ' + param.type_ + L' ' + param.outputFile_).c_str()
				, NULL
				, SW_SHOWDEFAULT);
		}
	}
	catch (const std::exception&)
	{
	}

	return 0;
}