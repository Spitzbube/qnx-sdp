/*
 * $QNXLicenseC:
 * Copyright 2017-2019, QNX Software Systems.  All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software.  Free development
 * licenses are available for evaluation and non-commercial purposes.  For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others.  Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

#ifndef _QH_LOG_H_INCLUDED
#define _QH_LOG_H_INCLUDED

/**
 * @file log.h
 * QNX Helpers - Logging related helpers.  The whole logging API is thread-safe.
 */

/* sorted headers from this project */
#ifndef _QH_ATTR_H_INCLUDED
#include "qh/attr.h"
#endif

/* sorted library headers */

/* sorted system headers */
#ifndef _INTTYPES_H_INCLUDED
#include <inttypes.h>
#endif

#ifndef _STDARG_H_INCLUDED
#include <stdarg.h>
#endif

#ifndef _STDBOOL_H_INCLUDED
#include <stdbool.h>
#endif

/** @cond */
__BEGIN_DECLS
/** @endcond */

/**
 * Identifiers representing the verbosity levels when logging messages.
 *
 * The qh_log functions treat verbosity level 0 as if it was QH_LOG_SEVERITY_INFO when logging. This
 * is just for convenience, allowing simpler qh_log calls, where 0 can be specified instead of a
 * QH_LOG_SEVERITY_* identifier.
 */
#define QH_LOG_SEVERITY_SHUTDOWN                                                         UINT64_C(1)
#define QH_LOG_SEVERITY_CRITICAL                                                         UINT64_C(2)
#define QH_LOG_SEVERITY_ERROR                                                            UINT64_C(3)
#define QH_LOG_SEVERITY_WARNING                                                          UINT64_C(4)
#define QH_LOG_SEVERITY_NOTICE                                                           UINT64_C(5)
#define QH_LOG_SEVERITY_INFO                                                             UINT64_C(6)
#define QH_LOG_SEVERITY_DEBUG1                                                           UINT64_C(7)
#define QH_LOG_SEVERITY_DEBUG2                                                           UINT64_C(8)

/**
 * Convenience identifiers equivalent to the QH_LOG_SEVERITY_* identifiers above, but with shorter
 * names, making them easier to use in the code.
 */
#define QH_LOG_SHUTDOWN                                                     QH_LOG_SEVERITY_SHUTDOWN
#define QH_LOG_CRITICAL                                                     QH_LOG_SEVERITY_CRITICAL
#define QH_LOG_ERROR                                                           QH_LOG_SEVERITY_ERROR
#define QH_LOG_WARNING                                                       QH_LOG_SEVERITY_WARNING
#define QH_LOG_NOTICE                                                         QH_LOG_SEVERITY_NOTICE
#define QH_LOG_INFO                                                             QH_LOG_SEVERITY_INFO
#define QH_LOG_DEBUG1                                                         QH_LOG_SEVERITY_DEBUG1
#define QH_LOG_DEBUG2                                                         QH_LOG_SEVERITY_DEBUG2

/** Log flags related to the destinations of the log messages. */
#define QH_LOG_DEST_STDOUT                                                      UINT64_C(0x00100000)
#define QH_LOG_DEST_STDERR                                                      UINT64_C(0x00200000)
#define QH_LOG_DEST_SLOG                                                        UINT64_C(0x00400000)
#define QH_LOG_DEST_SLOG2                                                       UINT64_C(0x00800000)

/**
 * Convenience log flags combining a log level and the stderr destination, indicating an attempt
 * should be made to output the log message to stderr.  This is common when a process has something
 * that should not go unnoticed at startup (e.g. if it is exiting due to a critical error), in which
 * case it is often desirable to output a log message to stderr.  Only the critical, shutdown and
 * error levels are considered for this given all other levels are typically not considered
 * problematic enough to mandate a message specifically going to stderr.
 */
#define QH_LOG_SHUTDOWN_STDERR                       (QH_LOG_SEVERITY_SHUTDOWN | QH_LOG_DEST_STDERR)
#define QH_LOG_CRITICAL_STDERR                       (QH_LOG_SEVERITY_CRITICAL | QH_LOG_DEST_STDERR)
#define QH_LOG_ERROR_STDERR                             (QH_LOG_SEVERITY_ERROR | QH_LOG_DEST_STDERR)

/** Identifiers representing the combination of all or no destinations. */
#define QH_LOG_DEST_NONE                                                                 UINT64_C(0)
#define QH_LOG_DEST_ALL                                                         UINT64_C(0xFFF00000)

/** Log flags related to the blocked destinations of the log messages. */
#define QH_LOG_NO_DEST_STDOUT                                                   UINT64_C(0x00000100)
#define QH_LOG_NO_DEST_STDERR                                                   UINT64_C(0x00000200)
#define QH_LOG_NO_DEST_SLOG                                                     UINT64_C(0x00000400)
#define QH_LOG_NO_DEST_SLOG2                                                    UINT64_C(0x00000800)

