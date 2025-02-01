/*
 * $QNXLicenseC:
 * Copyright 2009, 2010, QNX Software Systems. All Rights Reserved.
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

#ifndef __JSON_H_INCLUDED
#define __JSON_H_INCLUDED

#ifndef _STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef _STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef _STDBOOL_H_INCLUDED
#include <stdbool.h>
#endif

#ifndef _STDARG_H_INCLUDED
#include <stdarg.h>
#endif

/**
 * @file json.h
 *
 * JSON encoding and decoding API
 *
 * @par JSON API Overview
 *
 * @todo Provide some further detail and examples here for this API.
 */

/** @cond */
__BEGIN_DECLS
/** @endcond */

/**
 * JSON decoder error codes.
 *
 * Decoder error codes are returned with each @c json_decoder_ function call, and are also available with json_decoder_get_status().
 *
 * @see json_decoder_get_status
 */
typedef enum {
    /** No error condition exists. */
    JSON_DECODER_OK = 0,
    /** The operation failed to allocate memory. */
    JSON_DECODER_NO_MEMORY = 1,
    /** The requested data doesn't match the data type of the element or property. */
    JSON_DECODER_BAD_TYPE = 2,
    /** The requested element or property was not found. */
    JSON_DECODER_NOT_FOUND = 3,
    /** An error occurred parsing JSON data. */
    JSON_DECODER_PARSE_ERROR = 4,
    /** A numeric conversion failed because the result would lose precision or be out of range. */
    JSON_DECODER_CONVERSION_FAILED = 5,
    /** A call to json_decoder_pop() was made while at the root of the decoder tree. */
    JSON_DECODER_POP_AT_ROOT = 6,
    /** A call to json_decoder_set_state() was made with an invalid state instance. */
    JSON_DECODER_INVALID_STATE = 7,
    /** @details Indicates an external error has occurred. This is not used internally but may be set via json_decoder_set_status()
     * to signal a caller-detected error. */
    JSON_DECODER_EXTERNAL_ERROR = 8,
    /** An error occurred when using an encoder from json_decoder_get_encoder() to add or modify data in the decoder tree. */
    JSON_DECODER_ENCODING_ERROR = 9,
    /** An error occurred reading JSON data from a file. The specific error is indicated by errno. */
    JSON_DECODER_FILE_READ_ERROR = 10,
} json_decoder_error_t;

/**
 * JSON decoder parser error codes.
 *
 * In the case that a call to json_decoder_parse_json_str() fails with a #JSON_DECODER_PARSE_ERROR, the specific error and the
 * location of the error may be obtained by calling json_decoder_get_parse_error().
 *
 * @see json_decoder_get_parse_error
 */
typedef enum {
    /** No parsing error exists. */
    JSON_DECODER_PARSE_OK = 0,
    /** A @c \" character was expected. */
    JSON_DECODER_DOUBLE_QUOTE_EXPECTED = 1,
    /** A @c : character was expected following a property name. */
    JSON_DECODER_COLON_EXPECTED = 2,
    /** A @c } or @c , character was expected. */
    JSON_DECODER_RIGHT_BRACE_OR_COMMA_EXPECTED = 3,
    /** A @c ] or @c , character was expected. */
    JSON_DECODER_RIGHT_BRACKET_OR_COMMA_EXPECTED = 4,
    /** The string @c true was expected. */
    JSON_DECODER_TRUE_EXPECTED = 5,
    /** The string @c false was expected. */
    JSON_DECODER_FALSE_EXPECTED = 6,
    /** The string @c null was expected. */
    JSON_DECODER_NULL_EXPECTED = 7,
    /** An unexpected character was found. */
    JSON_DECODER_UNEXPECTED_CHARACTER = 8,
    /** A string was not terminated by a @c ". */
    JSON_DECODER_UNTERMINATED_STRING = 9,
    /** No further data was found following a @c \\ character. */
    JSON_DECODER_INCOMPLETE_ESCAPED_CHARACTER = 10,
    /** A character in a @c \\uxxxx sequence is not a hexadecimal digit. */
    JSON_DECODER_INVALID_HEXADECIMAL_DIGIT = 11,
    /** A unicode utf-16 trailing surrogate was not found following a lead surrogate. */
    JSON_DECODER_MISSING_TRAIL_SURROGATE = 12,
    /** A unicode utf-16 trailing surrogate was encountered where it is not permitted. */
    JSON_DECODER_UNEXPECTED_TRAIL_SURROGATE = 13,
    /** An array or object contained a @c , with no following element or property. */
    JSON_DECODER_UNEXPECTED_COMMA = 14,
    /** JSON string is followed by extra characters or unterminated comment. */
    JSON_DECODER_EXTRA_CHARACTERS = 15,
    /** JSON string ended unexpectedly. */
    JSON_DECODER_UNEXPECTED_END_OF_STRING = 16,
} json_decoder_parse_error_t;

/**
 * JSON decoder node types.
 *
 * JSON decoder node types reflect the type of properties in a JSON object or the type of elements in a JSON array. With respect
 * to this API, the term @a node is used to refer to either an object property or an array element. Enumeration of array elements
 * and object properties is largely the same, aside from the fact that nodes representing array elements do not have
 * an associated name.
 *
 * Refer to json_decoder_next(), json_decoder_name(), json_decoder_type() and the @c json_decoder_get_<type> functions for more
 * detail on node access from the JSON decoder tree.
 */
typedef enum {
    /** The node is a @e null data type. */
    JSON_TYPE_NULL,
    /** The node is a @e boolean data type. */
    JSON_TYPE_BOOL,
    /** The node is a @e number data type. */
    JSON_TYPE_NUMBER,
    /** The node is a @e string data type. */
    JSON_TYPE_STRING,
    /** The node is an @e array containing other elements. */
    JSON_TYPE_ARRAY,
    /** The node is an @e object containing other properties. */
    JSON_TYPE_OBJECT,
    /** This type is returned for a non-existent node. */
    JSON_TYPE_NONE,
} json_node_type_t;


/**
 * @struct _json_decoder
 *
 * JSON decoder.
 *
 * This structure is used to represent an instance of a JSON decoder.
 *
 * @see json_decoder_create & json_decoder_destroy
 */
struct _json_decoder;
typedef struct _json_decoder json_decoder_t;

/**
 * @struct _json_encoder
 *
 * JSON encoder.
 *
 * This structure is used to represent an instance of a JSON encoder.
 *
 * @see json_encoder_create & json_encoder_destroy
 */
struct _json_encoder;
typedef struct _json_encoder json_encoder_t;

/**
 * @struct _json_decoder_state
 *
 * JSON decoder state.
 *
 * This structure is used to represent an instance a JSON decoder state.
 *
 * @see json_decoder_get_state & json_decoder_set_state
 */
struct _json_decoder_state;
typedef struct _json_decoder_state json_decoder_state_t;

/**
 * Create a new JSON decoder instance.
 *
 * @return      The newly allocated decoder instance.
 * @retval NULL There is not enough memory to perform the operation.
 */
json_decoder_t *json_decoder_create(void);

/**
 * Destroy an instance of a JSON decoder.
 *
 * @param[in] decoder The JSON decoder instance to destroy. It is safe to call json_decoder_destroy() with a @c NULL pointer.
 *
 * @retval None
 */
void json_decoder_destroy(json_decoder_t *decoder);

