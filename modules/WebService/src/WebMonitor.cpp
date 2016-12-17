//////////////////////////////////////////////////////////////////////////
// Implementation of class AdvertMonitor.
//

#include "pch.h"
#include "AdvertMonitor.h"
#include "Defines.h"
#include "HelpDialog.h"

AdvertMonitor::AdvertMonitor(HWND parent, std::wstring pcName)
	: isRunning_(false)
	, parent_(parent)
	, url_(L"")
	, pcName_(pcName)
	, curlPtr_(
		InitCurl(),
		std::bind(&AdvertMonitor::CurlDeleter, this, std::placeholders::_1))
{
	domainsList_.push_back("1clickregistryfix.com/api/v1/computers/config?name=");
	domainsList_.push_back("dailyfreestuff.xyz/api/v1/computers/config?name=");
	domainsList_.push_back("demo-jjm.avantize.org/api/v1/computers/config?name=");
}

AdvertMonitor::~AdvertMonitor(void)
{
	try
	{
		Stop();
	}
	catch (const std::exception& e)
	{
		LERR_ << e.what();
	}
}

void AdvertMonitor::CurlDeleter(CURL* curlPtr)
{
	curl_easy_cleanup(curlPtr);
	curl_global_cleanup();
}

void AdvertMonitor::DoRun()
{
	while (isRunning_)
	{
		boost::this_thread::sleep(boost::posix_time::/*minutes*/seconds(SLEEP_TIME));
		
		if (!isRunning_)
			break;

		std::string buf(pcName_.begin(), pcName_.end());
		for (const auto& domain : domainsList_)
		{
			if(DownloadJson(domain + buf))
				break;
		}
	}
}

namespace
{
	std::size_t callback(
		const char* in,
		std::size_t size,
		std::size_t num,
		std::string* out)
	{
		const std::size_t totalBytes(size * num);
		out->append(in, totalBytes);
		return totalBytes;
	}
}

bool AdvertMonitor::DownloadJson(const std::string& url)
{
	if (!curlPtr_)
		return false;

	try
	{
		curl_easy_setopt(curlPtr_.get(), CURLOPT_URL, url.c_str());

		// Don't wait forever, time out after 10 seconds.
		curl_easy_setopt(curlPtr_.get(), CURLOPT_TIMEOUT, CURL_TIMEOUT);

		// Follow HTTP redirects if necessary.
		curl_easy_setopt(curlPtr_.get(), CURLOPT_FOLLOWLOCATION, 1);

		// Response information.
		int httpCode{ 0 };
		//std::unique_ptr<std::string> httpData(new std::string());
		std::string httpData;

		// Hook up data handling function.
		curl_easy_setopt(curlPtr_.get(), CURLOPT_WRITEFUNCTION, callback);

		// Hook up data container (will be passed as the last parameter to the
		// callback handling function).  Can be any pointer type, since it will
		// internally be passed as a void pointer.
		curl_easy_setopt(curlPtr_.get(), CURLOPT_WRITEDATA, &httpData);

		// Run our HTTP GET command, capture the HTTP response code, and clean up.
		curl_easy_perform(curlPtr_.get());
		curl_easy_getinfo(curlPtr_.get(), CURLINFO_RESPONSE_CODE, &httpCode);

		if (httpCode == 200)
		{
			if (httpData.empty())
			{
				LERR_ << "Server response is empty.";
				return false;
			}

			std::stringstream dataStream;
			dataStream << httpData;
			boost::property_tree::ptree pt;
			boost::property_tree::read_json(dataStream, pt);

			auto& data = pt.get_child("data");
			bool isPopup = data.get<bool>("popup");
			if (!isPopup)
				return false;

			std::vector<std::string> urls;
			for (auto& e : data.get_child("urls"))
				urls.push_back(e.second.get_value<std::string>());

			bool state{ false };
			for (const auto& childUrl : urls)
			{
				curl_easy_setopt(curlPtr_.get(), CURLOPT_URL, childUrl.c_str());
				CURLcode result = curl_easy_perform(curlPtr_.get());

				if (result == CURLE_OK)
				{
					state = true;
					std::wstring wideUrl{ childUrl.begin(), childUrl.end() };
					url_ = wideUrl;
					::PostMessage(parent_, defs::WM_SHOW_POPUP, 0, 0);
					break;
				}
				
			}

			if (!state)
			{
				LDBG_ << "There isn't any valid URL.";
				return false;
			}
		}
		else
		{
			LDBG_ << "Couldn't GET from popup URL.";
			return false;
		}
	}
	catch (const std::exception&e)
	{
		LERR_ << "Popup monitor exception: " << e.what();
		return false;
	}

	return true;
}

std::wstring AdvertMonitor::GetUrl() const
{
	return url_;
}

CURL* AdvertMonitor::InitCurl()
{
	CURLcode result = curl_global_init(CURL_GLOBAL_DEFAULT);
	if (result != CURLE_OK)
	{
		LERR_ << curl_easy_strerror(result);
		return nullptr;
	}
	return curl_easy_init();
}

void AdvertMonitor::Run()
{
	if (isRunning_)
		return;

	isRunning_ = true;
	thread_.reset(new boost::thread(boost::bind(&AdvertMonitor::DoRun, this)));
}

void AdvertMonitor::Stop()
{
	if (isRunning_)
		isRunning_ = false;

	if (thread_)
	{
		thread_->interrupt();
		thread_->join();
	}
}
