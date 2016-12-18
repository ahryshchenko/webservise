#include "pch.h"
#include "Log.h"
#include "WebMonitor.h"

int main(int argc, char* argv[])
{
	try
	{
		if (argc < 3)
		{
			LERR_ << "Wrong number of arguments. Closing.";
			return 0;
		}

		std::string typeStr{ argv[1] };
		WebMonitor::ConversionType type;
		if (!typeStr.compare("XML_JSON"))
			type = WebMonitor::ConversionType::XML_JSON;
		else if (!typeStr.compare("JSON_XML"))
			type = WebMonitor::ConversionType::JSON_XML;

		std::unique_ptr<WebMonitor> monitor
			= std::make_unique<WebMonitor>(
				argv[2]
				, type);

		if (!monitor)
			BOOST_THROW_EXCEPTION(
				std::runtime_error("Error during WebMonitor creation."));

		//! TODO: for test purposes stop all threads
		//! and close application after 10 seconds.
		const UINT workDuration = 10;
		auto start = std::chrono::system_clock::now();
		monitor->Run();
		while (monitor->IsRunning())
		{
			LAPP_ << "WebMonitor is running";

			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> diff = end - start;

			if (diff.count() >= workDuration)
			{
				monitor->Stop();
				break;
			}
		}
	}
	catch (const std::exception& e)
	{
		LERR_ << e.what();
	}

	return 0;
}