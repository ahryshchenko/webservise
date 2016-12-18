//////////////////////////////////////////////////////////////////////////
// Interface of the WebMonitor class.
//

#ifndef WEBMONITOR_H_
#define WEBMONITOR_H_

#pragma once

class WebMonitor
{
	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	WebMonitor(const std::string& url);
	//! Constructor.
	//! Destructor.
	~WebMonitor();

	//
	// Public interface.
	//
public:
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
	//! Downloads file.
	bool Download(const std::string& url);
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
	//! CURL pointer.
	std::unique_ptr<CURL, std::function<void(CURL*)>> curlPtr_;
	//! Download url.
	std::string url_;
};

#endif // WEBMONITOR_H_
