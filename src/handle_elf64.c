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

    if (!chk_ptr(obj, obj->ptr + hdr->e_phoff, hdr->e_phnum*sizeof(Elf64_Phdr)))
        return (er(TRUNCATED, obj->filename));
    phdr = obj->ptr + hdr->e_phoff;
    while (i < hdr->e_phnum)
    {
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

    (void)sec_found;
    hdr = (Elf64_Ehdr*)obj->ptr;
    if (seg_writable(obj, hdr) < 0)
        return (-1);
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
        if (!ft_strcmp(sectionName, ".text"))
        {
            if (sectionHeader->sh_flags == 0xdeadbeef)
                return (er(ALR_PACKD, obj->filename));
            // sectionHeader->sh_flags = 0xdeadbeef;
            obj->text_off = sectionHeader->sh_offset;
            obj->text_vaddr = sectionHeader->sh_addr;
            obj->text_size = sectionHeader->sh_size;
        }
        if (!ft_strcmp(sectionName, ".interp"))
        {
            obj->sec_off = (int)sectionHeader->sh_offset;
            obj->sec_vaddr = (int)sectionHeader->sh_addr;
        }
        sectionHeader++;
        i++;
    }
    if (obj->sec_off && obj->text_off)
        return 1;
    return er(INVALID, obj->filename);
}

static int check_null_space(t_wdy *obj)
{
    size_t         i;
    int             err;
    size_t         nbyte;
    char       *tmp_ptr;

    err = find_offset(obj);
    if (err < 0)
		return (-1);
    i = obj->sec_off;
    nbyte = i;
    tmp_ptr = (char *)obj->ptr;
    while (i < obj->size)
    {
        if (tmp_ptr[i])
            nbyte = i + 1;
        else if (i - nbyte >= obj->payloadLen)
            {
                obj->sec_vaddr += (nbyte - obj->sec_off);
                obj->sec_off = nbyte;
                return (nbyte);
            }
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