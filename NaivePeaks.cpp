#include "NaivePeaks.h"

#include "Peak.h"

/// STL
#include <map>
#include <set>

namespace FeatureAlgorithm
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NaivePeaks::NaivePeaks( Mode mode, size_t maxNumPeaks ) :
   m_mode( mode ),
   m_maxNumPeaks( maxNumPeaks )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// execute
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector< Feature::Peak* > NaivePeaks::execute( const RealVector& realArray, Monitor* monitor )
{
   std::vector< Feature::Peak* > peaks;

   double derivLeft = 0;
   double derivRight = realArray[1] - realArray[0];

   std::vector< Mode > typeOfExtremum( realArray.size() );
   std::vector< size_t > peakPos;

   int firstPeakMaxOrMin = 0;

   /// Get derivative sign changes and determine type of first peak (max or min)
   for ( size_t iSample = 1; iSample < realArray.size() - 1; ++iSample )
   {
      derivLeft = derivRight;
      derivRight = realArray[ iSample + 1 ] - realArray[ iSample ];

      if ( derivLeft * derivRight < 0 )
      {
         peakPos.push_back( iSample );
         if ( firstPeakMaxOrMin == 0 )
         {
            firstPeakMaxOrMin = derivLeft > 0 ? 1 : -1;
         }
      }
   }

   /// Return here if no derivative sign changes were found
   size_t numPeaks = peakPos.size();
   if ( numPeaks == 0 )
   {
      return peaks;
   }

   /// Get the array indices of the pedestal points
   std::vector< size_t > leftPedestalIndex( numPeaks );
   std::vector< size_t > rightPedestalIndex( numPeaks );

   leftPedestalIndex[ 0 ] = 0;
   rightPedestalIndex[ numPeaks - 1 ] = realArray.size() - 1;
   for ( size_t i = 0; i < peakPos.size(); ++i )
   {
      if ( i != 0 )
      {
         leftPedestalIndex[ i ] = peakPos[ i - 1 ];
      }
      if ( i != numPeaks - 1 )
      {
         rightPedestalIndex[ i ] = peakPos[ i + 1 ];
      }
   }

   /// Determine the pedestal under the peak position and calculate the prominence
   std::vector< double > centrePeakPedestals( numPeaks );
   std::vector< double > peakProminence( numPeaks );
   typedef std::map< double, std::vector< size_t > > MMapDoubleInt;
   MMapDoubleInt prominenceSorted;
   for ( size_t i = 0; i < peakPos.size(); ++i )
   {
      size_t lpi = leftPedestalIndex[i];
      size_t rpi = rightPedestalIndex[i];
      double pedLeft = realArray[lpi];
      double pedRight = realArray[rpi];
      double pedSlope = ( pedRight - pedLeft ) / ( rpi - lpi );
      double pedCentre = pedSlope * ( peakPos[i] - lpi ) + pedLeft;
      centrePeakPedestals[i] = pedCentre;
      peakProminence[i] = fabs( realArray[ peakPos[i] ] - pedCentre );
      MMapDoubleInt::iterator it = prominenceSorted.find( peakProminence[i] );
      if ( it == prominenceSorted.end() )
      {
         prominenceSorted.insert( std::pair< double, std::vector< size_t > >( peakProminence[i], std::vector< size_t >(1, i) ) );
      }
      else
      {
         it->second.push_back( i );
      }
   }

   /// Select peaks by prominence
   size_t peakCount = 0;
   std::set< size_t > selectedPeaks;
   for ( MMapDoubleInt::reverse_iterator it = prominenceSorted.rbegin(); it != prominenceSorted.rend(); ++it )
   {
      if ( m_maxNumPeaks && peakCount >= m_maxNumPeaks )
      {
         break;
      }
      else
      {
         for ( size_t i = 0; i < it->second.size(); ++i )
         {
            size_t peakIndex = it->second[i];
            bool peakIsMax;
            peakIsMax = (peakIndex % 2) == 0 ? firstPeakMaxOrMin == 1 : firstPeakMaxOrMin == -1;
            Mode thisPeakMode = peakIsMax ? Max : Min;
            if ( m_mode == MaxAndMin || thisPeakMode == m_mode )
            {
               selectedPeaks.insert( peakIndex );
               ++peakCount;
            }
         }
      }
   }

   /// Dress the peak objects
   for ( std::set< size_t >::iterator it = selectedPeaks.begin(); it != selectedPeaks.end(); ++it )
   {
      size_t peakIndex = *it;


      size_t arrayIndex = peakPos[ peakIndex ];
      double height = realArray[ arrayIndex ];
      double width = 0;
      double pedestal = centrePeakPedestals[ peakIndex ];
      double position = arrayIndex;
      double prominence = height - pedestal;

      Feature::Peak* peak = new Feature::Peak( peakIndex );
      peak->setPosition( position );
      peak->setProminence( prominence );
      peak->setWidth( width );
      peak->setPedestal( pedestal );
      peaks.push_back( peak );
   }

   return peaks;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// calculateHalfWidth
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double NaivePeaks::calculateHalfWidth( const RealVector& realArray, size_t position, const std::vector< size_t >& peakPositions, size_t posVecIndex ) const
{
   double peakVal = realArray[ position ];

   size_t leftNeighbourPeakIndex = posVecIndex > 0 ? peakPositions[posVecIndex - 1] : 0;
   size_t rightNeighbourPeakIndex = posVecIndex < peakPositions.size() - 1 ? peakPositions[ posVecIndex + 1 ] : peakPositions.size();

   double pedestalLeft = realArray[ leftNeighbourPeakIndex ];
   double pedestalRight = realArray[ rightNeighbourPeakIndex ];

   /// Search half-width to left
   size_t iHwLeft = position - 1;
   while ( iHwLeft >= leftNeighbourPeakIndex )
   {
      double ratio = ( realArray[iHwLeft] - pedestalLeft ) / ( peakVal - pedestalLeft );
      if ( fabs( ratio ) < 0.5 )
      {
         break;
      }
      --iHwLeft;
   }

   /// Search half-width to right
   size_t iHwRight = position + 1;
   while ( iHwRight < rightNeighbourPeakIndex )
   {
      double ratio = ( realArray[iHwRight] - pedestalRight ) / ( peakVal - pedestalRight );
      if ( fabs( ratio ) < 0.5 )
      {
         break;
      }
      ++iHwRight;
   }
   double width = ( -iHwLeft + iHwRight ) / 2.;
   return width;
}

} /// namespace FeatureAlgorithm