/**
 * Parse a string of JSON formatted data.
 *
 * @param[in] decoder The JSON decoder instance.
 * @param[in] str     A pointer to a null terminated string containing JSON data.
 *
 * @retval #JSON_DECODER_OK          Success.
 * @retval #JSON_DECODER_PARSE_ERROR An error occurred parsing the string. Use json_decoder_get_parse_error() to determine the
 *                                   location and the reason for the error.
 *
 * @see json_decoder_error_t & json_decoder_get_parse_error
 */
json_decoder_error_t json_decoder_parse_json_str(json_decoder_t *decoder, const char *str);

/**
 * Parse a file of JSON formatted data.
 *
 * @param[in] decoder The JSON decoder instance.
 * @param[in] file    The path to a file containing the JSON formatted data.
 *
 * @retval #JSON_DECODER_OK              Success.
 * @retval #JSON_DECODER_FILE_READ_ERROR An error occurred retrieving the JSON data from the file.
 *                                       The specific error is indicated by errno.
 * @retval #JSON_DECODER_PARSE_ERROR     An error occurred parsing the JSON data. Use
 *                                       json_decoder_get_parse_error() to determine the location
 *                                       and the reason for the error.
 *
 * @see json_decoder_error_t & json_decoder_get_parse_error
 */
json_decoder_error_t json_decoder_parse_file(json_decoder_t *decoder, const char *file);

/**
 * Get the reason and location of a parse error.
 *
 * If json_decoder_parse_json_str() returns a value of #JSON_DECODER_PARSE_ERROR, this function will return the cause of the error
 * and the offset into the JSON string at which the error was detected. If no parsing error occurred, this function will return
 * #JSON_DECODER_PARSE_OK and not update @a location.
 *
 * @param[in]  decoder  The JSON decoder instance.
 * @param[out] location A pointer to the @c int that will be updated with the offset of the error in the original JSON data. If
 *                      @c NULL, the location of the error is not returned.
 * @param[in] error     A pointer to a char pointer that will be updated with a string indicating the type of error. If
 *                      @c NULL, the error string is not returned.
 *
 * @return The parse error code.
 *
 * @see json_decoder_parse_error_t
 */
json_decoder_parse_error_t json_decoder_get_parse_error(json_decoder_t *decoder, int *location, const char **error);

/**
 * Clean up a JSON decoder instance, freeing any allocated memory associated with the decoder tree.
 *
 * Calling this function is not typically required as the decoder is cleaned up when json_decoder_destroy() is called. It may be
 * used to reduce memory usage if particularly large strings are parsed and it is not desirable to destroy the decoder itself. Calls
 * to json_decoder_parse_json_str() will make use of already allocated memory so there is no need to call json_decoder_cleanup()
 * prior to calling these functions.
 *
 * @param[in] decoder The JSON decoder instance.
 *
 * @retval None
 */
void json_decoder_cleanup(json_decoder_t *decoder);

/**
 * Reset the JSON decoder instance.
 *
 * The decoder instance is positioned at the root node, reflecting the state it is in immediately after parsing data.
 *
 * @param[in] decoder The JSON decoder instance.
 *
 * @retval None
 */
void json_decoder_reset(json_decoder_t *decoder);

/**
 * Get the current error status of the JSON decoder instance.
 *
 * Any attempt to extract data or push into objects that results in an error sets the the decoder to an error status. Rather than
 * checking return codes after every operation, callers may alternatively perform a series of operations and check if the entire
 * set was completed successfully or not with json_decoder_get_status().
 *
 * @param[in] decoder The JSON decoder instance.
 * @param[in] clear   Pass @c true to clear the error status following the call.
 *
 * @return The current error status of the decoder.
 *
 * @see json_decoder_error_t
 */
json_decoder_error_t json_decoder_get_status(json_decoder_t *decoder, bool clear);

/**
 * Set the JSON decoder status.
 *
 * Setting the status of a decoder instance has no effect on the behavior of the decoder. This function is intended to allow a
 * caller to reset the status of the decoder to #JSON_DECODER_OK, such that the success of subsequent operations can be determined,
 * or to set decoder to a status that is not detectable directly by the decoder (for example: out of memory or a range error) for
 * subsequent handling by the caller.
 *
 * @param[in] decoder The JSON decoder instance.
 * @param[in] status  The decoder status to set.
 *
 * @retval None
 *
 * @see json_decoder_error_t
 */
void json_decoder_set_status(json_decoder_t *decoder, json_decoder_error_t status);

/**
 * Position the decoder to the specified property.
 *
 * This function searches for a property with the given name in the current object and if found positions the decoder at that
 * property.
 *
 * @param[in] decoder The JSON decoder instance.
 * @param[in] name    The name of the property to set the position to.
 *
 * @retval #JSON_DECODER_OK        Success.
 * @retval #JSON_DECODER_NOT_FOUND The given property does not exist in the current object.
 *
 * @see json_decoder_error_t
 */
json_decoder_error_t json_decoder_position_property(json_decoder_t *decoder, const char *name);

/**
 * Returns the type of the current node or the type of the node with the given name.
 *
 * @note The caller can pass @c "." in @a name to determine if the decoder is positioned within an object or an array.
 *
 * @param[in] decoder The JSON decoder instance.
 * @param[in] name    The name of the node to provide the type for or @c NULL to return the type of the current node.
 *
 * @return                 The type of the referenced node.
 * @retval #JSON_TYPE_NONE The specified node does not exist.
 *
 * @see json_node_type_t
 */
json_node_type_t json_decoder_type(json_decoder_t *decoder, const char *name);

/**
 * Removes a node from the decoder tree.
 *
 * After this call the node will no longer be accessible from the decoder instance.
 *
 * @note Information previously returned from the node, in particular name or value strings, continue to be valid
 * until a subsequent call to json_decoder_destroy(), json_decoder_parse_json_str() or json_decoder_cleanup(), or until
 * @c json_encoder_ calls are made on an encoder returned from json_decoder_get_encoder() to modify the decoder tree.
 *
 * @param[in] decoder The JSON decoder instance.
 * @param[in] name    The name of the node to remove or @c NULL to remove the current node.
 *
 * @retval #JSON_DECODER_OK        Success.
 * @retval #JSON_DECODER_NOT_FOUND The specified node does not exist.
 */
json_decoder_error_t json_decoder_remove_node(json_decoder_t *decoder, const char *name);

/**
 * Determines if a node represents an integer value or not.
 *
 * Returns @c true if the specified or current node is an integer, or @c false otherwise. This call, in conjunction with
 * json_decoder_type() returning #JSON_TYPE_NUMBER, can be used to determine if the number value for the node can be returned
 * without error using json_decoder_get_int_ll() or json_decoder_get_double().
 *
 * @param[in] decoder The JSON decoder instance.
 * @param[in] name    The name of the node to examine, or @c NULL to examine the current node.
 *
 * @retval true  The node represents an integer value which can be safely accessed using json_decoder_get_int_ll().
 * @retval false The node does not represent an integer value. In this case, if the node's type is #JSON_TYPE_NUMBER, its value
 *               can be safely accessed using json_decoder_get_double(). Otherwise its value should be accessed using the type's
 *               corresponding @c json_get_ function.
 */
bool json_decoder_is_integer(json_decoder_t *decoder, const char *name);

/**
 * Get the offset of the node's value within the original JSON string.
 *
 * @param[in] decoder The JSON decoder instance.
 * @param[in] name    The name of the node to get the offset of. If @c NULL is passed, the offset of the
 *                    current node is returned.
 * @param[out] offset The offset within the original JSON data that represents the value of this node.
 * @param[out] length The length of the string representing the value of this node. If the node was not part
 *                    of the original string the length will be 0.
 *
 * @retval #JSON_DECODER_OK Success.
 * @retval #JSON_DECODER_NOT_FOUND Node does not exist.
 *
 * @see json_decoder_error_t
 */
