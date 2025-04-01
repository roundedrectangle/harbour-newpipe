#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "json.h"

#include "appwrapper.h"

Json* extract_next_page(Json* json) {
    Json* parsePage;
    Json* nextPage;
    char const* string;

    parsePage = json_get_dict(json, "nextPage");
    nextPage = json_new();
    if (parsePage) {
        string = json_get_string(parsePage, "url");
        if (string) {
            json_add_string(nextPage, "url", string);
        }
        string = json_get_string(parsePage, "id");
        if (string) {
            json_add_string(nextPage, "id", string);
        }
        json_add_list(nextPage, "ids", jsonlist_new());
        json_add_dict(nextPage, "cookies", json_new());
        json_add_string(nextPage, "body", "");
    }
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

void create_search_structure(Json* json, char const* const service, char const* const searchString, char const* const filter, Json* nextPage) {
    JsonList* list;

    json_clear(json);
    json_add_string(json, "service", service);
    json_add_string(json, "searchString", searchString);

    list = jsonlist_new();
    jsonlist_push_string(list, filter);
    json_add_list(json, "contentFilter", list);

    json_add_string(json, "sortFilter", "");

    if (nextPage) {
        json_add_dict(json, "page", nextPage);
    }
}

void getFilter(graal_isolatethread_t* thread, char const* const service, Buffer *filter) {
    Json* json;
    Buffer* buffer;
    buffer = buffer_new(1024);
    JsonList* list;
    size_t index;
    char* result;
    char const* available;
    size_t length;

    json = json_new();

    // Get search filters
    json_add_string(json, "string", service);
    json_serialize_buffer(json, buffer);

    result = invoke(thread, "getAvailableContentFilter", buffer_get_buffer(buffer));

    json_clear(json);
    json_deserialize_string(json, result, strlen(result));
    buffer_clear(buffer);

    list = json_get_list(json, "stringList");

    length = jsonlist_get_len(list);
    switch (length) {
        case 0: {
            buffer_append_string(buffer, "No filters");
            break;
        }
        case 1: {
            buffer_append_string(buffer, "Available filter: ");
            break;
        }
        default: {
            buffer_append_string(buffer, "Available filters: ");
            break;
        }
    }

    for (index = 0; index < length; ++index) {
        available = jsonlist_get_string(list, index);
        buffer_append_string(buffer, available);
        if (index < (length - 1)) {
            buffer_append_string(buffer, ", ");
        }
        if ((index == 0) && filter) {
            buffer_clear(filter);
            buffer_append_string(filter, available);
        }
    }

    buffer_print(buffer);
    if (filter) {
        printf("Selected: \"%s\"\n", buffer_get_buffer(filter));
    }

    buffer_delete(buffer);
    json_delete(json);
}

void search(graal_isolatethread_t* thread, char const* const service) {
    char* input;
    char* searchString;
    Json* json;
    Buffer* buffer;
    Buffer* filter;
    char* result;
    Json* nextPage;
    size_t count;

    json = json_new();
    buffer = buffer_new(1024);
    filter = buffer_new(64);

    getFilter(thread, service, filter);

    printf("\n");
    printf("Search %s\n", service);
    searchString = readline("Please enter a search phrase (or leave blank to return): ");
    printf("Searching for: \"%s\"\n", searchString);

    if (searchString[0] != 0) {
        // Populate the JSON input parameter structure
        create_search_structure(json, service, searchString, buffer_get_buffer(filter), NULL);
        json_serialize_buffer(json, buffer);

        // Call the Extractor
        result = invoke(thread, "searchFor", buffer_get_buffer(buffer));

        json_clear(json);
        json_deserialize_string(json, result, strlen(result));
        nextPage = extract_next_page(json);

        // Display the results
        count = 0;
        count += display_search_results(json, "relatedItems", 0);

        if (json_get_type(nextPage, "url") == JSONTYPE_STRING) {
            printf("\n");
            input = readline("Enter \"c\" to continue or leave blank to return: ");
            while (strcmp(input, "c") == 0) {
                printf("Continue searching for: \"%s\"\n", searchString);

                // Populate the JSON input parameter structure
                create_search_structure(json, service, searchString, buffer_get_buffer(filter), nextPage);
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
        }
        else {
            printf("No more results\n\n");
        }

        buffer_delete(filter);
        buffer_delete(buffer);
        json_delete(nextPage);
        json_delete(json);
    }
}

char const* select_service() {
    char const* const services[] = {
        "Bandcamp",
        "MediaCCC",
        "PeerTube",
        "SoundCloud",
        "YouTube"
    };
    size_t index;
    char* input;
    size_t service;
    size_t converted;
    char const* serviceName;
    size_t const length = sizeof(services) / sizeof(services[0]);

    serviceName = NULL;
    converted = 0;
    while (converted != 1) {
        printf("Which service would you like to use?\n");
        printf("  %lu: %s\n", 0lu, "Exit application");
        for (index = 0; index < length; ++index) {
            printf("  %lu: %s\n", (index + 1), services[index]);
        }
        input = readline("Please enter a number: ");

        converted = sscanf(input, "%lu", &service);
        if (converted && (service > 0) && (service <= length)) {
            printf("Number: %lu\n", service);
            serviceName = services[(service - 1)];
        }
    }
    return serviceName;
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
    char const* service = "";

    graal_isolate_t* isolate = NULL;
    graal_isolatethread_t* thread = NULL;
    
    if (graal_create_isolate(NULL, &isolate, &thread) != 0) {
        fprintf(stderr, "initialization error\n");
        return 1;
    }

    init(thread);
    
    while (service) {
        service = select_service();

        if (service) {
            search(thread, service);
        }
    }

    result = invoke(thread, "tearDown", "{}");

    graal_tear_down_isolate(thread);

    return 0;
}

