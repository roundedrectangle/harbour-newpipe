#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <curl/curl.h>
#include "json.h"

#include "appwrapper.h"

#define SERVICE_NUM (5)

char const* minetype_to_extension(char const* mimetype) {
    char* mapping[][2]= {
        {"video/mp4", "mp4"},
        {"", "dat"}
    };
    size_t length = sizeof(mapping) / sizeof(mapping[0]);
    size_t index;
    char const* result;

    result = NULL;
    index = 0;
    while ((!result) && (index < length)) {
        if ((strcmp(mimetype, mapping[index][0]) == 0) || (mapping[index][0][0] == 0)) {
            result = mapping[index][1];
        }
        ++index;
    }
    return result;
}

void sanitise_name(Buffer* buffer, char const* const extension) {
    Buffer* result;
    size_t length;
    size_t index;
    char character;

    length = buffer_get_pos(buffer);
    if (length > 20) {
        length = 20;
    }

    result = buffer_new(length + strlen(extension) + 1);
    buffer_append_string(result, "./downloads/");

    for (index = 0; index < length; ++index) {
        character = buffer_get_buffer(buffer)[index];
        if (((character >= 'a') && (character <= 'z'))
            || ((character >= 'A') && (character <= 'Z'))
            || ((character >= '0') && (character <= '9'))) {
            buffer_append(result, &character, 1);
        }
        if (character == ' ') {
            buffer_append(result, &"_", 1);
        }
    }
    if (buffer_get_pos(result) < 3) {
        buffer_append_string(result, "media");
    }
    buffer_append_string(result, ".");
    buffer_append_string(result, extension);

    buffer_clear(buffer);
    buffer_append_buffer(buffer, result);
    buffer_delete(result);
}

