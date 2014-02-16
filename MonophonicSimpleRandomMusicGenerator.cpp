#include "MonophonicSimpleRandomMusicGenerator.h"

#include "IGenerator.h"
#include "Logger.h"
#include "RootUtilities.h"
#include "SineGenerator.h"

#include <math.h>

namespace {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper class
/// Stores the note and the calculated duration in number of samples
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GeneratedNote
{
   public:
      GeneratedNote( const Music::Note& note, size_t numSamples );
      Music::Note  note;
      size_t       numSamples;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GeneratedNote::GeneratedNote( const Music::Note& note, size_t numSamples ) :
   note( note ),
   numSamples( numSamples )
{}

} /// Namespace private to this source file

namespace Music {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MonophonicSimpleRandomMusicGenerator::MonophonicSimpleRandomMusicGenerator( const std::vector<Note>& availableNotes, int seed ) :
   m_availableNotes( availableNotes ),
   m_random( seed ),
   m_synth( 0 )
{
   Logger msg( "MonophonicSimpleRandomMusicGenerator" );
   msg << Msg::Info << "Initialised with " << m_availableNotes.size() << " notes to choose from." << Msg::EndReq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// useSynthesizer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MonophonicSimpleRandomMusicGenerator::useSynthesizer( Synthesizer::IGenerator* synth )
{
   m_synth = synth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// generateRandomMusic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RawPcmData::Ptr MonophonicSimpleRandomMusicGenerator::generateRandomMusic( size_t numNotes, std::vector<Note>* generatedNotesResult )
{
   Logger msg( "MonophonicSimpleRandomMusicGenerator" );

   SamplingInfo samplingInfo( 44100 );

   /// Construct a default synthesizer of no synthesizer is given
   Synthesizer::IGenerator* generator = m_synth;
   std::auto_ptr< Synthesizer::SineGenerator > defaultSynth( 0 );
   if ( !generator )
   {
      defaultSynth.reset( new Synthesizer::SineGenerator( samplingInfo ) );
      m_synth = defaultSynth.get();
      m_synth->setAmplitude( 0.2 );
   }

   /// Generate random notes
   std::vector< GeneratedNote > generatedNotes;

   size_t numSamplesTotal = 0;
   for ( size_t iNote = 0; iNote < numNotes; ++iNote )
   {
      /// Draw a random note
      size_t noteIndex = floor( m_random.Uniform( 0, m_availableNotes.size() ) );

      /// Get duration
      const Note& note = m_availableNotes[noteIndex];
      const Duration& duration = note.getDuration();

      /// Determine the number of samples to be added to the current PCM data
      size_t numSamples = duration.getSeconds( 120 ) * samplingInfo.getSamplingRate();
      numSamplesTotal += numSamples;

      generatedNotes.push_back( GeneratedNote( note, numSamples ) );
      if ( generatedNotesResult )
      {
         generatedNotesResult->push_back( note );
      }
   }


   RawPcmData::Ptr rawPcmData( new RawPcmData( samplingInfo, numSamplesTotal, 0 ) );

   size_t mixOffset = 0;
   for ( size_t iNote = 0; iNote < generatedNotes.size(); ++iNote )
   {
      const Note& note = generatedNotes[iNote].note;
      size_t numSamples = generatedNotes[iNote].numSamples;

      msg << Msg::Verbose << "Generating new note " << note << " with duration of " << numSamples << " samples." << Msg::EndReq;

      /// Generate wave data for each note
      m_synth->setFrequency( note.getFrequency() );
      RawPcmData::Ptr noteData = m_synth->generate( numSamples );
      rawPcmData->mixAdd( *noteData, mixOffset );
      mixOffset += numSamples;
   }

   /// Restore m_synth if a backup synth has been used.
   if ( defaultSynth.get() )
   {
      m_synth = 0;
   }

   return rawPcmData;
}

} /// namespace Music
