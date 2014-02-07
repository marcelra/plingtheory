#include "GlobalParameters.h"

#include "boost/filesystem.hpp"
// #include <boost/filesystem>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getTrunkPath
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string GlobalParameters::getTrunkPath()
{
   return "/Users/marcelra/Dev/QtCreator/trunk/";
}

std::string GlobalParameters::getRunDir()
{
   const boost::filesystem::path& path = boost::filesystem::initial_path();
   return path.string() + "/";
}

const char* GlobalParameters::getProgramName()
{
   return "plingtheory";
}

void GlobalParameters::setTestDataDir( const std::string& dir )
{
   s_testDataDir = dir;
}

const std::string& GlobalParameters::getTestDataDir()
{
   return s_testDataDir;
}

// std::string GlobalParameters::s_testDataDir = "/Users/marcelra/Dev/Suite_soundArchive/";
std::string GlobalParameters::s_testDataDir = "/home/marcelra/mac/Dev/Suite_soundArchive/";