/** Set of masks representing the different sections of the severity level. */
#define QH_LOG_SEVERITY_MASK                                                    UINT64_C(0x000000FF)
#define QH_LOG_DEST_MASK                                                        UINT64_C(0xFFF00000)
#define QH_LOG_NO_DEST_MASK                                                     UINT64_C(0x000FFF00)
#define QH_LOG_RESERVED_MASK                                            UINT64_C(0x000FFFFE00000000)

/** Shift for converting between DEST and NO_DEST defines and masks. */
#define QH_LOG_DEST_TO_NO_DEST_SHIFT                                                              12

/** Optional log flags controlling the log message output. */
#define QH_LOG_OPTION_TID                                               UINT64_C(0x0000000100000000)

/**
 * Log flags mask and shift representing the user reserved area of the flags.  The logging functions
 * do not look at this area when logging messages, so the callers are free to set anything they like
 * in this area, allowing finer grained control over specific types of messages related to their
 * needs.
 */
#define QH_LOG_USER_RESERVED_MASK                                       UINT64_C(0xFFF0000000000000)
#define QH_LOG_USER_RESERVED_SHIFT                                                                52

/** Additional severity identifiers used when setting and getting the verbosity. */
#define QH_LOG_SEVERITY_NONE                                                             UINT64_C(0)
#define QH_LOG_SEVERITY_ALL                                                     QH_LOG_SEVERITY_MASK

/**
 * Identifier representing the version number of the first version of the qh_log_info_s structure.
 */
#define QH_LOG_INFO_VERSION_1                                                   UINT32_C(0x00010000)

/**
 * Identifier representing the version number of the current version of the qh_log_info_s structure.
 */
#define QH_LOG_INFO_VERSION                                                    QH_LOG_INFO_VERSION_1

/**
 * QH log's opaque context type.
 */
typedef struct qh_log_context_s qh_log_context_t;

/**
 * @struct qh_log_info_s
 *
 * The information structure used with logging functions.
 */
typedef struct qh_log_info_s
{
    /** The version number of this structure.  Always set this to QH_LOG_INFO_VERSION. */
    uint32_t version;
    /** The line on which the log message originates. */
    uint32_t line;
    /** The file name where the log message originates. */
    const char *filename;
    /** Whether the filename contains the full path or not */
    bool short_filename;
    /** The function where the log message originates. */
    const char *function;
    /**
     * The QH_LOG_SEVERITY_* severity identifier for this message.  Can be combined with
     * QH_LOG_DEST_* and QH_LOG_NO_DEST_* when specific messages also need to control destinations.
     */
    uint64_t severity;
    /** The caller's logging context. */
    qh_log_context_t **context;
} qh_log_info_t;

/**
 * The default user context unless already defined by the user of the log header.  If not defined by
 * the user of the log header, it is set to qh_log_user_context_, which refers to the automatically
 * created module's context.  This is used with the logging macros.
 */

#ifdef QH_LOG_NULL_USER_CONTEXT
#if defined(QH_LOG_USER_CONTEXT_NAME) || defined(QH_LOG_USER_CONTEXT_VAR)
#error "QH_LOG_NULL_USER_CONTEXT cannot be used together with QH_LOG_USER_CONTEXT_NAME or QH_LOG_USER_CONTEXT_VAR."
#endif
#define QH_LOG_USER_CONTEXT_REF                                                                 NULL
#else
#ifndef QH_LOG_USER_CONTEXT_VAR
#define QH_LOG_USER_CONTEXT_VAR                                                 qh_log_user_context_
#endif
extern qh_log_context_t *QH_LOG_USER_CONTEXT_VAR QH_ATTR_VAR_VISIBILITY("hidden");
#define QH_LOG_USER_CONTEXT_REF                                             &QH_LOG_USER_CONTEXT_VAR
#endif

/**
 * A helper macro used by logging macros to simplify the creation of anonymous log info structures.
 */
#define QH_LOG_INFO_ANON(__severity, __filename, __function, __line) \
            &(qh_log_info_t){ .version = QH_LOG_INFO_VERSION, \
                              .line = __line, \
                              .filename = __filename, \
                              .short_filename = !QH_FILENAME_HAS_FULLPATH, \
                              .function = __function, \
                              .severity = __severity, \
                              .context = QH_LOG_USER_CONTEXT_REF }

/**
 * Macro to log a message without any location information associated with it.
 *
 * This macro guarantees errno remains the same when it returns as it was upon entry.
 *
 * Note that the fmt parameter is omitted from the macro signature in order to prevent the possible
 * compile-time warnings (only when there would be no vaargs) when the -Wpedantic flag is used.
 *
 * @param[in] severity The QH_LOG_SEVERITY_* severity level for this log message.  Can optionally be
 *                     combined with QH_LOG_DEST_* and / or QH_LOG_NO_DEST_* when specific messages
 *                     also need to add or block destinations.
 * @param[in] fmt      A string that specifies the format of the output.  The formatting string
 *                     determines what additional arguments you need to provide.  For more
 *                     information, see printf().
 *
 * @return EOK on success or a standard errno on failure.  errno is unmodified.
 */