json_decoder_error_t json_decoder_get_offset(json_decoder_t *decoder, const char *name, int *offset, int *length);

/**
 * Descend into an object.
 *
 * Positions the decoder within the specified object. If successful, subsequent calls can be made to return the nodes representing
 * the properties of the object.
 *
 * @param[in] decoder  The JSON decoder instance.
 * @param[in] name     The name of the object to descend into, or @c NULL to descend into the current object.
 * @param[in] optional Indicates if the presence of this object is optional or not. When descending into an optional object, the
 *                     decoder will not register an error status if the object does not exist.
 *
 * @retval #JSON_DECODER_OK        Success.
 * @retval #JSON_DECODER_BAD_TYPE  The specified node is not an object.
 * @retval #JSON_DECODER_NOT_FOUND The specified node does not exist.
 *
 * @see json_decoder_error_t
 */
json_decoder_error_t json_decoder_push_object(json_decoder_t *decoder, const char *name, bool optional);

/**
 * Descend into an array.
 *
 * Positions the decoder within the specified array. If successful, subsequent calls can be made to return the nodes representing
 * the elements of the array.
 *
 * @param[in] decoder  The JSON decoder instance.
 * @param[in] name     The name of the array to descend into, or @c NULL to descend into the current array.
 * @param[in] optional Indicates if the presence of this array is optional or not. When descending into an optional array, the
 *                     decoder will not register an error status if the array does not exist.
 *
 * @retval #JSON_DECODER_OK        Success.
 * @retval #JSON_DECODER_BAD_TYPE  The specified node is not an array.
 * @retval #JSON_DECODER_NOT_FOUND The specified node does not exist.
 *
 * @see json_decoder_error_t
 */
json_decoder_error_t json_decoder_push_array(json_decoder_t *decoder, const char *name, bool optional);

/**
 * Ascend out of an object or array.
 *
 * Positions the decoder to the parent of the current object or array. Following this call, the current node will be the next node
 * following the object or array being popped from.
 *
 * @param[in] decoder The JSON decoder instance.
 *
 * @retval #JSON_DECODER_OK          Success.
 * @retval #JSON_DECODER_POP_AT_ROOT The decoder is already positioned at the root node.
 *
 * @see json_decoder_error_t
 */
json_decoder_error_t json_decoder_pop(json_decoder_t *decoder);

/**
 * Get the size of the current node.
 *
 * Returns the number of elements in the current array, or the number of properties in the current object.
 *
 * @param[in] decoder The JSON decoder instance.
 *
 * @return The element or property count.
 */
int json_decoder_length(json_decoder_t *decoder);

/**
 * Get the name of the current property.
 *
 * This is only applicable if the decoder is currently positioned within an object element.
 *
 * @note Name strings returned by this function continue to be valid until a subsequent call to json_decoder_destroy(),
 * json_decoder_parse_json_str() or json_decoder_cleanup(), or until @c json_encoder_ calls are made on an encoder returned from
 * json_decoder_get_encoder() to modify the decoder tree.
 *
 * @param[in] decoder The JSON decoder instance.
 *
 * @return      The current property's name.
 * @retval NULL The decoder is not positioned within an object.
 */
const char *json_decoder_name(json_decoder_t *decoder);

/**
 * Position to the next node in the current array or object.
 *
 * Positions the decoder to the next element in the current array, or the next property in the current object, if one exists.
 *
 * @param[in] decoder The JSON decoder instance.
 *
 * @retval #JSON_DECODER_OK        Success.
 * @retval #JSON_DECODER_NOT_FOUND The next node does not exist.
 *
 * @see json_decoder_error_t
 */
json_decoder_error_t json_decoder_next(json_decoder_t *decoder);

/**
 * Position to the previous node in the current array or object.
 *
 * Positions the decoder to the previous element in the current array, or the previous property in the current object, if one exists.
 *
 * @param[in] decoder The JSON decoder instance.
 *
 * @retval #JSON_DECODER_OK        Success.
 * @retval #JSON_DECODER_NOT_FOUND The previous node does not exist.
 *
 * @see json_decoder_error_t
 */
json_decoder_error_t json_decoder_previous(json_decoder_t *decoder);

/**
 * Return the index of the current node.
 *
 * Returns the 0-based index of the current node within its parent.

 * @param[in] decoder The JSON decoder instance.
 *
 * @return The index of the node.
 */
int json_decoder_index(json_decoder_t *decoder);

/**
 * Position to the specified node in the current array or object.
 *
 * Positions the decoder to the specified node in the current array, or the specified property in the current object, if it
 * exists.
 *
 * @param[in] decoder The JSON decoder instance.
 * @param[in] index   The 0-based index of the node to be positioned at.
 *
 * @retval #JSON_DECODER_OK        Success.
 * @retval #JSON_DECODER_NOT_FOUND No node exists at the specified index.
 *
 * @see json_decoder_error_t
 */
json_decoder_error_t json_decoder_position_index(json_decoder_t *decoder, int index);

/**
 * Get the current state of the decoder.
 *
 * Returns the current state of the decoder so that it may be restored at a later time with json_decoder_set_state(). The
 * state that is returned is no longer valid following calls to @c json_decoder_cleanup(), @c json_decoder_parse_json_str() or
 * @c json_decoder_free_state().
 *
 * @param[in] decoder The JSON decoder instance.
 *
 * @return      The current JSON decoder state instance.
 * @retval NULL There was insufficient memory to get the state.
 *
 * @see json_decoder_set_state
 */
json_decoder_state_t *json_decoder_get_state(json_decoder_t *decoder);

/**
 * Set the state of the decoder.
 *
 * Restores the decoder to a state previously obtained with json_decoder_get_state().
 *
 * @param[in] decoder The JSON decoder instance.
 * @param[in] state   The JSON decoder state instance returned by a previous call to json_decoder_get_state().
 *
 * @retval #JSON_DECODER_OK            Success.
 * @retval #JSON_DECODER_INVALID_STATE The given @a state was not valid.
 *
 * @see json_decoder_get_state
 */
json_decoder_error_t json_decoder_set_state(json_decoder_t *decoder, json_decoder_state_t *state);

/**
 * Free a state returned by json_decoder_get_state().
 *
 * Frees the memory associated with the decoder state.  It is not usually necessary to explicitly free states as they
 * will be freed automatically by calls to @c json_decoder_cleanup(), @c json_decoder_parse_json_str() and
 * @c json_decoder_destroy().  Calling @c json_decoder_free_state() is important only if there are large numbers
 * of calls to @c json_decoder_get_state() without the decoder being cleaned up or a new JSON string being parsed.
 *
 * @param[in] decoder The JSON decoder instance.
 * @param[in] state   The JSON decoder state to be freed.
 *
 * @retval None
 *
 * @see json_decoder_get_state
 */
void json_decoder_free_state(json_decoder_t *decoder, json_decoder_state_t *state);

/**
 * Get the boolean value from the current node or the node with the given name (if the decoder is currently positioned in an
 * object).
 *
 * Returns the boolean value from the current or the specified node. If successful, the decoder will advance its position to the
 * next node in the object or array.
 *
 * @param[in]  decoder  The JSON decoder instance.
 * @param[in]  name     The name of the node to return the value for, or @c NULL to return the value for the current node.
 * @param[out] value    A pointer to the @c bool that will be updated with the value.
 * @param[in]  optional Indicates if the presence of this node is optional or not. When returning values for optional nodes, the
 *                      decoder will not register an error status if the node does not exist.
 *
 * @retval #JSON_DECODER_OK        Success.
 * @retval #JSON_DECODER_BAD_TYPE  The specified node is not a boolean type.
 * @retval #JSON_DECODER_NOT_FOUND The specified node does not exist.
 *
 * @see json_decoder_error_t
 */
