#include "Note.h"

#include <math.h>
#include <sstream>

namespace Music
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Create a note
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Note::Note( PitchClass pitch, size_t octave, const Duration& duration ) :
   m_pitch( pitch ),
   m_octave( octave ),
   m_duration( duration )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Note constructor (frequency based)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Note::Note( double frequency, const Duration& duration ) :
   m_duration( duration )
{
   double x = frequency / s_basePitchList[0];
   double semiToneFac = pow( 2.0, 1.0/12.0 );
   double nSemiD = log(x) / log( semiToneFac );

   size_t nSemi = floor( nSemiD + 0.5 );

   m_pitch = static_cast< PitchClass >( nSemi % 12 );
   m_octave = nSemi / 12;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Destroy a note
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Note::~Note()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getFrequency
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Note::getFrequency() const
{
   return getBasePitch( m_pitch ) * getOctaveMultiplier( m_octave );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getDuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Duration& Note::getDuration() const
{
  return m_duration;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getBasePitch
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Note::getBasePitch( PitchClass pitch ) const
{
   return s_basePitchList[ pitch ];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getOctaveMultiplier
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Note::getOctaveMultiplier( size_t octave ) const
{
   return ( static_cast<int>(1) << octave);
}

std::string Note::toString() const
{
   std::ostringstream result;
   switch ( m_pitch )
   {
      case ( C ):
         result << "C";
         break;
      case ( Cis ):
         result << "Cis";
         break;
      case ( D ):
         result << "D";
         break;
      case ( Dis ):
         result << "Dis";
         break;
      case ( E ):
         result << "E";
         break;
      case ( F ):
         result << "F";
         break;
      case ( Fis ):
         result << "Fis";
         break;
      case ( G ):
         result << "G";
         break;
      case ( Gis ):
         result << "Gis";
         break;
      case ( A ):
         result << "A";
         break;
      case ( Ais ):
         result << "Ais";
         break;
      case ( B ):
         result << "B";
         break;
   }
   result << m_octave;
   return result.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// modifyPitch
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Note::modifyPitch( int numSemitones )
{
   int currentPitchClass = m_pitch;
   currentPitchClass += numSemitones;

   int numOctavesChange = numSemitones / 12;
   m_octave += numOctavesChange;

   /// Correct negative pitch classes
   if ( currentPitchClass < 0 )
   {
      currentPitchClass = (currentPitchClass % 12) + 12;
   }
   else
   {
      currentPitchClass = currentPitchClass % 12;
      if ( currentPitchClass == 0 && numSemitones > 0 )
      {
         ++m_octave;
      }
   }

   /// Set current pitch
   m_pitch = static_cast< PitchClass >( currentPitchClass );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getTuningA4
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Note::getTuningA4()
{
   return s_tuningA4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base pitch list
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const double Note::s_tuningA4 = 440;

const double c0Freq = pow(2.0, 3.0/12.0)*Note::getTuningA4()/pow(2.0, 5.0);

const double Note::s_basePitchList[12] = { c0Freq,
                                           pow(2.0, 1.0/12.0)*c0Freq,
                                           pow(2.0, 2.0/12.0)*c0Freq,
                                           pow(2.0, 3.0/12.0)*c0Freq,
                                           pow(2.0, 4.0/12.0)*c0Freq,
                                           pow(2.0, 5.0/12.0)*c0Freq,
                                           pow(2.0, 6.0/12.0)*c0Freq,
                                           pow(2.0, 7.0/12.0)*c0Freq,
                                           pow(2.0, 8.0/12.0)*c0Freq,
                                           pow(2.0, 9.0/12.0)*c0Freq,
                                           pow(2.0, 10.0/12.0)*c0Freq,
                                           pow(2.0, 11.0/12.0)*c0Freq
                                         };

} /// Namespace Music

std::ostream& operator<<( std::ostream& os, const Music::Note& note )
{
   os << note.toString();
   return os;
}

