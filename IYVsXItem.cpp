#include "IYVsXItem.h"

IYVsXItem::IYVsXItem( const std::vector< double >& xData, const std::vector< double >& yData ) :
   m_data( xData, yData )
{}

IYVsXItem::~IYVsXItem()
{}
