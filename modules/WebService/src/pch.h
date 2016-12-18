#pragma once

#include "targetver.h"

//////////////////////////////////////////////////////////////////////////
// Platform specific includes and definitions.
//

#include <atomic>

//////////////////////////////////////////////////////////////////////////
// STL includes and definitions.
//

#include <string>

//////////////////////////////////////////////////////////////////////////
// boost includes and definitions.
//

#pragma warning(push)

// warning C4459 : declaration of 'boost_scope_exit_aux_args' hides global declaration
#pragma warning(disable: 4459)
// warning C4503: 'name':  decorated name length exceeded, name was truncated
#pragma warning(disable: 4503)
// warning C4512: assignment operator could not be generated.
#pragma warning(disable: 4512)
// warning C4706: assignment within conditional expression.
#pragma warning(disable: 4706)
// warning C4996: 'name': This function or variable may be unsafe.
#pragma warning(disable: 4996)

#if defined _M_IX86
#define BOOST_BIND_ENABLE_STDCALL
#endif

#define BOOST_LOG_DONOT_USE_WCHAR_T

//#include <boost/algorithm/string.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/core/core.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
// #include <boost/program_options.hpp>
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/json_parser.hpp>
//#include <boost/scope_exit.hpp>
//#include <boost/spirit/include/qi.hpp>
//#include <boost/spirit/home/qi/string.hpp>
#include <boost/thread.hpp>

//namespace qi = boost::spirit::qi;
//namespace po = boost::program_options;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;

#pragma warning(pop)

//////////////////////////////////////////////////////////////////////////
// libcurl includes and definitions.
//

#include <curl/curl.h>
