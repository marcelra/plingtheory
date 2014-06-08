#include "Exceptions.h"

#include "Logger.h"

#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BaseException constructors
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BaseException::BaseException()
{}

BaseException::BaseException( const std::string& msg ) :
   msg ( msg )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BaseException destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BaseException::~BaseException()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BaseException::setMessage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BaseException::setMessage( const std::string& msg )
{
   this->msg = msg;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BaseException::getMessage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::string& BaseException::getMessage() const
{
   return msg;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Logging
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os, const BaseException& exception )
{
   os << exception.getType() << ": " << exception.getMessage();
   return os;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionGeneral methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionGeneral::ExceptionGeneral( const std::string& msg ) :
   BaseException( msg )
{}

const char* ExceptionGeneral::getType() const
{
   return "ExceptionGeneral";
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionFileNotFound methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionFileNotFound::ExceptionFileNotFound( const std::string& fileName ) :
   BaseException()
{
   std::ostringstream msg;
   msg << "File not found: " << fileName;
   setMessage( msg.str() );
}

const char* ExceptionFileNotFound::getType() const
{
   return "ExceptionFileNotFound";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionFileCannotOpen methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionFileCannotOpen::ExceptionFileCannotOpen( const std::string& fileName )
{
   std::ostringstream msg;
   msg << "Cannot open file: " << fileName;
   setMessage( msg.str() );
}

const char* ExceptionFileCannotOpen::getType() const
{
   return "ExceptionFileCannotOpen";
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionKeyNotFound methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionKeyNotFound::ExceptionKeyNotFound( const std::string& key, const std::string& whereLooked )
{
   std::ostringstream msg;
   msg << "Key <" << key << "> was not found in " << whereLooked;
   setMessage( msg.str() );
}

const char* ExceptionKeyNotFound::getType() const
{
   return "ExceptionKeyNotFound";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionTypeConflict methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionTypeConflict::ExceptionTypeConflict( const std::string& context )
{
   std::ostringstream msg;
   msg << "Type conflict in " << context;
   setMessage( msg.str() );
}

const char* ExceptionTypeConflict::getType() const
{
   return "ExceptionTypeConflict";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionRead methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionRead::ExceptionRead( const std::string& objectBeingRead, const std::string& reason )
{
   std::ostringstream msg;
   msg << "While reading " << objectBeingRead << ": " << reason;
   setMessage( msg.str() );
}

const char* ExceptionRead::getType() const
{
   return "ExceptionRead";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionNotInit methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionNotInit::ExceptionNotInit( const std::string& object )
{
   std::ostringstream msg;
   msg << "Object " << object << " is not initialised.";
   setMessage( msg.str() );
}

const char* ExceptionNotInit::getType() const
{
   return "ExceptionNotInit";
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionNoSound methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionNoSound::ExceptionNoSound()
{
   setMessage( "Sound playback is not possible on this system." );
}

const char* ExceptionNoSound::getType() const
{
   return "ExceptionNoSound";
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionEmptyMultiChannelRawPcmData methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionEmptyMultiChannelRawPcmData::ExceptionEmptyMultiChannelRawPcmData()
{
   setMessage( "Empty MultiChannelRawPcmData object." );
}

const char* ExceptionEmptyMultiChannelRawPcmData::getType() const
{
   return "ExceptionEmptyMultiChannelRawPcmData";
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionTestFailed methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionTestFailed::ExceptionTestFailed( const std::string& testMethod, const std::string& reason )
{
   std::ostringstream msg;
   msg << "Method " << testMethod << " failed: " << reason;
   setMessage( msg.str() );
}

const char* ExceptionTestFailed::getType() const
{
   return "ExceptionTestFailed";
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionDataNotPrepared
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionDataNotPrepared::ExceptionDataNotPrepared( const std::string& requester, const std::string& dataName )
{
   std::ostringstream msg;
   msg << "Data " << dataName << " requested by " << requester << " is not prepared or available.";
   setMessage( msg.str() );
}

const char* ExceptionDataNotPrepared::getType() const
{
   return "ExceptionDataNotPrepared";
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionOptionUnknown
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionOptionUnknown::ExceptionOptionUnknown( const std::string& option )
{
   std::stringstream msg;
   msg << "Option " << option << " is not a valid option.";
   setMessage( msg.str() );
}

const char* ExceptionOptionUnknown::getType() const
{
   return "ExceptionOptionUnknown";
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionOptionUnknown
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionOptionArgumentParsing::ExceptionOptionArgumentParsing( const std::string& option )
{
   std::stringstream msg;
   msg << "Arguments given for option " << option << " are not valid.";
   setMessage( msg.str() );
}

const char* ExceptionOptionArgumentParsing::getType() const
{
   return "ExceptionOptionArgumentParsing";
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionNoPlotAvailable
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionNoPlotAvailable::ExceptionNoPlotAvailable() :
   BaseException( "No plot is available to plot on." )
{}

const char* ExceptionNoPlotAvailable::getType() const
{
   return "ExceptionNoPlotSelected";
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ExceptionUninitialisedPlotFactory
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExceptionUninitialisedPlotFactory::ExceptionUninitialisedPlotFactory() :
   BaseException( "No plot factory is available." )
{}

const char* ExceptionUninitialisedPlotFactory::getType() const
{
   return "ExceptionUninitialisedPlotFactory";
}
