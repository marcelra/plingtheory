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
       * Status enum.
       */
      enum ReturnStatus
      {
         Pending = 0,
         Finished,
         Failed
      };

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
       * Wait for thread to be finished at this method.
       */
      void join();
      /**
       * Kill the thread. This will stop the execution, but the user still needs to destroy the thread object.
       */
      void kill();

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
      /**
       * Check if the thread is killed.
       */
      bool isKilled() const;

      /**
       * Retrieve the thread status.
       */
      ReturnStatus getReturnStatus() const;

   private:
      /**
       * Worker method. Derived class should override this method.
       */
      virtual ReturnStatus run();

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
      bool          m_isFinished;
      bool          m_isKilled;
      bool          m_isRunning;
      ReturnStatus  m_returnStatus;
      size_t        m_pollTimeMilliSeconds;

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
   return m_isRunning || m_isFinished || m_isKilled;
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// isKilled
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool IThread::isKilled() const
{
   return m_isKilled;
}

#endif // ITHREAD_H