json_decoder_error_t json_decoder_get_bool(json_decoder_t *decoder, const char *name, bool *value, bool optional);

/**
 * Get the integer value from the current node or the node with the given name (if the decoder is currently positioned in an
 * object).
 *
 * Returns the @c int value from the current or the specified node. If successful, the decoder will advance its position to the next
 * node in the object or array.
 *
 * @param[in]  decoder  The JSON decoder instance.
 * @param[in]  name     The name of the node to return the value for, or @c NULL to return the value for the current node.
 * @param[out] value    A pointer to the @c int that will be updated with the value.
 * @param[in]  optional Indicates if the presence of this node is optional or not. When returning values for optional nodes, the
 *                      decoder will not register an error status if the node does not exist.
 *
 * @retval #JSON_DECODER_OK                Success.
 * @retval #JSON_DECODER_BAD_TYPE          The specified node is not a number type.
 * @retval #JSON_DECODER_CONVERSION_FAILED The specified node is not an integer type or is out of the range of an @c int type.
 * @retval #JSON_DECODER_NOT_FOUND         The specified node does not exist.
 *
 * @see json_decoder_error_t
 */
json_decoder_error_t json_decoder_get_int(json_decoder_t *decoder, const char *name, int *value, bool optional);

/**
 * Get the <tt>long long</tt> integer value from the current node or the node with the given name (if the decoder is currently
 * positioned in an object).
 *
 * Returns the <tt>long long int</tt> value from the current or the specified node. If successful, the decoder will advance
 * its position to the next node in the object or array.
 *
 * @param[in]  decoder  The JSON decoder instance.
 * @param[in]  name     The name of the node to return the value for, or @c NULL to return the value for the current node.
 * @param[out] value    A pointer to the <tt>long long int</tt> that will be updated with the value.
 * @param[in]  optional Indicates if the presence of this node is optional or not. When returning values for optional nodes, the
 *                      decoder will not register an error status if the node does not exist.
 *
 * @retval #JSON_DECODER_OK                Success.
 * @retval #JSON_DECODER_BAD_TYPE          The specified node is not a number type.
 * @retval #JSON_DECODER_CONVERSION_FAILED The specified node is not an integer type.
 * @retval #JSON_DECODER_NOT_FOUND         The specified node does not exist.
 *
 * @see json_decoder_error_t
 */
json_decoder_error_t json_decoder_get_int_ll(json_decoder_t *decoder, const char *name, long long *value, bool optional);

/**
 * Get the @c double value from the current node or the node with the given name (if the decoder is currently positioned in an
 * object).
 *
 * Returns the @c double value from the current or the specified node. If successful, the decoder will advance its position to the
 * next node in the object or array.
 *
 * @param[in]  decoder  The JSON decoder instance.
 * @param[in]  name     The name of the node to return the value for, or @c NULL to return the value for the current node.
 * @param[out] value    A pointer to the @c double that will be updated with the value.
 * @param[in]  optional Indicates if the presence of this node is optional or not. When returning values for optional nodes, the
 *                      decoder will not register an error status if the node does not exist.
 *
 * @retval #JSON_DECODER_OK                Success.
 * @retval #JSON_DECODER_BAD_TYPE          The specified node is not a number type.
 * @retval #JSON_DECODER_CONVERSION_FAILED The specified node is an integer type which cannot be accurately represented in a @c double.
 * @retval #JSON_DECODER_NOT_FOUND         The specified node does not exist.
 *
 * @see json_decoder_error_t
 */
json_decoder_error_t json_decoder_get_double(json_decoder_t *decoder, const char *name, double *value, bool optional);

/**
 * Get the string value from the current node or the node with the given name (if the decoder is currently positioned in an
 * object).
 *
 * Returns the string value from the current or the specified node. If successful, the decoder will advance its position to the
 * next node in the object or array.
 *
 * @note Value strings returned by this function continue to be valid until a subsequent call to json_decoder_destroy(),
 * json_decoder_parse_json_str() or json_decoder_cleanup(), or until @c json_encoder_ calls are made on an encoder returned from
 * json_decoder_get_encoder() to update the decoder tree.
 *
 * @param[in] decoder   The JSON decoder instance.
 * @param[in]  name     The name of the node to return the value for, or @c NULL to return the value for the current node.
 * @param[out] value    A pointer to the string that will be updated with the value.
 * @param[in]  optional Indicates if the presence of this node is optional or not. When returning values for optional nodes, the
 *                      decoder will not register an error status if the node does not exist.
 *
 * @retval #JSON_DECODER_OK        Success.
 * @retval #JSON_DECODER_BAD_TYPE  The specified node is not a string type.
 * @retval #JSON_DECODER_NOT_FOUND The specified node does not exist.
 *
 * @see json_decoder_error_t
 */
json_decoder_error_t json_decoder_get_string(json_decoder_t *decoder, const char *name, const char **value, bool optional);

/**
 * Return an encoder instance for use in adding to or updating nodes in the decoder tree.
 *
 * This function returns a JSON encoder instance that enables adding of new nodes to the decoder tree or modifying existing ones. New
 * nodes are inserted within the object or array prior to the node the decoder is currently positioned at. Adding a new node that
 * has the same name as an existing node will update the value of the existing node. The encoder remains valid until
 * @c json_decoder_cleanup(), @c json_decoder_destroy() or @c json_encoder_destroy() is called.
 *
 * @param[in] decoder The JSON decoder instance.
 *
 * @return The JSON encoder instance.
 */
json_encoder_t *json_decoder_get_encoder(json_decoder_t *decoder);

/**
 * Return the current data version for the decoder instance.
 *
 * The data version is available purely to provide information to callers about changes to the contents of a decoder instance. The
 * data version can be set by the caller using json_decoder_set_data_version(), and will be updated automatically when parsing new
 * data with json_decoder_parse_json_str(), removing nodes with json_decoder_remove_node(), or when using the encoder returned from
 * @c json_decoder_get_encoder() to add or update nodes in the decoder tree.
 *
 * @param[in] decoder The JSON decoder instance.
 *
 * @return The current data version.
 *
 * @see json_decoder_set_data_version
 */
int json_decoder_get_data_version(json_decoder_t *decoder);

/**
 * Set the current data version for the decoder instance.
 *
 * This function sets the current data version for the given decoder.
 *
 * The data version is available purely to provide information to callers about changes to the contents of a decoder instance.
 * Since the data version is not used for anything internally, callers may change the value as they see fit. The version will also
 * be updated automatically when parsing new data with json_decoder_parse_json_str(), removing nodes with
 * json_decoder_remove_node(), or when using the encoder returned from @c json_decoder_get_encoder() to add or update nodes in the
 * tree.
 *
 * @param[in] decoder The JSON decoder instance.
 * @param[in] version The new data version.
 *
 * @retval None
 */
void json_decoder_set_data_version(json_decoder_t *decoder, int version);

/**
 * Dump the tree contents for the JSON decoder.
 *
 * Dump the contents of the data in the JSON decoder tree in a human readable format to the given @c FILE pointer.
 *
 * @param[in] decoder The JSON decoder instance.
 * @param[in] fp      The file pointer to write the contents to.
 *
 * @retval None
 */
void json_decoder_dump_tree(json_decoder_t *decoder, FILE *fp);


