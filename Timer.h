#ifndef TIMER_H
#define TIMER_H

#include <string>

#include "Logger.h"
#include "boost/date_time/posix_time/posix_time.hpp"


class Timer
{
   typedef boost::posix_time::ptime ptime;

   public:
      Timer( Logger& logger, Msg::LogLevel logLevel, const std::string& timerName, bool doReportStart=true );
      void end();

   private:
      Logger&        m_logger;
      Msg::LogLevel  m_logLevel;
      std::string    m_timerName;
      ptime          m_timerStart;

};

#endif // TIMER_H
