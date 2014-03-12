#ifndef ITWODIMPLOTDATA_H
#define ITWODIMPLOTDATA_H

namespace Plotting
{

/**
 * @class ITwoDimPlotData
 * @brief Requirements for two dimensional plot data.
 * TODO: consider usefulness of this class (use for signals?)?
 */
class ITwoDimPlotData
{
   public:
      /**
       * Virtual destructor.
       */
      virtual ~ITwoDimPlotData();

   public:
      /**
       * Get the minimum value of the data along the x-axis.
       */
      virtual double getMinX() const = 0;
      /**
       * Get the maximum value of the data along the x-axis.
       */
      virtual double getMaxX() const = 0;
      /**
       * Get the minimum value of the data along the y-axis.
       */
      virtual double getMinY() const = 0;
      /**
       * Get the maximum value of the data along the y-axis.
       */
      virtual double getMaxY() const = 0;

};

} /// namespace Plotting

#endif // ITWODIMPLOTDATA_H