/**
 * JSON encoder error codes.
 *
 * These error codes are returned with most @c json_encoder_ function calls during encoding operations, and are also available
 * through json_encoder_get_status().
 *
 * @see json_encoder_get_status
 */
typedef enum
{
    /** No error condition exists. */
    JSON_ENCODER_OK = 0,
    /** The operation failed to allocate memory. */
    JSON_ENCODER_NO_MEMORY = 1,
    /** Encoding failed because of improper object or array nesting. */
    JSON_ENCODER_BAD_NESTING = 2,
    /** Encoding failed because of an invalid value. */
    JSON_ENCODER_INVALID_VALUE = 3,
    /** Encoding failed attempting to add a value with no property name. */
    JSON_ENCODER_MISSING_PROPERTY_NAME = 4,
    /** Encoding failed attempting to encode from a non-existent node of a decoder. */
    JSON_ENCODER_NOT_FOUND = 5,
    /** The operation is not supported. */
    JSON_ENCODER_NOT_SUPPORTED = 6,
    /** Encoding failed attempting to add a second value when only a single value is allowed. */
    JSON_ENCODER_TOO_MANY_VALUES = 7,
    /** @details Encoding failed attempting to add a named property when only nameless elements are allowed (e.g. within an array).
     */
    JSON_ENCODER_UNEXPECTED_NAME = 8,
    /** Encoding failed attempting to exceed the maximum nesting level. */
    JSON_ENCODER_MAX_LEVEL_REACHED = 9,
    /** The state provided to json_encoder_set_state() was invalid. */
    JSON_ENCODER_INVALID_STATE = 10,
    /** @details Indicates an external error has occurred. This is not used internally but may be set via json_encoder_set_status()
     * to signal a caller-detected error. */
    JSON_ENCODER_EXTERNAL_ERROR = 11,
} json_encoder_error_t;

/**
 * JSON encoder data format codes.
 *
 * These values determine the resulting format of data encoded with the JSON encoder API.
 *
 * @see json_encoder_set_format
 */
typedef enum
{
    /** Encode in plain JSON format (see http://www.json.org). */
    JSON_ENCODER_FORMAT_JSON = 0
} json_encoder_format_t;

/**
 * JSON encoder option codes.
 *
 * These options control encoding behavior of the JSON encoder API.
 *
 * @see json_encoder_set_option & json_encoder_clear_option
 */
typedef enum
{
    /** Clear (zero out) allocated memory prior to freeing it. */
    JSON_ENCODER_CLEAR_MEMORY = 1,
    /** Include new lines and spaces to make output more readable (though less compact). */
    JSON_ENCODER_AUTO_FORMAT = 2,
    /** Include new lines and spaces on first level properties. */
    JSON_ENCODER_FILE_FORMAT = 4,
} json_encoder_option_t;


/**
 * @struct _json_encoder_state
 *
 * JSON encoder state.
 *
 * This structure is used to represent an instance a JSON encoder state.
 *
 * @see json_encoder_get_state & json_encoder_set_state
 */
struct _json_encoder_state;
typedef struct _json_encoder_state json_encoder_state_t;

/**
 * Create a new JSON encoder instance.
 *
 * @return      The newly allocated encoder instance.
 * @retval NULL There is not enough memory to perform the operation.
 */
json_encoder_t *json_encoder_create(void);

/**
 * Destroy an instance of a JSON encoder.
 *
 * @param[in] encoder The JSON encoder instance to destroy. It is safe to call json_encoder_destroy() with a @c NULL pointer.
 *
 * @retval None
 */
void json_encoder_destroy(json_encoder_t *encoder);

/**
 * Clean up a JSON encoder instance, freeing any memory associated with the encoder.
 *
 * Calling this function is not typically required as the encoder is cleaned up when json_encoder_destroy() is called. It may be
 * used to reduce memory usage if particularly large strings have been encoded and it is not desirable to destroy the encoder
 * itself. Calls to json_encoder_reset() will make use of already allocated memory so there is no need to call
 * json_encoder_cleanup() prior to calling these functions.
 *
 * @param[in] encoder The JSON encoder instance.
 *
 * @retval None
 *
 * @see json_encoder_reset
 */
void json_encoder_cleanup(json_encoder_t *encoder);

/**
 * Reset a JSON encoder instance.
 *
 * Resets a JSON encoder instance for encoding of new data. It is typically preferred to call json_encoder_reset() rather than
 * json_encoder_cleanup() when performing multiple encoding operations, to reduce memory allocation operations since theencoder's
 * buffer will automatically grow to accommodate the largest size required.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] offset  The offset within the encoder's buffer to position the encoder at. Typically this would be 0, but providing
 *                    another value can allow callers to encode multiple data sets into a single buffer.
 *
 * @retval None
 *
 * @see json_encoder_cleanup
 */
void json_encoder_reset(json_encoder_t *encoder, int offset);

/**
 * Get the current error status of the JSON encoder instance.
 *
 * The encoder status is sticky, so if an encoding operation fails, all subsequent calls will show the failure until the encoder
 * status is reset through a call to json_encoder_cleanup(), json_encoder_reset() or json_encoder_set_state().
 *
 * @param[in] encoder The JSON encoder instance.
 *
 * @return The current error status of the encoder.
 *
 * @see json_encoder_error_t
 */
json_encoder_error_t json_encoder_get_status(const json_encoder_t *encoder);

/**
 * Set the JSON encoder status.
 *
 * Setting the encoder status to an error can be used to signal that data cannot be encoded. When in an error state the encoder will
 * not encode any further data as it may result in JSON that is invalid.
 *
 * @note It is not possible to set the encoder status to #JSON_ENCODER_OK using this function, as the encoded data may be
 * unreliable. To clear an error status, callers should use json_encoder_cleanup() or json_encoder_reset() (and encode their data
 * from the beginning), or call json_encoder_set_state() to restore the state to a previous good state.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] status  The new encoder error status.
 *
 * @retval None
 *
 * @see json_encoder_error_t
 */
void json_encoder_set_status(json_encoder_t *encoder, json_encoder_error_t status);

/**
 * Set a JSON encoder option.
 *
 * Enables the given option on the JSON encoder instance. Options should only be adjusted immediately after calling
 * json_encoder_create(), json_encoder_cleanup() or json_encoder_reset().
 *
 * @note This call has no affect on an encoder instance obtained from json_decoder_get_encoder(), since JSON items encoded with
 * these encoders are added directly to the decoder tree rather than into a JSON formatted buffer.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] option  The option being enabled.
 *
 * @retval None
 *
 * @see json_encoder_option_t, json_encoder_clear_option
 */
void json_encoder_set_option(json_encoder_t *encoder, json_encoder_option_t option);

/**
 * Clear a JSON encoder option.
 *
 * Disables the given option on the JSON encoder instance. Options should only be adjusted immediately after calling
 * json_encoder_create(), json_encoder_cleanup() or json_encoder_reset().
 *
 * @note This call has no affect on an encoder instance obtained from json_decoder_get_encoder(), since JSON items encoded with
 * these encoders are added directly to the decoder tree rather than into a JSON formatted buffer.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] option  The option being disabled.
 *
 * @retval None
 *
 * @see json_encoder_option_t, json_encoder_set_option
 */
void json_encoder_clear_option(json_encoder_t *encoder, json_encoder_option_t option);