#define qh_log_noloc(__severity, ...) qh_log_ex(QH_LOG_INFO_ANON(__severity, NULL, NULL, 0), ##__VA_ARGS__)

/**
 * Macro to log a message along with the originating file name and line number.
 *
 * This macro guarantees errno remains the same when it returns as it was upon entry.
 *
 * Note that the fmt parameter is omitted from the macro signature in order to prevent the possible
 * compile-time warnings (only when there would be no vaargs) when the -Wpedantic flag is used.
 *
 * @param[in] severity The QH_LOG_SEVERITY_* severity level for this log message.  Can optionally be
 *                     combined with QH_LOG_DEST_* and / or QH_LOG_NO_DEST_* when specific messages
 *                     also need to add or block destinations.
 * @param[in] fmt      A string that specifies the format of the output.  The formatting string
 *                     determines what additional arguments you need to provide.  For more
 *                     information, see printf().
 *
 * @return EOK on success or a standard errno on failure.  errno is unmodified.
 */
#define qh_log(__severity, ...) qh_log_ex(QH_LOG_INFO_ANON(__severity, QH_FILENAME, NULL, __LINE__), ##__VA_ARGS__)

/**
 * Convenience logging macros that implicitly specify the logging severity normally specified when
 * calling the qh_log macro.
 */
#define qh_log_shutdown(...) qh_log(QH_LOG_SEVERITY_SHUTDOWN, ##__VA_ARGS__)
#define qh_log_critical(...) qh_log(QH_LOG_SEVERITY_CRITICAL, ##__VA_ARGS__)
#define qh_log_error(...) qh_log(QH_LOG_SEVERITY_ERROR, ##__VA_ARGS__)
#define qh_log_warning(...) qh_log(QH_LOG_SEVERITY_WARNING, ##__VA_ARGS__)
#define qh_log_notice(...) qh_log(QH_LOG_SEVERITY_NOTICE, ##__VA_ARGS__)
#define qh_log_info(...) qh_log(QH_LOG_SEVERITY_INFO, ##__VA_ARGS__)
#define qh_log_debug1(...) qh_log(QH_LOG_SEVERITY_DEBUG1, ##__VA_ARGS__)
#define qh_log_debug2(...) qh_log(QH_LOG_SEVERITY_DEBUG2, ##__VA_ARGS__)

/**
 * Macro to log a message along with the originating file name, function and line number.
 *
 * This macro guarantees errno remains the same when it returns as it was upon entry.
 *
 * Note that the fmt parameter is omitted from the macro signature in order to prevent the possible
 * compile-time warnings (only when there would be no vaargs) when the -Wpedantic flag is used.
 *
 * @param[in] severity The QH_LOG_SEVERITY_* severity level for this log message.  Can optionally be
 *                     combined with QH_LOG_DEST_* and / or QH_LOG_NO_DEST_* when specific messages
 *                     also need to add or block destinations.
 * @param[in] fmt      A string that specifies the format of the output.  The formatting string
 *                     determines what additional arguments you need to provide.  For more
 *                     information, see printf().
 *
 * @return EOK on success or a standard errno on failure.  errno is unmodified.
 */
#define qh_log_loc(__severity, ...) qh_log_ex(QH_LOG_INFO_ANON(__severity, QH_FILENAME, __func__, __LINE__), ##__VA_ARGS__)

/**
 * Macro to log a debug linetrace message along with the originating file name, function and line
 * number, using the QH_LOG_SEVERITY_DEBUG2 severity level.
 *
 * This macro can be called with or without a message:
 * qh_log_linetrace();
 * qh_log_linetrace("extra info");
 * qh_log_linetrace(extra_info_string);
 *
 * This macro guarantees errno remains the same when it returns as it was upon entry.
 *
 * Note that the fmt parameter is omitted from the macro signature in order to prevent the possible
 * compile-time warnings (only when there would be no vaargs) when the -Wpedantic flag is used.
 *
 * @param[in] msg A typically short string that gets put in the line trace log message, or it can be
 *                left out if desired if no specific information needs to be added.
 *
 * @return EOK on success or a standard errno on failure.  errno is unmodified.
 */
#define qh_log_linetrace(__msg) qh_log_loc(QH_LOG_SEVERITY_DEBUG2, "DBG trace: %s", "" __msg)

/**
 * Get the verbosity of the given destination.
 *
 * Gets the highest level of verbosity of a given set of destinations for a context.  Specifying
 * NULL for the context assumes the default context.
 *
 * @param[in] context      The context for which to get the verbosity.
 * @param[in] destinations The logging destination flags.
 *
 * @return The highest verbosity level associated to the destination.
 */
uint64_t qh_log_get_verbosity(qh_log_context_t **__context, uint64_t __destinations);

