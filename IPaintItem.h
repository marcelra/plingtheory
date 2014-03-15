#ifndef IPAINTITEM_H
#define IPAINTITEM_H

namespace Plotting
{

/// Forward declares
class PaintArea;

/**
 * @class IPaintItem.
 * @brief Interface class for items that can be painted. Curves, scatter plots, etc. should all derive from this class.
 */
class IPaintItem
{
   public:
      /**
       * Virtual destructor for interface class.
       */
      virtual ~IPaintItem();

      /**
       * Query the bounds of the data.
       */
      virtual double getMinX() const = 0;
      virtual double getMaxX() const = 0;
      virtual double getMinY() const = 0;
      virtual double getMaxY() const = 0;

   /**
    * Generate the plot commands that will be sent to the paint area in order to draw itself.
    */
   private:
      virtual void generatePlotCommands( PaintArea* paintArea ) const = 0;

   private:
      friend class PaintArea;

};

} /// namespace Plotting

#endif // IPAINTITEM_H