/**
 * Set the JSON encoder format.
 *
 * Sets the format for data subsequently encoded with the encoder instance. The format should only be set immediately after
 * calling json_encoder_create(), json_encoder_cleanup() or json_encoder_reset(). The default JSON encoder format is
 * #JSON_ENCODER_FORMAT_JSON.
 *
 * @note This call has no affect on an encoder instance obtained from json_decoder_get_encoder(), since JSON items encoded with
 * these encoders are added directly to the decoder tree rather than into a JSON formatted buffer.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] format  The new format for the encoder.
 *
 * @retval None
 *
 * @see json_encoder_format_t
 */
void json_encoder_set_format(json_encoder_t *encoder, json_encoder_format_t format);

/**
 * Get the current nesting level of a JSON encoder.
 *
 * The JSON encoder maintains a nesting level for the data being encoded. Each time json_encoder_start_array() or
 * json_encoder_start_object() is called, or json_encoder_end_array() or json_encoder_end_object() is called, the nesting level
 * increases or decreases by one.
 *
 * The nesting level is initialized to zero after a call to json_encoder_create(), json_encoder_cleanup() or json_encoder_reset().
 *
 * @param[in] encoder The JSON encoder instance.
 *
 * @return The current nesting level.
 */
int json_encoder_nesting_level(json_encoder_t *encoder);

/**
 * Request white space be added in a JSON encoder prior to the next name, value or @c {, @c }, @c [, or @c ] character.
 *
 * For situations where the JSON being generated wants to be more easily readable, additional white space can be added with this
 * function. Calling this function allows a newline and any number of spaces to be inserted prior to any subsequent name, value,
 * bracket or brace appended to the JSON buffer. This call must be repeated any time white space is wanted.
 *
 * @note This call has no affect on an encoder instance obtained from json_decoder_get_encoder(), since JSON items encoded with
 * these encoders are added directly to the decoder tree rather than into a JSON formatted buffer.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] newline Pass @c true to have a new line character inserted prior to the insertion of any spaces.
 * @param[in] spaces  The number of spaces to insert when appropriate.
 *
 * @retval None
 */
void json_encoder_add_whitespace(json_encoder_t *encoder, bool newline, int spaces);

/**
 * Get the current encoding length of a JSON encoder.
 *
 * Returns the current length of the data encoded by the JSON encoder instance.
 *
 * @note This call is not applicable to encoder instances obtained from json_decoder_get_encoder(), since JSON items encoded with
 * these encoders are added directly to the decoder tree rather than into a JSON formatted buffer.
 *
 * @param[in] encoder The JSON encoder instance.
 *
 * @return    The current length of the encoded data.
 * @retval -1 The encoder instance was obtained from json_decoder_get_encoder().
 */
int json_encoder_length(const json_encoder_t *encoder);

/**
 * Get the current state of the encoder.
 *
 * Getting the state allows the encoder to be returned to a state prior to subsequent encoding operations with
 * json_encoder_set_state(), effectively undoing these operations. A saved state is invalidated by calls to json_encoder_reset(),
 * json_encoder_cleanup() or json_encoder_set_state() with an earlier state.
 *
 * @note This call is not applicable on an encoder instance obtained from json_decoder_get_encoder(), since JSON items encoded with
 * these encoders are added directly to the decoder tree rather than into a JSON formatted buffer.
 *
 * @param[in] encoder The JSON encoder instance.
 *
 * @return      The current state of the encoder.
 * @retval NULL There was insufficient memory to get the state, or the encoder instance was obtained from
 *              json_decoder_get_encoder().
 *
 * @see json_encoder_set_state
 */
json_encoder_state_t *json_encoder_get_state(json_encoder_t *encoder);

/**
 * Set the state of the encoder.
 *
 * Restores the encoder to a state previously obtained with json_encoder_get_state().
 *
 * @note This call is not applicable on an encoder instance obtained from json_decoder_get_encoder(), since JSON items encoded with
 * these encoders are added directly to the decoder tree rather than into a JSON formatted buffer.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] state   The JSON encoder state instance returned by a previous call to json_encoder_get_state().
 *
 * @retval #JSON_ENCODER_OK            Success.
 * @retval #JSON_ENCODER_INVALID_STATE The given state was not valid.
 * @retval #JSON_ENCODER_NOT_SUPPORTED The encoder instance was obtained from json_decoder_get_encoder().
 *
 * @see json_encoder_get_state
 */
json_encoder_error_t json_encoder_set_state(json_encoder_t *encoder, json_encoder_state_t *state);

/**
 * Ensure a JSON encoder's buffer has at least @a length bytes free.
 *
 * Reserving space can speed up encoding if it is known in advance that a large amount of space is required. Additionally, if
 * json_encoder_access_buffer() is used to obtain a read/write pointer to allow additional data to be written at the end of the
 * JSON string, this function can be used to ensure that there is space for this data.
 *
 * @note This call is not applicable on an encoder instance obtained from json_decoder_get_encoder(), since JSON items encoded with
 * these encoders are added directly to the decoder tree rather than into a JSON formatted buffer.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] length  The number of bytes of free space required.
 *
 * @retval #JSON_ENCODER_OK            Success.
 * @retval #JSON_ENCODER_NO_MEMORY     There is not enough memory to perform the operation.
 * @retval #JSON_ENCODER_NOT_SUPPORTED The encoder instance was obtained from json_decoder_get_encoder().
*/
json_encoder_error_t json_encoder_reserve_space(json_encoder_t *encoder, int length);

/**
 * Get a pointer to the JSON encoder's data buffer.
 *
 * Returns a pointer to the JSON encoder's buffer holding the encoded data. This returns a valid pointer if there have been no
 * errors and all objects have been properly ended. The pointer returned is not valid after any subsequent @c json_encoder_ calls.
 *
 * @note This call is not applicable on an encoder instance obtained from json_decoder_get_encoder(), since JSON items encoded with
 * these encoders are added directly to the decoder tree rather than into a JSON formatted buffer.
 *
 * @param[in] encoder The JSON encoder instance.
 *
 * @return      The pointer to the encoded buffer.
 * @retval NULL An error status exists in the encoder, the encoded data is incomplete, or the encoder instance was obtained from
 *              json_decoder_get_encoder().
 *
 * @see json_encoder_get_status
 */
const char *json_encoder_buffer(const json_encoder_t *encoder);

/**
 * Get a r/w pointer to the JSON encoder's data buffer.
 *
 * Returns a r/w pointer to the buffer holding the encoded data. This returns a valid pointer if there have been no errors and all
 * objects have been properly ended. The pointer returned is not valid after any subsequent @c json_encoder_ calls.
 *
 * @note This call is not applicable on an encoder instance obtained from json_decoder_get_encoder(), since JSON items encoded with
 * these encoders are added directly to the decoder tree rather than into a JSON formatted buffer.
 *
 * @param[in] encoder The JSON encoder instance.
 *
 * @return      The pointer to the encoded buffer.
 * @retval NULL An error status exists in the encoder, the encoded data is incomplete, or the encoder instance was obtained from
 *              json_decoder_get_encoder().
 *
 * @see json_encoder_get_status
 */
char *json_encoder_access_buffer(json_encoder_t *encoder);

/**
 * Name a property in the JSON encoder.
 *
 * This function starts a new property with the given name in the current object or array in the JSON encoder. The value for the
 * property is provided in a subsequent @c json_encoder_add_ or @c json_encoder_start_ call with @c NULL passed in their
 * corresponding @a name parameter.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] name    The new property's name.
 *
 * @retval #JSON_ENCODER_OK                    Success.
 * @retval #JSON_ENCODER_NO_MEMORY             There is not enough memory to perform the operation.
 * @retval #JSON_ENCODER_MISSING_PROPERTY_NAME No name was provided for the property.
 *
 * @see json_encoder_error_t & json_encoder_get_status
 */
