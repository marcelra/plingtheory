#include "IGenerator.h"

namespace Synthesizer
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Virtual destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IGenerator::~IGenerator()
{}

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
      RawPcmData::Ptr oneNoteData = generate( numSamples, samplingInfo );
      result->mixAdd( *oneNoteData, currentOffset );
      currentOffset += numSamples;
   }
   return RawPcmData::Ptr( result );
}

} /// namespace Synthesizer
