#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void check_elf(unsigned char *e_ident);
void print_elf_header_info(char *filename);

/**
 * check_elf - Checks if the given file is in ELF format
 * @e_ident: Pointer to the ELF identification bytes
 */
void check_elf(unsigned char *e_ident)
{
	int index;

	for (index = 0; index < 4; index++)
	{
		if (e_ident[index] != 127 &&
		    e_ident[index] != 'E' &&
		    e_ident[index] != 'L' &&
		    e_ident[index] != 'F')
		{
			dprintf(STDERR_FILENO, "Error: Not an ELF file\n");
			exit(98);
		}
	}
}

/**
 * print_elf_header_info - Prints the ELF header information
 * @filename: Name of the ELF file
 */
void print_elf_header_info(char *filename)
{
	Elf64_Ehdr *header;
	int fd, read_result;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't read file %s\n", filename);
		exit(98);
	}

	header = malloc(sizeof(Elf64_Ehdr));
	if (header == NULL)
	{
		close(fd);
		dprintf(STDERR_FILENO, "Error: Can't read file %s\n", filename);
		exit(98);
	}

	read_result = read(fd, header, sizeof(Elf64_Ehdr));
	if (read_result == -1)
	{
		free(header);
		close(fd);
		dprintf(STDERR_FILENO, "Error: `%s`: No such file\n", filename);
		exit(98);
	}

	check_elf(header->e_ident);
	printf("ELF Header:\n");
	/* Print the ELF header information here */

	free(header);
	close(fd);
}

/**
 * main - Entry point of the program
 * @argc: Number of command-line arguments
 * @argv: Array of command-line argument strings
 * Return: 0 on success, 98 on error
 */
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		dprintf(STDERR_FILENO, "Usage: %s <filename>\n", argv[0]);
		exit(98);
	}

	print_elf_header_info(argv[1]);

	return (0);
}