Json* extract_next_page(Json * json) {
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

void get_item_info(graal_isolatethread_t* thread, char const* service, char const* url, Json * json) {
    char* result;
    Buffer* buffer;

    buffer = buffer_new(1024);
    json_clear(json);
    json_add_string(json, "service", service);
    json_add_string(json, "url", url);

    json_serialize_buffer(json, buffer);
    result = invoke(thread, "downloadExtract", buffer_get_buffer(buffer));

    buffer_delete(buffer);
    json_deserialize_string(json, result, strlen(result));
}

size_t download_write(void* ptr, size_t size, size_t nmemb, FILE* userdata) {
    return fwrite(ptr, size, nmemb, userdata);
}

void download_file(char const* const url, char const* const name) {
    CURL* curl;
    FILE* output;
    CURLcode result;
    Buffer* filename;
    char* contentType;
    char const* extension;

    printf("Downloading: %s\n", url);
    printf("\n");

    mkdir("./downloads", 0777);

    curl = curl_easy_init();
    if (curl) {
        output = fopen("./downloads/download.dat", "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, download_write);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, output);
        result = curl_easy_perform(curl);

        extension = "dat";
        if (result == CURLE_OK) {
            result = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);
        }
        if (result == CURLE_OK) {
            printf("Content type: %s\n", contentType);
            extension = minetype_to_extension(contentType);
            printf("Extension: %s\n", extension);
        }

        filename = buffer_new(1024);
        buffer_append_string(filename, name);
        sanitise_name(filename, extension);
        rename("./downloads/download.dat", buffer_get_buffer(filename));
        printf("File saved to: %s\n", buffer_get_buffer(filename));
        buffer_delete(filename);

        curl_easy_cleanup(curl);
        fclose(output);
    }
}

void open_file(char const* const url) {
    printf("Opening: %s\n", url);
    printf("\n");
}

void display_metadata(graal_isolatethread_t* thread, Json* json, char const* const service, char const* const key, size_t count, size_t selection) {
    JsonList* list;
    JsonList* metaInfo;
    size_t metaInfoLength;
    Json* params;
    bool valid;
    char* action;

    list = json_get_list(json, key);

    size_t length = jsonlist_get_len(list);

    if ((selection <= count) || (selection > count + length)) {
        printf("Select falls outside bounds (must be between %lu and %lu inclusive)\n", count + 1, (count + length));
    }
    else {
        Json* item = jsonlist_get_dict(list, selection - count - 1);
        char const* name = json_get_string(item, "name");
        printf("\n");
        printf("Title: %s\n", name);

        params = json_new();

        char const* url = json_get_string(item, "url");
        printf("Service: %s\n", service);

        // Returns DownloadExtracted
        get_item_info(thread, service, url, params);

        char const* uploaderName = json_get_string(params, "uploaderName");
        char const* category = json_get_string(params, "category");
        long long int likeCount = json_get_integer(params, "likeCount");
        long long int viewCount = json_get_integer(params, "viewCount");
        char const* content = json_get_string(params, "content");

        printf("Uploader: %s\n", uploaderName);
        printf("Category: %s\n", category);
        printf("Like count: %llu\n", likeCount);
        printf("View count: %llu\n", viewCount);
        printf("\n");

        valid = true;
        while (valid) {
            valid = true;
            printf("Enter \"d\" to download the media file.\n");
            printf("Enter \"o\" to open the file.\n");
            action = readline("Leave blank to return: ");
            switch (action[0]) {
                case 'd': {
                    download_file(content, name);
                    break;
                }
                case 'o': {
                    open_file(content);
                    break;
                }
                case 0: {
                    // Leave the loop
                    valid = false;
                    break;
                }
                default: {
                    // Go around the loop again
                    break;
                }
            }
        }

        json_delete(params);
    }
}

void get_search_results(graal_isolatethread_t* thread, Json* json, char const* const service, char const* const searchString, char const* const filter, Json* nextPage) {
    Buffer* buffer;
    char* result;

    buffer = buffer_new(1024);

    // Populate the JSON input parameter structure
    create_search_structure(json, service, searchString, filter, nextPage);
    json_serialize_buffer(json, buffer);

    // Call the Extractor
    if (!nextPage) {
        result = invoke(thread, "searchFor", buffer_get_buffer(buffer));
    } else {
        result = invoke(thread, "getMoreSearchItems", buffer_get_buffer(buffer));
    }

    json_clear(json);
    json_deserialize_string(json, result, strlen(result));

    buffer_delete(buffer);
}

void search(graal_isolatethread_t* thread, char const* const service) {
    char* input;
    char* searchString;
    Json* json;
    Buffer* buffer;
    Buffer* filter;
    char* result;
    Json* nextPage;
    size_t startCount;
    size_t count;
    size_t selection;
    int successCount;
    char* key;
    bool repeat;

    json = json_new();
    buffer = buffer_new(1024);
    filter = buffer_new(64);
    nextPage = NULL;
    input = "c";

    getFilter(thread, service, filter);

    printf("\n");
    printf("Search %s\n", service);
    printf("Enter a search phrase\n");
    searchString = readline("Leave blank to return: ");

    // Display the results
    count = 0;
    startCount = count;
    key = "relatedItems";
    repeat = false;

    while ((searchString[0] != 0) && (strcmp(input, "c") == 0)) {
        printf("Searching for: \"%s\"\n", searchString);

        if (!repeat) {
            get_search_results(thread, json, service, searchString, buffer_get_buffer(filter), nextPage);
            nextPage = extract_next_page(json);
        }

        // Display the results
        startCount = count;
        count += display_search_results(json, key, count);

        printf("\n");

        if (json_get_type(nextPage, "url") == JSONTYPE_STRING) {
            printf("Enter \"c\" to continue.\n");
        }
        else {
            printf("No more results\n\n");
            searchString = "";
        }

        printf("Enter a number to select an entry.\n");
        input = readline("Leave blank to return: ");

        successCount = sscanf(input, "%lu", &selection);
        if (successCount == 1) {
            display_metadata(thread, json, service, key, startCount, selection);
            input = "c";
            repeat = true;
            count = startCount;
        }
        else {
            key = "itemsList";
        }
    }

    buffer_delete(filter);
    buffer_delete(buffer);
    json_delete(nextPage);
    json_delete(json);
}

char const* select_service() {
    char const* const services[] = {
        "Bandcamp",
        "MediaCCC",
        "PeerTube",
        "SoundCloud",
        "YouTube"
    };
    size_t const length = sizeof(services) / sizeof(services[0]);
    size_t index;
    char* input;
    size_t service;
    size_t converted;
    char const* serviceName;

    serviceName = NULL;
    converted = 0;
    while (converted != 1) {
        printf("Which service would you like to use?\n");
        printf("  %lu: %s\n", 0lu, "Exit application");
        for (index = 0; index < length; ++index) {
            printf("  %lu: %s\n", (index + 1), services[index]);
        }
        printf("Enter a number to select an entry.\n");
        input = readline("Leave blank to exit: ");

        service = 0;
        converted = input[0] == 0 ? 1 : sscanf(input, "%lu", &service);
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