/**
 * Get the verbosity of the given destination.
 *
 * Gets the highest level of verbosity of a given set of destinations for contexts matching the
 * specified name.
 *
 * Specifying "all" for the context name results in all contexts being matched, and specifying
 * "default" returns the verbosity for the default context.
 *
 * @param[in] context_name The name to use for context matching.
 * @param[in] destinations The logging destination flags.
 *
 * @return The highest verbosity level associated to the destination.
 */
uint64_t qh_log_get_verbosity_by_name(const char *__context_name, uint64_t __destinations);

/**
 * Get the destinations of a context.
 *
 * Gets the logging destination flags (e.g. QH_LOG_DEST_*) of a given context.  Specifying NULL for
 * the context assumes the default context.
 *
 * @param[in] context The context for which to get the destinations.
 *
 * @return The logging destination flags.
 */
uint64_t qh_log_get_destinations(qh_log_context_t **__context);

/**
 * Get the destinations based on context name.
 *
 * Gets the combined logging destination flags (e.g. QH_LOG_DEST_*) for contexts matching the
 * specified name.
 *
 * Specifying "all" for the context name results in all contexts being matched, and specifying
 * "default" returns the destinations for the default context.
 *
 * @param[in] context_name The name to use for context matching.
 *
 * @return The logging destination flags.
 */
uint64_t qh_log_get_destinations_by_name(const char *__context_name);

