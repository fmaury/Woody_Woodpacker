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

static size_t       count_ptload(Elf64_Ehdr *hdr, Elf64_Phdr * seg)
{
    size_t          count = 0;
    size_t          i = 0;

    while (i < hdr->e_phnum)
    {
        if (seg->p_type == PT_LOAD)
            count++;
        seg++;
        i++;
    }
    return (count - 1);
}

static int updseg(t_wdy *obj, Elf64_Ehdr *hdr)
{
    Elf64_Phdr      *seg;
    unsigned int    i = 0;
    size_t          found = 0;
    size_t          pload;

    if (!chk_ptr(obj, obj->ptr + hdr->e_phoff, hdr->e_phnum * sizeof(Elf64_Phdr)))
        return (er(TRUNCATED, obj->filename));
    seg = obj->ptr + hdr->e_phoff;
    pload = count_ptload(hdr, seg);
    while (i < hdr->e_phnum)
    {
        if (!chk_ptr(obj, obj->ptr, seg->p_offset))
            return (er(TRUNCATED, obj->filename));
        if (hdr->e_entry >= seg->p_vaddr && hdr->e_entry <= seg->p_vaddr + seg->p_filesz)
        {
            obj->txt_seghdr = seg;
            obj->text_addr = seg->p_vaddr;
            obj->text_offset = seg->p_offset;
            obj->text_size = seg->p_filesz;
            seg->p_flags |= PF_W;
        }
        if (found >= pload)
            seg->p_offset += SIZE;
        if (seg->p_type == PT_LOAD)
            found++;
        if (found == pload)
        {
            obj->sg_addr = seg->p_vaddr;
            obj->sg_offset = seg->p_offset;
            obj->sg_size = seg->p_filesz;
            seg->p_memsz += obj->payloadLen;
            seg->p_filesz += obj->payloadLen;
            seg->p_flags |= PF_X;
        }
        i++;
        seg++;
    }
    return (0);
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
    if (sec->sh_flags == 0xDEADBEEF)
         return (er(ALR_PACKD, obj->filename));
    sec->sh_flags = 0xDEADBEEF;
    while (i < hdr->e_shnum)
    {
        if (!chk_ptr(obj, obj->ptr, sec->sh_offset))
            return (er(TRUNCATED, obj->filename));
        if (found)
            sec->sh_offset += SIZE;
        if (hdr->e_entry >= sec->sh_addr && hdr->e_entry < sec->sh_addr + sec->sh_size)
        {
            obj->sc_addr = sec->sh_addr;
            obj->sc_offset = sec->sh_offset;
            obj->sc_size = sec->sh_size;
            obj->sc_index = i;
        }
        if (sec->sh_offset + sec->sh_size == obj->sg_offset + obj->sg_size)
        {
            obj->lastsc_index = i + 1;
            found = true;
        }
        sec++;
        i++;
    }
    return (0);
}

int				handle_elf64(t_wdy *obj)
{
	int				fd;
    void            *tmp;
    Elf64_Ehdr      *hdr = (Elf64_Ehdr*)obj->ptr;
    Elf64_Ehdr      *newhdr;
    Elf64_Shdr      *newsec;

    if (update(obj) < 0)
        return (-1);
    if ((tmp = mmap(0, obj->size + SIZE + sizeof(Elf64_Shdr), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0)) == MAP_FAILED)
        return (er(MMAP, obj->filename));
    if (!chk_ptr(obj, obj->ptr, obj->sg_offset + obj->sg_size))
        return (er(TRUNCATED, obj->filename));
    // copy header 
    ft_memcpy(tmp, obj->ptr, obj->sg_offset + obj->sg_size);
    ft_memcpy(tmp, obj->ptr, sizeof(Elf64_Ehdr) + hdr->e_phnum * sizeof(Elf64_Phdr));
    newhdr = (Elf64_Ehdr*)tmp;
    newhdr->e_shoff += SIZE;
    newhdr->e_shnum++;
    newhdr->e_shstrndx++;
    // copy all segments /sections data /first until section header
    ft_memcpy(tmp + obj->sg_offset + obj->sg_size + SIZE, obj->ptr + obj->sg_offset + obj->sg_size, hdr->e_shoff + obj->lastsc_index * sizeof(Elf64_Shdr) - (obj->sg_offset + obj->sg_size));
    //add new sec
    newsec = (Elf64_Shdr *)(tmp + newhdr->e_shoff + obj->lastsc_index * sizeof(Elf64_Shdr));
    ft_memcpy(newsec, obj->ptr + hdr->e_shoff + sizeof(Elf64_Shdr) * obj->sc_index, sizeof(Elf64_Shdr));
    newsec->sh_addr = obj->sg_addr + obj->sg_size;
    newsec->sh_offset = obj->sg_offset + obj->sg_size;
    newsec->sh_size = obj->payloadLen;
    newsec->sh_name++;
    // join last sections
    ft_memcpy(newsec + 1, obj->ptr + hdr->e_shoff + obj->lastsc_index * sizeof(Elf64_Shdr), obj->size - (hdr->e_shoff + obj->lastsc_index * sizeof(Elf64_Shdr)));
    if (munmap(obj->ptr, obj->size) < 0)
		return (er(MUNMAP, obj->filename));
    obj->ptr = tmp;
    newhdr->e_entry = newsec->sh_addr;
    g_payloads[obj->payloadIndex].fencrypt(obj);
	g_payloads[obj->payloadIndex].finsert(obj, newsec->sh_offset);
    if ((fd = open(BIN_NAME, O_CREAT | O_WRONLY, 0777)) == -1)
		return (er(OPEN_NEW, obj->filename));
    write(fd, obj->ptr, obj->size + SIZE + sizeof(Elf64_Phdr) + sizeof(Elf64_Shdr));
    if (close(fd) == -1) 
    	return (er(CLOSE, obj->filename));
    // ...
	return (0);
}