#ifndef EFFECTTRIANGLIZER_H
#define EFFECTTRIANGLIZER_H

#include "RawPcmData.h"

/**
 * @class EffectTrianglizer
 * An effect linearly extrapolates between every extremal value in the source data
 */
class EffectTrianglizer
{
   public:
      /**
       * Apply the effect to the source data
       */
      static RawPcmData::Ptr apply( const RawPcmData& source );
};

#endif // EFFECTTRIANGLIZER_H
