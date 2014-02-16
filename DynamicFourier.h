#ifndef DYNAMICFOURIER_H
#define DYNAMICFOURIER_H

class RawPcmData;

#include <vector>
#include <complex>

namespace WaveAnalysis
{

typedef std::vector< std::vector< double > > ResonanceMatrix;

/**
 * @class DynamicFourier
 * @brief Fourier-like transform that multiplies the signal with exp( i*omega*t ) and then integrates
 * over nPeriods periods. Note that since the integration bounds are (t, t + delta_t), so the time
 * dependence is not removed during integration.
 */
class DynamicFourier
{
   public:
      /**
       * Constructor
       * @param testFrequencies, the omegas that are used
       * @param nPeriods, the number of periods that are integrated. This is dependent on the test frequency
       */
      DynamicFourier( const std::vector< double >& testFrequencies, double nPeriods );
      /**
       * Destructor
       */
      virtual ~DynamicFourier();

   public:
      /**
       * Apply dynamic fourier transform on data. Data is left untouched.
       */
      ResonanceMatrix execute( const RawPcmData& data ) const;

   private:
      /**
       * Readability typedef
       */
      typedef std::complex< double > Complex;
      /**
       * Calculate the product of the signal with exp( i*omega*t )
       */
      std::vector< Complex > getIntegrand( const RawPcmData& data, double frequency ) const;
      /**
       * Forward integration of integrand with nSamplesInt
       */
      std::vector< Complex > integrate( const std::vector< Complex >& integrand, size_t nSamplesInt ) const;

   private:
      std::vector< double > m_testFrequencies;
      double                m_nPeriods;
};

} /// namespace WaveAnalysis

#endif // DYNAMICFOURIER_H
