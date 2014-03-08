#include "YVsXData.h"

#include <cassert>
#include <cstddef>
#include <limits>

YVsXData::YVsXData( const std::vector< double >& xData, const std::vector< double >& yData ) :
   m_xData( xData ),
   m_yData( yData ),
   m_xMin( std::numeric_limits< double >::max() ),
   m_xMax( -std::numeric_limits< double >::max() ),
   m_yMin( std::numeric_limits< double >::max() ),
   m_yMax( -std::numeric_limits< double >::max() )
{
   assert( xData.size() == yData.size() );
   assert( xData.size() > 0 );

   for ( size_t i = 0; i < xData.size(); ++i )
   {
      if ( xData[ i ] < m_xMin )
      {
         m_xMin = xData[ i ];
      }
      if ( xData[ i ] > m_xMax )
      {
         m_xMax = xData[ i ];
      }
      if ( yData[ i ] < m_yMin )
      {
         m_yMin = yData[ i ];
      }
      if ( yData[ i ] > m_yMax )
      {
         m_yMax = yData[ i ];
      }
   }
}

YVsXData::~YVsXData()
{}
