#ifndef ISYNTHENVELOPE_H
#define ISYNTHENVELOPE_H

#include <cstddef>

namespace Synthesizer {

/**
 * @class ISynthEnvelope
 * @brief Interface for synthesizer envelopes
 */
class ISynthEnvelope
{
   public:
      /**
       * Virtual destructor
       */
      virtual ~ISynthEnvelope();
      /**
       * Override this method to implement a synthesizer envelope
       */
      virtual double getEnvelope( size_t iSample ) const = 0;
};

/**
 * @class TrivialEnvelope
 * @brief Zero attack, infinite sustain
 */
class TrivialEnvelope : public ISynthEnvelope
{
   public:
      double getEnvelope( size_t iSample ) const;
};

} /// namespace Synthesizer

#endif // ISYNTHENVELOPE_H
