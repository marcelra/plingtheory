#include "Tone.h"

namespace Feature
{

Tone::Tone( double baseFrequency, const RealVector& relativeAmps ) :
   m_baseFreq( baseFrequency ),
   m_relativeAmps( relativeAmps )
{
}

} /// namespace Feature