json_encoder_error_t json_encoder_add_property_name(json_encoder_t *encoder, const char *name);

/**
 * Start a new object in the JSON encoder.
 *
 * Starts encoding a new object with the given name in the current object or array in the JSON encoder. Properties that are
 * subsequently encoded by the encoder will be added to this object until the matching json_encoder_end_object() call.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] name    The name of the object if being started within an object, or @c NULL if the object is being started within an
 *                    array or if it was already named previously with json_encoder_add_property_name().
 *
 * @retval #JSON_ENCODER_OK                    Success.
 * @retval #JSON_ENCODER_NO_MEMORY             There is not enough memory to perform the operation.
 * @retval #JSON_ENCODER_UNEXPECTED_NAME       A name was provided for an item which has already been named, or for an element in an
 *                                             array which can't accept a name.
 * @retval #JSON_ENCODER_MISSING_PROPERTY_NAME No name was provided for the property.
 *
 * @see json_encoder_error_t, json_encoder_end_object & json_encoder_get_status
 */
json_encoder_error_t json_encoder_start_object(json_encoder_t *encoder, const char *name);

/**
 * End the current object in the JSON encoder.
 *
 * Ends the current object in the JSON encoder, returning the nesting level of the encoder to the object's parent.
 *
 * @param[in] encoder The JSON encoder instance.
 *
 * @retval #JSON_ENCODER_OK          Success.
 * @retval #JSON_ENCODER_NO_MEMORY   There is not enough memory to perform the operation.
 * @retval #JSON_ENCODER_BAD_NESTING The encoder was not previously encoding an object.
 *
 * @see json_encoder_error_t, json_encoder_start_object & json_encoder_get_status
 */
json_encoder_error_t json_encoder_end_object(json_encoder_t *encoder);

/**
 * Start a new array in the JSON encoder.
 *
 * Starts encoding a new array with the given name in the current object or array in the JSON encoder. Elements that are
 * subsequently encoded by the encoder will be added to this array until the matching json_encoder_end_array() call.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] name    The name of the array if being started within an object, or @c NULL if the array is being started within an
 *                    array or if it was already named previously with json_encoder_add_property_name().
 *
 * @retval #JSON_ENCODER_OK                    Success.
 * @retval #JSON_ENCODER_NO_MEMORY             There is not enough memory to perform the operation.
 * @retval #JSON_ENCODER_UNEXPECTED_NAME       A name was provided for an item which has already been named, or for an element in an
 *                                             array which can't accept a name.
 * @retval #JSON_ENCODER_MISSING_PROPERTY_NAME No name was provided for the property.
 *
 * @see json_encoder_error_t, json_encoder_end_array & json_encoder_get_status
 */
json_encoder_error_t json_encoder_start_array(json_encoder_t *encoder, const char *name);

/**
 * End the current array in the JSON encoder.
 *
 * Ends the current array in the JSON encoder, returning the nesting level of the encoder to the array's parent.
 *
 * @param[in] encoder The JSON encoder instance.
 *
 * @retval #JSON_ENCODER_OK          Success.
 * @retval #JSON_ENCODER_NO_MEMORY   There is not enough memory to perform the operation.
 * @retval #JSON_ENCODER_BAD_NESTING The encoder was not previously encoding an array.
 *
 * @see json_encoder_error_t, json_encoder_start_array & json_encoder_get_status
 */
json_encoder_error_t json_encoder_end_array(json_encoder_t *encoder);

/**
 * Add a null value to the JSON encoder.
 *
 * Adds a null value with the given name to the current object or array in the JSON encoder.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] name    The name of the value if being added to an object, or @c NULL if the value is being added to an array or if it
 *                    was already named previously with json_encoder_add_property_name().
 *
 * @retval #JSON_ENCODER_OK                    Success.
 * @retval #JSON_ENCODER_NO_MEMORY             There is not enough memory to perform the operation.
 * @retval #JSON_ENCODER_UNEXPECTED_NAME       A name was provided for an item which has already been named, or for an element in an
 *                                             array which can't accept a name.
 * @retval #JSON_ENCODER_MISSING_PROPERTY_NAME No name was provided for the property.
 *
 * @see json_encoder_error_t & json_encoder_get_status
 */
json_encoder_error_t json_encoder_add_null(json_encoder_t *encoder, const char *name);

/**
 * Add a boolean value to the JSON encoder.
 *
 * Adds a boolean value with the given name to the current object or array in the JSON encoder.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] name    The name of the value if being added to an object, or @c NULL if the value is being added to an array or if it
 *                    was already named previously with json_encoder_add_property_name().
 * @param[in] value   The @c bool value being encoded.
 *
 * @retval #JSON_ENCODER_OK                    Success.
 * @retval #JSON_ENCODER_NO_MEMORY             There is not enough memory to perform the operation.
 * @retval #JSON_ENCODER_UNEXPECTED_NAME       A name was provided for an item which has already been named, or for an element in an
 *                                             array which can't accept a name.
 * @retval #JSON_ENCODER_MISSING_PROPERTY_NAME No name was provided for the property.
 *
 * @see json_encoder_error_t & json_encoder_get_status
 */
json_encoder_error_t json_encoder_add_bool(json_encoder_t *encoder, const char *name, bool value);

/**
 * Add an integer value to the JSON encoder.
 *
 * Adds an @c int value with the given name to the current object or array in the JSON encoder.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] name    The name of the value if being added to an object, or @c NULL if the value is being added to an array or if it
 *                    was already named previously with json_encoder_add_property_name().
 * @param[in] value   The @c int value being encoded.
 *
 * @retval #JSON_ENCODER_OK                    Success.
 * @retval #JSON_ENCODER_NO_MEMORY             There is not enough memory to perform the operation.
 * @retval #JSON_ENCODER_UNEXPECTED_NAME       A name was provided for an item which has already been named, or for an element in an
 *                                             array which can't accept a name.
 * @retval #JSON_ENCODER_MISSING_PROPERTY_NAME No name was provided for the property.
 *
 * @see json_encoder_error_t & json_encoder_get_status
 */
json_encoder_error_t json_encoder_add_int(json_encoder_t *encoder, const char *name, int value);

/**
 * Add a <tt>long long</tt> integer value to the JSON encoder.
 *
 * Adds a <tt>long long int</tt> value with the given name to the current object or array in the JSON encoder.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] name    The name of the value if being added to an object, or @c NULL if the value is being added to an array or if it
 *                    was already named previously with json_encoder_add_property_name().
 * @param[in] value   The <tt>long long int</tt> value being encoded.
 *
 * @retval #JSON_ENCODER_OK                    Success.
 * @retval #JSON_ENCODER_NO_MEMORY             There is not enough memory to perform the operation.
 * @retval #JSON_ENCODER_UNEXPECTED_NAME       A name was provided for an item which has already been named, or for an element in an
 *                                             array which can't accept a name.
 * @retval #JSON_ENCODER_MISSING_PROPERTY_NAME No name was provided for the property.
 *
 * @see json_encoder_error_t & json_encoder_get_status
 */
json_encoder_error_t json_encoder_add_int_ll(json_encoder_t *encoder, const char *name, long long value);

