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
//
//		NOTE:    This makes debugging significantly easier, utilize it. Please believe me!
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
#include "../../PS/config.h"
#include "Logger.h"
//#include "Logger.h"
//#include "Settings.h"
#include <ctime>
#include <cstdio>
#include <cstdarg>

// namespace Cioffi
// {
	Logger *Logger::_highlander = NULL;
	// Author: Joel Cioffi
	// Authored: 1/18/2012
	// Purpose: This function will create the singleton using the passed
	//			file name as the log file name.  If that log file exists,
	//			a number will be appended to the file name until an unused
	//			file name is found.
	//
	// Arguments: FileName - desired file for logger output.
	void Logger::Initialize(const char *FileName)
	{
		if (_highlander != NULL)
		{
			LOG(LOG_WARN, "Logger::Initialize", "Initialize called after logger has been initialized.")
			return;
		}
		_highlander = new Logger(FileName);
	}

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
	void Logger::Log(char erno, const char *ScopedFunctionName, const char *msg, ...)
	{
		if (!_highlander)
			return;
		// don't waste time if nothing will print it anyway
		if (erno > _highlander->_fileLogLevel && erno > _highlander->_consoleLogLevel)
			return;

		va_list args;
		va_start(args, msg);

		_highlander->_Log(erno, ScopedFunctionName, msg, &args);

		va_end(args);
	}

	// Author: Joel Cioffi
	// Authored: 1/18/2012
	// Purpose: This function will clear STDOUT
	void Logger::ClearConsole()
	{
		// This is awful, I'm ashamed of this, fix this later
		system("cls");
	}

	// Author: Joel Cioffi
	// Authored: 1/18/2012
	// Purpose: This function will clean up the Logger singleton
	void Logger::Shutdown()
	{
		if (_highlander != NULL)
		{
			delete _highlander;
			_highlander = NULL;
		}
	}

	// Author: Joel Cioffi
	// Authored: 1/18/2012
	// Purpose: This function defaults all values, and calls _Initialize
	//			to setup the Logger.
	Logger::Logger(const char *LogFile)
	{
		_fileLogLevel = DEFAULT_FILE_LOG_LEVEL;
		_consoleLogLevel = DEFAULT_CONSOLE_LOG_LEVEL;
		_baseFolder = NULL;
		_logFileName = NULL;
		_initialized = false;
		_Initialize(LogFile);
	}

	// Author: Joel Cioffi
	// Authored: 1/18/2012
	// Purpose: This function closes the file, if it is open, and cleans
	//			up any resources allocated by the logger.
	Logger::~Logger()
	{
		if (_logFile.is_open())
		{
			_logFile.close();
			_logFile.clear();
		}
		if (_baseFolder != NULL)
		{
			delete [] _baseFolder;
			_baseFolder = NULL;
		}
		if (_logFileName != NULL)
		{
			delete [] _logFileName;
			_logFileName = NULL;
		}
	}

	Logger::Logger(const Logger &)
	{
	}

	Logger &Logger::operator=(const Logger &)
	{
		return *this;
	}

	// Author: Joel Cioffi
	// Authored: 1/18/2012
	// Purpose: This function will find the directory and file
	//			used for logging. The directory is used to look
	//			for the log file in order to append a number if
	//			the log file exists, e.g. If DeadWest.log exists,
	//			the new log file would be DeadWest01.log
	void Logger::_Initialize(const char *FileName)
	{
		_fileLogLevel = _fileLogLevel;
		_consoleLogLevel = _consoleLogLevel;
		if (FileName == NULL)
		{
			unsigned short dirlen = strlen(DEFAULT_LOG_DIR) + 1;
			unsigned short namelen = strlen("Pludge and Sludge") + 10 + 1;
			_baseFolder = (char *)calloc(dirlen, 1);
			_logFileName = (char *)calloc(namelen, 1);
			strcpy_s(_baseFolder, dirlen, DEFAULT_LOG_DIR);
			sprintf_s(_logFileName, namelen, "%s.log", "Pludge and Sludge");
		}
		else
		{
			unsigned short len = strlen(FileName);
			unsigned short fileLen = len;
			const char *splitter = strrchr(FileName, '/');
			if (splitter != NULL)
			{
				fileLen = (splitter - FileName) + 1;
				_baseFolder = new char[fileLen];
				strcpy_s(_baseFolder, fileLen, FileName);
			}
			else
			{
				splitter = strrchr(FileName, '\\');
				if (splitter != NULL)
				{
					fileLen = (splitter - FileName) + 1;
					_baseFolder = new char[fileLen];
					strcpy_s(_baseFolder, fileLen, FileName);
				}
			}

			fileLen = len - (splitter - FileName) + 1;
			_logFileName = new char[fileLen];
			strcpy_s(_logFileName, fileLen, &FileName[splitter - FileName]);
		}

		// TODO: Fix this to check if FileName exists, and append incrementing numbers until
		//			a unique file name is found.
		//_logFile.open(FileName, std::ios::trunc);

		_initialized = true;
	}

	// Author: Joel Cioffi
	// Authored: 1/18/2012
	// Purpose: This function will filter log messages by erno, and call
	//			_BuildLogString to get a formatted log message. This function
	//			behaves similarly to printf, where variables can be inserted
	//			into the string.
	//			It should only ever be called internally, don't use it.
	void Logger::_Log(char Error, const char *ScopedFunctionName, const char *str, va_list *args)
	{
		const char *LogMessage = _BuildLogString(Error, ScopedFunctionName, str, args);
		if (LogMessage != NULL)
		{
			if (Error <= _fileLogLevel)
				_LogToFile(LogMessage);
			if (Error <= _consoleLogLevel)
				_LogToConsole(LogMessage);
			delete [] LogMessage;
		}
		else
			printf("Something went very very wrong\n\n Check Logger::_Log\n\n");
	}

	// Author: Joel Cioffi
	// Authored: 1/18/2012
	// Purpose: This function will take the provided log values and output
	//			a string in the following format:
	//			  time      type     scoped file name   reformatted string with inserted variables
	//			[HH:MM:SS] ERROR in <class>::<function> <msg>
	//			this function assumes that the returned value is deleted elsewhere
	char *Logger::_BuildLogString(char Error, const char *ScopedFunctionName, const char *str, va_list *args)
	{
		const char *MsgType = "";
		char msgbfr[1024], finalbfr[1024];

		tm t;
		time_t now;
		time(&now);
		localtime_s(&t, &now);
		// this localtime is the one CRT warning I'm not listening to.
		strftime(&msgbfr[0], 11, "[%I:%M:%S]", &t);
		// we've now written 10 characters and a NULL terminator (\0)
		switch(Error)
		{
		case LOG_ERROR:
			MsgType = "ERROR!";
			break;
		case LOG_WARN:
			MsgType = "WARNING!";
			break;
		case LOG_INFO:
			MsgType = "INFO";
			break;
		case LOG_DEBUG:
			MsgType = "DEBUG.";
			break;
		default:
			MsgType = "Unknown Message Type.";
			break;
		};
		unsigned charcount = sprintf_s(&msgbfr[10], 1013, "[%d] %s in %s: %s", GetCurrentThreadId(), MsgType, ScopedFunctionName, str);
		charcount += 10; // for the 10 we wrote before that

		// This makes sure every log statement ends in a newline.
		if (msgbfr[charcount - 1] != '\n')
		{
			msgbfr[charcount] = '\n';
			msgbfr[charcount + 1] = '\0';
		}
		vsprintf_s(&finalbfr[0], 1024, &msgbfr[0], *args);

		unsigned len = strlen(finalbfr) + 1;
		char *returnString = new char[len];
		strcpy_s(returnString, len, &finalbfr[0]);

		return returnString;
	}

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
	void Logger::_LogToFile(const char *ErrorString)
	{
		/* TODO: enable re-openning (but put a max try count)
		if (!_logFile.is_open() && !_logFile.good())
		{
			_logFile.close();
			_logFile.clear();
			_logFile.open();
		}*/
		//_fileLock.Acquire();
		//if (!_logFile.is_open())
			//_logFile.open(_logFileName, 'r');
		if (_logFile.is_open() && _logFile.good())
			_logFile.write(ErrorString, strlen(ErrorString));
		//_fileLock.Release();
	}

	// Author: Joel Cioffi
	// Authored: 1/18/2012
	// Purpose: This function will simply call printf.
	//			A lock is used to make sure a single line is fully printed
	//			before another is able to start. Issues with threads using
	//			stdout cause splicing
	void Logger::_LogToConsole(const char *ErrorString)
	{
		//_consoleLock.Acquire();
		printf(ErrorString);
		//_consoleLock.Release();
	}
//};