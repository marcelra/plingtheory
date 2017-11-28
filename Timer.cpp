#include "Timer.h"


Timer::Timer( Logger& logger, Msg::LogLevel logLevel, const std::string& timerName, bool doReportStart ) :
   m_logger(logger),
   m_logLevel(logLevel),
   m_timerName(timerName),
   m_timerStart(boost::posix_time::microsec_clock::local_time())
{
   if ( doReportStart )
   {
      m_logger << m_logLevel << "Timer '" << m_timerName << "' started..." << Msg::EndReq;
   }
}

void Timer::end()
{
   ptime timerEnd = boost::posix_time::microsec_clock::local_time();

   boost::posix_time::time_duration duration = timerEnd - m_timerStart;

   m_logger << m_logLevel << "Timer '" << m_timerName << "' ended. Total duration is " << duration.total_milliseconds() << " ms." << Msg::EndReq;
}

