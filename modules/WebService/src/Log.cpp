#include "pch.h"
#include "Log.h"

BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", logging::trivial::severity_level)

BOOST_LOG_GLOBAL_LOGGER_INIT(logger, src::severity_logger_mt)
{
	src::severity_logger_mt<boost::log::trivial::severity_level> logger;

	// add attributes
	logger.add_attribute("TimeStamp", attrs::local_clock());

	// add a text sink
	typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
	boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();

	// add a logfile stream to our sink
	sink->locked_backend()->add_stream(boost::make_shared<std::ofstream>(LOGFILE));

	// add "console" output stream to our sink
	sink->locked_backend()->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));

	// specify the format of the log message
	logging::formatter formatter = expr::stream
		<< expr::format_date_time(timestamp, "%Y-%m-%d, %H:%M:%S") << " "
		<< "[" << logging::trivial::severity << "]"
		<< " - " << expr::smessage;
	sink->set_formatter(formatter);

	// only messages with severity >= SEVERITY_THRESHOLD are written
	sink->set_filter(severity >= SEVERITY_THRESHOLD);

	// "register" our sink
	logging::core::get()->add_sink(sink);

	return logger;
}
