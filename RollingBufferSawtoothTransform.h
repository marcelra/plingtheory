#ifndef ROLLINGBUFFERSAWTOOTHTRANSFORM_H
#define ROLLINGBUFFERSAWTOOTHTRANSFORM_H

#include "RawPcmData.h"

/// TODO: docu/remove?
class RollingBufferSawtoothTransform
{
   public:
      RollingBufferSawtoothTransform( const RawPcmData& data, double period, size_t nPeriods );

   private:
      void executeTransform();

      static const double  pi;

      const RawPcmData&    data;
      double               period;
      size_t               nPeriods;

      std::vector<double>  resultReal;
      std::vector<double>  resultImag;
};

#endif // ROLLINGBUFFERSAWTOOTHTRANSFORM_H
