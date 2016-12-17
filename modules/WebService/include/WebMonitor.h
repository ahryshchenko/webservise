//////////////////////////////////////////////////////////////////////////
// Interface of the AdvertMonitor class.
//

#ifndef ADVERTMONITOR
#define ADVERTMONITOR

#pragma once

class HelpDialog;

class AdvertMonitor
{
	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	AdvertMonitor(HWND parent, std::wstring pcName);
	//! Constructor.
	//! Destructor.
	~AdvertMonitor();

	//
	// Public interface.
	//
public:
	//! Returns url.
	std::wstring GetUrl() const;
	//! Starts monitor.
	void Run();
	//! Stops monitor.
	void Stop();

	//
	// Private constants.
	//
private:
	//! Time to sleep in minutes.
	static const UINT SLEEP_TIME = 5;
	//! CURL timeout in seconds.
	static const UINT CURL_TIMEOUT = 10;

	//
	// Private interface.
	//
private:
	//! Deleter for CURL member.
	void CurlDeleter(CURL* curlPtr);
	//! Does the real work.
	void DoRun();
	//! Downloads json request.
	bool DownloadJson(const std::string& url);
	//! Initializes CURL object.
	CURL* InitCurl();

	//
	// Private data members.
	//
private:
	//! True if thread is running.
	std::atomic<bool> isRunning_;
	//! Monitor thread.
	std::unique_ptr<boost::thread> thread_;
	//! Main window.
	HWND parent_;
	//! Advert url.
	std::wstring url_;
	//! Computer name.
	std::wstring pcName_;
	//! CURL pointer.
	std::unique_ptr<CURL, std::function<void(CURL*)>> curlPtr_;
	//! List of domains.
	std::vector<std::string> domainsList_;
};

#endif // !ADVERTMONITOR