/**
 * Update the logging configuration based on a configuration string.
 *
 * There are currently 4 logging destinations, namely slog, slog2, stdout and stderr.  Logging is
 * controlled by a combination of the destination flags and verbosity of a context, and of the
 * message specific flags, where the destination flags are the base, and the message specific flags
 * are applied on top.  In order for logging to occur, at least one destination needs to be active,
 * and the message severity level needs to satisfy the verbosity requirements of the configured
 * destination(s) for the specified context.
 *
 * The configuration string is made of a series of context tokens separated by ";", and each token
 * is a key / value pair separated by ":", where the keys represent a context name, and the values
 * represent the configuration of the named context.  The context names can be composed of any alpha
 * numeric characters, along with any characters from the set "#()+-.<>[]_" (excluding the double
 * quotes).  Any other character is disallowed, and the maximum length (including null-termination)
 * of the context name is 32 characters.  When "all" is specified as the context name, the specified
 * configuration applies to all contexts, except the default one.  When "default" is specified as
 * the context name, the default context is updated with the specified configuration.
 *
 * The value part of the context token is itself a string that is made of a series of tokens
 * separated by ",", and each token is a key / value pair separated by "=".  The allowed keys are
 * "all", "default", slog", "slog2", "stdout" and "stderr", without the quotes, and represent the
 * logging destinations.  The allowed values are "all", "none", "shutdown", "critical", "error",
 * "warning", "notice", "info", "debug1" and "debug2", also without the quotes, and represent the
 * severity levels used for the verbosity.  The value is assigned to the specified key, and when the
 * "all" key is used, all logging destinations get the value.  When the "default" key is used, it
 * means the context is using the values from the default context, and a value cannot be specified
 * in that case.  When the default context configuration changes, those values also change with it.
 * In all other cases, the value is optional (no equal sign then), and if not specified, the
 * verbosity is not changed for the specified destination.  If using "default" with the default
 * context, the default context reverts back to the original defaults, as if it was never
 * configured.
 *
 * When a logging destination is specified, it will be used as an active destination (using its
 * assigned verbosity if one is specified) for the specified context, unless the key is prefixed
 * with "~", in which case it will be made an inactive destination, though logging can still
 * occur if a log call specifically requested this destination (and assuming it passes the verbosity
 * requirements).
 *
 * The whole logging configuration is applied from left to right, and contexts and destinations can
 * be specified multiple times, so in some cases, order will matter.  The existing logging
 * configuration prior to making this call is not cleared before the configuration updates are
 * applied.
 *
 * The configuration also applies to contexts that are not yet created.  When a context is created,
 * the configuration information is looked up and applied to the newly created context, and reverts
 * to the default context's configuration otherwise.
 *
 * Note that there cannot be any white space in the configuration.
 *
 * In case of a parsing error, the function returns an error and does not modify the logging
 * configuration.
 *
 * Examples:
 * "default:slog2=info;all:~stdout=none;qh:stdout=default" would set slog2's verbosity to
 *     QH_LOG_SEVERITY_INFO for the default context, then specify that all other context should not
 *     use stdout as an active destination, also setting its verbosity to QH_LOG_SEVERITY_NONE, and
 *     then specify that the "qh" context should use the settings from the default context for the
 *     stdout destination.
 * "default:slog2=notice,~stderr=error" would add slog2 as an active destination (with verbosity
 *     level QH_LOG_SEVERITY_NOTICE) to the default context, make stderr an inactive destination
 *     (but setting its verbosity level to QH_LOG_SEVERITY_ERROR), and leave the rest of the
 *     destinations and their associated verbosity untouched.
 * "all:~all=none,slog2=notice,~stderr=error" applicable to all contexts (except the default), it
 *     would first make all destinations inactive and set their verbosity to none (don't log
 *     anything), then add slog2 as an active destination (with verbosity level
 *     QH_LOG_SEVERITY_NOTICE), and make stderr an inactive destination (but setting its verbosity
 *     level to QH_LOG_SEVERITY_ERROR).
 * "my_ctx:slog2" would simply add slog2 as an active destination (without changing its verbosity)
 *     to the context "my_ctx", and leave the rest of the destinations and their associated
 *     verbosity untouched.
 *
 * If the environment variables QH_LOG_CONFIG_CTX or QH_LOG_CONFIG_CTX__<progname> are set to such a
 * string, the library will automatically use them in succession (starting with QH_LOG_CONFIG_CTX,
 * and only if set and valid) to update the destination flags at startup, prior to the process
 * entering its main function, and will then prevent any further changes by the application.  This
 * allows system integrators to setup the logging configuration as they want it to be without the
 * possibility for the application to change it.  Because this would not be practical for
 * development, the implied locking is prevented if the environment variable QH_LOG_CONFIG_LOCKED
 * is set to 0.
 *
 * The actual name of the QH_LOG_CONFIG_CTX__<progname> environment variable is made of:
 * 1- QH_LOG_CONFIG_CTX__ (two underscores suffix)
 * 2- The program name as reported by __progname, where if the first character is a non-alpha one,
 *    it is converted to an underscore, where other non-alphanumeric characters are converted to
 *    underscores, and where the name is truncated to 63 characters.  Character casing is kept.
 * For example, if __progname is "03%aB[-t", the environment variable being looked up for it will be
 * "QH_LOG_CONFIG_CTX___3_aB__t".
 *
 * The program-specific environment variable allows system integrators to have system-wide settings
 * in QH_LOG_CONFIG_CTX, and program-specific settings in the other one.
 *
 *
 * QH_LOG_CONFIG_SLOG2
 * ===================
 * Similarly to the QH_LOG_CONFIG_CTX environment variable, given slog2 needs to be configured very
 * early in the QH setup, an environment variable named QH_LOG_CONFIG_SLOG2, or one named
 * QH_LOG_CONFIG_SLOG2__<progname> (with naming conventions being the same as for the
 * QH_LOG_CONFIG_CTX equivalent) can be used by system integrators to configure the set of slog2
 * buffers QH uses.  In this case, QH chooses only one of the environment variables for the
 * configuration, given combined or successive application of multiple slog2 configurations don't
 * make sense.  The first one it tries to use is the program-specific one, and if it is absent (or
 * invalid), the system-wide one will be used if present (and valid), and as a last resort, the
 * defaults will be used.
 *
 * By default (i.e. when the environment variables aren't set or are invalid), 3 slog2 buffers are
 * created:
 * 1- The first one is named "high", for logs with severity levels QH_LOG_SEVERITY_WARNING and
 *    above, and it has a size of 4 slog2 pages.
 * 2- The second one is named "low", and is for logs with severity levels below
 *    QH_LOG_SEVERITY_WARNING, and it has a size of 8 slog2 pages.
 * 3- The third one is named "default", and is for slog2 calls made directly through the slog2 api
 *    (not meant to be used in conjunction with QH), where the specified buffer is set to NULL
 *    (meaning, use the default buffer).  This buffer only has a single page.
 *
 * If the configuration string is "no_slog2", it completely disables slog2 in QH (i.e. no buffer
 * sets are created, and no logging to slog2 happens through QH).
 *
 * In cases where QH should configure slog2, the configuration string is made of a series of
 * configuration sections each separated by ";".  Each configuration section is itself made of a
 * key / value pair separated by ":", where each key represents the section name (one of "high",
 * "low", "default", or "flags"), and the values represent the configuration of the named section.
 * It then looks something like this:
 * "<section_1_name>:<section1_config>;...;<sectionN_name>:<sectionN_config>"
 * The order in which sections appear is not important, and they are all optional, though at least
 * one needs to be present.  When a section isn't specified, QH default values (as described above)
 * are used.
 *
 * The section named "flags" contains a set of slog2 flags to use when registering the slog2 buffer
 * set.  By default, QH doesn't use any flags when registering the slog2 buffer set.  The
 * configuration value for the "flags" section is itself a comma-separated list of flag names.
 * Possible values are:
 *     "alloc_phys", equivalent to SLOG2_ALLOC_TYPE_PHYSICAL
 *     "alloc_shmem", equivalent to SLOG2_ALLOC_TYPE_SHMEM
 *     "discard_newline", equivalent to SLOG2_DISCARD_NEWLINE
 * For more information on these flags, see the QNX documentation for slog2_register.
 *
 * The sections named "high", "low", and "default" each represent their respective slog2 buffer
 * configuration.  These configurations are made of a comma-separated list of configuration options.
 * As mentioned above, the "high" buffer gets the logs with higher severity, the "low" buffer gets
 * those that don't make it to the "high" buffer, and the "default" buffer gets the logs from slog2
 * calls that were made directly through the slog2 api using a NULL value for the buffer.
 * Here's a description of the available configuration options for the individual buffers:
 * - For all sections:
 *       "pages=X":  When the "pages" options is specified, its value following the equal sign
 *                   represents the number of 4k pages to be allocated for the specified buffer.  If
 *                   this option isn't specified, the default number of pages is used (4 for the
 *                   "high" buffer, 8 for the "low" buffer and 1 for the "default" buffer).
 * - For the "high" buffer section:
 *       "min_sev=X":  When the "min_sev" option is specified, its value represents the severity
 *                    level at which messages start being logged to the "high" severity buffer, and
 *                    other messages go to the "low" severity buffer.  Values for the "min_sev" key
 *                    are one of "all", "none", "shutdown", "critical", "error", "warning",
 *                    "notice", "info", "debug1" and "debug2".  When "none" is used, it means that
 *                    there will not be a "low" and "high" buffer set, and they'll instead be
 *                    combined into a single buffer logging all messages.  In this case, the "pages"
 *                    config option for the "high" buffer must be set to 0.  When "all" or "debug2"
 *                    is used, it also means the use of a single buffer, but in this case, pages
 *                    must not be 0 for the "high" buffer.
 *                    In the absence of this configuration option, QH will use "warning" as the
 *                    value for it.
 * - For the "high" and "low" buffer sections:
 *       "is_default":  When the "is_default" option is specified, it means that there will not be a
 *                      "default" slog2 buffer for messages logged through slog2 directly, and those
 *                      will instead go to the buffer that has the "is_default" option.
 *
 * Note that there cannot be any white space in the slog2 configuration.
 *
 * In case of an error in parsing the slog2 configuration from the QH_LOG_CONFIG_SLOG2 environment
 * variable, a message to that effect is logged, and QH will use its default configuration for the
 * slog2 buffers.
 *
 * The default slog2 configuration uses a high severity buffer with 4 pages with a min_sev of
 * "warning", a low severity buffer with 8 pages, a 1 page default buffer, and no flags.
 *
 * Examples:
 * "high:pages=5,min_sev=notice;low:is_default;flags:alloc_shmem,discard_newline"
 *     The high severity buffer gets 5 pages and all messages with "notice" severity and above go to
 *     it.  The low severity buffer gets the default number of pages (8) and is also used as the
 *     default buffer for non-qh slog2 calls.  The SLOG2_ALLOC_TYPE_SHMEM and SLOG2_DISCARD_NEWLINE
 *     flags are used.
 * "high:min_sev=error;default:pages=5"
 *     The high severity buffer gets the default number of pages (4) and all messages with error and
 *     above go to it.  The low severity buffer gets the default number of pages (8), and the
 *     default buffer gets 5 pages.
 *
 * @param[in] config_str The logging configuration string.
 *
 * @return EOK on success or a standard errno on failure (errno is also set).
 */
