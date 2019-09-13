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

static int updseg(t_wdy *obj, Elf64_Ehdr *hdr)
{
    Elf64_Phdr      *seg;
    unsigned int    i = 0;
    bool            found = false;

    if (!chk_ptr(obj, obj->ptr + hdr->e_phoff, hdr->e_phnum * sizeof(Elf64_Phdr)))
        return (er(TRUNCATED, obj->filename));
    seg = obj->ptr + hdr->e_phoff;
    while (i < hdr->e_phnum)
    {
        if (found)
        {
            seg->p_offset += SIZE;
            seg->p_flags |= PF_W;
        }
        if (hdr->e_entry >= seg->p_vaddr && hdr->e_entry <= seg->p_vaddr + seg->p_filesz)
        {
            obj->text_addr = seg->p_vaddr;
            obj->text_offset = seg->p_offset;
            obj->text_size = seg->p_filesz;
            seg->p_memsz += obj->payloadLen;
            seg->p_filesz += obj->payloadLen;
            seg->p_flags |= PF_W;
            found = true;
        }
        i++;
        seg++;
    }
    return (1);
}

static int update(t_wdy *obj)
{
    unsigned int    i = 0;
    bool            found = false;
    Elf64_Ehdr      *hdr;
    Elf64_Shdr      *sec;

    hdr = (Elf64_Ehdr*)obj->ptr;
    if (!chk_ptr(obj, obj->ptr, sizeof(Elf64_Ehdr)))
        return (er(TRUNCATED, obj->filename));
    if (hdr->e_machine != 0x3e) // not x86_64 architecture
        return (er(DEFAULT_ERR, "wrong architecture"));
    if (updseg(obj, hdr) < 0)
        return (-1);
    obj->old_entry = hdr->e_entry;
    if (!chk_ptr(obj, obj->ptr, hdr->e_shoff + sizeof(Elf64_Shdr) * hdr->e_shnum))
        return (er(TRUNCATED, obj->filename));
    sec = obj->ptr + hdr->e_shoff;
    if (sec->sh_flags == 0xdeadbeef)
         return (er(ALR_PACKD, obj->filename));
    sec->sh_flags = 0xdeadbeef;
    while (i < hdr->e_shnum)
    {
        if (found)
            sec->sh_offset += SIZE;
        if (hdr->e_entry >= sec->sh_addr && hdr->e_entry < sec->sh_addr + sec->sh_size)
        {
            obj->sc_addr = sec->sh_addr;
            obj->sc_offset = sec->sh_offset;
            obj->sc_size = sec->sh_size;
        }
        if (sec->sh_offset + sec->sh_size == obj->text_offset + obj->text_size)
        {
            sec->sh_size += obj->payloadLen;
            found = true;
        }
        sec++;
        i++;
    }
    hdr->e_shoff += SIZE;
    hdr->e_entry = obj->text_addr + obj->text_size;
    return (0);
}

int				handle_elf64(t_wdy *obj)
{
	int				fd;
    void            *tmp;

    if (update(obj) < 0)
        return (-1);
    if ((tmp = mmap(0, obj->size + SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0)) == MAP_FAILED)
        return (er(MALLOC, obj->filename));
    ft_memcpy(tmp, obj->ptr, obj->text_offset + obj->text_size);
    ft_memmove(tmp + obj->text_offset + obj->text_size + SIZE, obj->ptr + obj->text_offset + obj->text_size, obj->size - (obj->text_offset + obj->text_size));
    if (munmap(obj->ptr, obj->size) < 0)
		return (er(MUNMAP, obj->filename));
    obj->ptr = tmp;
    g_payloads[obj->payloadIndex].fencrypt(obj);
	g_payloads[obj->payloadIndex].finsert(obj, obj->text_offset + obj->text_size);
    if ((fd = open(BIN_NAME, O_CREAT | O_WRONLY, 0777)) == -1)
		return (er(OPEN_NEW, obj->filename));
    write(fd, obj->ptr, obj->size + SIZE);
    if (close(fd) == -1){
    	return (er(CLOSE, obj->filename));}
	return (0);
}