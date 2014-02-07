#include "FocalTones.h"

#include "AlgorithmExceptions.h"
#include "Logger.h"
#include "Peak.h"
#include "Tone.h"
#include "Typedefs.h"

/// TODO: temp
#include <iostream>
#include "RootUtilities.h"
#include "TGraph2D.h"

#include "IObjectiveFunction.h"

#include <set>
#include <algorithm>

namespace
{

class GroundToneLikelihood : public Math::IObjectiveFunction
{
   public:
      GroundToneLikelihood( const std::vector< RealVector >& groundToneCandidates,
                            const std::vector< RealVector >& groundToneUncertainty );

      double evaluate( const RealVector& x ) const;
      size_t getNumParameters() const;

      void setTemperature( double temperature );
      void setChi2Cutoff( double chi2Cuttoff );

   private:
      double calcWeight( double chi2Term ) const;
      void updateCutoffTerm();

   private:
      const std::vector< RealVector >& m_groundToneCandidates;
      const std::vector< RealVector >& m_groundToneUncertainty;
      double                           m_temperature;
      double                           m_chi2Cutoff;
      double                           m_cutoffTerm;
};

GroundToneLikelihood::GroundToneLikelihood( const std::vector< RealVector >& groundToneCandidates,
                                            const std::vector< RealVector >& groundToneUncertainty ) :
   m_groundToneCandidates( groundToneCandidates ),
   m_groundToneUncertainty( groundToneUncertainty ),
   m_temperature( 1 ),
   m_chi2Cutoff( 7 )
{
   assert( m_groundToneUncertainty.size() == m_groundToneCandidates.size() );
   updateCutoffTerm();
}

double GroundToneLikelihood::evaluate( const RealVector& x ) const
{
   double t = x[0];

   assert( m_groundToneCandidates.size() > 0 );
   double sumWeights = 0;

   for ( size_t iHarmonic = 0; iHarmonic < m_groundToneCandidates.size(); ++iHarmonic )
   {
      for ( size_t iPeak = 0; iPeak < m_groundToneCandidates[iHarmonic].size(); ++iPeak )
      {
         double relDiff = ( t - m_groundToneCandidates[iHarmonic][ iPeak ] ) / m_groundToneUncertainty[iHarmonic][iPeak];
         double chi2Term = relDiff * relDiff;
         double weight = calcWeight( chi2Term );
         sumWeights += weight;
      }
   }
   return sumWeights;
}

size_t GroundToneLikelihood::getNumParameters() const
{
   return 1;
}

double GroundToneLikelihood::calcWeight( double chi2Term ) const
{
   double argTerm = -chi2Term / m_temperature;
   if ( argTerm < -50 )
   {
      return 0;
   }
   double expTerm = exp( argTerm );
   return expTerm / ( expTerm + m_cutoffTerm );
}

void GroundToneLikelihood::setTemperature( double temperature )
{
   m_temperature = temperature;
   updateCutoffTerm();
}

void GroundToneLikelihood::setChi2Cutoff( double chi2Cutoff )
{
   m_chi2Cutoff = chi2Cutoff;
   updateCutoffTerm();
}

void GroundToneLikelihood::updateCutoffTerm()
{
   double argCutoff = -m_chi2Cutoff / m_temperature;
   m_cutoffTerm = exp( argCutoff );
}

} /// anonymous namespace

