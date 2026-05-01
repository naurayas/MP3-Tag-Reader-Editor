/* Name         : Naura Yasmeen U
   Start Date   : 28/11/2025
   End Date     : 30/12/2025
   Project Name : MP3 Tag reader/editor
*/

#include <string.h>
#include "common.h"
#include "view.h"
#include "edit.h"

int main(int argc, char *argv[])
{
    mp3File contxt;
    int opr = check_opr_type(argv);
    if (opr == view)
    {
        if (argc >= 3)
        {
            if (check_vfile_type(argv, &contxt) == success)
            {
                if (view_tag_details(&contxt) == failure)
                {
                    printf("ERROR: Failed to get tag details.\n");
                }
            }
            else
            {
                printf("ERROR: Input file is not mp3.\n");
            }
        }
        else
        {
            printf("ERROR: Insufficient number of command line arguments.\n");
        }
    }
    else if (opr == edit)
    {
        if (argc >= 5)
        {
            if (check_efile_type(argv, &contxt) == success)
            {
                if (check_edit_type(argv) == success)
                {
                    char opt = argv[2][1];
                    char *new_text = argv[3];
                    if (edit_tag_details(&contxt, opt, new_text) == failure)
                    {
                        printf("ERROR: Failed to edit tag details.\n");
                    }
                }
                else
                {
                    printf("ERROR: Invalid edit option.\n");
                }
            }
            else
            {
                printf("ERROR: Input files are not mp3.\n");
            }
        }
        else
        {
            printf("ERROR: Insufficient number of command line arguments.\n");
        }
    }
    else if (opr == help)
    {
        help_menu();
    }
    else
    {
        usage_msg();
    }

    return 0;
}

void usage_msg(void)
{
    /* print error message */
    printf("Invalid arguments!\n");
    printf("To view please pass like : ./mp3 -v mp3filename.mp3\n");
    printf("To edit please pass like : ./mp3 -e -t/-a/-A/-m/-y/-c \"changing_text\" mp3filename.mp3\n");
    printf("To get help pass like : ./mp3 --help\n");
}

void help_menu(void)
{
    /* display help menu */
    printf("---------------------HELP MENU---------------------\n");
    printf("1. -v -> to view mp3 file contents\n");
    printf("2. -e -> to edit mp3 file contents\n");
    printf("      2.1. -t -> to edit song title\n");
    printf("      2.2. -a -> to edit artist name\n");
    printf("      2.3. -A -> to edit album name\n");
    printf("      2.4. -y -> to edit year\n");
    printf("      2.5. -m -> to edit content\n");
    printf("      2.6. -c -> to edit comment\n");
    printf("---------------------HELP MENU---------------------\n");
}

OperationType check_opr_type(char *argv[])
{
    if (argv[1] == NULL)
    {
        return error;
    }
    else if (strcmp(argv[1], "-v") == 0)
    {
        return view;
    }
    else if (strcmp(argv[1], "-e") == 0)
    {
        return edit;
    }
    else if (strcmp(argv[1], "--help") == 0)
    {
        return help;
    }
    return error;
}

Status check_edit_type(char *argv[])
{
    if(!strcmp(argv[2], "-t") || !strcmp(argv[2], "-a") || !strcmp(argv[2], "-A") || 
       !strcmp(argv[2], "-y") || !strcmp(argv[2], "-m") || !strcmp(argv[2], "-c"))
        return success;
    return failure;
}