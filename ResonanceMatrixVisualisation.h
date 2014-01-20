#ifndef RESONANCEMATRIXVISUALISATION_H
#define RESONANCEMATRIXVISUALISATION_H

#include "TCanvas.h"
#include "TH2F.h"

#include "DynamicFourier.h"

#include <cstddef>

namespace Visualisation
{

/**
 * @class ResonanceMatrixVisualisation
 * @brief Helper class for creating histograms from ResonanceMatrix as visualisation
 */
class ResonanceMatrixVisualisation
{
   public:
      /**
       * Constructor
       */
      ResonanceMatrixVisualisation( const WaveAnalysis::ResonanceMatrix& matrix );
      /**
       * Destructor
       */
      virtual ~ResonanceMatrixVisualisation();

   public:
      /**
       * Draw. Specify the number of time bins with nTimeBins
       */
      TCanvas* draw( size_t nTimeBins );
      /**
       * Draw a selection. iSelectionBegin and iSelectionEnd specify samples numbers of the seletion
       */
      TCanvas* drawSelection( size_t nTimeBins, size_t iSelectionBegin, size_t iSelectionEnd );
      /**
       * Get the canvas
       */
      TCanvas* getCanvas();
      /**
       * Get the histogram
       */
      TH2F* getHistogram();

   private:
      const WaveAnalysis::ResonanceMatrix&   m_resonanceMatrix;
      TCanvas*                               m_canvas;
      TH2F*                                  m_hist;
};

} /// namespace Visualisation

#endif // RESONANCEMATRIXVISUALISATION_H