int qh_log_update_config(const char *__config_str);

/**
 * Set the logging configuration to command line mode.
 *
 * By default, the diagnostics logged through QH go to slog2, unless configured otherwise.  For
 * command line tools, it is more practical to output diagnostics to stderr instead of slog2.  In
 * order to facilitate system configuration by typically only requiring the default settings, the
 * QH logging configuration can be set to use command line defaults instead of its normal defaults
 * by calling this function.  When it is called, the function reverts any previous configuration,
 * and then applies QH's command line defaults, and then command line defaults from the following
 * env vars (in order):
 *
 * QH_LOG_CONFIG_CTX_CL
 * QH_LOG_CONFIG_CTX_CL__<progname> (normalized progname)
 *
 * The QH_LOG_CONFIG_CTX_CL* environment variables have exactly the same format as their
 * QH_LOG_CONFIG_CTX* counterparts, and like for those, the configuration can be locked if the
 * environment variables were set.  For more information on the format, and on the locking
 * mechanism, refer to the qh_log_update_config documentation.
 *
 * @return None
 */
void qh_log_set_cl_mode(void);

/**
 * Check if a message would be logged given a log information structure.
 *
 * Given a log information structure, this function will determine if a message would be logged if
 * used in a logging called.  A useful usage case for this function is when a message would be
 * expensive to build prior to calling the logging function.  In this case, the caller can verify
 * that the message would be logged prior to building the message itself so that building could be
 * avoided if the message does not end up being logged.
 *
 * @param[in] log_info The log information structure that would be used with the log message.
 *
 * @return true if the message would be logged, and false otherwise.
 */
bool qh_log_check(const qh_log_info_t *__log_info);

#define QH_LOG_CHECK(__severity) qh_log_check(QH_LOG_INFO_ANON(__severity, NULL, NULL, 0))

