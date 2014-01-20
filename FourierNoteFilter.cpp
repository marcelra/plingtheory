#include "FourierNoteFilter.h"

#include "Note.h"
#include "NoteList.h"
#include "RawPcmData.h"

#include "TCanvas.h"
#include "TH2F.h"

#include "SineEnvelopeGenerator.h"

#include <math.h>
#include <iostream>

namespace WaveAnalysis {

FourierNoteFilter::FourierNoteFilter( const RawPcmData& data ) :
   m_data( data ),
   m_nPeriods( 10 ),
   m_msg( "FourierNoteFilter" )
{
   execute();
}

FourierNoteFilter::~FourierNoteFilter()
{
   for ( size_t i = 0; i < m_fourierFilters.size(); ++i )
   {
      delete m_fourierFilters[i];
      delete m_results[i];
   }
}

void FourierNoteFilter::buildFrequencyList()
{
   /// TODO: HACK!!!
   size_t lowestIndex = 0;
   size_t highestIndex = 12*9 + 7;

   Music::Note note( Music::Note::C, 0, Music::Duration(1) );
   note.modifyPitch( +lowestIndex );

   for ( size_t i = lowestIndex; i < highestIndex; ++i )
   {
      m_frequencies.push_back( note );
      note.modifyPitch( +1 );
      m_msg << Msg::Debug << "Adding test frequency " << note.getFrequency() << Msg::EndReq;
   }
}

void FourierNoteFilter::initialiseFourierFilters()
{
   for ( size_t i = 0; i < m_frequencies.size(); ++i )
   {
      m_fourierFilters.push_back( new FourierFilter( m_frequencies[i].getFrequency(), m_nPeriods, m_data.getSamplingInfo() ) );
   }
}

void FourierNoteFilter::execute()
{
   /// Build frequency list
   buildFrequencyList();

   /// Initialise Fourier Filters
   initialiseFourierFilters();

   /// Execute fourier filters
   for ( size_t i = 0; i < m_fourierFilters.size(); ++i )
   {
      m_msg << Msg::Info << "Applying Fourier filtering on " << m_data.size() << " samples for frequency " << m_frequencies[i] << Msg::EndReq;
      m_results.push_back( m_fourierFilters[i]->apply( m_data ).release() );
   }
   m_msg << Msg::Info << "FourierNoteFilter::execute successful" << Msg::EndReq;
}

/// TODO: HACK METHOD!!!
void FourierNoteFilter::executeAndReverse( RawPcmData& data )
{
   assert( false );
   buildFrequencyList();
   // initialiseFourierFilters();

   std::vector<double> frequencies;
   for ( size_t i = 0; i < m_frequencies.size(); ++i )
   {
      frequencies.push_back( m_frequencies[i].getFrequency() );
   }
   for ( size_t j = 0; j < m_frequencies.size(); ++j )
   {
      frequencies.push_back( 1.1618 * frequencies[j] );
   }
   for ( size_t j = 0; j < m_frequencies.size(); ++j )
   {
      frequencies.push_back( 1.1618 * 1.1618 * frequencies[j] );
   }
   for ( size_t j = 0; j < m_frequencies.size(); ++j )
   {
      frequencies.push_back( 1.1618 * 1.1618 * 1.1618 * frequencies[j] );
   }

   for ( size_t i = 0; i < frequencies.size(); ++i )
   {
      m_msg << Msg::Info << "Freq = " << frequencies[i] << Msg::EndReq;
      m_fourierFilters.push_back( new FourierFilter( frequencies[i], m_nPeriods, data.getSamplingInfo() ) );
   }
   for ( size_t i = 0; i < m_fourierFilters.size(); ++i )
   {
      m_msg << Msg::Info << "Applying Fourier filtering on " << m_data.size() << " samples for frequency " << frequencies[i] << Msg::EndReq;
      RawPcmData::Ptr envData = m_fourierFilters[i]->apply( m_data );
      Temp::SineEnvelopeGenerator seg( frequencies[i], *envData );
      m_msg << Msg::Info << "Mixing data into buffer..." << Msg::EndReq;
      seg.mixIntoBuffer( data );
      /* TODO: BIGGEST HACK EVER, (empty class creation) */ m_results.push_back( new RawPcmData( data.getSamplingInfo() ) );
   }
   data.normaliseToPeak();
}

RawPcmData* FourierNoteFilter::findData( const Music::Note& note) const
{
   for ( size_t iNote = 0; iNote < m_frequencies.size(); ++iNote )
   {
      if ( fabs( note.getFrequency() - m_frequencies[iNote].getFrequency() ) < 1e-12 )
      {
         return m_results[iNote];
      }
   }
   return 0;
}

TCanvas* FourierNoteFilter::visualiseResults() const
{
   TCanvas* can = new TCanvas("FourierFilter");

   size_t nBinsX = 100;

   TH2F* h2 = new TH2F( "h2", "h2", nBinsX, -0.5, nBinsX - 0.5, m_frequencies.size(), -0.5, m_frequencies.size() - 0.5 );

   for ( size_t iNote = 0; iNote < m_frequencies.size(); ++iNote )
   {
      RawPcmData* data = findData( m_frequencies[iNote] );
      m_msg << Msg::Info << "Found data with " << data->size() << " points" << Msg::EndReq;

      h2->GetYaxis()->SetBinLabel( iNote + 1, m_frequencies[iNote].toString().c_str() );

      for ( size_t i = 0; i < nBinsX; ++i )
      {
         double fracLow = i/(nBinsX+1.0);
         double fracHigh = (i + 1)/(nBinsX + 1.0);
         double val = 0;
         for ( size_t jAvg = fracLow*data->size(); jAvg < fracHigh*data->size(); ++jAvg )
         {
            val += data->at( jAvg );
         }
         size_t binIndex = i + 1;
         h2->SetBinContent( binIndex, iNote + 1, val );
         std::cout << val << std::endl;
      }
   }

   h2->Draw( "COLZ" );

   return can;
}

void FourierNoteFilter::reverseGenerate( RawPcmData& data ) const
{
   for ( size_t iNote = 0; iNote < m_frequencies.size(); ++iNote )
   {
      RawPcmData* envData = findData( m_frequencies[iNote] );
      m_msg << Msg::Info << "Reverse generating note " << m_frequencies[iNote] << Msg::EndReq;
      Temp::SineEnvelopeGenerator seg( m_frequencies[iNote].getFrequency(), *envData );
      seg.mixIntoBuffer( data );
   }
   data.normaliseToPeak();
   return;
}

} /// namespace Analysis
