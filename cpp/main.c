#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"

#include "appwrapper.h"

void printExtracted(char* response) {
    Json* json;
    json = json_new();

    bool success = json_deserialize_string(json, response, strlen(response));
    if (success) {
        char const* string;
        long long int number;

        string = json_get_string(json, "name");
        printf("Name: %s\n", string);

        string = json_get_string(json, "uploaderName");
        printf("Uploader name: %s\n", string);

        string = json_get_string(json, "category");
        printf("Category: %s\n", string);

        number = json_get_integer(json, "likeCount");
        printf("Like count: %lld\n", number);

        number = json_get_integer(json, "viewCount");
        printf("View count: %lld\n", number);

        string = json_get_string(json, "content");
        printf("Content: %s\n", string);
    }
    else {
        printf("Json deserialization failed: %s\n", response);
    }

    json_delete(json);
}

int main(int argc, char** argv) {
    graal_isolate_t* isolate = NULL;
    graal_isolatethread_t* thread = NULL;
    
    if (graal_create_isolate(NULL, &isolate, &thread) != 0) {
        fprintf(stderr, "initialization error\n");
        return 1;
    }
    
    init(thread);
    
    Buffer* buffer = buffer_new(1024);
    Json* json;
    char* result;

    json = json_new();

    // First call
    json_add_string(json, "service", "YouTube");
    json_add_string(json, "url", "https://www.youtube.com/watch?v=NWqC9cORKi0");
    json_serialize_buffer(json, buffer);

    result = invoke(thread, "downloadExtract", buffer_get_buffer(buffer));
    printExtracted(result);

    json_delete(json);
    json = json_new();

    // Second call
    json_add_string(json, "service", "SoundCloud");
    json_add_string(json, "url", "https://soundcloud.com/highviberecordsca/matt-williams-sailfish");
    json_serialize_buffer(json, buffer);

    result = invoke(thread, "downloadExtract", buffer_get_buffer(buffer));
    printExtracted(result);

    json_delete(json);
    json = json_new();

    // Third call
    json_add_string(json, "service", "YouTube");
    json_add_string(json, "query", "Sailfis");
    json_serialize_buffer(json, buffer);

    result = invoke(thread, "getSuggestions", buffer_get_buffer(buffer));
    printf("Suggestion results: \n%s\n", result);

    json_delete(json);
    buffer_delete(buffer);    

    result = invoke(thread, "tearDown", "{}");

    graal_tear_down_isolate(thread);
    return 0;
}

