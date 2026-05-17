#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/entity.h"

/* =========================================================
 * entity.c
 * Implementarea ciclului de viață al entității și a funcțiilor
 * de conversie.
 * ========================================================= */

/* ----------------------------------------------------------
 * Funcții de conversie
 * ---------------------------------------------------------- */

const char *entity_type_to_str(EntityType type)
{
    switch(type)
    {
        case 0:
            return "PERSON";
        case 1:
            return "COMPANY";
        case 2:
            return "LOCATION";
        case 3:
            return "EVENT";
        default:
            return "UNKNOWN";
    }
}

EntityType str_to_entity_type(const char *str)
{
    if(str == NULL)
        return (EntityType)-1;

    if(strcmp(str,"PERSON") == 0)
        return PERSON;
    if(strcmp(str,"LOCATION") == 0)
        return LOCATION;
    if(strcmp(str,"COMPANY") == 0)
        return COMPANY;
    if(strcmp(str,"EVENT") == 0)
        return EVENT;

    return (EntityType)-1;
}

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

Entity *entity_create(const char *name, EntityType type, int id)
{
    if(name == NULL)
        return NULL;

    Entity *e = malloc(sizeof(Entity));
    if(e == NULL)
        return NULL;

    e->name = malloc(strlen(name) + 1);
    if(e->name == NULL)
    {
        free(e);
        return NULL;
    }
    strcpy(e->name,name);
    e->type = type;
    e->id = id;

    return e;
}

void entity_free(Entity *e)
{
    if(e == NULL)
        return;
    free(e->name);
    free(e);
}

/* ----------------------------------------------------------
 * Afișare
 * ---------------------------------------------------------- */

void entity_print(const Entity *e)
{
    if(e == NULL)
        return;

    printf("%d %s %s\n",e->id,e->name,entity_type_to_str(e->type));
}
