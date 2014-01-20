#ifndef NAIVETONES_H
#define NAIVETONES_H

#include <vector>
#include <cstddef>

namespace Feature
{
   class Peak;
   class Tone;
}

namespace FeatureAlgorithm
{

class FocalTones
{
   public:
      FocalTones( const std::vector< Feature::Peak* >& fourierSpectrumPeaks );
      virtual ~FocalTones();

      void execute();

      size_t getNumTones() const;
      const Feature::Tone& getTone( size_t iTone ) const;
      const std::vector< Feature::Tone* >& getAllTones() const;

      void setDoMonitor( bool doMonitor );

   private:
      std::vector< Feature::Tone* >  m_tones;
      std::vector< Feature::Peak* >  m_freqPeaks;
      size_t                         m_iteration;
      double                         m_weightCutoff;
      bool                           m_doMonitor;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline size_t FocalTones::getNumTones() const
{
   return m_tones.size();
}

inline const Feature::Tone& FocalTones::getTone( size_t iTone ) const
{
   return *m_tones[iTone];
}

inline const std::vector< Feature::Tone* >& FocalTones::getAllTones() const
{
   return m_tones;
}

inline void FocalTones::setDoMonitor( bool doMonitor )
{
   m_doMonitor = doMonitor;
}

} /// namespace FeatureAlgorithm

#endif // NAIVETONES
