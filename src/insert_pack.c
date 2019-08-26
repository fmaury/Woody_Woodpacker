#include <woody.h>

int seg_writable(t_wdy *obj, Elf64_Ehdr *hdr)
{
    Elf64_Phdr *phdr;
    int i = 0;

    phdr = obj->ptr + hdr->e_phoff;
    while (i < hdr->e_phnum)
    {
        if (phdr->p_type == PT_LOAD)
            phdr->p_flags = PF_R | PF_W | PF_X;
        phdr++;
        i++;
    }
    return (1);
}

int find_offset(t_wdy *obj)
{
    int i = 0;
    int sec_found = 0;
    Elf64_Ehdr *hdr;
    Elf64_Shdr *shdr;
    Elf64_Shdr *secname_section;
    char *secname;

    hdr = (Elf64_Ehdr*)obj->ptr;
    seg_writable(obj, hdr);
    obj->entry = hdr->e_entry;
    obj->entry_addr = &hdr->e_entry;
    if (!chk_ptr(obj, obj->ptr, hdr->e_shoff))
    {
        ft_putstr("Pointer error\n");
        return (0);
    }
    shdr = obj->ptr + hdr->e_shoff;
    if (!chk_ptr(obj, shdr, hdr->e_shstrndx))
    {
        ft_putstr("Pointer error\n");
        return (0);
    }
    secname_section = shdr + hdr->e_shstrndx;
    while (i < hdr->e_shnum)
    {
        if (!chk_ptr(obj, obj->ptr, secname_section->sh_offset + shdr->sh_name) ||
        !chk_ptr(obj, shdr, sizeof(*shdr)))
        {
            ft_putstr("Pointer error\n");
            return (0);
        }
        secname = (char*)(obj->ptr + secname_section->sh_offset + shdr->sh_name);
        if (!strcmp(secname, ".text"))
        {
            shdr->sh_flags = SHF_WRITE | SHF_ALLOC | SHF_EXECINSTR ;
            obj->text_addr = shdr->sh_addr;
            obj->text_offset = (int)shdr->sh_offset;
            obj->text_size = shdr->sh_size;
        }
        if (!strcmp(secname, ".interp"))
            sec_found = (int)shdr->sh_offset;
        shdr++;
        i++;
    }
    if (sec_found && obj->text_size)
        return sec_found;
    return 0;
    
}

int check_null_space(t_wdy *obj)
{
    size_t         i;
    size_t         nbyte;
    char       *tmp_ptr;

    i = find_offset(obj);
    if (!i)
    {
        ft_putstr("Can't write in binary's sections\n");
        return (-1);
    }
    nbyte = i;
    tmp_ptr = (char *)obj->ptr;
    while (i < obj->size)
    {
        if (tmp_ptr[i])
            nbyte = i + 1;
        else if (i - nbyte >= SHELLCODE_LEN)
            return (nbyte);
        i++;
    }
    ft_putstr("Not enough NULL bytes to write in this binary\n");
    return (-2);
}

void insert_shellcode(t_wdy *obj, int offset)
{
    uint32_t jump_offset;

    // Copie le shellcode dans la section qui va bien
    ft_memcpy(obj->ptr + offset, ELF64_SHELLCODE, SHELLCODE_LEN);
    // Calcule l'offset du jump. Debut de la section texte, moins l'offset de la fin de notre shellcode (-1 pour l'octet du jump)
    jump_offset = (obj->entry) - (offset + SHELLCODE_LEN - 1);
    // On ajoute a l'octet du jump la valeur de jump_offset pour jumper au debut de la section text
    ft_memcpy(obj->ptr + offset + SHELLCODE_LEN, (void *)&jump_offset, 4);
    // Ensuite on change la valeur du mov dans notre shellcode par l'offset pour aller au debut de la sec text
    // Debut de la section text + la valeur deja presente dans le mov ($$) moins l'offset du debut de notre shellcode
    jump_offset = obj->entry - offset + *((uint32_t*)(obj->ptr + offset + 27));
    ft_memcpy(obj->ptr + offset + 27, (void *)&jump_offset, 4);
    // La taille de notre section text
    ft_memcpy(obj->ptr + offset + 32, (void *)&obj->text_size, 4);
    *(uint64_t *)obj->entry_addr = offset;
}

int encrypt_text_sec(t_wdy *obj)
{
    int i = 0;
    char *encr = (char*)obj->ptr;
    while (i < obj->text_size)
    {
        encr[obj->entry + i] ^= 42;
        i++;
    }
    return (1); 
}

int insert_pack(t_wdy *obj)
{
    int offset;
    int fp;

    if ((offset = check_null_space(obj)) == -1)
        return (0);
    insert_shellcode(obj, offset);
    encrypt_text_sec(obj);
    if ((fp = open("packed", O_CREAT | O_WRONLY, 0777)) == -1)
    {
        printf("Can't open %s\n", "packed");
        return (0);
    }
    write(fp, obj->ptr, obj->size);
    if (close(fp) == -1)
        return (0);
    return (1);
}