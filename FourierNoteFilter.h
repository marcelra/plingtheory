#ifndef FOURIERNOTEFILTER_H
#define FOURIERNOTEFILTER_H

#include "FourierFilter.h"
#include "Logger.h"
#include "NoteList.h"

#include <vector>

class RawPcmData;
class TCanvas;

namespace WaveAnalysis {

class FourierNoteFilter
{
   public:
      FourierNoteFilter( const RawPcmData& data );
      ~FourierNoteFilter();

      TCanvas* visualiseResults() const;
      RawPcmData* findData( const Music::Note& note ) const;

      void reverseGenerate( RawPcmData& data ) const;
      void executeAndReverse( RawPcmData& data );

   private:
      void execute();

      void buildFrequencyList();
      void initialiseFourierFilters();

   private:
      const RawPcmData&            m_data;
      std::vector< RawPcmData* >   m_results;

   private:
      size_t               m_nPeriods;

   private:
      Music::NoteList             m_frequencies;
      std::vector<FourierFilter*> m_fourierFilters;

   private:
      mutable Logger              m_msg;
};

} /// namespace Analysis

#endif // FOURIERNOTEFILTER_H