/**
 * Log a formatted message.
 *
 * Logs a formatted message to the set of active destinations, combined with the severity level set
 * in the log information structure.  For more information regarding the log information structure
 * (qh_log_info parameter), see the qh_log_info_t structure documentation.
 *
 * Logging calls are typically done using the provided logging macros (which in turn call the
 * qh_log_ex function) given they greatly simplify the calls, but users are free to use this
 * function directly if necessary / desired.
 *
 * Though it is not recommended, it is safe to call this function with a NULL qh_log_info structure
 * when absolutely necessary, in which case all default values will be used, along with the default
 * context configuration.
 *
 * @param[in] log_info The log information structure to use with this log message, or NULL to use
 *                     default values.
 * @param[in] fmt      A string that specifies the format of the output.  The formatting string
 *                     determines what additional arguments you need to provide.  For more
 *                     information, see printf().
 *
 * @return EOK on success or a standard errno on failure.  errno is unmodified.
 *
 * @see qh_log_info_t & printf
 */
int qh_log_ex(const qh_log_info_t *__log_info, const char *__fmt, ...) QH_ATTR_FUNC_FORMAT_PRINTF(2, 3);

/**
 * Log a formatted message using vargs.
 *
 * Logs a formatted message to the set of active destinations, combined with the severity level set
 * in the log information structure.  qh_vlog_ex is a varargs version of qh_log_ex.  For more
 * information regarding the log information structure (qh_log_info parameter), see the
 * qh_log_info_t structure documentation.
 *
 * Logging calls are typically done using the provided logging macros (which in turn call the
 * qh_log_ex function) given they greatly simplify the calls, but users are free to use this
 * function directly if necessary / desired.
 *
 * Though it is not recommended, it is safe to call this function with a NULL qh_log_info structure
 * when absolutely necessary, in which case all default values will be used, along with the default
 * context configuration.
 *
 * @param[in] log_info The log information structure to use with this log message, or NULL to use
 *                     default values.
 * @param[in] fmt      A string that specifies the format of the output.  The formatting string
 *                     determines what additional arguments you need to provide.  For more
 *                     information, see vprintf().
 * @param[in] arglist  A variable-argument list of the additional arguments, which must be
 *                     initialized with the va_start() macro.
 *
 * @return EOK on success or a standard errno on failure.  errno is unmodified.
 *
 * @see qh_log_info_t & vprintf
 */
int qh_vlog_ex(const qh_log_info_t *__log_info, const char *__fmt, va_list __arglist) QH_ATTR_FUNC_FORMAT_PRINTF(2, 0);

/**
 * Create a logging context.
 *
 * Creates a logging context with a given name for use with the qh logging interface.  The context
 * name is used as a prefix when the context is used for logging, and is also used to match the
 * context when specified by name in the api.
 *
 * Applications and libraries would not normally call this function directly, and would instead make
 * use of the QH_LOG_DEFAULT_CONTEXT_INIT() macro, and define QH_LOG_USER_CONTEXT_NAME to hold the
 * context name.  QH_LOG_USER_CONTEXT_NAME would typically be defined in the project's makefiles,
 * but it could also be defined in the code prior to including <qh/log.h> in the file making use of
 * the QH_LOG_DEFAULT_CONTEXT_INIT() macro.  The macro would be called only once in a source file
 * throughout the entire project, and would be called at file scope, without a semicolon at the end.
 * When this macro is not used for initializing the context, the qh_log, qh_log_noloc, qh_log_loc
 * and QH_LOG_CHECK macros could not be used with the context, as they all make use of the default
 * context variable reference (QH_LOG_USER_CONTEXT_VAR).
 *
 * A library or application normally has a single logging context as it would be used to identify it
 * (by name) in the logs.
 *
 * The context pointer must be declared as extern (see declaration of QH_LOG_USER_CONTEXT_VAR), with
 * hidden visibility so that it is not globally seen by other loaded modules.  Use of a static
 * variable at file scope in a source file is also acceptable if logging calls will only be made
 * from that file.
 *
 * All these details are totally abstracted when making use of the QH_LOG_DEFAULT_CONTEXT_INIT()
 * macro, so all an application really needs to do is the following:
 *
 *     <somewhere in the makefiles>:
 *     CPPFLAGS += -DQH_LOG_USER_CONTEXT_NAME="\"$(NAME)\""
 *
 *     <in a single source file (non-header)>
 *     #include <qh/log.h>
 *     ...
 *     QH_LOG_DEFAULT_CONTEXT_INIT()
 *     ...
 *     ...<rest of source file>
 *
 * The context names "all", "default", and "qh" are reserved, so if the name matches one of those,
 * its first character is replaced by an underscore character.  The name can be composed of any
 * alpha numeric characters, along with any characters from the set "#()+-.<>[]_" (excluding the
 * double quotes).  Any other character is converted to an underscore, the maximum length (including
 * null-termination) of the name is 32 characters (longer names are truncated), and it cannot have
 * zero length.  This is something to keep in mind when using the qh_log_get_*_by_name() functions,
 * as the normalized name needs to be used with those.  Note that the qh_log_context_normalize_name
 * function can be used to either prepare a context name prior to creation, or to get the name that
 * ended up being used by the creation process given the input name.
 *
 * Though it is suboptimal, calling this function more than once with the same context pointer is
 * allowed, but if a context at that address exists already, the name in the second call will be
 * ignored.
 *
 * @param[in]  name    The name of the context to be created.
 * @param[out] context The location where the context can be stored.
 *
 * @return EOK on success or a standard errno on failure.
 *
 * @see qh_log_context_destroy, QH_LOG_DEFAULT_CONTEXT_INIT, QH_LOG_CONTEXT_INIT, QH_LOG_USER_CONTEXT_NAME,
 *      qh_log_context_normlize_name
 */
