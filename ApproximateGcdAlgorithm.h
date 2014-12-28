#ifndef APPROXIMATEGCDALGORITHM_H
#define APPROXIMATEGCDALGORITHM_H

#include "AlgorithmBase.h"

#include <vector>

namespace Math
{

class ApproximateGcdAlgorithm : public AlgorithmBase
{
   public:
      struct Result
      {
         double gcd;
         double remainder;
      };
   public:
      ApproximateGcdAlgorithm( double tolerance, const std::string& name = "ApproximateGcdAlgorithm", const AlgorithmBase* parent = 0 );

      Result execute( double a, double b ) const;
      Result execute( const std::vector< double >& vec ) const;

   private:
      double         m_tolerance;
};

} /// namespace Math

#endif // APPROXIMATEGCDALGORITHM_H
