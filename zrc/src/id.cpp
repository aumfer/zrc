#include <id.hpp>
#include <Windows.h>

void id_create(id_t *id) {
	GUID *pguid = (GUID *)id;
	CoCreateGuid(pguid);
}