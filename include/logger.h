/**
 * @file logger.h
 * @author Richard Johnston (techpowerawaits@outlook.com)
 * 
 * @brief Provides a simple re-configurable logger.
 * @details The following logging categories are provided: 'VERBOSE', 'INFO',
 * 'WARNING', 'ERROR', and 'FATAL'. By default, the 'VERBOSE' category is not
 * handled, although a handler for it is provided. Every category can be
 * ignored or handled by a different function using
 * setHandler(LogCategory cat, void ((*handler)(int cond, const char *msg))).
 * 
 * @version 1.0.0
 * @copyright Copyright 2023 Richard Johnston
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#define NUM_LOG_CATS 5
#define MAX_CHARS_IN_LOG_CAT 8

typedef char LogCategory[MAX_CHARS_IN_LOG_CAT + 1];

#define MAX_CHARS_IN_MSG 60

/* Has room for a punctuation mark. */
typedef char MsgStr[MAX_CHARS_IN_MSG + 2];

#define MAX_CHARS_IN_DATE 30

typedef char DateStr[MAX_CHARS_IN_DATE + 1];

/* Has room for extra characters added during formatting. */
#define MAX_CHARS_IN_FINAL_MSG (MAX_CHARS_IN_MSG + MAX_CHARS_IN_DATE + \
MAX_CHARS_IN_LOG_CAT + 12)

typedef char LogEntryStr[MAX_CHARS_IN_FINAL_MSG + 1];

extern FILE *logOutput;

/**
 * @brief Initializes the logger and outputs a message to the given file if it
 * succeeds.
 * @details By default, all log categories are handled except 'VERBOSE'.
 * 
 * @param dest The file to log to. Must not be NULL.
 * @warning If the file is closed during the operation of the logger, the
 * behaviour is undefined.
 */
#define logStart(dest) \
if (initLog(dest)) \
	info(1, "Logger has been successfully initialized.")

/**
 * @brief Initializes the logger.
 * @details By default, all log categories are handled except 'VERBOSE'.
 * 
 * @param dest The file to log to.
 * @warning If the file is closed during the operation of the logger, the
 * behaviour is undefined.
 * 
 * @return 1 if the log is successfully initialized, 0 otherwise.
 */
int initLog(FILE *dest);

/**
 * @brief Logs a message to the given category.
 * @details No message will be logged if the category given is disabled. If the
 * category is invalid, a 'WARNING' indicating this will be logged and the
 * original message will be lost.
 * 
 * Nothing will be logged if the logger has not been initialized.
 * 
 * @param cat The category (represented by a null-terminated string) to utilize
 * for the message. It can be either all uppercase or lowercase.
 * @param msg The null-terminated string representing the desired message. It
 * should not contain any control characters.
 */
void logger(const char *cat, const char *msg);

/**
 * @brief Logs a 'VERBOSE' message.
 * @details No message will be logged if the 'VERBOSE' category is disabled.
 * Nothing will be logged if the logger has not been initialized.
 * 
 * @param cond The condition which must be TRUE in order to print the message.
 * @param msg The null-terminated string representing the desired message. It
 * should not contain any control characters.
 */
void verbose(int cond, const char *msg);

/**
 * @brief Logs a 'INFO' message.
 * @details No message will be logged if the 'INFO' category is disabled.
 * Nothing will be logged if the logger has not been initialized.
 * 
 * @param cond The condition which must be TRUE in order to print the message.
 * @param msg The null-terminated string representing the desired message. It
 * should not contain any control characters.
 */
void info(int cond, const char *msg);

/**
 * @brief Logs a 'WARNING' message.
 * @details No message will be logged if the 'WARNING' category is disabled.
 * Nothing will be logged if the logger has not been initialized.
 * 
 * @param cond The condition which must be TRUE in order to print a message.
 * @param msg The null-terminated string representing the desired message. It
 * should not contain any control characters.
 */
void warning(int cond, const char *msg);

