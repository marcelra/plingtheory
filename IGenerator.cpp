#include "IGenerator.h"

#include <math.h>

namespace Synthesizer
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IGenerator::IGenerator( const SamplingInfo& samplingInfo ) :
   m_envelope( new TrivialEnvelope() ),
   m_samplingInfo( samplingInfo ),
   m_amplitude( 1 ),
   m_frequency( 440 ),
   m_phase( 0 ),
   m_envelopePhase( 0 )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Virtual destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IGenerator::~IGenerator()
{
   delete m_envelope;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Generate sequence of notes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RawPcmData::Ptr IGenerator::generate( const std::vector< Music::Note >& notes, double bpm, const SamplingInfo& samplingInfo )
{
   /// Calculate total number of samples that will be generated
   size_t numSamplesTotal = 0;
   for ( size_t iNote = 0; iNote < notes.size(); ++iNote )
   {
      numSamplesTotal += notes[iNote].getDuration().getNumSamples( bpm, samplingInfo );
   }

   size_t currentOffset = 0;
   RawPcmData* result = new RawPcmData( samplingInfo, numSamplesTotal, 0 );
   for ( size_t iNote = 0; iNote < notes.size(); ++iNote )
   {
      const Music::Note& note = notes[iNote];
      size_t numSamples = note.getDuration().getNumSamples( bpm, samplingInfo );
      double frequency = note.getFrequency();
      setFrequency( frequency );
      resetEnvelopePhase();
      RawPcmData::Ptr oneNoteData = generate( numSamples );
      result->mixAdd( *oneNoteData, currentOffset );
      currentOffset += numSamples;
   }
   return RawPcmData::Ptr( result );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getEnvelope
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IGenerator::setEnvelope( ISynthEnvelope* envelope )
{
   delete m_envelope;
   m_envelope = envelope;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getEnvelope (const)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const ISynthEnvelope& IGenerator::getEnvelope() const
{
   return *m_envelope;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getEnvelope
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISynthEnvelope& IGenerator::getEnvelope()
{
   return *m_envelope;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getModPhase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double IGenerator::getModPhase() const
{
   double rawPhase = fmod( getPhase(), 2*M_PI );
   if ( rawPhase < 0 )
   {
      return rawPhase + 2*M_PI;
   }
   else
   {
      return rawPhase;
   }
}

} /// namespace Synthesizer
