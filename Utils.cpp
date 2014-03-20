#include "Utils.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// findMinima
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IndexVector Utils::findMinima( const RealVector& vector )
{
   /// Define the result vector with the indices of the minima.
   IndexVector minPos;

   /// Define the left difference and right difference of current point.
   /// The right difference is calculated at the begin of every loop iteration, the left derivative is set to the right
   /// derivative at the end of the loop. This means that continue statements in the loop are dangerous.
   double leftDeriv = vector[ 1 ] - vector[ 0 ];
   double rightDeriv = leftDeriv;

   /// Define bool that will be set to true if there is a number of samples with the same sample value.
   bool isFlat = false;

   /// Store the last left derivative before a flat region as well as where it is located.
   double lastLeftDerivNotFlat = 0;
   size_t lastSampleNotFlat = 0;

   /// The first sample is a minimum if derivative > 0.
   if ( rightDeriv > 0 )
   {
      minPos.push_back( 0 );
   }

   /// Loop over all the points.
   for ( size_t i = 1; i < vector.size() - 1; ++i )
   {
      /// Calculate new right derivative.
      rightDeriv = vector[ i + 1 ] - vector[ i ];

      /// Update the isFlat state if rightDeriv is not equal to zero anymore.
      if ( isFlat && rightDeriv != 0 )
      {
         isFlat = false;
         /// This is an extended, flat, minimum (e.g. the sequence of zeroes in {2, 0, 0, 0, 1}.
         /// Both the first and the last zero samples will be detected as minima.
         if ( lastLeftDerivNotFlat < 0 && rightDeriv > 0 )
         {
            minPos.push_back( lastSampleNotFlat );
            minPos.push_back( i );
         }
      }

      /// Determine an ordinary minimum. (TODO: isFlat condition needed?)
      if ( !isFlat && leftDeriv < 0 && rightDeriv > 0 )
      {
         minPos.push_back( i );
      }

      /// If the right derivative is zero, there are at least two samples with identical values. This is already called
      /// a flat region.
      if ( !isFlat && rightDeriv == 0 )
      {
         isFlat = true;
         lastLeftDerivNotFlat = leftDeriv;
         lastSampleNotFlat = i;
      }

      /// Prepare for the next iteration by setting the left derivative equal to the right derivative.
      leftDeriv = rightDeriv;
   }

   /// The last sample is identified as a minimum if slope is downwards at boundary.
   if ( leftDeriv < 0 )
   {
      minPos.push_back( vector.size() - 1 );
   }

   /// Return all positions of minima.
   return minPos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// getSelection
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector Utils::createSelection( const RealVector& vector, const IndexVector& selectedIndices )
{
   RealVector result( selectedIndices.size() );
   for ( size_t i = 0; i < selectedIndices.size(); ++i )
   {
      result[ i ] = vector[ selectedIndices[ i ] ];
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createRange
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IndexVector Utils::createRange( size_t minIndex, size_t maxIndex )
{
   assert( minIndex < maxIndex );
   IndexVector result( maxIndex - minIndex );
   size_t i = 0;
   for ( size_t index = minIndex; index < maxIndex; ++index, ++i )
   {
      result[ i ] = index;
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createRangeReal
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector Utils::createRangeReal( size_t minIndex, size_t maxIndex )
{
   assert( minIndex < maxIndex );
   RealVector result( maxIndex - minIndex );
   size_t i = 0;
   for ( size_t index = minIndex; index < maxIndex; ++index, ++i )
   {
      result[ i ] = index;
   }
   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// createRangeReal
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RealVector Utils::createRangeReal( double min, double max, size_t nSteps )
{
   assert( nSteps >= 2 );
   RealVector result( nSteps );

   double step = ( max - min ) / ( nSteps - 1 );
   double x = min;
   for ( size_t i = 0; i < nSteps; ++i )
   {
      result[ i ] = x;
      x+= step;
   }
   return result;
}

