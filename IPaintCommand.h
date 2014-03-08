#ifndef IPAINTCOMMAND_H
#define IPAINTCOMMAND_H

class PaintArea;

class IPaintCommand
{
   public:
      virtual ~IPaintCommand();

      virtual void execute( PaintArea& paintArea ) const = 0;
};

#endif // IPAINTCOMMAND_H
