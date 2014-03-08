#include "IPaintItem.h"

#include "PaintArea.h"

IPaintItem::IPaintItem()
{}

IPaintItem::~IPaintItem()
{}

void IPaintItem::paintOn( PaintArea* paintArea ) const
{
   paintArea->addPaintItem( this );
}
