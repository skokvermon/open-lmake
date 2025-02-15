// This file is part of the lmake distribution (git@github.com:cesar-douady/open-lmake.git)
// Copyright (c) 2023 Doliam
// This program is free software: you can redistribute/modify under the terms of the GPL-v3 (https://www.gnu.org/licenses/gpl-3.0.html).
// This program is distributed WITHOUT ANY WARRANTY, without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <link.h>   // all dynamic link related

#include "lib.hh"

bool        g_force_orig = false   ;
Lmid_t      g_libc_lmid  = 0       ;                                           // acquired during initial audit
const char* g_libc_name  = nullptr ;                                           // .


struct Ctx {
private :
	static void* _s_get_orig_errno() {
		void* s_libc_handle = ::dlmopen( g_libc_lmid , g_libc_name , RTLD_NOW|RTLD_NOLOAD ) ;
		swear_prod(s_libc_handle,"cannot find libc") ;
		Save  save { g_force_orig , true }                     ;               // avoid loop during dlsym execution
		void* res  = ::dlsym(s_libc_handle,"__errno_location") ;               // gather errno from app space // XXX : find a way to stick to documented interfaces
		swear_prod(res,"cannot find __errno_location in libc") ;
		return res ;
	}
public :
	int get_errno() const {
		static int* (*orig)() = reinterpret_cast<int* (*)()>(_s_get_orig_errno()) ;
		return *orig() ;
	}
	void save_errno   () {}
	void restore_errno() {}
} ;

struct Lock {
	static bool s_busy() { return false ; }
	static ::mutex s_mutex ;
	Lock () { s_mutex.lock  () ; }
	~Lock() { s_mutex.unlock() ; }
} ;
::mutex Lock::s_mutex ;

struct SymEntry {
	SymEntry(void* f,LnkSupport ls=LnkSupport::None) : func{f},lnk_support{ls} {}
	void*         func        = nullptr ;
	LnkSupport    lnk_support = LnkSupport::None ;         // above this level of link support, we need to catch this syscall
	mutable void* orig        = nullptr ;
} ;
extern ::umap_s<SymEntry> const g_syscall_tab ;

void* get_orig(const char* syscall) {
	if (!g_libc_name) exit(2,"cannot use autodep method ld_audit or ld_preload with statically linked libc") ;
	return g_syscall_tab.at(syscall).orig ;
}

#define LD_AUDIT 1
#include "autodep_ld.cc"

