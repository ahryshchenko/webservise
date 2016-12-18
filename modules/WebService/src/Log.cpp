#include "pch.h"
#include "Log.h"

BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", logging::trivial::severity_level)

BOOST_LOG_GLOBAL_LOGGER_INIT(logger, src::severity_logger_mt)
{
	src::severity_logger_mt<boost::log::trivial::severity_level> logger;
	logger.add_attribute("TimeStamp", attrs::local_clock());
	typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
	boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
	sink->locked_backend()->add_stream(boost::make_shared<std::ofstream>(LOGFILE, std::ios::out | std::ios::app));
	sink->locked_backend()->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
	logging::formatter formatter = expr::stream
		<< expr::format_date_time(timestamp, "%Y-%m-%d, %H:%M:%S") << " "
		<< "[" << logging::trivial::severity << "]"
		<< " - " << expr::smessage;
	sink->set_formatter(formatter);
	sink->set_filter(severity >= SEVERITY_THRESHOLD);
	logging::core::get()->add_sink(sink);

	return logger;
}
