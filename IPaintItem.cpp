#include "IPaintItem.h"

#include "PaintArea.h"

namespace Plotting
{

IPaintItem::IPaintItem()
{}

IPaintItem::~IPaintItem()
{}

void IPaintItem::paintOn( PaintArea* paintArea ) const
{
   paintArea->addPaintItem( this );
}

} /// namespace Plotting
