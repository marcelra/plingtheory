#include "WaveletTransform.h"

#include "FftwAlgorithm.h"
#include "WaveletContainer.h"
#include "WaveletSpectrum.h"

#include "RootUtilities.h"

#include <iostream>

namespace WaveAnalysis
{

WaveletTransform::WaveletTransform( size_t size, size_t nLayers, size_t zeroPadding, size_t hopRate, const WindowFuncDef& windowFunction ) :
   m_fourierSize( size ),
   m_nLayers( nLayers ),
   m_zeroPadding( zeroPadding ),
   m_hopRate( hopRate ),
   m_windowFuncDef( windowFunction.clone() )
{
   assert( ( size / ( nLayers ) % hopRate ) == 0 );
}

WaveletTransform::~WaveletTransform()
{}

void WaveletTransform::initialise()
{
   delete m_windowFuncDef;
}

WaveletContainer WaveletTransform::execute( const RawPcmData& data )
{
   Logger msg( "WaveletTransform" );
   msg << Msg::Debug << "In WaveletTransform::execute" << Msg::EndReq;
   WaveletContainer waveletContainer( m_fourierSize, m_nLayers, m_zeroPadding, m_hopRate, data.getSamplingInfo() );

   size_t windowSize = m_fourierSize;
   for ( size_t iLayer = 0; iLayer < m_nLayers; ++iLayer )
   {
      std::vector< WaveletSpectrum* > spectraForThisLayer;

      size_t hopSamples = windowSize / m_hopRate;

      FftwAlgorithm fftwAlg( m_fourierSize * m_zeroPadding );
      WindowFunction* windowFunction = m_windowFuncDef->createWindowFunction( windowSize );

      size_t offset = 0;
      while ( offset < data.size() )
      {
         // std::cout << "offset = " << offset << std::endl;
         const RealVector& winData = createWindowedData( data, offset, *windowFunction, m_fourierSize * m_zeroPadding );
         const std::complex< double >* ft = fftwAlg.transform( &winData[0] );
         offset += hopSamples;

         WaveletSpectrum* waveletSpectrum = new WaveletSpectrum( data.getSamplingInfo(), *windowFunction, m_zeroPadding, ft, ft + fftwAlg.getSpectrumDimension() );
         spectraForThisLayer.push_back( waveletSpectrum );
      }

      waveletContainer.setSpectraForLayer( iLayer, spectraForThisLayer, *windowFunction );

      windowSize /= 2;
   }
   msg << Msg::Debug << "Leaving WaveletTransform::execute" << Msg::EndReq;
   return waveletContainer;
}

RealVector WaveletTransform::createWindowedData( const RawPcmData& data, size_t offset, const WindowFunction& windowFunction, size_t fourierSize )
{
   // std::cout << "in createWindowedData..." << std::endl;
   /// TODO: write directly in FFTW array?
   assert( offset < data.size() );

   const double* itBegin = &data[offset];
   bool doExpand = offset + windowFunction.getSize() >= data.size();
   // std::cout << "doExpand = " << doExpand << ", " << offset + windowFunction.getSize() << std::endl;
   const double* itEnd = !doExpand? &data[0] + offset + windowFunction.getSize() : &data[ data.size() - 1 ];

   RealVector result( itBegin, itEnd );
   // std::cout << "Created the resultvec" << std::endl;
   if ( doExpand )
   {
      result.resize( windowFunction.getSize(), 0 );
   }

   // std::cout << "result.size = " << result.size() << std::endl;

   for ( size_t i = 0; i < windowFunction.getSize(); ++i )
   {
      result[i] *= windowFunction.calc( i );
   }

   // std::cout << "Windowfunction.getSize() = " << windowFunction.getSize() << std::endl;
   result.resize( fourierSize, 0 );
   if ( offset == 0 )
   {
      new TCanvas();
      TGraph* g = RootUtilities::createGraph( result );
      g->Draw( "LA" );
   }

   // std::cout << "leaving createWindowedData" << std::endl;
   return result;

}

} /// namespace WaveAnalysis
