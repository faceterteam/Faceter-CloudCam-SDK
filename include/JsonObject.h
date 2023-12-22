#ifndef JSONOBJECT_H
#define JSONOBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct JsonObject JsonObject;

JsonObject* JsonObjectCreateEmpty(void);

JsonObject* JsonObjectCreateAndParse(const char* jsonString);

void JsonObjectDestroy(JsonObject* json);

int JsonObjectHasField(JsonObject* json, const char* key);

void JsonObjectDeleteField(JsonObject* json, const char* key);

void JsonObjectSetStringField(JsonObject* json, const char* key, const char* value);

const char* JsonObjectGetStringField(JsonObject* json, const char* key, const char* defaultValue);

void JsonObjectSetInt64Field(JsonObject* json, const char* key, int64_t value);

int64_t JsonObjectGetInt64Field(JsonObject* json, const char* key, int64_t defaultValue);

void JsonObjectSetDoubleField(JsonObject* json, const char* key, double value);

double JsonObjectGetDoubleField(JsonObject* json, const char* key, double defaultValue);

void JsonObjectSetBooleanField(JsonObject* json, const char* key, int value);

int JsonObjectGetBooleanField(JsonObject* json, const char* key, int defaultValue);

const char* JsonObjectToString(JsonObject* json, int prettyFormat);

#ifdef __cplusplus
}
#endif

#endif