::umap_s<SymEntry> const g_syscall_tab = {
	{ "chdir"            , { reinterpret_cast<void*>(Audited::chdir            ) } }
,	{ "close"            , { reinterpret_cast<void*>(Audited::close            ) } }
,	{ "__close"          , { reinterpret_cast<void*>(Audited::__close          ) } }
,	{ "creat"            , { reinterpret_cast<void*>(Audited::creat            ) } }
,	{ "creat64"          , { reinterpret_cast<void*>(Audited::creat64          ) } }
//,	{ "dlmopen"          , { reinterpret_cast<void*>(Audited::dlmopen          ) } } // handled by la_objopen (calling Audited::dlmopen does not work for a mysterious reason)
//,	{ "dlopen"           , { reinterpret_cast<void*>(Audited::dlopen           ) } } // .
,	{ "dup2"             , { reinterpret_cast<void*>(Audited::dup2             ) } }
,	{ "dup3"             , { reinterpret_cast<void*>(Audited::dup3             ) } }
,	{ "execl"            , { reinterpret_cast<void*>(Audited::execl            ) } }
,	{ "execle"           , { reinterpret_cast<void*>(Audited::execle           ) } }
,	{ "execlp"           , { reinterpret_cast<void*>(Audited::execlp           ) } }
,	{ "execv"            , { reinterpret_cast<void*>(Audited::execv            ) } }
,	{ "execve"           , { reinterpret_cast<void*>(Audited::execve           ) } }
,	{ "execveat"         , { reinterpret_cast<void*>(Audited::execveat         ) } }
,	{ "execvp"           , { reinterpret_cast<void*>(Audited::execvp           ) } }
,	{ "execvpe"          , { reinterpret_cast<void*>(Audited::execvpe          ) } }
,	{ "fchdir"           , { reinterpret_cast<void*>(Audited::fchdir           ) } }
,	{ "fopen"            , { reinterpret_cast<void*>(Audited::fopen            ) } }
,	{ "fopen64"          , { reinterpret_cast<void*>(Audited::fopen64          ) } }
,	{ "freopen"          , { reinterpret_cast<void*>(Audited::freopen          ) } }
,	{ "freopen64"        , { reinterpret_cast<void*>(Audited::freopen64        ) } }
,	{ "link"             , { reinterpret_cast<void*>(Audited::link             ) } }
,	{ "linkat"           , { reinterpret_cast<void*>(Audited::linkat           ) } }
//,	{ "mkostemp"         , { reinterpret_cast<void*>(Audited::mkostemp         ) } } // normally only accesses $TMPDIR which is not tracked for deps (and difficult to implement)
//,	{ "mkostemp64"       , { reinterpret_cast<void*>(Audited::mkostemp64       ) } } // .
//,	{ "mkostemps"        , { reinterpret_cast<void*>(Audited::mkostemps        ) } } // .
//,	{ "mkostemps64"      , { reinterpret_cast<void*>(Audited::mkostemps64      ) } } // .
//,	{ "mkstemp"          , { reinterpret_cast<void*>(Audited::mkstemp          ) } } // .
//,	{ "mkstemp64"        , { reinterpret_cast<void*>(Audited::mkstemp64        ) } } // .
//,	{ "mkstemps"         , { reinterpret_cast<void*>(Audited::mkstemps         ) } } // .
//,	{ "mkstemps64"       , { reinterpret_cast<void*>(Audited::mkstemps64       ) } } // .
,	{ "open"             , { reinterpret_cast<void*>(Audited::open             ) } }
,	{ "__open"           , { reinterpret_cast<void*>(Audited::__open           ) } }
,	{ "__open_nocancel"  , { reinterpret_cast<void*>(Audited::__open_nocancel  ) } }
,	{ "__open_2"         , { reinterpret_cast<void*>(Audited::__open_2         ) } }
,	{ "open64"           , { reinterpret_cast<void*>(Audited::open64           ) } }
,	{ "__open64"         , { reinterpret_cast<void*>(Audited::__open64         ) } }
,	{ "__open64_nocancel", { reinterpret_cast<void*>(Audited::__open64_nocancel) } }
,	{ "__open64_2"       , { reinterpret_cast<void*>(Audited::__open64_2       ) } }
,	{ "openat"           , { reinterpret_cast<void*>(Audited::openat           ) } }
,	{ "__openat_2"       , { reinterpret_cast<void*>(Audited::__openat_2       ) } }
,	{ "openat64"         , { reinterpret_cast<void*>(Audited::openat64         ) } }
,	{ "__openat64_2"     , { reinterpret_cast<void*>(Audited::__openat64_2     ) } }
,	{ "readlink"         , { reinterpret_cast<void*>(Audited::readlink         ) } }
,	{ "readlinkat"       , { reinterpret_cast<void*>(Audited::readlinkat       ) } }
,	{ "__readlinkat_chk" , { reinterpret_cast<void*>(Audited::__readlinkat_chk ) } }
,	{ "__readlink_chk"   , { reinterpret_cast<void*>(Audited::__readlink_chk   ) } }
,	{ "rename"           , { reinterpret_cast<void*>(Audited::rename           ) } }
,	{ "renameat"         , { reinterpret_cast<void*>(Audited::renameat         ) } }
,	{ "renameat2"        , { reinterpret_cast<void*>(Audited::renameat2        ) } }
,	{ "symlink"          , { reinterpret_cast<void*>(Audited::symlink          ) } }
,	{ "symlinkat"        , { reinterpret_cast<void*>(Audited::symlinkat        ) } }
,	{ "truncate"         , { reinterpret_cast<void*>(Audited::truncate         ) } }
,	{ "truncate64"       , { reinterpret_cast<void*>(Audited::truncate64       ) } }
,	{ "unlink"           , { reinterpret_cast<void*>(Audited::unlink           ) } }
,	{ "unlinkat"         , { reinterpret_cast<void*>(Audited::unlinkat         ) } }
,	{ "vfork"            , { reinterpret_cast<void*>(Audited::vfork            ) } }
,	{ "__vfork"          , { reinterpret_cast<void*>(Audited::__vfork          ) } }
//
// mere path accesses, no actual accesses to file data
//
,	{ "access"    , { reinterpret_cast<void*>(Audited::access   ) , LnkSupport::File } }
,	{ "faccessat" , { reinterpret_cast<void*>(Audited::faccessat) , LnkSupport::File } }
,	{ "opendir"   , { reinterpret_cast<void*>(Audited::opendir  ) , LnkSupport::File } }
,	{ "rmdir"     , { reinterpret_cast<void*>(Audited::rmdir    ) , LnkSupport::File } }
,	{ "mkdir"     , { reinterpret_cast<void*>(Audited::mkdir    ) , LnkSupport::File } }
,	{ "mkdirat"   , { reinterpret_cast<void*>(Audited::mkdirat  ) , LnkSupport::File } }
,	{ "statx"     , { reinterpret_cast<void*>(Audited::statx    ) , LnkSupport::File } }
//
,	{ "__xstat"      , { reinterpret_cast<void*>(Audited::__xstat     ) , LnkSupport::File } }
,	{ "__xstat64"    , { reinterpret_cast<void*>(Audited::__xstat64   ) , LnkSupport::File } }
,	{ "__lxstat"     , { reinterpret_cast<void*>(Audited::__lxstat    ) , LnkSupport::Full } }
,	{ "__lxstat64"   , { reinterpret_cast<void*>(Audited::__lxstat64  ) , LnkSupport::Full } }
,	{ "__fxstatat"   , { reinterpret_cast<void*>(Audited::__fxstatat  ) , LnkSupport::File } }
,	{ "__fxstatat64" , { reinterpret_cast<void*>(Audited::__fxstatat64) , LnkSupport::File } }
#if !NEED_STAT_WRAPPERS
	,	{ "stat"      , { reinterpret_cast<void*>(Audited::stat     ) , LnkSupport::File } }
	,	{ "stat64"    , { reinterpret_cast<void*>(Audited::stat64   ) , LnkSupport::File } }
	,	{ "lstat"     , { reinterpret_cast<void*>(Audited::lstat    ) , LnkSupport::Full } }
	,	{ "lstat64"   , { reinterpret_cast<void*>(Audited::lstat64  ) , LnkSupport::Full } }
	,	{ "fstatat"   , { reinterpret_cast<void*>(Audited::fstatat  ) , LnkSupport::File } }
	,	{ "fstatat64" , { reinterpret_cast<void*>(Audited::fstatat64) , LnkSupport::File } }
#endif
,	{ "realpath"               , { reinterpret_cast<void*>(Audited::realpath              ) , LnkSupport::File } }
,	{ "__realpath_chk"         , { reinterpret_cast<void*>(Audited::__realpath_chk        ) , LnkSupport::File } }
,	{ "canonicalize_file_name" , { reinterpret_cast<void*>(Audited::canonicalize_file_name) , LnkSupport::File } }
,	{ "scandir"                , { reinterpret_cast<void*>(Audited::scandir               ) , LnkSupport::File } }
,	{ "scandir64"              , { reinterpret_cast<void*>(Audited::scandir64             ) , LnkSupport::File } }
,	{ "scandirat"              , { reinterpret_cast<void*>(Audited::scandirat             ) , LnkSupport::File } }
,	{ "scandirat64"            , { reinterpret_cast<void*>(Audited::scandirat64           ) , LnkSupport::File } }
} ;