/**
 * Add a double value to the JSON encoder.
 *
 * Adds a @c double value with the given name to the current object or array in the JSON encoder.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] name    The name of the value if being added to an object, or @c NULL if the value is being added to an array or if it
 *                    was already named previously with json_encoder_add_property_name().
 * @param[in] value   The @c double value being encoded.
 *
 * @retval #JSON_ENCODER_OK                    Success.
 * @retval #JSON_ENCODER_NO_MEMORY             There is not enough memory to perform the operation.
 * @retval #JSON_ENCODER_MISSING_PROPERTY_NAME No name was provided for the property.
 * @retval #JSON_ENCODER_UNEXPECTED_NAME       A name was provided for an item which has already been named, or for an element in an
 *                                             array which can't accept a name.
 * @retval #JSON_ENCODER_INVALID_VALUE         An invalid @c double was passed in @a value.
 *
 * @see json_encoder_error_t & json_encoder_get_status
 */
json_encoder_error_t json_encoder_add_double(json_encoder_t *encoder, const char *name, double value);

/**
 * Add a string value to the JSON encoder.
 *
 * Adds a string value with the given name to the current object or array in the JSON encoder.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] name    The name of the value if being added to an object, or @c NULL if the value is being added to an array or if it
 *                    was already named previously with json_encoder_add_property_name().
 * @param[in] value   A pointer to the string value being encoded.
 *
 * @retval #JSON_ENCODER_OK                    Success.
 * @retval #JSON_ENCODER_NO_MEMORY             There is not enough memory to perform the operation.
 * @retval #JSON_ENCODER_UNEXPECTED_NAME       A name was provided for an item which has already been named, or for an element in an
 *                                             array which can't accept a name.
 * @retval #JSON_ENCODER_MISSING_PROPERTY_NAME No name was provided for the property.
 *
 * @see json_encoder_error_t & json_encoder_get_status
 */
json_encoder_error_t json_encoder_add_string(json_encoder_t *encoder, const char *name, const char *value);

/**
 * Add a JSON encoded string value to the JSON encoder.
 *
 * Adds a JSON encoded string value with the given name to the current object or array in the JSON encoder.
 *
 * @note This call is not applicable on an encoder instance obtained from json_decoder_get_encoder(), since JSON items encoded with
 * these encoders are added directly to the decoder tree rather than into a JSON formatted buffer.
 *
 * @param[in] encoder The JSON encoder instance.
 * @param[in] name    The name of the value if being added to an object, or @c NULL if the value is being added to an array or if it
 *                    was already named previously with json_encoder_add_property_name().
 * @param[in] value   A pointer to the JSON string being added.
 *
 * @retval #JSON_ENCODER_OK                    Success.
 * @retval #JSON_ENCODER_NO_MEMORY             There is not enough memory to perform the operation.
 * @retval #JSON_ENCODER_UNEXPECTED_NAME       A name was provided for an item which has already been named, or for an element in an
 *                                             array which can't accept a name.
 * @retval #JSON_ENCODER_MISSING_PROPERTY_NAME No name was provided for the property.
 * @retval #JSON_ENCODER_NOT_SUPPORTED         Operation not supported with the given encoder instance.
 *
 * @see json_encoder_error_t & json_encoder_get_status
 */
json_encoder_error_t json_encoder_add_json(json_encoder_t *encoder, const char *name, const char *value);

/**
 * Add elements from a JSON decoder to the JSON encoder.
 *
 * Adds part or all of the elements from a JSON decoder with the given name to the current object or array in the JSON encoder. This
 * allows data to be re-encoded that was previously decoded. Upon completion of the call the decoder is left in the same state as at
 * the time of the call.
 *
 * @param[in] encoder   The JSON encoder instance.
 * @param[in] name      The name of the value if being added to an object, or @c NULL if the value is being added to an array or if
 *                      it was already named previously with json_encoder_add_property_name().
 * @param[in] decoder   The JSON decoder positioned at the desired node.
 * @param[in] prop_name The name of the decoder property to add from, or @c NULL to add the node at the current decoder position.
 *
 * @retval #JSON_ENCODER_OK                    Success.
 * @retval #JSON_ENCODER_NO_MEMORY             There is not enough memory to perform the operation.
 * @retval #JSON_ENCODER_UNEXPECTED_NAME       A name was provided for an item which has already been named, or for an element in an
 *                                             array which can't accept a name.
 * @retval #JSON_ENCODER_MISSING_PROPERTY_NAME No name was provided for the property.
 * @retval #JSON_ENCODER_NOT_FOUND             Element in the decoder does not exist.
 * @retval #JSON_ENCODER_INVALID_VALUE         Element value in the decoder is not valid.
 *
 * @see json_encoder_error_t & json_encoder_get_status
 */
json_encoder_error_t json_encoder_add_from_decoder(json_encoder_t *encoder, const char *name, json_decoder_t *decoder,
                                                   const char *prop_name);


/**
 * Writes JSON formatted output to a character array up to a given number of
 * characters. If the buffer length is at least one, the buffer will be null
 * terminated.
 *
 * The format string is expected to be valid JSON with the following exceptions:
 *   + String, Boolean and numeric values can formatted from additional arguments
 *     following the format string much like @c printf. Conversion specifiers
 *     are similar to printf with the following differences:
 *      - Field width is not supported.
 *      - The only type length specifier supported is ll (double "el") to indicate
 *        a long long or unsigned long long.
 *      - The printf conversion type specifiers supported are 'd', 'i', 'e', 'E', 'f',
 *        'F', 'g', 'G', 's' and 'u'.
 *      - A 'b' conversion type specifier can be used to indicate that
 *        an int argument should be formatted as a Boolean value.
 *      - A 'j' conversion type specifier can be used to indicate that a string
 *        argument contains either a single pre-formatted JSON-like value or a comma
 *        separated list of JSON-like values. The rules for what constitutes a JSON-like
 *        value depends on the context in which it is used. If used within an object,
 *        for example { %j, %j }, the JSON-like value or each JSON-like value in a list must
 *        be in the form "name":value when @c value is a JSON value. When used outside of
 *        an object, each value or value in a list must be valid JSON.
 *   + Property names can be specified as '%s' in which case the property name
 *     is taken from the next function argument.
 *   + Property names can be contained within the format string without being
 *     enclosed in double quotes; the output string will have the double quotes
 *     inserted.
 *
 * When an error is detected within the format string that leads to an invalid
 * JSON string, a question mark will be inserted into the output string and the
 * function will return -1. Not all errors are detected.
 *
 * @param[in,out] buffer  A pointer to a buffer to store the formatted string.
 * @param[in]     len     The length of the buffer.
 * @param[in]     format  A string that specifies the format of the output.
 *
 * @return Returns the number of characters that were written to the output
 *         buffer or the number that would have been written had it been large
 *         enough not including the terminating nul character.
 * @retval -1  The format string was invalid.
 */
int jsnprintf(char *buffer, int len, const char *format, ...);


/**
 * Writes JSON formatted output to a character array up to a given number of
 * characters. If the buffer length is at least one, the buffer will be nul
 * terminated.
 *
 * The vjsnprintf() function is a “varargs” version of jsnprintf().
 *
 * @param[in,out] buffer  A pointer to a buffer to store the formatted string.
 * @param[in]     len     The length of the buffer.
 * @param[in]     format  A string that specifies the format of the output.
 * @param[in]     args    A variable-argument list of the additional arguments, which you must have
 *                        initialized with the @c va_start() macro.
 *
 * @return Returns the number of characters that were written to the output
 *         buffer or the number that would have been written had it been large
 *         enough not including the terminating nul character.
 * @retval -1  The format string was invalid.
 *
 * @see jsnprintf
 */
int vjsnprintf(char *buffer, int len, const char *format, va_list args);


/** @cond */
__END_DECLS
/** @endcond */

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.0.0/trunk/lib/json/public/sys/json.h $ $Rev: 816646 $")
#endif
