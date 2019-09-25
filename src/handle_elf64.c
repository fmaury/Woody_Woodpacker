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
        printf("%lx - %lx = %lx\n", seg->p_vaddr, seg->p_offset, seg->p_vaddr - seg->p_offset);
        // printf("p align %ld  calc %ld\n", seg->p_align, seg->p_offset % seg->p_align);
        
        if (found)
        {
            // seg->p_offset += sizeof(Elf64_Phdr);
            // seg->p_vaddr += obj->payloadLen;
        }
        if (hdr->e_entry >= seg->p_vaddr && hdr->e_entry <= seg->p_vaddr + seg->p_filesz)
        {
            obj->txt_seghdr = seg;
            obj->text_addr = seg->p_vaddr;
            obj->text_offset = seg->p_offset;
            obj->text_size = seg->p_filesz;
            seg->p_memsz += obj->payloadLen;
            seg->p_filesz += obj->payloadLen;
            seg->p_flags |= PF_W;
            found = true;
        }
        seg->p_offset += sizeof(Elf64_Phdr);
        // seg->p_vaddr += sizeof(Elf64_Phdr);
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
    // if (sec->sh_flags == 0xDEADBEEF)
    //      return (er(ALR_PACKD, obj->filename));
    // sec->sh_flags = 0xDEADBEEF;
    while (i < hdr->e_shnum)
    {
        // if (sec->sh_addr >= obj->text_addr + obj->text_size)
        //     sec->sh_addr += obj->payloadLen;
        if (found)
        {
            // sec->sh_offset += SIZE;
        }
        if (hdr->e_entry >= sec->sh_addr && hdr->e_entry < sec->sh_addr + sec->sh_size)
        {
            obj->sc_addr = sec->sh_addr;
            obj->sc_offset = sec->sh_offset;
            obj->sc_size = sec->sh_size;
            obj->sc_index = i;
        }
        if (sec->sh_offset + sec->sh_size == obj->text_offset + obj->text_size)
        {
            sec->sh_size += obj->payloadLen;
            found = true;
        }
        sec->sh_offset += sizeof(Elf64_Phdr);
        // sec->sh_addr += sizeof(Elf64_Phdr);
        sec++;
        i++;
    }
    printf("entrysize %hd\n", hdr->e_phentsize);
    printf("shstrndx %hd\n", hdr->e_shstrndx);
    return (0);
}

int				handle_elf64(t_wdy *obj)
{
	int				fd;
    void            *tmp;
    Elf64_Ehdr      *hdr = (Elf64_Ehdr*)obj->ptr;
    Elf64_Ehdr      *newhdr;
    Elf64_Phdr      *newseg;
    Elf64_Shdr      *newsec;

    if (update(obj) < 0)
        return (-1);
    if ((tmp = mmap(0, obj->size + SIZE + sizeof(Elf64_Phdr) + sizeof(Elf64_Shdr), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0)) == MAP_FAILED)
        return (er(MMAP, obj->filename));
    if (!chk_ptr(obj, obj->ptr, obj->text_offset + obj->text_size))
        return (er(TRUNCATED, obj->filename));
    // copy header 
    ft_memcpy(tmp, obj->ptr, sizeof(Elf64_Ehdr) + hdr->e_phnum * sizeof(Elf64_Phdr));
    newhdr = (Elf64_Ehdr*)tmp;
    newhdr->e_shoff += sizeof(Elf64_Phdr);
    newhdr->e_phnum++;
    newhdr->e_shnum++;
    // copy new seg
    newseg = tmp + sizeof(Elf64_Ehdr) + hdr->e_phnum * sizeof(Elf64_Phdr);
    ft_memcpy(newseg, obj->txt_seghdr, sizeof(Elf64_Phdr));
    newseg->p_offset = obj->size + sizeof(Elf64_Phdr) + sizeof(Elf64_Shdr);
    newseg->p_vaddr = newseg->p_offset + (obj->text_addr - obj->text_offset);
    newseg->p_filesz = SIZE;
    newseg->p_memsz = SIZE;
    newseg->p_type = 0;
    // copy all segments /sections data
    ft_memcpy(newseg + 1, obj->ptr + sizeof(Elf64_Ehdr) + hdr->e_phnum * sizeof(Elf64_Phdr), obj->size - sizeof(Elf64_Ehdr) - hdr->e_phnum * sizeof(Elf64_Phdr));
    //add new sec
    newsec = (Elf64_Shdr *)(tmp + obj->size + sizeof(Elf64_Phdr));
    ft_memcpy(newsec, obj->ptr + hdr->e_shoff + sizeof(Elf64_Shdr) * obj->sc_index, sizeof(Elf64_Shdr));
    newsec->sh_addr = newseg->p_vaddr;
    newsec->sh_offset = newseg->p_offset;
    newsec->sh_size = obj->payloadLen;
    newsec->sh_name++;
    if (munmap(obj->ptr, obj->size) < 0)
		return (er(MUNMAP, obj->filename));
    obj->ptr = tmp;
    // newhdr->e_entry = newsec->sh_addr;
    // newhdr->e_entry += sizeof(Elf64_Phdr);
    printf("entry : %lx\n", newhdr->e_entry);
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