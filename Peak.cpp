#include "Peak.h"

namespace Feature
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Peak::Peak( double position, double prominence, double width, double pedestal ) :
   m_position( position ),
   m_prominence( prominence ),
   m_width( width ),
   m_pedestal( pedestal )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Peak::~Peak()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setLeftNeighbourPeak
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Peak::setLeftNeighbourPeak( Peak* peak )
{
   m_leftNeighbour = peak;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// setRightNeighbourPeak
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Peak::setRightNeighbourPeak( Peak* peak )
{
   m_rightNeighbour = peak;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getLeftNeighbourPeak
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Peak* Peak::getLeftNeighbourPeak()
{
   return m_leftNeighbour;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getRightNeighbourPeak
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Peak* Peak::getRightNeighbourPeak()
{
   return m_rightNeighbour;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getLeftNeighbourPeak
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Peak* Peak::getLeftNeighbourPeak() const
{
   return m_leftNeighbour;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getRightNeighbourPeak
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Peak* Peak::getRightNeighbourPeak() const
{
   return m_rightNeighbour;
}

} /// namespace Feature
