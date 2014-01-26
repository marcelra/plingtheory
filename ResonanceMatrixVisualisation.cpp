#include "ResonanceMatrixVisualisation.h"

#include "DynamicFourier.h"
#include "RootUtilities.h"

#include <iostream>

namespace Visualisation
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ResonanceMatrixVisualisation::ResonanceMatrixVisualisation( const WaveAnalysis::ResonanceMatrix& matrix ) :
   m_resonanceMatrix( matrix ),
   m_canvas( 0 ),
   m_hist( 0 )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ResonanceMatrixVisualisation::~ResonanceMatrixVisualisation()
{
   /// Do nothing, root objects are deleted by ROOT system
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// draw
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TCanvas* ResonanceMatrixVisualisation::draw( size_t nBinsTime )
{
   size_t nBinsFreq = m_resonanceMatrix.size();
   if ( nBinsFreq < 1 )
   {
      return 0;
   }
   size_t iSampleBeg = 0;
   size_t iSampleEnd = m_resonanceMatrix[0].size();
   return drawSelection( nBinsTime, iSampleBeg, iSampleEnd );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// drawSelection
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TCanvas* ResonanceMatrixVisualisation::drawSelection( size_t nBinsTime, size_t iSampleBeg, size_t iSampleEnd )
{
   assert( iSampleEnd > iSampleBeg );

   if ( nBinsTime < 1 || m_resonanceMatrix.size() < 1 )
   {
      return 0;
   }

   const TString& histName = RootUtilities::getInstance().generateUniqueName( "ResonanceMatrixVisualisation" );
   size_t nBinsFreq = m_resonanceMatrix.size();
   m_hist = new TH2F( histName, histName, nBinsTime, -0.5, nBinsTime - 0.5, nBinsFreq, -0.5, nBinsFreq + 0.5 );
   m_canvas = new TCanvas();

   for ( size_t iFreq = 0; iFreq < nBinsFreq; ++iFreq )
   {
      double sampleIndexDouble = 0;
      double sampleIndexStep = ( iSampleEnd - iSampleBeg ) / ( static_cast<double>( nBinsTime + 1) );
      for ( size_t iBin = 0; iBin < nBinsTime; ++iBin )
      {
         sampleIndexDouble += sampleIndexStep;
         size_t sampleIndex = sampleIndexDouble;
         // std::cout << sampleIndex << std::endl;
         double value = m_resonanceMatrix[iFreq][sampleIndex];
         m_hist->SetBinContent( iBin + 1, iFreq + 1, value );
      }
      for ( size_t iSample = iSampleBeg; iSample < iSampleEnd; ++iSample )
      {

      }
   }

   m_hist->Draw( "COLZ" );
   return m_canvas;
}

} /// namespace Visualisation