namespace FeatureAlgorithm
{

FocalTones::FocalTones( const std::vector< Feature::Peak* >& fourierSpectrumPeaks ) :
   m_freqPeaks( fourierSpectrumPeaks ),
   m_iteration( 0 ),
   m_weightCutoff( 3.5 ),
   m_doMonitor( false )
{
}

FocalTones::~FocalTones()
{
   for ( size_t i = 0; i < m_tones.size(); ++i )
   {
      delete m_tones[i];
   }
}

/// TODO: remove development code
void FocalTones::execute()
{
   Logger msg( "FocalTones" );
   msg << Msg::Debug << "In execute..." << Msg::EndReq;

   if ( m_freqPeaks.size() == 0 )
   {
      msg << Msg::Debug << "Execute completed" << Msg::EndReq;
      return;
   }
   if ( m_iteration > 100 )
   {
      throw AlgorithmTooManyIterations( "FocalTones", m_iteration );
   }

   size_t nHarmonicMax = 20;

   /// TODO: development
   RealVector graphX;
   RealVector graphY;
   RealVector graphXErr;
   RealVector graphYErr;

   double minGroundTone = 1e99;
   double maxGroundTone = -1e99;

   std::vector< RealVector > groundToneCandidates( nHarmonicMax, RealVector( m_freqPeaks.size() ) );
   std::vector< RealVector > groundToneUncertainty( nHarmonicMax, RealVector( m_freqPeaks.size() ) );
   for ( size_t iHarmonic = 1; iHarmonic <= nHarmonicMax; ++iHarmonic )
   {
      double factor = 1.0 / iHarmonic;
      RealVector& allPeaks = groundToneCandidates[ iHarmonic - 1 ];
      RealVector& uncertaintyVec = groundToneUncertainty[ iHarmonic - 1 ];
      for ( size_t iPeak = 0; iPeak < m_freqPeaks.size(); ++iPeak )
      {
         double val = factor * m_freqPeaks[iPeak]->getPosition();
         allPeaks[iPeak] = val;
         uncertaintyVec[ iPeak ] = factor;

         /// TODO: development
         graphX.push_back( iHarmonic );
         graphY.push_back( allPeaks[iPeak] );
         graphXErr.push_back( 0 );
         graphYErr.push_back( factor );

         minGroundTone = val < minGroundTone? val : minGroundTone;
         maxGroundTone = val > maxGroundTone? val : maxGroundTone;
      }
   }

   if ( m_doMonitor )
   {
      new TCanvas();
      RootUtilities::createGraphErrors( graphX, graphY, graphXErr, graphYErr )->Draw( "AEP" );
   }

   GroundToneLikelihood lhf( groundToneCandidates, groundToneUncertainty );

   double cutoff = 5;
   lhf.setTemperature( 1 );
   lhf.setChi2Cutoff( cutoff );

   double weight = -1;
   std::vector< double > lhEval;
   std::vector< double > gtX;
   double freq = 0;

   for ( size_t iHarmonic = 0; iHarmonic < groundToneCandidates.size(); ++iHarmonic )
   {
      for ( size_t iCand = 0; iCand < groundToneCandidates[iHarmonic].size(); ++iCand )
      {
         double x = groundToneCandidates[iHarmonic][iCand];
         double val = lhf.evaluate( RealVector( 1, x ) );
         if ( val > weight )
         {
            weight = val;
            freq = x;
         }
         gtX.push_back( x );
         lhEval.push_back( val );
      }
   }

   if ( weight < m_weightCutoff )
   {
      msg << Msg::Verbose << "No more ground-tones with weight > " << m_weightCutoff << ". Recursion stopped after " << m_iteration << " iterations." << Msg::EndReq;
      return;
   }
   msg << Msg::Verbose << "Ground tone detected with frequency " << freq << ". Weight = " << weight << Msg::EndReq;

   m_tones.push_back( new Feature::Tone( freq, RealVector() ) );

   std::set< size_t > freqPeaksToRemove;
   for ( size_t iHarmonic = 0; iHarmonic < groundToneCandidates.size(); ++iHarmonic )
   {
      for ( size_t iCand = 0; iCand < groundToneCandidates[iHarmonic].size(); ++iCand )
      {
         double val = groundToneCandidates[iHarmonic][iCand];
         double err = groundToneUncertainty[iHarmonic][iCand];
         if ( ( val - freq ) / err < cutoff )
         {
            freqPeaksToRemove.insert( iCand );
         }
      }
   }

   std::vector< Feature::Peak* > newFreqPeakVector;
   for ( size_t i = 0; i < m_freqPeaks.size(); ++i )
   {
      if ( std::find( freqPeaksToRemove.begin(), freqPeaksToRemove.end(), i ) == freqPeaksToRemove.end() )
      {
         newFreqPeakVector.push_back( m_freqPeaks[i] );
      }
   }

   m_freqPeaks = newFreqPeakVector;
   ++m_iteration;
   execute();


   // for ( double gt = 0; gt < 60; gt += 1 )
   // {
   //    double lh = lhf.evaluate( RealVector( 1, gt ) );
   //    lhEval.push_back( lh );
   //    gtX.push_back( gt );
   // }
//
}

} /// namespace FeatureAlgorithm
