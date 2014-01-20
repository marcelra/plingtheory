#include "ShortTimeFftw.h"

#include "RawPcmData.h"

#include "TCanvas.h"
#include "TH2F.h"

/// TODO: temp
#include "RootUtilities.h"

#include <iostream>

namespace WaveAnalysis
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ShortTimeFftw::ShortTimeFftw( size_t nSamples, const RawPcmData& data, const WindowFuncDef& windowFunction ) :
   FftwAlgorithm( nSamples ),
   m_data( data),
   m_windowFunction( windowFunction.createWindowFunction( nSamples ) )
{
   calculateSpectra();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ShortTimeFftw::~ShortTimeFftw()
{
   delete m_windowFunction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// calculateSpectra
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ShortTimeFftw::calculateSpectra()
{
   size_t nSpectra = m_data.size() / getFourierSize();
   size_t nLastSamples = m_data.size() % getFourierSize();

   Logger msg( "ShortTimeFftw" );
   msg << Msg::Info << "Fourier transforming " << m_data.size() << " samples in batches of " << getFourierSize() << " samples."<< Msg::EndReq;

   for ( size_t i = 0; i < nSpectra; ++i )
   {
      const RealVector& windowData = applyWindow( &m_data[0], i * getFourierSize() );
      // const std::complex<double>* spectrum = transform( &m_data[0] + i * getNumSamples() );
      const std::complex<double>* spectrum = transform ( &windowData[0] );
      m_spectra.push_back( FourierSpectrum( m_data.getSamplingInfo(), spectrum, spectrum + getSpectrumDimension() ) );
   }

   /// Last fourier might need padding
   if ( nLastSamples == 0 )
   {
      /// ... but not in this rare case
      return;
   }

   /// Compute the sample offset at beginning of last, incomplete batch
   size_t offsetLastBatch = nSpectra * getFourierSize();

   /// Zero padding at end of last batch
   std::vector<double> lastBatch( &m_data[ offsetLastBatch ], &m_data[ offsetLastBatch + nLastSamples - 1 ] );
   lastBatch.reserve( getFourierSize() );
   for ( size_t iPaddingSample = nLastSamples; iPaddingSample < getFourierSize(); ++iPaddingSample )
   {
      lastBatch.push_back( 0 );
   }

   /// Compute the fourier transform for the last batch
   const RealVector& windowData = applyWindow( &lastBatch[0], 0 );
   const std::complex<double>* spectrum = transform( &windowData[0] );
   m_spectra.push_back( FourierSpectrum( m_data.getSamplingInfo(), spectrum, spectrum + getSpectrumDimension() ) );

   msg << Msg::Info << "Done" << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// applyWindow
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector ShortTimeFftw::applyWindow( const double* data, size_t offset )
{
   RealVector result( getFourierSize() );
   for ( size_t i = 0; i < getFourierSize(); ++i, ++offset )
   {
      result[i] = data[offset]*m_windowFunction->calc( i );
   }
   return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getNumSpectra
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t ShortTimeFftw::getNumSpectra() const
{
   return m_spectra.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getSpetrum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FourierSpectrum& ShortTimeFftw::getSpectrum( size_t index )
{
   return m_spectra[index];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getSpectrum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const FourierSpectrum& ShortTimeFftw::getSpectrum( size_t index ) const
{
   return const_cast< ShortTimeFftw* >( this )->getSpectrum(index);
}

} /// namespace WaveAnalysis