/**
 * @brief Logs a 'ERROR' message.
 * @details No message will be logged if the 'ERROR' category is disabled.
 * Nothing will be logged if the logger has not been initialized.
 * 
 * @param cond The condition which must be TRUE in order to print a message.
 * @param msg The null-terminated string representing the desired message. It
 * should not contain any control characters.
 */
void error(int cond, const char *msg);

/**
 * @brief Logs a 'FATAL' message.
 * @details No message will be logged if the 'FATAL' category is disabled.
 * Nothing will be logged if the logger has not been initialized.
 * 
 * @param cond The condition which must be TRUE in order to print a message.
 * @param msg The null-terminated string representing the desired message. It
 * should not contain any control characters.
 */
void fatal(int cond, const char *msg);

/**
 * @brief Sets the log handler for a given category.
 * @details If NULL is provided as the function pointer, the category is
 * disabled.
 * 
 * @param cat The category to set the handler for. If the given category is
 * invalid, a warning will be logged.
 * @return 1 if setting the handler succeeded, 0 otherwise.
 */
int setHandler(char *cat, void ((*handler)(int cond, const char *msg)));

/**
 * @brief Is the default function used to handle 'VERBOSE' messages.
 * @details When the condition is TRUE and the logger has been initialized, it
 * formats the message and outputs it to the log's file stream. Every message
 * is appended with a new line character.
 * 
 * @param cond The condition which must be TRUE in order to print a message.
 * @param msg The null-terminated string representing the desired message. It
 * should not contain any control characters.
 */
void defaultVerboseHandler(int cond, const char *msg);

/**
 * @brief Is the default function used to handle 'INFO' messages.
 * @details When the condition is TRUE and the logger has been initialized, it
 * formats the message and outputs it to the log's file stream. Every message
 * is appended with a new line character.
 * 
 * @param cond The condition which must be TRUE in order to print a message.
 * @param msg The null-terminated string representing the desired message. It
 * should not contain any control characters.
 */
void defaultInfoHandler(int cond, const char *msg);

/**
 * @brief Is the default function used to handle 'WARNING' messages.
 * @details When the condition is TRUE and the logger has been initialized, it
 * formats the message and outputs it to the log's file stream. Every message
 * is appended with a new line character.
 * 
 * @param cond The condition which must be TRUE in order to print a message.
 * @param msg The null-terminated string representing the desired message. It
 * should not contain any control characters.
 */
void defaultWarningHandler(int cond, const char *msg);

/**
 * @brief Is the default function used to handle 'ERROR' messages.
 * @details When the condition is TRUE and the logger has been initialized, it
 * formats the message and outputs it to the log's file stream. Every message
 * is appended with a new line character.
 * 
 * @param cond The condition which must be TRUE in order to print a message.
 * @param msg The null-terminated string representing the desired message. It
 * should not contain any control characters.
 */
void defaultErrorHandler(int cond, const char *msg);

/**
 * @brief Is the default function used to handle 'FATAL' messages.
 * @details When the condition is TRUE and the logger has been initialized, it
 * formats the message and outputs it to the log's file stream. Every message
 * is appended with a new line character.
 * 
 * @param cond The condition which must be TRUE in order to print a
 * message.
 * @param msg The null-terminated string representing the desired message. It
 * should not contain any control characters.
 */
void defaultFatalHandler(int cond, const char *msg);

/**
 * @brief Generates a string in the format of a log entry.
 * @details '''{locale-specific date}		{CATEGORY}:		{MESSAGE}'''
 * 
 * If the dest given is NULL, then no string will be generated.
 * @warning The function is designed for internal use. As a result, the
 * given category is not checked for validity.
 * 
 * @param dest The pointer to a character array to store the string in.
 * @param cat The category to include in the string. If NULL, the category will
 * not be in the final string.
 * @param msg The message to include in the string. If NULL, the message will
 * not be in the final string.
 * 
 * @return A formatted string for use as a log entry. It contains no new line
 * character. NULL will be returned if dest is NULL.
 */
char *logFormat(char *dest, const char *cat, const char *msg);

#endif