int qh_log_context_create(const char *__name, qh_log_context_t **__context);

/**
 * Normalize a context name.
 *
 * Normalizes a context name in preparation for context creation (converting as
 * qh_log_context_create would), or to get the name that ended up being used by the creation process
 * given the provided context name.
 *
 * @note The conversion process happens in-place, replacing the passed-in buffer with the normalized
 *       version of the name.
 *
 * @param[in,out] context_name The name of the context to convert on input, and the converted name
 *                             on output.  If the context_name is NULL or has zero length, an error
 *                             is returned.
 *
 * @return EOK on success or a standard errno on failure.
 *
 * @see qh_log_context_create
 */
int qh_log_context_normalize_name(char *__context_name);

/**
 * Destroys a logging context.
 *
 * Destroys a logging context previously created with qh_log_context_create.
 *
 * Applications and libraries would not normally call this function directly, as it would
 * automatically be handled by the QH_LOG_DEFAULT_CONTEXT_INIT() macro.
 *
 * Though it is suboptimal, calling this function more than once with the same context pointer is
 * allowed, and so is calling it with a NULL context pointer.
 *
 * @param[in] context The location of the context.  A NULL value has no effect.
 *
 * @return EOK on success or a standard errno on failure.
 *
 * @see qh_log_context_create, QH_LOG_DEFAULT_CONTEXT_INIT, QH_LOG_CONTEXT_INIT
 */
void qh_log_context_destroy(qh_log_context_t **__context);

/*
 * Using constructor priority 115.  The minimum is 101, and we want to leave a little space for
 * others to get in front if necessary.
 */
#define QH_LOG_CONTEXT_INIT(__ctx_name, __ctx_var) \
    qh_log_context_t *__ctx_var; \
    static void qh_log_context_init_##__ctx_var##_(void) QH_ATTR_FUNC_CONSTRUCTOR_PRIORITY(115); \
    static void qh_log_context_fini_##__ctx_var##_(void) QH_ATTR_FUNC_DESTRUCTOR_PRIORITY(115); \
    static void qh_log_context_init_##__ctx_var##_(void) \
    { \
        const int __retval = qh_log_context_create((__ctx_name), &__ctx_var); \
        if (__retval != EOK) { \
            const char * const __reason = ((__ctx_name) == NULL) ? " (NULL context name)" : \
                                          ((__ctx_name)[0] == '\0') ? " (Empty context name)" : ""; \
            qh_log_loc(QH_LOG_CRITICAL_STDERR, \
                "qh log context creation failed%s, aborting (%d, %s)", __reason, errno, strerror(errno)); \
            if ((__ctx_name) != NULL) { \
                qh_log(QH_LOG_CRITICAL_STDERR, "context name was: %s", (__ctx_name)); \
            } \
            if (__retval == EEXIST) { \
                qh_log(QH_LOG_CRITICAL_STDERR, "A context at that address exists with a different context name.  " \
                          "Define a unique QH_LOG_USER_CONTEXT_VAR in your static libraries' make file to avoid this.  " \
                          "Refer to QH documentation for more details."); \
            } \
            abort(); \
        } \
    } \
    static void qh_log_context_fini_##__ctx_var##_(void) \
    { \
        qh_log_context_destroy(&__ctx_var); \
    }

#ifdef QH_LOG_USER_CONTEXT_NAME
#define QH_LOG_DEFAULT_CONTEXT_INIT() QH_LOG_CONTEXT_INIT(QH_LOG_USER_CONTEXT_NAME, QH_LOG_USER_CONTEXT_VAR)
#else
#define QH_LOG_DEFAULT_CONTEXT_INIT() _Static_assert(false, "QH_LOG_USER_CONTEXT_NAME must be defined in order to use QH_LOG_DEFAULT_CONTEXT_INIT");
#endif

/** @cond */
__END_DECLS
/** @endcond */

#endif /* _QH_LOG_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/qh/public/qh/log.h $ $Rev: 901921 $")
#endif
