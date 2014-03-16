#ifndef ITHREAD_H
#define ITHREAD_H

#include <cstddef>
#include <string>

/// Forward declares.
namespace boost
{
class thread;
}
class Logger;

/**
 * @class IThread
 * @brief Base class for threads. New threads can be created by deriving from IThread an reimplementing the virtual method run.
 *
 * Usage:
 * DerivedFromThread thread( parameters );
 * /// configure thread here...
 * thread.start();
 * /// Do some other stuff.
 * /// Wait for the thread to finish.
 * thread.join();
 * /// Get results etc.
 *
 * Pattern is a worker thread pattern which is a short lived thread that is joined before the destructor. The thread destructor
 * will assert if the thread is not finished. Either finish the thread by calling join or by waiting for it manually using the
 * isFinished method.
 */
class IThread
{
   public:
      /**
       * Create thread with name @param threadName.
       */
      IThread( const std::string& threadName );
      /**
       * Virtual destructor.
       */
      virtual ~IThread();

   public:
      /**
       * Get Logger.
       */
      Logger& getLogger();

   public:
      /**
       * Start the thread.
       */
      void start();
      /**
       * Wait for thread finished at this method.
       */
      void join();

      /**
       * Check if the thread has started already.
       */
      bool hasStarted() const;
      /**
       * Check if the thread is running.
       */
      bool isRunning() const;
      /**
       * Check if the thread is finished.
       */
      bool isFinished() const;

   private:
      /**
       * Worker method. Derived class should override this method.
       */
      virtual void run();

   private:
      /**
       * Static wrapper around the virtual run method.
       */
      static void runWrapper( IThread* instance );
      /**
       * Waits for a number of milliseconds (@see member m_pollTimeMilliSeconds).
       */
      void waitNextPoll();

   /**
    * State administration members.
    */
   private:
      bool     m_isFinished;
      bool     m_isRunning;
      size_t   m_pollTimeMilliSeconds;

   /**
    * The boost worker thread.
    */
   private:
      boost::thread*   m_thread;

   private:
      Logger*    m_logger;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inline methods IThread
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// hasStarted
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool IThread::hasStarted() const
{
   return m_isRunning || m_isFinished;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// isRunning
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool IThread::isRunning() const
{
   return m_isRunning;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// isFinished
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool IThread::isFinished() const
{
   return m_isFinished;
}



#endif // ITHREAD_H
