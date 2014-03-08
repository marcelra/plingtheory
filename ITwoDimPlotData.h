#ifndef ITWODIMPLOTDATA_H
#define ITWODIMPLOTDATA_H

class ITwoDimPlotData
{
   public:
      ITwoDimPlotData();
      virtual ~ITwoDimPlotData();

   public:
      virtual double getMinX() const = 0;
      virtual double getMaxX() const = 0;
      virtual double getMinY() const = 0;
      virtual double getMaxY() const = 0;

};

#endif // ITWODIMPLOTDATA_H
