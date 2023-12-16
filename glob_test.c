//
// Created by gobasi on 12/10/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <glob.h>

void perform_globbing(const char* pattern) {
    glob_t glob_result;
    // Add the GLOB_TILDE_CHECK flag to the glob function call
    int ret = glob(pattern, GLOB_TILDE_CHECK, NULL, &glob_result);

    if (ret == 0) {
        printf("Globbing result for pattern '%s':\n", pattern);
        for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
            printf("%s\n", glob_result.gl_pathv[i]);
        }
    } else {
        printf("No matches found for pattern '%s', or tilde expansion failed.\n", pattern);
    }

    globfree(&glob_result);
}



int main() {
    // Example using '*' pattern
    const char* pattern1 = "*.c"; // Matches all files with .c extension
    perform_globbing(pattern1);

    // Example using '?' pattern
    const char* pattern2 = "file?.txt"; // Matches files like file1.txt, file2.txt, etc.
    perform_globbing(pattern2);

    // Example using '[]' pattern
    const char* pattern3 = "file[1-3].txt"; // Matches file1.txt, file2.txt, and file3.txt
    perform_globbing(pattern3);

    // Example using '~' pattern
    const char* pattern4 = "~/ssh/*.txt"; // Matches all .pdf files in the user's Documents directory
    perform_globbing(pattern4);

    return 0;
}


//#include <stdio.h>
//#include <stdlib.h>
//#include <glob.h>
//
//char** perform_globbing(const char* pattern) {
//    glob_t glob_result;
//    int ret = glob(pattern, GLOB_TILDE_CHECK, NULL, &glob_result);
//
//    if (ret == 0) {
//        printf("Globbing result for pattern '%s':\n", pattern);
//        char** matches = malloc(glob_result.gl_pathc * sizeof(char*));
//        for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
//            printf("%s\n", glob_result.gl_pathv[i]);
//            matches[i] = strdup(glob_result.gl_pathv[i]);
//        }
//        globfree(&glob_result);
//        return matches;
//    } else {
//        printf("No matches found for pattern '%s', or tilde expansion failed.\n", pattern);
//        globfree(&glob_result);
//        return NULL;
//    }
//}
//
//int main() {
//    const char* pattern = "*.c"; // Replace with your pattern
//    char** matched_files = perform_globbing(pattern);
//
//    // Use the matched_files as needed
//    // ...
//
//    // Remember to free the allocated memory
//    if (matched_files) {
//        for (size_t i = 0; matched_files[i]; ++i) {
//            free(matched_files[i]);
//        }
//        free(matched_files);
//    }
//
//    return 0;
//}
