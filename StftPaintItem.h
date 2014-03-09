#ifndef STFTPAINTITEM_H
#define STFTPAINTITEM_H

#include "IPaintItem.h"
#include "StftData.h"

#include <QColor>

namespace Plotting
{

class PaintArea;

class StftPaintItem : public IPaintItem
{
   public:
      StftPaintItem( const WaveAnalysis::StftData& stftData );
      virtual ~StftPaintItem();

      double getMinX() const;
      double getMaxX() const;
      double getMinY() const;
      double getMaxY() const;

   private:
      void generatePlotCommands( PaintArea* paintArea ) const;

   private:
      const RealVector& getFrequencies() const;
      QColor calcColor( double z ) const;

   private:
      WaveAnalysis::StftData     m_stftData;
      std::vector< RealVector >  m_magnitudes;
      double                     m_maxZ;

};

} /// namespace Plotting

#endif // STFTPAINTITEM_H
