#ifndef TONE_H
#define TONE_H

#include "RealVector.h"

namespace Feature
{

class Tone
{
   public:
      Tone( double baseFrequency, const RealVector& relativeAmps );

      double getBaseFreq() const;
      const RealVector& getHarmonics() const;

   private:
      double       m_baseFreq;
      RealVector   m_relativeAmps;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double Tone::getBaseFreq() const
{
   return m_baseFreq;
}

inline const RealVector& Tone::getHarmonics() const
{
   return m_relativeAmps;
}

} /// namespace Feature

#endif // TONE_H
