#ifndef IPAINTCOMMAND_H
#define IPAINTCOMMAND_H

namespace Plotting
{

class PaintArea;

/**
 * @class IPaintCommand
 * @brief Draws itself on a paintarea with the execute command. Usually a paint command will ask the paint area for access
 * of the current painter using getPainter.
 */
class IPaintCommand
{
   public:
      /**
       * Virtual destructor for interface class.
       */
      virtual ~IPaintCommand();

      /**
       * Derived classes should implement their draw commands here.
       */
      virtual void execute( PaintArea& paintArea ) const = 0;
};

} /// namespace Plotting

#endif // IPAINTCOMMAND_H
