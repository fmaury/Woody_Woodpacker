/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho64.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 11:49:46 by cbarbier          #+#    #+#             */
/*   Updated: 2019/08/14 16:51:26 by cbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <woody.h>

static int seg_writable(t_wdy *obj, Elf64_Ehdr *hdr)
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

static int find_offset(t_wdy *obj)
{
    int i = 0;
    int sec_found = 0;
    Elf64_Ehdr *hdr;
    Elf64_Shdr *sectionHeader;
    Elf64_Shdr *tableNameSection;
    char *sectionName;

    hdr = (Elf64_Ehdr*)obj->ptr;
    seg_writable(obj, hdr);
    obj->entry = hdr->e_entry;
    obj->entry_addr = &hdr->e_entry;
    if (!chk_ptr(obj, obj->ptr, hdr->e_shoff))
        return (er(TRUNCATED, obj->filename));
    sectionHeader = obj->ptr + hdr->e_shoff;
    if (!chk_ptr(obj, sectionHeader, hdr->e_shstrndx))
        return (er(TRUNCATED, obj->filename));
    tableNameSection = sectionHeader + hdr->e_shstrndx;
    while (i < hdr->e_shnum)
    {
        if (!chk_ptr(obj, obj->ptr, tableNameSection->sh_offset + sectionHeader->sh_name) ||
        !chk_ptr(obj, sectionHeader, sizeof(*sectionHeader)))
	        return (er(TRUNCATED, obj->filename));
        sectionName = (char*)(obj->ptr + tableNameSection->sh_offset + sectionHeader->sh_name);
        if (!strcmp(sectionName, ".text"))
        {
            if (sectionHeader->sh_flags == 0xdeadbeef)
                return (er(ALR_PACKD, obj->filename));
            sectionHeader->sh_flags = 0xdeadbeef;
            obj->text_addr = sectionHeader->sh_addr;
            obj->text_offset = (int)sectionHeader->sh_offset;
            obj->text_size = sectionHeader->sh_size;
        }
        if (!strcmp(sectionName, ".rodata"))
            sec_found = (int)sectionHeader->sh_offset;
        sectionHeader++;
        i++;
    }
    if (sec_found && obj->text_size)
        return sec_found;
    return er(INVALID, obj->filename);
}

int check_null_space(t_wdy *obj)
{
    size_t         i;
    int             err;
    size_t         nbyte;
    char       *tmp_ptr;

    err = find_offset(obj);
    if (err < 0)
		return (-1);
    i = err;
    nbyte = i;
    tmp_ptr = (char *)obj->ptr;
    while (i < obj->size)
    {
        if (tmp_ptr[i])
            nbyte = i + 1;
        else if (i - nbyte >= obj->payloadLen)
            return (nbyte);
        i++;
    }
    return (er(NOSPACE, obj->filename));
}

int				handle_elf64(t_wdy *obj)
{
	int				fd;
	int			offset;

	if ((offset = check_null_space(obj)) == -1)
		return (-1);
    g_payloads[obj->payloadIndex].fencrypt(obj);
	g_payloads[obj->payloadIndex].finsert(obj, offset);
    if ((fd = open(BIN_NAME, O_CREAT | O_WRONLY, 0777)) == -1)
		return (er(OPEN_NEW, obj->filename));
    write(fd, obj->ptr, obj->size);
    if (close(fd) == -1){
    	return (er(CLOSE, obj->filename));}
	return (0);
}