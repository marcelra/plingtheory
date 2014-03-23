#ifndef HISTOGRAMITEM_H
#define HISTOGRAMITEM_H

#include "CurveItem.h"
#include "IAccumArray.h"

namespace Plotting
{

class HistogramItem : public CurveItem
{
   public:
      HistogramItem( const Math::IAccumArray& data );
};

} /// namespace Plotting

#endif // HISTOGRAMITEM_H
