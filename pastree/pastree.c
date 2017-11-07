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

#include <stdint.h>
#include <stdio.h>
#include <time.h>

const char
	*pastree_type_none = "none",
	*pastree_type_bool = "bool",
	*pastree_type_float = "float",
	*pastree_type_int = "int",
	*pastree_type_string = "string";
	*pastree_type_var = "var";

void
serialize(FILE *stream, Pastree *pastree)
{
	size_t i1;

	if (pastree->info) {
		Pastree_info *info;

		info = &pastree->info
		fprintf(stream,
			".info\n"
			"\t.source \"%s\"\n"
			"\t.modifyTime %llu\n"
			"\t.compileTime %llu\n"
			"\t.user \"%s\"\n"
			"\t.computer \"%s\"\n"
			".endInfo\n",
			info->source,
			info->modifyTime,
			info->compileTime,
			info->user,
			info->computer
		);
	}

	fputs(stream, ".userFlagsRef\n");
	for (i1 = 0; i1 < sizeof(pastree->userflagsref); i1++)
		fprintf(stream, "\t.flag %s %u\n", pastree->userflagsref[i1], i1);
	fputs(stream, ".endUserFlagsRef\n");

	for (i1 = 0; i1 < pastree->objectcount; i1++) {
		size_t i2;
		Pastree_object *object;

		object = pastree->objectcount[i1];
		fputs("\t.object ", stream);
		fputs(object->name, stream);
		if (object->parent) {
			putc(' ', stream);
			fputs(object->parent, stream);
		}
		if (object->mod & PASTREE_MOD_NATIVE)
			fputs(" native", stream);

		fprintf(stream, "\n"
			"\t\t.userFlags %u\n"
			"\t\t.docString \"%s\"\n"
			"\t\t.autoState %s\n"
			"\t\t.variableTable\n",
			object->userflags,
			object->docstring,
			object->autostate
		);

		for (i2 = 0; i2 < object->variablecount; i2++) {
			Pastree_variable *variable;

			variable = object->variabletable[i2];
			fprintf(stream,
				"\t\t\t.variable %s %s\n"
				"\t\t\t\t.userFlags %u\n"
				"\t\t\t\t.docString \"%s\"\n"
				"\t\t\t.endVariable\n",
				variable->name, variable->type,
				variable->userflags,
				variable->docstring
			);
		}
		fputs("\t\t.endVariableTable\n\t\t.propertyTable\n", stream);
		for (i2 = 0; i2 < object->propertycount; i2++) {
			Pastree_property *property;

			property = object->propertytable[i2];
			fprintf(stream, "\t\t\t.property %s %s", property->name, property->type);
			if (property->mod & PASTREE_MOD_AUTO) {
				fputs(" auto", stream);
				if (property->mod & PASTREE_MOD_CONST)
					fputs(" const", stream);
			} else if (property->mod & PASTREE_MOD_AUTOREADONLY) {
				fputs(" autoreadonly");
			}
			fprintf(stream, "\n"
				"\t\t\t\t.userFlags %u\n"
				"\t\t\t\t.docString \"%s\"\n"
				"\t\t\t\t.autoVar %s\n"
				"\t\t\t.endProperty\n",
				property->userflags,
				property->docstring,
				property->autovar->name
			);
		}
		fputs("\t\t.endPropertyTable\n\t\t.stateTable\n", stream);
		for (i2 = 0; i2 < object->statetable; i2++) {
			size_t i3;
			Pastree_state *state;

			state = object->statetable[i2];
			fputs("\t\t\t.state", stream);
			if (state->name) {
				putc(' ', stream);
				fputs(state->name, stream);
			}
			putc('\n', stream);
			for (i3 = 0; i3 < state->functioncount; i3++) {
				size_t i4;
				Pastree_function *function;

				function = state->functiontable[i3];
				fputs("\t\t\t\t.function ", stream);
				fputs(function->name, stream);
				if (function->mod & PASTREE_MOD_NATIVE)
					fputs(" native");
				if (function->mod & PASTREE_MOD_STATIC)
					fputs(" static");
				fprintf(stream, "\n"
					"\t\t\t\t\t.userFlags %u\n"
					"\t\t\t\t\t.docString \"%s\"\n"
					"\t\t\t\t\t.return %s\n"
					"\t\t\t\t\t.paramTable\n",
					function->userflags,
					function->docstring,
					function->ret
				);
				for (i4 = 0; i4 < function->paramcount; i4++) {
					Pastree_local *param;

					param = function->paramtable[i4];
					fprintf(stream, "\t\t\t\t\t\t.param %s %s", param->name, param->type);
				}
				fputs("\t\t\t\t\t.endParamTable\n\t\t\t\t\t.localTable", stream);
				for (i4 = 0; i4 < function->localcount; i4++) {
					Pastree_local *local;

					local = function->localtable[i4];
					fprintf(stream, "\t\t\t\t\t\t.local %s %s", local->name, local->type);
				}
				fputs("\t\t\t\t\t.endLocalTable\n\t\t\t\t\t.code", stream);
				//TODO: serialize code
				fputs("\t\t\t\t\t.endCode\n\t\t\t\t.endFunction\n\t\t\t.endState\n", stream);
			}
			fputs("\t\t.endStateTable\n", stream);
		}
		fputs("\t.endObject\n", stream);
	}
	fputs(".endObjectTable\n", stream);
}

//TODO
Pastree
*deserialize(FILE *stream)
{
	return NULL;
}
