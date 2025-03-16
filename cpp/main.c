#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"

#include "appwrapper.h"

int main(int argc, char **argv) {
    graal_isolate_t *isolate = NULL;
    graal_isolatethread_t *thread = NULL;
    
    if (graal_create_isolate(NULL, &isolate, &thread) != 0) {
        fprintf(stderr, "initialization error\n");
        return 1;
    }
    
    init(thread);
    
    Buffer *buffer = buffer_new(1024);
    Json *json;

    json = json_new();

    // First call
    json_add_string(json, "name", "World");
    json_serialize_buffer(json, buffer);

    char * result = invoke(thread, "helloWorld", buffer_get_buffer(buffer));

    bool success = json_deserialize_string(json, result, strlen(result));
    if (success) {
        char const * name = json_get_string(json, "result");
        printf("Result: %s\n", name);
    }
    else {
        printf("Json deserialization filed: %s\n", result);
    }

    // Second call
    json_delete(json);
    json = json_new();

    json_add_string(json, "result", "World");
    json_serialize_buffer(json, buffer);

    result = invoke(thread, "worldHello", buffer_get_buffer(buffer));

    success = json_deserialize_string(json, result, strlen(result));
    if (success) {
        char const * name = json_get_string(json, "name");
        printf("Result: %s\n", name);
    }
    else {
        printf("Json deserialization filed: %s\n", result);
    }

    json_delete(json);
    buffer_delete(buffer);    

    graal_tear_down_isolate(thread);
    return 0;
}

