#include <woody.h>

void find_offset(t_wdy *obj)
{
    Elf64_Ehdr *hdr;
    Elf64_Shdr *shdr;
    int i = 0;
    Elf64_Shdr *sec_name;

    hdr = (Elf64_Ehdr*)obj->ptr;
    obj->entry = hdr->e_entry;
    printf("entry:%ld\n",hdr->e_shoff);
    shdr = obj->ptr + hdr->e_shoff;
    sec_name = shdr + hdr->e_shstrndx;
    while (i < hdr->e_shnum)
    {
    printf("name:%s\n", (char*)(obj->ptr + sec_name->sh_offset + shdr->sh_name));
        shdr++;
        i++;
    }
    
}

int check_null_space(t_wdy *obj)
{
    // size_t     i;
    // size_t   nbyte;
    // char       *tmp_ptr;

    // i = 0;
    find_offset(obj);
    // nbyte = i;
    // tmp_ptr = (char *)obj->ptr;
    // while (i < obj->size)
    // {
    //     if (tmp_ptr[i])
    //         nbyte = i + 1;
    //     else if (i - nbyte >= SHELLCODE_LEN)
    //         return (nbyte);
    //     i++;
    // }
    return (-1);
}

void insert_shellcode(t_wdy *obj, int offset)
{
    ft_memcpy(obj->ptr + offset, ELF64_SHELLCODE, SHELLCODE_LEN);
}

int insert_pack(t_wdy *obj)
{
    int offset;
    int fp;

    if ((offset = check_null_space(obj)) == -1)
        return (0);
    insert_shellcode(obj, offset);
    fp = open("packed", O_CREAT | O_WRONLY, 0777);
    write(fp, obj->ptr, obj->size);
    close(fp);
    return (1);
}