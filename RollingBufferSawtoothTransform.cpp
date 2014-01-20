#include "RollingBufferSawtoothTransform.h"

#include "Logger.h"

#include <math.h>

/// DEBUG includes
#include <iostream>
#include "RootUtilities.h"

RollingBufferSawtoothTransform::RollingBufferSawtoothTransform( const RawPcmData& data, double period, size_t nPeriods ) :
   data( data ),
   period( period ),
   nPeriods( nPeriods )
{
   Logger msg("RollingBufferSawtoothTransform");
   msg << Msg::Info << "Executing transform..." << Msg::EndReq;
   executeTransform();
}

void RollingBufferSawtoothTransform::executeTransform()
{
   std::vector<double> transformedBufferReal( data.size() );
   std::vector<double> transformedBufferImag( data.size() );

   double transformAmplitude = 1.0 / pi / period / nPeriods;
   double transformStep      = -2 * transformAmplitude / period;
   double transformValReal   = 0;
   double transformValImag   = 1;

   for ( size_t i = 0; i < data.size(); ++i )
   {
      if ( transformValReal < -transformAmplitude )
      {
         transformValReal += 2 * transformAmplitude;
      }
      if ( transformValImag < -transformAmplitude )
      {
         transformValImag += 2 * transformAmplitude;
      }
      transformedBufferReal[i] = data[i] * transformValReal;
      transformedBufferImag[i] = data[i] * transformValImag;
      transformValReal += transformStep;
      transformValImag += transformStep;
      // std::cout << transformedBufferReal[i] << std::endl;
   }

   resultReal.resize( data.size(), 0 );
   resultImag.resize( data.size(), 0 );

   /// Initial loop
   size_t nBufferSamples = nPeriods * period;
   for ( size_t i = 0; i < data.size(); ++i )
   {
      double integralReal = 0;
      double integralImag = 0;
      for ( size_t iInner = i; (iInner < i + nBufferSamples) && ( iInner < data.size() ); ++iInner )
      {
         integralReal += transformedBufferReal[ iInner ];
         integralImag += transformedBufferImag[ iInner ];
      }
      // std::cout << integralReal << std::endl;
      resultReal[ i ] = integralReal;
      resultImag[ i ] = integralImag;
   }

   TGraph* graph = RootUtilities::createGraph( resultReal );
   graph->Draw("AL");
}

const double RollingBufferSawtoothTransform::pi = acos(-1);
