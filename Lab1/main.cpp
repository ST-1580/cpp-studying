#include <cstdlib>
#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "usage: " << argv[0] << " <filename>\n";
        return EXIT_FAILURE;
    }
    if (strlen(argv[1]) > 8192) {
        cerr << "string size might be less than 8192" << endl;
        return EXIT_FAILURE;
    }
    FILE* f = fopen(argv[2], "r");
    if (!f) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    bool flag = false;
    char bufferX2[16384];
    while (true) {
        char buffer[8192];
        size_t bytes_read = fread(buffer, sizeof(char), size(buffer), f);
        if (bytes_read < 0) {
            if (ferror(f)) {
                perror("fread failed");
                fclose(f);
                return EXIT_FAILURE;
            }
        } else if (bytes_read == 0) {
            break;
        } else {
            for (size_t i = 0; i < 8192; i++) {
                bufferX2[i] = bufferX2[i + 8192];
            }
            for (size_t i = 0; i < 8192; i++) {
                bufferX2[i + 8192] = buffer[i];
            }
            for (size_t i = 0; i < 16384 - strlen(argv[1]); i++) {
                flag = true;
                for (size_t j = 0; j < strlen(argv[1]); j++) {
                    if (argv[1][j] != bufferX2[i + j]) {
                        flag = false;
                        break;
                    }
                }
                if (flag) {
                    cerr << "true" << endl;
                    fclose(f);
                    return EXIT_SUCCESS;
                }
            }
        }

    }
    cerr << "false" << endl;
    fclose(f);
    return EXIT_SUCCESS;
}
