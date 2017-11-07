/* pastree - serialize/deserialize papyrus assembly
 * Copyright (C) 2017  Oskar Sveinsen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

extern const char
	*pastree_type_none,
	*pastree_type_bool,
	*pastree_type_float,
	*pastree_type_int,
	*pastree_type_var;

typedef struct {
	const char *source;
	time_t modifytime;
	time_t compiletime;
	const char *user;
	const char *computer;
} Pastree_info;

typedef uint_least32_t Pastree_userflags;

typedef union {
	int_least32_t i;
	float f;
	const char *s;
} Pastree_value;

typedef struct {
	const char *name;
	const char *type;
	uint_least32_t userflags;
	Pastree_value initialvalue;
} Pastree_variable;

typedef enum {
	PASTREE_MOD_NATIVE = 1,
	PASTREE_MOD_STATIC = 2,
	PASTREE_MOD_CONST = 1,
	PASTREE_MOD_AUTOREADONLY = 1,
	PASTREE_MOD_AUTO = 2,
} Pastree_modifier;

typedef struct {
	const char *name;
	const char *type;
	Pastree_modifier mod;
	uint_least32_t userflags;
	const char *docstring;
	Pastree_variable *autovar;
} Pastree_property;

typedef struct {
	const char *name;
	const char *type;
} Pastree_local;

typedef struct {
	const char *name;
	Pastree_modifier mod;
	uint_least32_t userflags;
	const char *docstring;
	const char *ret;
	size_t paramcount;
	Pastree_local **paramtable;
	size_t localcount;
	Pastree_local **localtable;
	size_t codesize;
	Pastree_code *code;
} Pastree_function;

typedef struct {
	const char *name;
	size_t functioncount;
	Pastree_function **functiontable;
} Pastree_state;

typedef struct {
	const char *name;
	const char *parent;
	Pastree_modifier mod;
	const char *docstring;
	size_t autostate;
	size_t variablecount;
	Pastree_variable **variabletable;
	size_t propertycount;
	Pastree_property **propertytable;
	size_t statecount;
	Pastree_state **statetable;
} Pastree_object;

typedef struct {
	Pastree_info info;
	const char *userflagsref[32];
	size_t objectcount;
	Pastree_object **objecttable;
} Pastree;

void serialize(FILE *, Pastree *);
Pastree *deserialize(FILE *);
