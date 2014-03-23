#ifndef IYVSXITEM_H
#define IYVSXITEM_H

#include "IPaintItem.h"
#include "YVsXData.h"

namespace Plotting
{

/**
 * @class IYVsXItem
 * @brief An paint item that is described by paint commands that have an YVsXData data source.
 */
class IYVsXItem : public IPaintItem
{
   public:
      IYVsXItem( const std::vector< double >& xData, const std::vector< double >& yData );

      double getMinX() const;
      double getMaxX() const;
      double getMinY() const;
      double getMaxY() const;

   protected:
      IYVsXItem();

   protected:
      YVsXData       m_data;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods IYVsXItem
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double IYVsXItem::getMinX() const
{
   return m_data.getMinX();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMaxX
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double IYVsXItem::getMaxX() const
{
   return m_data.getMaxX();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMinY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double IYVsXItem::getMinY() const
{
   return m_data.getMinY();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getMaxY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline double IYVsXItem::getMaxY() const
{
   return m_data.getMaxY();
}

} /// namespace Plotting

#endif // IYVSXITEM_H