// EPOS Run-Time System Information Implementation

#include <utility/debug.h>
#include <system/info.h>

__BEGIN_SYS

OStream & operator<<(OStream & os, const System_Info & si)
{
    os << "{"
       << "\nBoot_Map={"
       << "n_cpus=" << si.bm.n_cpus
       << ",mem_base=" << reinterpret_cast<void *>(si.bm.mem_base)
       << ",mem_top=" << reinterpret_cast<void *>(si.bm.mem_top)
       << ",io_base=" << reinterpret_cast<void *>(si.bm.mio_base)
       << ",io_top=" << reinterpret_cast<void *>(si.bm.mio_top)
       << ",node_id=" << si.bm.node_id
       << ",space=(" << si.bm.space_x << "," << si.bm.space_y << "," << si.bm.space_z << ")"
       << ",uuid=" << si.bm.uuid
       << ",img_size=" << si.bm.img_size
       << ",setup_offset=" << hex << si.bm.setup_offset
       << ",init_offset=" << si.bm.init_offset
       << ",system_offset=" << si.bm.system_offset
       << ",application_offset=" << si.bm.application_offset
       << ",extras_offset=" << si.bm.extras_offset << dec
       << "}"

#if defined(__pc__) || defined(__riscv__) || defined(__cortex_a53__)
       << "\nPhysical_Memory_Map={"
       << "sys_info=" << reinterpret_cast<void *>(si.pmm.sys_info)
#ifdef __pc__
       << ",idt=" << reinterpret_cast<void *>(si.pmm.idt)
       << ",gdt=" << reinterpret_cast<void *>(si.pmm.gdt)
       << ",tss=" << reinterpret_cast<void *>(si.pmm.tss)
#endif
       << ",sys_pt=" << reinterpret_cast<void *>(si.pmm.sys_pt)
       << ",sys_pd=" << reinterpret_cast<void *>(si.pmm.sys_pd)
       << ",phy_mem_pts=" << reinterpret_cast<void *>(si.pmm.phy_mem_pts)
       << ",io_pts=" << reinterpret_cast<void *>(si.pmm.io_pts)
       << ",usr_mem_base=" << reinterpret_cast<void *>(si.pmm.usr_mem_base)
       << ",usr_mem_top=" << reinterpret_cast<void *>(si.pmm.usr_mem_top)
       << ",sys_code=" << reinterpret_cast<void *>(si.pmm.sys_code)
       << ",sys_data=" << reinterpret_cast<void *>(si.pmm.sys_data)
       << ",sys_stack=" << reinterpret_cast<void *>(si.pmm.sys_stack)
       << ",app_code=" << reinterpret_cast<void *>(si.pmm.app_code)
       << ",app_data=" << reinterpret_cast<void *>(si.pmm.app_data)
       << ",app_extra=" << reinterpret_cast<void *>(si.pmm.app_extra)
       << ",free1_base=" << reinterpret_cast<void *>(si.pmm.free1_base)
       << ",free1_top=" << reinterpret_cast<void *>(si.pmm.free1_top)
       << ",free2_base=" << reinterpret_cast<void *>(si.pmm.free2_base)
       << ",free2_top=" << reinterpret_cast<void *>(si.pmm.free2_top)
       << ",free3_base=" << reinterpret_cast<void *>(si.pmm.free3_base)
       << ",free3_top=" << reinterpret_cast<void *>(si.pmm.free3_top)
       << "}"
#endif

       << "\nLoad_Map={"
#if defined(__pc__) || defined(__riscv__) || defined(__cortex_a53__)
       << "has_stp=" << si.lm.has_stp
       << ",has_ini=" << si.lm.has_ini
       << ",has_sys=" << si.lm.has_sys
       << ",has_app=" << si.lm.has_app
       << ",has_ext=" << si.lm.has_ext
       << ",stp_entry=" << reinterpret_cast<void *>(si.lm.stp_entry)
       << ",stp_segments=" << si.lm.stp_segments
       << ",stp_code={b=" << reinterpret_cast<void *>(si.lm.stp_code) << ",s=" << si.lm.stp_code_size << "}"
       << ",stp_data={b=" << reinterpret_cast<void *>(si.lm.stp_data) << ",s=" << si.lm.stp_data_size << "}"
       << ",ini_entry=" << reinterpret_cast<void *>(si.lm.ini_entry)
       << ",ini_segments=" << si.lm.ini_segments
       << ",ini_code={b=" << reinterpret_cast<void *>(si.lm.ini_code) << ",s=" << si.lm.ini_code_size << "}"
       << ",ini_data={b=" << reinterpret_cast<void *>(si.lm.ini_data) << ",s=" << si.lm.ini_data_size << "}"
       << ",sys_entry=" << reinterpret_cast<void *>(si.lm.sys_entry)
       << ",sys_segments=" << si.lm.sys_segments
       << ",sys_code={b=" << reinterpret_cast<void *>(si.lm.sys_code) << ",s=" << si.lm.sys_code_size << "}"
       << ",sys_data={b=" << reinterpret_cast<void *>(si.lm.sys_data) << ",s=" << si.lm.sys_data_size << "}"
       << ",sys_stack=" << reinterpret_cast<void *>(si.lm.sys_stack)
       << ",app_entry=" << reinterpret_cast<void *>(si.lm.app_entry)
       << ",app_segments=" << si.lm.app_segments
       << ",app_code={b=" << reinterpret_cast<void *>(si.lm.app_code) << ",s=" << si.lm.app_code_size << "}"
       << ",app_data={b=" << reinterpret_cast<void *>(si.lm.app_data) << ",s=" << si.lm.app_data_size << "}"
       << ",app_stack=" << reinterpret_cast<void *>(si.lm.app_stack)
       << ",app_heap=" << reinterpret_cast<void *>(si.lm.app_heap)
#endif
       << ",app_extra={b=" << reinterpret_cast<void *>(si.lm.app_extra) << ",s=" << si.lm.app_extra_size << "}"
       << "}"
       << "}";

    return os;
}

__END_SYS

