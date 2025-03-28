/** \ingroup Datahandling
 * @file
 * @author  David Llewellyn-Jones <David.Llewellyn-Jones@cl.cam.ac.uk>
 * @version $(VERSION)
 *
 * @section LICENSE
 *
 * (C) Copyright Cambridge Authentication Ltd, 2017
 *
 * This file is part of libpico.
 *
 * Libpico is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Libpico is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with libpico. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 *
 * @brief Serialise and deserialies data to and from JSON and data structures
 * @section DESCRIPTION
 *
 * The json class provides serializing and deserializing for converting 
 * data in key-value pairs between json text and a linked list.
 * At present strings, integers and nested json structures are supported
 * but arrays are not supported (they're not needed by the Pico protocol).
 *
 */

/** \addtogroup Datahandling
 *  @{
 */

#ifndef __JSON_H
#define __JSON_H (1)

#include "buffer.h"

// Defines

// Structure definitions

/**
 * @brief Enumerates the allowed data-types for JSON data
 *
 * JSON structures can store string, decimal, sub-structure and array types.
 * The JSON interface supports the retrieval of key-value pairs, so it's
 * often necessary to be able to query or set the type of data to be used.
 * This enum defines the enumerated values to use for these queries.
 *
 * Note that at present array types are not supported (and so don't appear in
 * the enumerate).
 */
typedef enum _JSONTYPE {
	JSONTYPE_INVALID = -1,

	JSONTYPE_STRING,
	JSONTYPE_DECIMAL,
	JSONTYPE_INTEGER,
	JSONTYPE_BOOL,
	JSONTYPE_NULL,
	JSONTYPE_LIST,
	JSONTYPE_DICT,

	JSONTYPE_NUM
} JSONTYPE;

/**
 * The internal structure can be found in json.c
 */
typedef struct _Json Json;

typedef struct _JsonList JsonList;

// Function prototypes

Json * json_new();
void json_delete(Json * json);
void json_clear(Json * json);

void json_add_string(Json * json, char const * key, char const * value);
void json_add_buffer(Json * json, char const * key, Buffer const * value);
void json_add_decimal(Json * json, char const * key, double value);
void json_add_integer(Json * json, char const * key, long long int value);
void json_add_bool(Json * json, char const * key, bool value);
void json_add_null(Json * json, char const * key);
void json_add_dict(Json * json, char const * key, Json * dict);
void json_add_list(Json * json, char const * key, JsonList * list);

size_t json_serialize_size(Json * json);
size_t json_serialize(Json * json, char * buffer, size_t size);
size_t json_serialize_buffer(Json * json, Buffer * buffer);
bool json_deserialize_string(Json * json, char const * json_string, size_t length);
bool json_deserialize_buffer(Json * json, Buffer const * buffer);
void json_print(Json * json);
void json_log(Json * json);

JSONTYPE json_get_type(Json * json, char const * key);
char const * json_get_string(Json * json, char const * key);
double json_get_decimal(Json * json, char const * key);
long long int json_get_integer(Json * json, char const * key);
double json_get_number(Json * json, char const * key);
bool json_get_bool(Json * json, char const * key);
Json * json_get_dict(Json * json, char const * key);
JsonList * json_get_list(Json * json, char const * key);

JsonList * jsonlist_new();
void jsonlist_delete(JsonList * jsonlist);
void jsonlist_clear(JsonList * jsonlist);

void jsonlist_push_string(JsonList * jsonlist, char const * value);
void jsonlist_push_buffer(JsonList * jsonlist, Buffer const * value);
void jsonlist_push_decimal(JsonList * jsonlist, double value);
void jsonlist_push_integer(JsonList * jsonlist, long long int value);
void jsonlist_push_bool(JsonList * jsonlist, bool value);
void jsonlist_push_null(JsonList * jsonlist);
void jsonlist_push_dict(JsonList * jsonlist, Json * value);
void jsonlist_push_list(JsonList * jsonlist, JsonList * value);

size_t jsonlist_get_len(JsonList * jsonlist);
JSONTYPE jsonlist_get_type(JsonList * jsonlist, size_t index);
char const * jsonlist_get_string(JsonList * jsonlist, size_t index);
double jsonlist_get_decimal(JsonList * jsonlist, size_t index);
long long int jsonlist_get_integer(JsonList * jsonlist, size_t index);
double jsonlist_get_number(JsonList * jsonlist, size_t index);
bool jsonlist_get_bool(JsonList * jsonlist, size_t index);
Json * jsonlist_get_dict(JsonList * jsonlist, size_t index);
JsonList * jsonlist_get_list(JsonList * jsonlist, size_t index);

// Function definitions

#endif

/** @} addtogroup Datahandling */
