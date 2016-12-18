//////////////////////////////////////////////////////////////////////////
// Implementation of class AdvertMonitor.
//

#include "pch.h"
#include "Log.h"
#include "WebMonitor.h"

WebMonitor::WebMonitor(const std::string& url)
	: isRunning_(false)
	, url_(url)
	, curlPtr_(
		InitCurl(),
		std::bind(&WebMonitor::CurlDeleter, this, std::placeholders::_1))
{
}

WebMonitor::~WebMonitor(void)
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

void WebMonitor::CurlDeleter(CURL* curlPtr)
{
	curl_easy_cleanup(curlPtr);
	curl_global_cleanup();
}

void WebMonitor::DoRun()
{
	while (isRunning_)
	{
		boost::this_thread::sleep(boost::posix_time::seconds(SLEEP_TIME));
		
		if (!isRunning_)
			break;

		Download(url_);
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

bool WebMonitor::Download(const std::string& url)
{
	if (!curlPtr_)
		return false;

	try
	{
		curl_easy_setopt(curlPtr_.get(), CURLOPT_URL, url.c_str());
		curl_easy_setopt(curlPtr_.get(), CURLOPT_TIMEOUT, CURL_TIMEOUT);
		curl_easy_setopt(curlPtr_.get(), CURLOPT_FOLLOWLOCATION, 1);

		int httpCode{ 0 };
		std::string httpData;

		curl_easy_setopt(curlPtr_.get(), CURLOPT_WRITEFUNCTION, callback);
		curl_easy_setopt(curlPtr_.get(), CURLOPT_WRITEDATA, &httpData);
		curl_easy_perform(curlPtr_.get());
		curl_easy_getinfo(curlPtr_.get(), CURLINFO_RESPONSE_CODE, &httpCode);

		if (httpCode == 200)
		{
			if (httpData.empty())
			{
				LERR_ << "Server response is empty.";
				return false;
			}
			/*
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
			*/
		}
		else
		{
			LDBG_ << "Couldn't GET from URL.";
			return false;
		}
	}
	catch (const std::exception&e)
	{
		LERR_ << "Web monitor exception: " << e.what();
		return false;
	}

	return true;
}

CURL* WebMonitor::InitCurl()
{
	CURLcode result = curl_global_init(CURL_GLOBAL_DEFAULT);
	if (result != CURLE_OK)
	{
		LERR_ << curl_easy_strerror(result);
		return nullptr;
	}
	return curl_easy_init();
}

void WebMonitor::Run()
{
	if (isRunning_)
		return;

	isRunning_ = true;
	thread_.reset(new boost::thread(boost::bind(&WebMonitor::DoRun, this)));
}

void WebMonitor::Stop()
{
	if (isRunning_)
		isRunning_ = false;

	if (thread_)
	{
		thread_->interrupt();
		thread_->join();
	}
}