static bool _catch(const char* c_name) {
	// search for string (.*/)?libc.so(.<number>)*
	static const char LibC      [] = "libc.so"       ;
	static const char LibPthread[] = "libpthread.so" ;                         // some systems redefine entries such as open in libpthread
	//
	::string_view name { c_name }               ;
	size_t        end  = 0/*garbage*/           ;
	size_t        pos  = name.rfind(LibC      ) ; if (pos!=NPos) { end = pos+sizeof(LibC      )-1 ; goto Qualify ; }
	/**/          pos  = name.rfind(LibPthread) ; if (pos!=NPos) { end = pos+sizeof(LibPthread)-1 ; goto Qualify ; }
	return false ;
Qualify :
	/**/                             if ( pos!=0 && name[pos-1]!='/' ) return false ;
	for( char c : name.substr(end) ) if ( (c<'0'||c>'9') && c!='.'   ) return false ;
	/**/                                                               return true  ;
}

template<class Sym> static inline uintptr_t _la_symbind( Sym* sym , unsigned int /*ndx*/ , uintptr_t* /*ref_cook*/ , uintptr_t* def_cook , unsigned int* /*flags*/ , const char* sym_name ) {
	Audit::t_audit() ;                                                         // force Audit static init
	//
	if (g_force_orig) goto Ignore ;                                            // avoid recursion loop
	if (*def_cook   ) goto Ignore ;                                            // cookie is used to identify libc (when cookie==0)
	//
	{	auto it = g_syscall_tab.find(sym_name) ;
		if (it==g_syscall_tab.end()) goto Ignore ;
		//
		SymEntry const& entry = it->second ;
		SWEAR(Audit::s_lnk_support!=LnkSupport::Unknown) ;
		if (Audit::s_lnk_support>=entry.lnk_support) goto Catch ;
		if (!Audit::s_ignore_stat                  ) goto Catch ;// we need to generate deps for stat-like accesses
		goto Ignore ;
	Catch :
		entry.orig = reinterpret_cast<void*>(sym->st_value) ;
		return reinterpret_cast<uintptr_t>(entry.func) ;
	}
Ignore :
	return sym->st_value ;
}

