#ifndef IPAINTITEM_H
#define IPAINTITEM_H

class PaintArea;

class IPaintItem
{
   public:
      IPaintItem();
      virtual ~IPaintItem();

      virtual double getMinX() const = 0;
      virtual double getMaxX() const = 0;
      virtual double getMinY() const = 0;
      virtual double getMaxY() const = 0;

      void paintOn( PaintArea* paintArea ) const;

   private:
      virtual void generatePlotCommands( PaintArea* paintArea ) const = 0;

   private:
      friend class PaintArea;

};

#endif // IPAINTITEM_H
