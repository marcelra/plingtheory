#include "StftPaintItem.h"

#include "PaintArea.h"
#include "PcLinePaint.h"
#include "PcSetDrawAttr.h"
#include "Utils.h"

#include <QDebug>

namespace Plotting
{

StftPaintItem::StftPaintItem( const WaveAnalysis::StftData& stftData ) :
   m_stftData( stftData )
{
   for ( size_t iSpec = 0; iSpec < stftData.getNumSpectra(); ++iSpec )
   {
      m_magnitudes.push_back( stftData.getSpectrum( iSpec ).getMagnitude() );
   }

   RealVector maxValues( m_magnitudes.size() );
   for ( size_t iSpec = 0; iSpec < stftData.getNumSpectra(); ++iSpec )
   {
      maxValues[ iSpec ] = Utils::getMaxValue( m_magnitudes[ iSpec ] );
   }
   m_maxZ = Utils::getMaxValue( maxValues );

   qDebug() << "m_maxZ = " << m_maxZ;
}

StftPaintItem::~StftPaintItem()
{}

double StftPaintItem::getMinX() const
{
   return 0;
}

double StftPaintItem::getMaxX() const
{
   return m_stftData.getNumSpectra();
}

double StftPaintItem::getMinY() const
{
   return getFrequencies()[0];
}

double StftPaintItem::getMaxY() const
{
   return getFrequencies()[ getFrequencies().size() - 1 ];
}

const RealVector& StftPaintItem::getFrequencies() const
{
   return m_stftData.getConfig().getSpectrumFrequencies();
}

void StftPaintItem::generatePlotCommands( PaintArea* paintArea ) const
{
   size_t indexMin = getMinX();
   size_t indexMax = getMaxX();

   double lineWidth = paintArea->transformToCanvasCoordinates( QPointF( 0, m_stftData.getConfig().getSpectrumFrequencies()[0] ) ).y();
   lineWidth -= paintArea->transformToCanvasCoordinates( QPointF( 0, m_stftData.getConfig().getSpectrumFrequencies()[1] ) ).y();

   double lineLength = paintArea->transformToCanvasCoordinates( QPointF( 1, 0 ) ).x();
   lineLength -= paintArea->transformToCanvasCoordinates( QPointF( 0, 0 ) ).x();

   // qDebug() << "lineWidth = " << lineWidth;
   // qDebug() << "lineLength = " << lineLength;

   PcSetDrawAttr* pcDrawAttr = new PcSetDrawAttr( Qt::black, lineWidth, false );
   paintArea->addPaintCommand( pcDrawAttr );

   size_t numDrawCommands = 0;
   for ( size_t freqIndex = 0; freqIndex < getFrequencies().size(); ++freqIndex )
   {
      for ( size_t iSpec = indexMin; iSpec < indexMax; ++iSpec )
      {
         pcDrawAttr = new PcSetDrawAttr( calcColor( m_magnitudes[ iSpec ][ freqIndex ] ), lineWidth, false );
         paintArea->addPaintCommand( pcDrawAttr );

         PcLinePaint* linePaint = new PcLinePaint( QPointF( iSpec, getFrequencies()[ freqIndex ] ), QPointF( iSpec + 1, getFrequencies()[ freqIndex ] ) );
         paintArea->addPaintCommand( linePaint );
         ++numDrawCommands;
      }
   }

   qDebug() << "numDrawCommands = " << numDrawCommands;
}

QColor StftPaintItem::calcColor( double z ) const
{
   double colFrac = ( 1 - z / m_maxZ ) * 255;
   // qDebug() << "colFrac = " << colFrac;
   return QColor( colFrac, colFrac, colFrac );
}

} /// namespace Plotting
