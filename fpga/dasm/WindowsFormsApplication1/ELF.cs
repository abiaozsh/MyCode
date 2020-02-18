using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace WindowsFormsApplication1
{
	public class ELF
	{

		public class Head
		{
			//typedef __u32        Elf32_Addr;
			//typedef __u16        Elf32_Half;
			//typedef __u32        Elf32_Off;
			//typedef __s32        Elf32_Sword;
			//typedef __u32        Elf32_Word;
			//#define EI_NIDENT        16
			//unsigned char        e_ident[EI_NIDENT];
			//Elf32_Half        e_type;
			//Elf32_Half        e_machine;
			//Elf32_Word        e_version;
			//Elf32_Addr        e_entry;  /* Entry point */
			//Elf32_Off        e_phoff;
			//Elf32_Off        e_shoff;
			//Elf32_Word        e_flags;
			//Elf32_Half        e_ehsize;
			//Elf32_Half        e_phentsize;
			//Elf32_Half        e_phnum;
			//Elf32_Half        e_shentsize;
			//Elf32_Half        e_shnum;
			//Elf32_Half        e_shstrndx;

			public byte[] e_ident; /* ELF Identification */

			/* These constants define the different elf file types */
			//#define ET_NONE   0
			//#define ET_REL    1
			//#define ET_EXEC   2
			//#define ET_DYN    3
			//#define ET_CORE   4
			//#define ET_LOPROC 0xff00
			//#define ET_HIPROC 0xffff
			public ushort e_type;		/*Elf32_Half object file type */
			public ushort e_machine;	/* machine */
			public uint e_version;	/* object file version */
			public uint e_entry;	/* virtual entry point */
			public uint e_phoff;	/* program header table offset */
			public uint e_shoff;	/* section header table offset */
			public uint e_flags;	/* processor-specific flags */
			public ushort e_ehsize;	/* ELF header size */
			public ushort e_phentsize;	/* program header entry size */
			public ushort e_phnum;	/* number of program header entries */
			public ushort e_shentsize;	/* section header entry size */
			public ushort e_shnum;	/* number of section header entries */
			public ushort e_shstrndx;	/* section header table's "section 
								   header string table" entry offset */

			public ProgHead[] pheads;
			public SecHead[] sheads;
		}

		public static Head ReadHead(BinaryReader br)
		{
			Head head = new Head();
			head.e_ident = br.ReadBytes(16);
			head.e_type = br.ReadUInt16();
			head.e_machine = br.ReadUInt16();
			head.e_version = br.ReadUInt32();
			head.e_entry = br.ReadUInt32();
			head.e_phoff = br.ReadUInt32();
			head.e_shoff = br.ReadUInt32();
			head.e_flags = br.ReadUInt32();
			head.e_ehsize = br.ReadUInt16();
			head.e_phentsize = br.ReadUInt16();
			head.e_phnum = br.ReadUInt16();
			head.e_shentsize = br.ReadUInt16();
			head.e_shnum = br.ReadUInt16();
			head.e_shstrndx = br.ReadUInt16();

			head.pheads = new ProgHead[head.e_phnum];
			for (int i = 0; i < head.e_phnum; i++)
			{
				br.BaseStream.Seek(head.e_phoff + i * head.e_phentsize, 0);
				head.pheads[i] = ReadProgHead(br);
			}

			head.sheads = new SecHead[head.e_shnum];
			for (int i = 0; i < head.e_shnum; i++)
			{
				br.BaseStream.Seek(head.e_shoff + i * head.e_shentsize, 0);
				head.sheads[i] = ReadSecHead(br);
			}


			return head;
		}


		public class ProgHead
		{
			//		typedef struct elf32_phdr{
			/* These constants are for the segment types stored in the image headers */
			//#define PT_NULL    0
			//#define PT_LOAD    1
			//#define PT_DYNAMIC 2
			//#define PT_INTERP  3
			//#define PT_NOTE    4
			//#define PT_SHLIB   5
			//#define PT_PHDR    6
			//#define PT_TLS     7               /* Thread local storage segment */
			//#define PT_LOOS    0x60000000      /* OS-specific */
			//#define PT_HIOS    0x6fffffff      /* OS-specific */
			//#define PT_LOPROC  0x70000000
			//#define PT_HIPROC  0x7fffffff
			//#define PT_GNU_EH_FRAME                0x6474e550
			//
			//#define PT_GNU_STACK        (PT_LOOS + 0x474e551)
			public uint p_type;//  Elf32_Word        p_type;
			public uint p_offset;//  Elf32_Off        p_offset;
			public uint p_vaddr;//  Elf32_Addr        p_vaddr;
			public uint p_paddr;//  Elf32_Addr        p_paddr;
			public uint p_filesz;//  Elf32_Word        p_filesz;
			public uint p_memsz;//  Elf32_Word        p_memsz;
			/* These constants define the permissions on sections in the program
			   header, p_flags. */
			//#define PF_R                0x4
			//#define PF_W                0x2
			//#define PF_X                0x1
			public uint p_flags;//  Elf32_Word        p_flags;
			public uint p_align;//  Elf32_Word        p_align;
			//} Elf32_Phdr;
			//
			public byte[] data;
			public string dasm;
		}

		public static ProgHead ReadProgHead(BinaryReader br)
		{
			ProgHead phead = new ProgHead();
			phead.p_type = br.ReadUInt32();
			phead.p_offset = br.ReadUInt32();
			phead.p_vaddr = br.ReadUInt32();
			phead.p_paddr = br.ReadUInt32();
			phead.p_filesz = br.ReadUInt32();
			phead.p_memsz = br.ReadUInt32();
			phead.p_flags = br.ReadUInt32();
			phead.p_align = br.ReadUInt32();

			br.BaseStream.Seek(phead.p_offset, 0);
			phead.data = br.ReadBytes((int)phead.p_filesz);
			phead.dasm = DASM.decode(phead);
			return phead;
		}

		public class SecHead
		{
			public uint sh_name;//Elf32_Word        sh_name;
			public uint sh_type;//Elf32_Word        sh_type;
			public uint sh_flags;//Elf32_Word        sh_flags;
			public uint sh_addr;//Elf32_Addr        sh_addr;
			public uint sh_offset;//Elf32_Off         sh_offset;
			public uint sh_size;//Elf32_Word        sh_size;
			public uint sh_link;//Elf32_Word        sh_link;
			public uint sh_info;//Elf32_Word        sh_info;
			public uint sh_addralign;//Elf32_Word        sh_addralign;
			public uint sh_entsize;//Elf32_Word        sh_entsize;
			public byte[] data;
			public string str;
		}

		public static SecHead ReadSecHead(BinaryReader br)
		{
			SecHead shead = new SecHead();
			shead.sh_name = br.ReadUInt32();//Elf32_Word        sh_name;
			shead.sh_type = br.ReadUInt32();//Elf32_Word        sh_type;
			shead.sh_flags = br.ReadUInt32();//Elf32_Word        sh_flags;
			shead.sh_addr = br.ReadUInt32();//Elf32_Addr        sh_addr;
			shead.sh_offset = br.ReadUInt32();//Elf32_Off         sh_offset;
			shead.sh_size = br.ReadUInt32();//Elf32_Word        sh_size;
			shead.sh_link = br.ReadUInt32();//Elf32_Word        sh_link;
			shead.sh_info = br.ReadUInt32();//Elf32_Word        sh_info;
			shead.sh_addralign = br.ReadUInt32();//Elf32_Word        sh_addralign;
			shead.sh_entsize = br.ReadUInt32();//Elf32_Word        sh_entsize;

			br.BaseStream.Seek(shead.sh_offset, 0);
			shead.data = br.ReadBytes((int)shead.sh_size);
			shead.str = System.Text.Encoding.ASCII.GetString(shead.data);
			return shead;
		}

	}
}
