#include "EffectTrianglizer.h"

#include "Logger.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Apply trianglizer effect to source
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Peaks are detected when the product of the left derivative and right derivative is negative.
 */
RawPcmData::Ptr EffectTrianglizer::apply( const RawPcmData& source )
{
   /// Setup logger
   Logger msg("EffectTrianglizer");
   msg << Msg::Info << "Running Triangularization algorithm..." << Msg::EndReq;

   /// Create output data set
   RawPcmData* result = new RawPcmData( source.getSamplingInfo(), source.size() );

   /// Start peak detection algorithm
   msg << Msg::Info << "Running peak detection algorithm..." << Msg::EndReq;

   /// Define peak heights and postions
   std::vector< size_t > peakPosList;
   std::vector< double > peakHeightList;

   /// First peak is postioned at 0 (to avoid clipping at start)
   peakPosList.push_back(0);
   peakHeightList.push_back(0);

   /// Define values to dermine left and right derivates
   double leftVal = source[0];
   double midVal = source[1];
   double rightVal = 0;

   /// Compute initial left and right derivatives
   double leftDeriv = midVal - leftVal;
   double rightDeriv = 0;

   for ( size_t i = 2; i < source.size(); ++i )
   {
      /// Compute the right derivative
      rightVal = source[i];
      rightDeriv = rightVal - midVal;

      /// Check if it is a peak and write peak information to lists if that is the case
      if ( rightDeriv*leftDeriv < 0 )
      {
         peakPosList.push_back( i - 1 );
         peakHeightList.push_back( midVal );
      }

      /// Shift derivatives and values to next sample
      leftVal = midVal;
      midVal = rightVal;
      leftDeriv = midVal - leftVal;
   }

   /// Last sample is also defined as a peak
   peakPosList.push_back( source.size() - 1 );
   peakHeightList.push_back( source.back() );

   /// Report peak detection stats
   msg << Msg::Info << "Peak detection done: " << peakPosList.size() << " peaks have been detected." << Msg::EndReq;

   /// Start interpolation
   msg << Msg::Info << "Interpolating peaks..." << Msg::EndReq;

   /// Peak heights. y0 will be linearly extrapolated to y1
   double y0 = 0;
   double y1 = 0;

   /// The offset (i.e. sample number) at which y0 is located
   double offset = 0;

   /// Loop over the detected peaks
   for ( size_t iPeak = 1; iPeak < peakPosList.size(); ++iPeak )
   {
      y1 = peakHeightList[iPeak];

      /// Distance between y0 and y1
      int nSamples = peakPosList[iPeak] - peakPosList[iPeak - 1];

      /// Extrapolation step per sample
      double step = static_cast<double>(y1 - y0)/( nSamples - 1);

      /// Extrapolate
      for ( int iSample = 0; iSample < nSamples; ++iSample )
      {
         (*result)[ iSample + offset ] = y0;
         y0 += step;
      }

      /// Update for next peak
      offset += nSamples;
      y0 = y1;
   }

   /// Report finished
   msg << Msg::Info << "Data generation done." << Msg::EndReq;

   /// Return result
   return RawPcmData::Ptr( result );
}

