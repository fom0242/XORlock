/*
 * XORlock - Simple File Encryption Tool
 * Version: 1.0
 * Author: [fom0242]
 * Description: A minimal file encryption tool using XOR cipher
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void XORL_PrintVersion(void) {
  printf("XORlock Version 1.0\n");
  printf("Simple file encryption tool using XOR cipher\n");
  printf("Compiled on: %s %s\n", __DATE__, __TIME__);
}

void XORL_PrintHelp(void) {
  printf("\n");
  printf("XORlock - Simple File Encryption Tool\n");
  printf("=====================================\n\n");

  printf("Usage:\n");
  printf("  Encrypt file: xorlock -e filename password\n");
  printf("  Decrypt file: xorlock -d filename.enc password\n");
  printf("  Show help:    xorlock -h\n");
  printf("  Show version: xorlock -v\n\n");

  printf("Examples:\n");
  printf("  xorlock -e secret.txt mypassword123\n");
  printf("  xorlock -d secret.txt.enc mypassword123\n\n");

  printf("Notes:\n");
  printf("  - Encrypted files get .enc extension automatically\n");
  printf("  - .enc extension is removed during decryption\n");
  printf("  - XOR cipher uses same password for encryption and decryption\n\n");

  printf("Security Warning:\n");
  printf("  - XOR encryption provides basic protection only\n");
  printf("  - No password validation: wrong password will still 'decrypt' the "
         "file\n");
  printf("  - Decrypted files may be corrupted if password is incorrect\n");
  printf("  - Always verify decrypted files manually for correctness\n");
  printf("  - Not suitable for highly sensitive or critical data\n\n");
}

void XORL_XorEncryptFile(const char *input_file, const char *output_file,
                         const char *password) {
  FILE *input = fopen(input_file, "rb");
  FILE *output = fopen(output_file, "wb");

  if (!input || !output) {
    if (input)
      fclose(input);
    if (output)
      fclose(output);
    fprintf(stderr, "Error: File operation failed\n");
    exit(EXIT_FAILURE);
  }

  fseek(input, 0, SEEK_END);
  long file_size = ftell(input);
  fseek(input, 0, SEEK_SET);

  printf("File size: %.2f MB\n", file_size / (1024.0 * 1024.0));

  int password_len = strlen(password);
  int password_index = 0;
  int current_char;
  long processed_bytes = 0;
  int last_percent = -1;

  while ((current_char = fgetc(input)) != EOF) {
    fputc(current_char ^ password[password_index], output);
    password_index = (password_index + 1) % password_len;

    processed_bytes++;

    int percent = (int)((double)processed_bytes / file_size * 100);
    if (percent != last_percent && (percent % 1 == 0)) {
      printf("\rProcessing: %3d%% [", percent);
      int bar_width = 50;
      int pos = (int)(bar_width * processed_bytes / file_size);
      for (int i = 0; i < bar_width; i++) {
        if (i < pos)
          printf("=");
        else if (i == pos)
          printf(">");
        else
          printf(" ");
      }
      printf("] %.2f/%.2f MB", processed_bytes / (1024.0 * 1024.0),
             file_size / (1024.0 * 1024.0));
      fflush(stdout);
      last_percent = percent;
    }
  }

  printf("\n");
  fclose(input);
  fclose(output);
}

int XORL_IsEncryptedFile(const char *file_name) {
  int len = strlen(file_name);
  return (len > 4 && strcmp(file_name + len - 4, ".enc") == 0);
}

int main(int argc, char *argv[]) {
  if (argc == 1 || strcmp(argv[1], "-h") == 0 ||
      strcmp(argv[1], "--help") == 0) {
    XORL_PrintHelp();
    return 0;
  }

  if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
    XORL_PrintVersion();
    return 0;
  }

  if (argc != 4) {
    fprintf(stderr, "Error: Incorrect number of arguments\n");
    XORL_PrintHelp();
    return 1;
  }

  char *mode = argv[1];
  char *input_file = argv[2];
  char *password = argv[3];

  FILE *test_file = fopen(input_file, "rb");
  if (!test_file) {
    fprintf(stderr, "Error: Cannot open file '%s'\n", input_file);
    return 1;
  }
  fclose(test_file);

  if (strlen(password) == 0) {
    fprintf(stderr, "Error: Password cannot be empty\n");
    return 1;
  }

  char output_file[1024];

  if (strcmp(mode, "-e") == 0 || strcmp(mode, "--encrypt") == 0) {
    size_t result =
        snprintf(output_file, sizeof(output_file), "%s.enc", input_file);
    if (result >= sizeof(output_file)) {
      fprintf(stderr, "Error: Output filename too long (max %zu chars)\n",
              sizeof(output_file) - 1);
      return 1;
    }

    printf("Encrypting: %s -> %s\n", input_file, output_file);
    XORL_XorEncryptFile(input_file, output_file, password);
    printf("Encryption complete!\n");
  } else if (strcmp(mode, "-d") == 0 || strcmp(mode, "--decrypt") == 0) {
    if (XORL_IsEncryptedFile(input_file)) {
      size_t len = strlen(input_file);
      if (len - 4 >= sizeof(output_file)) {
        fprintf(stderr, "Error: Output filename too long\n");
        return 1;
      }
      strncpy(output_file, input_file, len - 4);
      output_file[len - 4] = '\0';
    } else {
      size_t result =
          snprintf(output_file, sizeof(output_file), "%s.dec", input_file);
      if (result >= sizeof(output_file)) {
        fprintf(stderr, "Error: Output filename too long (max %zu chars)\n",
                sizeof(output_file) - 1);
        return 1;
      }
    }
    printf("Decrypting: %s -> %s\n", input_file, output_file);
    XORL_XorEncryptFile(input_file, output_file, password);
    printf("Decryption complete!\n");
  } else {
    fprintf(stderr, "Error: Unknown mode '%s'\n", mode);
    XORL_PrintHelp();
    return 1;
  }

  return 0;
}
