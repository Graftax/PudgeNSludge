//
// Logger.h
//		Author: Joel Cioffi
//		Authored: 1/18/2012
//		Purpose: This class is a thread-safe global logger. It can
//				 log to a file, to a console, or both.  Log messages
//				 can be filtered by severity. Any log message with
//				 an erno equal to or less than the log level will
//				 be logged. E.g. If _consoleLogLevel is 2, all logs
//				 marked with LOG_INFO, LOG_WARN, or LOG_ERROR will
//				 be printed out to the console(stdout).
//				 The LOG function works printf style, so variables can
//				 easily be printed out to a file or console.
//
//		NOTE:    This makes debugging significantly easier. Please believe me!
//
//
//		Usage:	 To log a message to either the console or the log file, use the following call
//
//					LOG(<log_level>, <scoped_function_name>, <log_message>, <...>)
//																				  ^ notice the lack of a ';', don't put one
//			e.g.:	void Renderer::Render()
//					{
//						if (OpenGlErrorHappened())
//							LOG(LOG_ERROR, "Renderer::Render", "An openGL error occurred! Error Number: %d", GetOpenGLErrorNumber())
//					}
//
#ifndef JCLOGGER_H_
#define JCLOGGER_H_

#include "Lock.h"
#include <fstream>

// Log Levels for log message filtering
#define LOG_NONE -1
#define LOG_ERROR 0
#define LOG_WARN 1
#define LOG_INFO 2
#define LOG_DEBUG 3

// The following #defines allow the logger to be toggled on and of very easily
// for final turn in builds
//
// TODO: Switch the two #defines so all LOG calls are replaced with whitespace before final turn in.
#define LOG(LogLevel, ScopedFunctionName, LogMessage, ...) /*Cioffi::*/Logger::Log((char)(LogLevel), (const char *)(ScopedFunctionName), (const char *)(LogMessage), __VA_ARGS__);
// To disable the logger:
//#define LOG(LogLevel, ScopedFunctionName, LogMessage, ...) 

 //namespace Cioffi
 //{
	class Logger
	{
	private:
		bool _initialized;
		char _fileLogLevel;
		char _consoleLogLevel;

		char *_baseFolder;
		char *_logFileName;
		std::ofstream _logFile;
		//Lock _fileLock;
		//Lock _consoleLock;

		Logger(const char *LogFile);
		~Logger();
		Logger(const Logger &);
		Logger &operator=(const Logger &);

		// Author: Joel Cioffi
		// Authored: 1/18/2012
		// Purpose: This function will find the directory and file
		//			used for logging. The directory is used to look
		//			for the log file in order to append a number if
		//			the log file exists, e.g. If DeadWest.log exists,
		//			the new log file would be DeadWest01.log
		void _Initialize(const char *FileName);
		// Author: Joel Cioffi
		// Authored: 1/18/2012
		// Purpose: This function will filter log messages by erno, and call
		//			_BuildLogString to get a formatted log message. This function
		//			behaves similarly to printf, where variables can be inserted
		//			into the string.
		//			It should only ever be called internally, don't use it.
		void _Log(char Error, const char *ScopedFunctionName, const char *str, va_list *args = NULL);
		// Author: Joel Cioffi
		// Authored: 1/18/2012
		// Purpose: This function will take the provided log values and output
		//			a string in the following format:
		//			  time      type     scoped file name   reformatted string with inserted variables
		//			[HH:MM:SS] ERROR in <class>::<function> <msg>
		//			this function assumes that the returned value is deleted elsewhere
		char *_BuildLogString(char Error, const char *ScopedFunctionName, const char *str, va_list *args = NULL);
		// Author: Joel Cioffi
		// Authored: 1/18/2012
		// Purpose: This function will log the ErrorString to
		//			an open logfile, if the log file has been
		//			opened and is good().
		//
		// Arguments: ErrorString - The string to write to the log file, null terminated
		//
		//	TODO:	Add functionality to close and reopen a bad file, this action should not truncate.
		//			A retry count should be used to prevent an attempt to open a file multiple times per
		//			frame if the action is failing.
		void _LogToFile(const char *ErrorString);
		// Author: Joel Cioffi
		// Authored: 1/18/2012
		// Purpose: This function will simply call printf.
		//			A lock is used to make sure a single line is fully printed
		//			before another is able to start. Issues with threads using
		//			stdout cause splicing
		void _LogToConsole(const char *ErrorString);

		static Logger *_highlander;
	public:
		// Author: Joel Cioffi
		// Authored: 1/18/2012
		// Purpose: This function will create the singleton using the passed
		//			file name as the log file name.  If that log file exists,
		//			a number will be appended to the file name until an unused
		//			file name is found.
		//
		// Arguments: FileName - desired file for logger output.
		static void Initialize(const char *FileName = NULL);
		// Author: Joel Cioffi
		// Authored: 1/18/2012
		// Purpose: This function will filter log messages by erno, and call
		//			_BuildLogString to get a formatted log message. This function
		//			behaves similarly to printf, where variables can be inserted
		//			into the string.
		//
		// Arguments: erno - The log level in which this message falls
		//			  ScopedFunctionName - a C-style NULL terminated string representing: <class>::<function name> [to know where log messages come from]
		//			  msg - the actual log message, in printf format (%d, %f, %x, etc.)
		//			  ... - arguments corresponding to % variable markers in msg
		static void Log(char erno, const char *ScopedFunctionName, const char *msg, ...);

		// Author: Joel Cioffi
		// Authored: 1/18/2012
		// Purpose: This function will clear STDOUT
		static void ClearConsole();

		// Author: Joel Cioffi
		// Authored: 1/18/2012
		// Purpose: This function closes the file, if it is open, and cleans
		//			up any resources allocated by the logger.
		static void Shutdown();
	};
//};

#endif // JCLOGGER_H_