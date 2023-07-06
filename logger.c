/**
 * @file logger.h
 * @author Richard Johnston (techpowerawaits@outlook.com)
 * 
 * @brief Provides a simple re-configurable logger.
 * @details The following logging categories are provided: 'VERBOSE', 'INFO',
 * 'WARNING', 'ERROR', and 'FATAL'. By default, the 'VERBOSE' category is not
 * handled, although a handler for it is provided. Every category can be
 * ignored or handled by a different function using
 * setHandler(LogCategory cat, void ((*handler)(int cond, char *msg))).
 * 
 * @version 0.1
 * @copyright Copyright 2023 Richard Johnston
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "logger.h"

#define NUM_CAT_CASES 2
#define UPPER_INDEX 0
#define LOWER_INDEX 1

typedef enum
{
	VERBOSE_IDX = 0,
	INFO_IDX = 1,
	WARNING_IDX = 2,
	ERROR_IDX = 3,
	FATAL_IDX = 4
} CatIndex;

const LogCategory VALID_CATS[NUM_LOG_CATS][NUM_CAT_CASES] =
{
	{"VERBOSE", "verbose"},
	{"INFO", "info"},
	{"WARNING", "warning"},
	{"ERROR", "error"},
	{"FATAL", "fatal"}
};

void (*logHandlers[NUM_LOG_CATS])(int cond, const char *msg) = {NULL};
FILE *logOutput = NULL;
int initialized = 0;

void closeLog(void);

int initLog(FILE *dest)
{
	if (dest != NULL && !initialized)
	{
		logOutput = dest;
		initialized = 1;

		setHandler("VERBOSE", NULL);
		setHandler("INFO", defaultInfoHandler);
		setHandler("WARNING", defaultWarningHandler);
		setHandler("ERROR", defaultErrorHandler);
		setHandler("FATAL", defaultFatalHandler);

		atexit(closeLog);
	}

	return initialized;
}

void logger(const char *cat, const char *msg)
{
	int catValid = 0;
	int index;

	if (initialized)
	{
		if (cat != NULL)
		{
			for (index = 0; index < NUM_LOG_CATS && !catValid; index++)
			{
				if (strcmp(cat, VALID_CATS[index][UPPER_INDEX]) ||
					strcmp(cat, VALID_CATS[index][LOWER_INDEX]))
				{
					catValid = 1;

					if (logHandlers[index] != NULL)
					{
						logHandlers[index](1, msg);
					}
				}
			}
		}

		if (!catValid)
		{
			warning(1, "Invalid category provided. Message is not logged.");
		}
	}
}

void verbose(int cond, const char *msg)
{
	if (logHandlers[VERBOSE_IDX] != NULL)
	{
		logHandlers[VERBOSE_IDX](cond, msg);
	}
}

void info(int cond, const char *msg)
{
	if (logHandlers[INFO_IDX] != NULL)
	{
		logHandlers[INFO_IDX](cond, msg);
	}
}

void warning(int cond, const char *msg)
{
	if (logHandlers[WARNING_IDX] != NULL)
	{
		logHandlers[WARNING_IDX](cond, msg);
	}
}

void error(int cond, const char *msg)
{
	if (logHandlers[ERROR_IDX] != NULL)
	{
		logHandlers[ERROR_IDX](cond, msg);
	}
}

void fatal(int cond, const char *msg)
{
	if (logHandlers[FATAL_IDX] != NULL)
	{
		logHandlers[FATAL_IDX](cond, msg);
	}
}

int setHandler(char *cat, void ((*handler)(int cond, const char *msg)))
{
	int catValid = 0;
	int index;

	if (cat != NULL && initialized)
	{
		for (index = 0; index < NUM_LOG_CATS && !catValid; index++)
		{
			if (!strcmp(cat, VALID_CATS[index][UPPER_INDEX]) ||
				!strcmp(cat, VALID_CATS[index][LOWER_INDEX]))
			{
				catValid = 1;
				logHandlers[index] = handler;
			}
		}
	}

	if (!catValid)
	{
		warning(1, "Invalid category provided. Handler is not set.");
	}

	return catValid;
}

void defaultVerboseHandler(int cond, const char *msg)
{
	LogEntryStr formattedMsg;

	if (cond && logOutput != NULL)
	{
		logFormat(formattedMsg, "VERBOSE", msg);
		fprintf(logOutput, "%s\n", formattedMsg);
	}
}

void defaultInfoHandler(int cond, const char *msg)
{
	LogEntryStr formattedMsg;

	if (cond && logOutput != NULL)
	{
		logFormat(formattedMsg, "INFO", msg);
		fprintf(logOutput, "%s\n", formattedMsg);
	}
}

void defaultWarningHandler(int cond, const char *msg)
{
	LogEntryStr formattedMsg;

	if (cond && logOutput != NULL)
	{
		logFormat(formattedMsg, "WARNING", msg);
		fprintf(logOutput, "%s\n", formattedMsg);
	}
}

void defaultErrorHandler(int cond, const char *msg)
{
	LogEntryStr formattedMsg;

	if (cond && logOutput != NULL)
	{
		logFormat(formattedMsg, "ERROR", msg);
		fprintf(logOutput, "%s\n", formattedMsg);
	}
}

void defaultFatalHandler(int cond, const char *msg)
{
	LogEntryStr formattedMsg;

	if (cond && logOutput != NULL)
	{
		logFormat(formattedMsg, "FATAL", msg);
		fprintf(logOutput, "%s\n", formattedMsg);
	}
}

char *logFormat(char *dest, const char *cat, const char *msg)
{
	const int MSG_NUM_CHARS = strlen(msg);

	DateStr dateField = "";
	time_t currTime;
	struct tm *localTime = NULL;
	LogCategory catField = "";
	MsgStr msgField = "(null)";

	if (cat != NULL)
	{
		sprintf(catField, "%s:", cat);
	}

	if (msg != NULL)
	{
		strncpy(msgField, msg, MAX_CHARS_IN_MSG);

		if (msg[MSG_NUM_CHARS - 1] != '.' && msg[MSG_NUM_CHARS - 1] != '!' &&
			msg[MSG_NUM_CHARS - 1] != '?')
		{
			msgField[MSG_NUM_CHARS] = '.';
			msgField[MSG_NUM_CHARS + 1] = '\0';
		}
	}

	if (dest != NULL)
	{
		currTime = time(NULL);

		if (currTime != -1)
		{
			localTime = localtime(&currTime);

			if (localTime != NULL)
			{
				strftime(dateField, MAX_CHARS_IN_DATE, "%c", localTime);
			}
		}

		sprintf(dest, "%s\t\t%-*s\t\t%s", dateField, MAX_CHARS_IN_LOG_CAT,
				catField, msgField);
	}

	return dest;
}

/**
 * @brief Frees the file given during initialization.
 */
void closeLog(void)
{
	if (initialized)
	{
		fclose(logOutput);
	}
}