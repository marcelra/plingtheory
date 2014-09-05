#ifndef ANALYSISSUITE_H
#define ANALYSISSUITE_H

/**
 * @class AnalysisSuite
 * @brief Container class for analysis procedures.
 */
class AnalysisSuite
{
   public:
      /**
       * Execute all analyses.
       */
      static void execute();
      /**
       * Execute a single analysis.
       */
      static void singleAnalysis();

   public:
      /**
       * Analyse SR peak algorithm (@see AnalysisSrpa).
       */
      static void analyseSrpa();
};

#endif // ANALYSISSUITE_H
