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

static Elf64_Phdr   *getseg(t_wdy *obj, Elf64_Ehdr *hdr, unsigned int index)
{
    return (obj->ptr + hdr->e_phoff + index * sizeof(Elf64_Phdr));
}

static int updseg(t_wdy *obj, Elf64_Ehdr *hdr)
{
    Elf64_Phdr      *seg;
    unsigned int    i = 0;
    bool            found = false;

    if (!chk_ptr(obj, obj->ptr + hdr->e_phoff, hdr->e_phnum * sizeof(Elf64_Phdr)))
        return (er(TRUNCATED, obj->filename));
    while (i < hdr->e_phnum)
    {
        seg = getseg(obj, hdr, i);
        if (found)
        {
            seg->p_offset += SIZE;
        }
        if (hdr->e_entry > seg->p_vaddr && hdr->e_entry < seg->p_vaddr + seg->p_filesz)
        {
            seg->p_memsz += SIZE;
            seg->p_filesz += SIZE;
            found = true;
        }
        if (found)
            seg->p_flags = PF_W | PF_R | PF_X;
        i++;
    }
    return (1);
}

static int updsec(t_wdy *obj)
{
    int i = 0;
    int sec_found = 0;
    Elf64_Ehdr *hdr;
    Elf64_Shdr *sec;

    hdr = (Elf64_Ehdr*)obj->ptr;
    if (updseg(obj, hdr) < 0)
        return (-1);
    obj->entry = hdr->e_entry;
    obj->entry_addr = &hdr->e_entry;
    if (!chk_ptr(obj, obj->ptr, hdr->e_shoff))
        return (er(TRUNCATED, obj->filename));
    sec = obj->ptr + hdr->e_shoff;
    if (sec->sh_flags == 0xdeadbeef)
        return (er(ALR_PACKD, obj->filename));
    sec->sh_flags = 0xdeadbeef;
    while (i < hdr->e_shnum)
    {
        if (hdr->e_entry > sec->sh_addr && hdr->e_entry <= sec->sh_addr + sec->sh_size)
        {
            obj->text_addr = sec->sh_addr;
            obj->text_offset = (int)sec->sh_offset;
            obj->text_size = sec->sh_size;
        }
        sec++;
        i++;
    }
    if (sec_found && obj->text_size)
        return sec_found;
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