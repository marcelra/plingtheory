#ifndef IPAINTCOMMAND_H
#define IPAINTCOMMAND_H

namespace Plotting
{

class PaintArea;

class IPaintCommand
{
   public:
      virtual ~IPaintCommand();

      virtual void execute( PaintArea& paintArea ) const = 0;
};

} /// namespace Plotting

#endif // IPAINTCOMMAND_H
