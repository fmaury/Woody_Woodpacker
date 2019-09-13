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

static void sort_addr(t_wdy *obj, Elf64_Shdr **addr)
{
    int i = 0;
    Elf64_Shdr *tmp;

    while (addr[i + 1])
    {
        if (addr[i]->sh_offset > addr[i+1]->sh_offset)
        {
            tmp = addr[i];
            addr[i] = addr[i + 1];
            addr[i + 1] = tmp;
            if ( i > 0)
            i--;
        }
        else
            i++;
    }
    i = 0;
    while (addr[i + 1])
    {
        if (addr[i]->sh_offset > 0 && (int)(addr[i+1]->sh_offset - (addr[i]->sh_offset + addr[i]->sh_size)) > 0 && addr[i+1]->sh_offset - (addr[i]->sh_offset + addr[i]->sh_size) > obj->payloadLen)
        {
            obj->sec_vaddr = addr[i]->sh_addr + addr[i]->sh_size + 1;
            obj->sec_off = addr[i]->sh_offset + addr[i]->sh_size + 1;
            break ;
        }
        i++;
    }
}

static int seg_writable(t_wdy *obj, Elf64_Ehdr *hdr)
{
    Elf64_Phdr *phdr;
    int i = 0;

    if (hdr->e_machine != 0x3e)
        return (er(DEFAULT_ERR, "wrong architecture"));
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
    Elf64_Shdr *addr[40]= {NULL};


    (void)sec_found;
    hdr = (Elf64_Ehdr*)obj->ptr;
    if (!chk_ptr(obj, obj->ptr, sizeof(Elf64_Ehdr)))
        return (er(TRUNCATED, obj->filename));
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
            sectionHeader->sh_flags = 0xdeadbeef;
            obj->text_off = sectionHeader->sh_offset;
            obj->text_vaddr = sectionHeader->sh_addr;
            obj->text_size = sectionHeader->sh_size;
        }
        addr[i] = sectionHeader;
        sectionHeader++;
        i++;
    }
    addr[i] = NULL;
    if (!obj->sec_off)
    sort_addr(obj, addr);

    if (obj->sec_off && obj->text_off)
        return 1;
    return er(NOSPACE, obj->filename);
}

int				handle_elf64(t_wdy *obj)
{
	int				fd;

	if (find_offset(obj) == -1)
		return (-1);
    g_payloads[obj->payloadIndex].fencrypt(obj);
	g_payloads[obj->payloadIndex].finsert(obj);
    if ((fd = open(BIN_NAME, O_CREAT | O_WRONLY, 0777)) == -1)
		return (er(OPEN_NEW, obj->filename));
    write(fd, obj->ptr, obj->size);
    if (close(fd) == -1){
    	return (er(CLOSE, obj->filename));}
	return (0);
}