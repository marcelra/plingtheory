#ifndef ANALYSISSRPA_H
#define ANALYSISSRPA_H

#include "AlgorithmBase.h"
#include "SamplingInfo.h"

#include <vector>

namespace Analysis
{

class AnalysisSrpa : public AlgorithmBase
{
   public:
      AnalysisSrpa( const std::string& name = "AnalysisSrpa", const AlgorithmBase* parent = 0 );

   public:
      void studyFrequencyPerformance( const std::vector< double >& frequencies );
      void studyFrequencyProximity( const std::vector< double >& frequencies, const std::vector< double >& frequencyDifference, double amp2 );

   private:
      size_t         m_fourierSize;
      size_t         m_zeroPadSize;
      double         m_frequency;
      double         m_phase;
      double         m_amplitude;
      SamplingInfo   m_samplingInfo;
};

} /// namespace Analysis

#endif // ANALYSISSRPA_H
