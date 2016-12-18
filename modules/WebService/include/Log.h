#pragma once

#define LOGFILE "logfile.log"
#define SEVERITY_THRESHOLD logging::trivial::trace
// register a global logger
BOOST_LOG_GLOBAL_LOGGER(logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>)
#define LOG(severity) BOOST_LOG_SEV(logger::get(),boost::log::trivial::severity)

#define LTRC_    LOG(trace)
#define LDBG_    LOG(debug)
#define LAPP_    LOG(info)
#define LWRN_    LOG(warning)
#define LERR_    LOG(error)
#define LFTL_    LOG(fatal)
