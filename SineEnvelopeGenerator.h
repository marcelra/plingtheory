#ifndef SINEENVELOPEGENERATOR_H
#define SINEENVELOPEGENERATOR_H

class RawPcmData;

namespace Temp {

class SineEnvelopeGenerator
{
   public:
      SineEnvelopeGenerator( double frequency, const RawPcmData& envelope );

      void mixIntoBuffer( RawPcmData& buffer );

   private:
      double            m_frequency;
      const RawPcmData& m_envelope;
};

} /// namespace Temp

#endif // SINEENVELOPEGENERATOR_H
