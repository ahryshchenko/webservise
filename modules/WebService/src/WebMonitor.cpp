//////////////////////////////////////////////////////////////////////////
// Implementation of class AdvertMonitor.
//

#include "pch.h"

#include "jsonxx.h"
#include "Log.h"
#include "WebMonitor.h"
#include "xml2json.hpp"

WebMonitor::WebMonitor(
	const std::string& url
	, ConversionType type
	, const std::string& outputFile)
	: isRunning_(false)
	, url_(url)
	, outputFile_(outputFile)
	, type_(type)
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

void WebMonitor::DoConversion(const std::string& content)
{
	switch (type_)
	{
	case WebMonitor::ConversionType::XML_JSON:
		XMLtoJSONConversion(content);
		break;
	case WebMonitor::ConversionType::JSON_XML:
		JSONtoXMLConversion(content);
		break;
	case WebMonitor::ConversionType::YAML_XML:
		break;
	case WebMonitor::ConversionType::XML_YAML:
		break;
	default:
		LERR_ << "Wrong conversion type.";
		break;
	}
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

		//TODO: For HTTP responses CURLINFO_RESPONSE_CODE should be 200.
		// As for local file we don't receive any response just check Data size.
		if (/*httpCode == 200*/ true)
		{
			if (httpData.empty())
			{
				LERR_ << "Server response is empty.";
				return false;
			}

			DoConversion(httpData);
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

bool WebMonitor::IsRunning()
{
	return isRunning_;
}

void WebMonitor::JSONtoXMLConversion(const std::string& content)
{
	if (content.empty())
	{
		LERR_ << "Empty conversion content";
		return;
	}

	try
	{
		std::unique_ptr<jsonxx::Object> object
			= std::make_unique<jsonxx::Object>();

		if (!object)
			BOOST_THROW_EXCEPTION(
				std::runtime_error("Error during JSON parser creation."));
		
		if (!object->parse(content))
		{
			LERR_ << "Error during JSON parsing.";
			return;
		}

		io::stream_buffer<io::file_sink> buf(
			outputFile_.empty() ? "output.xml" : outputFile_);
		std::ostream out(&buf);
		out << object->xml(jsonxx::JXML);

		LAPP_ << "JSON to XML conversion succeeded.";
	}
	catch (const std::exception& e)
	{
		LERR_ << e.what();
	}
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

void WebMonitor::XMLtoJSONConversion(const std::string& content)
{
	if (content.empty())
	{
		LERR_ << "Empty conversion content.";
		return;
	}

	try
	{
		std::string jsonStr(xml2json(content.c_str()));

		if (jsonStr.empty())
			BOOST_THROW_EXCEPTION(
				std::runtime_error("Error during XML content convertion."));

		io::stream_buffer<io::file_sink> buf(
			outputFile_.empty() ? "output.json" : outputFile_);
		std::ostream out(&buf);
		out << jsonStr;

		LAPP_ << "XML to JSON conversion succeeded.";
	}
	catch (const std::exception& e)
	{
		LERR_ << e.what();
	}
}
