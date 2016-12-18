#include "pch.h"
#include "Log.h"
#include "WebMonitor.h"

int main(int /*argc*/, char* /*argv*/[])
{
	try
	{
		std::unique_ptr<WebMonitor> monitor
			= std::make_unique<WebMonitor>("file://");
	}
	catch (const std::exception& e)
	{
		LERR_ << e.what();
	}

	return 0;
}