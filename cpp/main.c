#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "json.h"

#include "appwrapper.h"

Json* extract_next_page(Json* json) {
    Json* parsePage;
    Json* nextPage;

    parsePage = json_get_dict(json, "nextPage");
    nextPage = json_new();
    json_add_string(nextPage, "url", json_get_string(parsePage, "url"));
    json_add_string(nextPage, "id", json_get_string(parsePage, "id"));
    json_add_list(nextPage, "ids", jsonlist_new());
    json_add_dict(nextPage, "cookies", json_new());
    json_add_string(nextPage, "body", "");
    return nextPage;
}

size_t display_search_results(Json* json, char const* const key, size_t count) {
    JsonList* list;

    list = json_get_list(json, key);

    size_t length = jsonlist_get_len(list);
    size_t index;
    for (index = 0; index < length; ++index) {
        Json* item = jsonlist_get_dict(list, index);
        char const* name = json_get_string(item, "name");
        count++;
        printf("Result %lu: %s\n", count, name);
    }

    return length;
}

void create_search_structure(Json* json, char const* const service, char const* const searchString, Json* nextPage) {
    JsonList* list;

    json_clear(json);
    json_add_string(json, "service", service);
    json_add_string(json, "searchString", searchString);

    list = jsonlist_new();
    jsonlist_push_string(list, "all");
    json_add_list(json, "contentFilter", list);

    json_add_string(json, "sortFilter", "");

    if (nextPage) {
        json_add_dict(json, "page", nextPage);
    }
}

void search(graal_isolatethread_t* thread, char const* const service) {
    char * input;
    char * searchString;
    Json* json;
    Buffer* buffer;
    char* result;
    Json* nextPage;
    size_t count;

    searchString = readline("Please enter a search phrase (or leave blank to return): ");
    printf("Searching for: \"%s\"\n", searchString);

    if (searchString[0] != 0) {
        json = json_new();
        buffer = buffer_new(1024);

        // Populate the JSON input parameter structure
        create_search_structure(json, service, searchString, NULL);
        json_serialize_buffer(json, buffer);

        // Call the Extractor
        result = invoke(thread, "searchFor", buffer_get_buffer(buffer));

        json_clear(json);
        json_deserialize_string(json, result, strlen(result));
        nextPage = extract_next_page(json);

        // Display the results
        count = 0;
        count += display_search_results(json, "relatedItems", 0);

        printf("\n");
        input = readline("Enter \"c\" to continue or leave blank to return: ");
        while (strcmp(input, "c") == 0) {
            printf("Continue searching for: \"%s\"\n", searchString);

            // Populate the JSON input parameter structure
            create_search_structure(json, service, searchString, nextPage);
            json_serialize_buffer(json, buffer);

            // Call the Extractor
            result = invoke(thread, "getMoreSearchItems", buffer_get_buffer(buffer));

            // The following will delete nextPage along with json
            json_clear(json);
            json_deserialize_string(json, result, strlen(result));
            nextPage = extract_next_page(json);

            // Display the results
            count += display_search_results(json, "itemsList", count);

            printf("\n");
            input = readline("Enter \"c\" to continue or leave blank to return: ");
        }

        buffer_delete(buffer);
        json_delete(nextPage);
        json_delete(json);
    }
}

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
    char* result;

    graal_isolate_t* isolate = NULL;
    graal_isolatethread_t* thread = NULL;
    
    if (graal_create_isolate(NULL, &isolate, &thread) != 0) {
        fprintf(stderr, "initialization error\n");
        return 1;
    }

    init(thread);
    
    search(thread, "YouTube");

    result = invoke(thread, "tearDown", "{}");

    graal_tear_down_isolate(thread);

    return 0;
}

