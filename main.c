#define _GNU_SOURCE
#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

const char *dialing_prefixes[] = {"*272", "*67", "*82", "011"};

char *tel_uri_pattern = "tel:([0-9|\\(|\\)|\\.|\\*|\\+|\\-]+);*";
char *sip_uri_pattern = "sip:([0-9|\\(|\\)|\\.|\\*|\\+|\\-]+)[:|;|@]*";

enum URI_Type
{
    Unknown_URI_Type,
    TEL,
    SIP,
};

enum TON_Type
{
    Unknown_TON_Type,
    International,
    National,
    NetworkSpecific,
    SubscriberNumber,
    Alphanumeric,
    Abbreviated
};

enum NPI_Type
{
    Unknown_NPI_TYPE = 0,
    ISDN = 1,
    X121 = 3,
    F69 = 4,
    E212 = 6,
    NationalNumberingPlan = 8,
    PrivateNumberingPlan = 9,
    ERMESNumberingPlan = 10,
    IP = 13,
    WAPClientId = 18
};

struct URI_Details
{
    enum URI_Type type;
    enum TON_Type ton;
    enum NPI_Type npi;
    char *tn;
    char *cleaned_tn;
    char value[4096];
};

bool user_is_phone(struct URI_Details *details);
bool starts_with(const char *pre, const char *str);
void clean_tn(struct URI_Details *details);

char *get_tn(char *source, char *regexString)
{
    size_t maxGroups = 3;
    char *tn = NULL;
    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups];

    if (regcomp(&regexCompiled, regexString, REG_EXTENDED))
    {
        printf("Could not compile regular expression.\n");
        return tn;
    }

    if (regexec(&regexCompiled, source, maxGroups, groupArray, 0) == 0)
    {
        unsigned int g = 1;
        int start_offset = groupArray[g].rm_so;
        int tn_length = groupArray[g].rm_eo - start_offset;
        tn = (char *)malloc(tn_length + 1);
        strncpy(tn, source + start_offset, tn_length);
        tn[tn_length] = 0;
    }

    regfree(&regexCompiled);

    return tn;
}

void set_uri_type(struct URI_Details *details)
{
    details->type = Unknown_URI_Type;

    char *tn;
    tn = get_tn(details->value, tel_uri_pattern);
    if (tn != NULL)
    {
        details->type = TEL;
        details->tn = tn;
        return;
    }

    tn = get_tn(details->value, sip_uri_pattern);
    if (tn != NULL)
    {
        details->type = SIP;
        details->tn = tn;
        return;
    }
}

void set_npi(struct URI_Details *details)
{
    details->npi = Unknown_NPI_TYPE;
    if (details->type == TEL || user_is_phone(details))
    {
        details->npi = ISDN;
    }
}

bool user_is_phone(struct URI_Details *details)
{
    return strstr(details->value, "user=phone") != NULL;
}

bool tn_is_international(struct URI_Details *details)
{
    return (starts_with("+", details->tn) || starts_with("011", details->tn));
}

bool starts_with(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

void set_ton(struct URI_Details *details)

{
    clean_tn(details);
    if (tn_is_international(details) && details->npi == ISDN)
    {
        details->ton = International;
        return;
    }

    if (user_is_phone(details) && !tn_is_international(details) && strlen(details->cleaned_tn) > 7)
    {
        details->ton = National;
        return;
    }

    if (strlen(details->cleaned_tn) <= 7)
    {
        details->ton = NetworkSpecific;
        return;
    }
}

void clean_tn(struct URI_Details *details)
{
    char buffer[strlen(details->tn)];
    char tn[strlen(details->tn)];
    char no_prefix_tn[strlen(details->tn)];
    strcpy(tn, details->tn);

    // remove dialing prefix @TODO refactor this code to a function
    int idx;
    int prefix_count = sizeof(dialing_prefixes) / sizeof(dialing_prefixes[0]);
    bool has_prefix = false;
    for (idx = 0; idx < prefix_count; idx++)
    {
        if (starts_with(dialing_prefixes[idx], tn))
        {
            strcpy(no_prefix_tn, tn + strlen(dialing_prefixes[idx]));
            has_prefix = true;
            break;
        }
    }
    if (!has_prefix)
        strcpy(no_prefix_tn, tn);

    // remove visual separators @TODO refactor this code to a function
    int offset;
    int buffer_offset = 0;
    for (offset = 0; offset < strlen(no_prefix_tn); offset++)
    {
        if (isdigit(no_prefix_tn[offset]))
        {
            buffer[buffer_offset] = no_prefix_tn[offset];
            buffer_offset = buffer_offset + 1;
        }
        buffer[buffer_offset] = 0;
    }
    details->cleaned_tn = malloc(buffer_offset);
    strcpy(details->cleaned_tn, buffer);
}

void print_row(struct URI_Details *details)
{
    char separator[4] = "\t\t\t";
    if (strlen(details->cleaned_tn) >= 11)
        strcpy(separator, "\t\t");
    printf("TN: %s %sTON: %d \t\tNPI: %d\n", details->cleaned_tn, separator, details->ton, details->npi);
}

int main(int argc, char **argv)
{
    FILE *input;

    if (argc > 1)
    {
        if (!strcmp(argv[1], "-"))
        {
            input = stdin;
        }
        else
        {
            input = fopen(argv[1], "r");
            if (NULL == input)
            {
                fprintf(stderr, "Unable to open '%s': %s\n",
                        argv[1], strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
    }
    else
    {
        input = stdin;
    }

    char *line;
    size_t buffer_size;
    size_t line_length;
    do
    {
        line = NULL;
        buffer_size = 0;
        line_length = getline(&line, &buffer_size, input);
        if (line_length != -1)
        {
            struct URI_Details details;

            strcpy(details.value, line);
            set_uri_type(&details);
            set_npi(&details);
            set_ton(&details);
            print_row(&details);
        }
    } while ((line != NULL) && (strcmp(line, "\n") != 0) && (line_length != -1));

    return EXIT_SUCCESS;
}