#pragma GCC visibility push(default)
extern "C" {

	unsigned int la_version(unsigned int /*version*/) { return LAV_CURRENT ; }

	unsigned int la_objopen( struct link_map* map , Lmid_t lmid , uintptr_t *cookie ) {
		Audit::t_audit() ;                                                              // force Audit static init
		if (!::string_view(map->l_name).starts_with("linux-vdso.so"))                   // linux-vdso.so is listed, but is not a real file
			Audit::read(AT_FDCWD,map->l_name,false/*no_follow*/,"la_objopen") ;
		bool catch_ = _catch(map->l_name) ;
		*cookie = !catch_ ;
		if (catch_) {
			g_libc_lmid = lmid        ;                                        // seems more robust to avoid directly calling dlmopen while in a call-back due to opening a dl
			g_libc_name = map->l_name ;
		}
		return LA_FLG_BINDFROM | (catch_?LA_FLG_BINDTO:0) ;
	}

	char* la_objsearch( const char* name , uintptr_t* /*cookie*/ , unsigned int flag ) {
		switch (flag) {
			case LA_SER_ORIG    : if (strrchr(name,'/')) { Lock lock ; Audit::read(AT_FDCWD,name) ; } break ;
			case LA_SER_LIBPATH :
			case LA_SER_RUNPATH :                        { Lock lock ; Audit::read(AT_FDCWD,name) ; } break ;
			default : ;
		}
		return const_cast<char*>(name) ;
	}

	uintptr_t la_symbind64(Elf64_Sym* s,unsigned int n,uintptr_t* rc,uintptr_t* dc,unsigned int* f,const char* sn) { return _la_symbind(s,n,rc,dc,f,sn) ; }
	uintptr_t la_symbind32(Elf32_Sym* s,unsigned int n,uintptr_t* rc,uintptr_t* dc,unsigned int* f,const char* sn) { return _la_symbind(s,n,rc,dc,f,sn) ; }

}
#pragma GCC visibility pop
