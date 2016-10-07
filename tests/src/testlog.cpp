
#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include <cmath>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

// severity levels
enum severity_level {INFO};
// Logging macro
#define LOG(level) BOOST_LOG_SEV(global_logger::get(), level)
// Initializing global boost::log logger
typedef boost::log::sources::severity_channel_logger_mt<
    severity_level, std::string> global_logger_type;

BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(global_logger, global_logger_type)
{
    return global_logger_type(boost::log::keywords::channel = "global_logger");
}

// Initialize terminal logger
void init_term()
{
    // create sink to stdout
    boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
    sink->locked_backend()->add_stream(
        boost::shared_ptr<std::ostream>(&out, boost::empty_deleter()));

    // flush
    sink->locked_backend()->auto_flush(true);

    // format sink
    sink->set_formatter
    (
        /// TODO add your desired formatting
    );

    // filter
    // TODO add any filters

    // register sink
    bl::core::get()->add_sink(sink);
}

// Initialize logfile
void init_logfile(const std::string& logfilename)
{
    // create sink to logfile
    boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
    sink->locked_backend()->add_stream(
        boost::make_shared<std::ofstream>(logfilename.c_str()));

    // flush
    sink->locked_backend()->auto_flush(true);

    // format sink
    sink->set_formatter
    (
        /// TODO add your desired formatting
    );

    // filter
    // TODO add any filters

    // register sink
    bl::core::get()->add_sink(sink);
}


BOOST_AUTO_TEST_SUITE( TestLog )

BOOST_AUTO_TEST_CASE( TestConstructor )
{
	BOOST_CHECK( true );
}

BOOST_AUTO_TEST_SUITE_END()

