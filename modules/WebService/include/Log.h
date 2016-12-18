#pragma once

// the logs are also written to LOGFILE
#define LOGFILE "logfile.log"

// just log messages with severity >= SEVERITY_THRESHOLD are written
#define SEVERITY_THRESHOLD logging::trivial::warning

// register a global logger
BOOST_LOG_GLOBAL_LOGGER(logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>)

// just a helper macro used by the macros below - don't use it in your code
#define LOG(severity) BOOST_LOG_SEV(logger::get(),boost::log::trivial::severity)

#define LTRC_    LOG(trace)
#define LDBG_    LOG(debug)
#define LAPP_    LOG(info)
#define LWRN_    LOG(warning)
#define LERR_    LOG(error)
#define LFTL_    LOG(fatal)
