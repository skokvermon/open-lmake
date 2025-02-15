// This file is part of the lmake distribution (git@github.com:cesar-douady/open-lmake.git)
// Copyright (c) 2023 Doliam
// This program is free software: you can redistribute/modify under the terms of the GPL-v3 (https://www.gnu.org/licenses/gpl-3.0.html).
// This program is distributed WITHOUT ANY WARRANTY, without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#pragma once

#include "config.hh"

#include "disk.hh"
#include "hash.hh"
#include "lib.hh"
#include "serialize.hh"
#include "time.hh"

ENUM_1( DFlag      // flags for deps
,	Private = Lnk  // >=Private means flag cannot be seen in Lmakefile.py
,	Essential      // show when generating user oriented graphs
,	Error          // propagate error if dep is in error
,	Required       // accept if dep cannot be built
,	Lnk            // syscall sees link    content if dep is a link
,	Reg            // syscall sees regular content if dep is regular
,	Stat           // syscall sees inode   content (implied by other accesses)
)
using DFlags = BitMap<DFlag> ;
constexpr DFlags StaticDFlags = DFlags::All                             ;
constexpr DFlags HiddenDFlags = DFlag::Error                            ;
constexpr DFlags AccessDFlags { DFlag::Lnk , DFlag::Reg , DFlag::Stat } ;

ENUM( DepOrder
,	Parallel       // dep is parallel with prev ont
,	Seq            // dep is sequential
,	Critical       // dep starts a new critical section
)

ENUM( JobProc
,	None
,	Start
,	ReportStart
,	Continue       // req is killed but job is necessary for some other req
,	NotStarted     // req was killed before it actually started
,	ChkDeps
,	DepInfos
,	LiveOut
,	End
)

ENUM_1( JobReasonTag                   // see explanations in table below
,	HasNode = ClashTarget              // if >=HasNode, a node is associated
,	None
// with reason
,	ChkDeps
,	Cmd
,	Force
,	Garbage
,	Killed
,	Lost
,	New
,	OldError
,	Rsrcs
// with node
,	ClashTarget
,	DepChanged
,	DepNotReady
,	DepOutOfDate
,	NoTarget
,	PrevTarget
// with error
,	DepErr                             // if >=DepErr, job did not complete because of a dep
,	DepMissingStatic
,	DepMissingRequired
,	DepOverwritten
)
static constexpr const char* JobReasonTagStrs[] = {
	"no reason"                                            // None
// with reason
,	"dep check requires rerun"                             // ChkDeps
,	"command changed"                                      // Cmd
,	"job forced"                                           // Force
,	"job ran with unstable data"                           // Garbage
,	"job was killed"                                       // Killed
,	"job was lost"                                         // Lost
,	"job was never run"                                    // New
,	"job was in error"                                     // OldError
,	"resources changed and job was in error"               // Rsrcs
// with node
,	"multiple simultaneous writes"                         // ClashTarget
,	"dep changed"                                          // DepChanged
,	"dep not ready"                                        // DepNotReady
,	"dep out of date"                                      // DepOutOfDate
,	"target missing"                                       // NoTarget
,	"target previously existed"                            // PrevTarget
// with error
,	"dep in error"                                         // DepErr
,	"static dep missing"                                   // DepMissingStatic
,	"required dep missing"                                 // DepMissingRequired
,	"dep has been overwritten"                             // DepOverwritten
} ;
static_assert(sizeof(JobReasonTagStrs)/sizeof(const char*)==+JobReasonTag::N) ;

ENUM( Status       // result of job execution
,	New            // job was never run
,	Lost           // job was lost (it disappeared for an unknown reason)
,	Killed         // <=Killed means job was killed
,	ChkDeps        // dep check failed
,	Garbage        // <=Garbage means job has not run reliably
,	Ok             // job execution ended successfully
,	Frozen         // job behaves as a source
,	Err            // >=Err means job ended in error
,	ErrFrozen      // job is frozen in error
,	Timeout        // job timed out
,	SystemErr      // a system error occurrred during job execution
)

ENUM_2( TFlag                          // flags for targets
,	RuleOnly = Incremental             // if >=RuleOnly, flag may only appear in rule
,	Private  = N                       // >=Private means flag cannot be seen in Lmakefile.py
,	Crc                                // generate a crc for this target (compulsery if Match)
,	Dep                                // reads not followed by writes trigger dependencies
,	Essential                          // show when generating user oriented graphs
,	Phony                              // unlinks are allowed (possibly followed by reads which are ignored)
,	SourceOk                           // ok to overwrite source files
,	Stat                               // inode accesses (stat-like) are not ignored
,	Write                              // writes are allowed (possibly followed by reads which are ignored)
//
,	Incremental                        // reads   are allowed (before earliest write if any)
,	ManualOk                           // ok to overwrite manual files
,	Match                              // make target non-official (no match on it)
,	Star                               // target is a star target, even if no star stems
,	Warning                            // warn if target is unlinked and was generated by another rule
)
using TFlags = BitMap<TFlag> ;
static constexpr TFlags DfltTFlags      { TFlag::Dep , TFlag::Stat , TFlag::Crc , TFlag::Match , TFlag::Warning , TFlag::Write } ; // default flags for targets
static constexpr TFlags UnexpectedTFlags{ TFlag::Dep , TFlag::Stat , TFlag::Incremental , TFlag::Star                          } ; // flags used for accesses that are not targets

static inline void chk(TFlags flags) {
	if (flags[TFlag::Match]) {
		if ( flags[TFlag::Dep] ) throw "cannot match on target and be a potential dep"s       ;
		if (!flags[TFlag::Crc] ) throw "cannot match on target without computing checksum"s   ;
	}
	if (flags[TFlag::Star]) {
		if (flags[TFlag::Phony]) throw "phony star targets not yet supported"s ;
	}
}

struct JobStats {
	using Delay = Time::Delay ;
	// data
	Delay  cpu   ;
	Delay  job   ;                     // elapsed in job
	Delay  total ;                     // elapsed including overhead
	size_t mem   = 0 ;                 // in bytes
} ;

struct DepDigest {
	friend ::ostream& operator<<( ::ostream& , DepDigest const& ) ;
	using Date = Time::DiskDate ;
	// cxtors & casts
	DepDigest(                                  ) = default ;
	DepDigest( Date d                           ) : date{d} , garbage{false}                         {}
	DepDigest( Date d , DFlags dfs , DepOrder o ) : date{d} , garbage{false} , flags{dfs} , order{o} {}
	DepDigest(          DFlags dfs , DepOrder o ) :           garbage{true } , flags{dfs} , order{o} {}
	// data
	Date     date    ;                                     // if !garbage
	bool     garbage = true              ;
	DFlags   flags   ;
	DepOrder order   = DepOrder::Unknown ;
} ;

struct TargetDigest {
	friend ::ostream& operator<<( ::ostream& , TargetDigest const& ) ;
	using Crc  = Hash::Crc  ;
	// cxtors & casts
	TargetDigest(                                       ) = default ;
	TargetDigest( DFlags d , bool w , TFlags t , bool u ) : dfs{d} , write{w} , tfs{t} , crc{u?Crc::None:Crc::Unknown} {}
	// data
	DFlags dfs    ;                    // how target was accessed before it was written
	bool   write  = false ;            // if true <=> file was written (and possibly further unlinked)
	TFlags tfs    ;
	Crc    crc    ;                    // if None <=> file was unlinked, if Unknown <=> file is idle (not written, not unlinked)
} ;

struct JobDigest {
	friend ::ostream& operator<<( ::ostream& , JobDigest const& ) ;
	// services
	template<IsStream T> void serdes(T& s) {
		::serdes(s,status ) ;
		::serdes(s,targets) ;
		::serdes(s,deps   ) ;
		::serdes(s,stderr ) ;
		::serdes(s,stats  ) ;
	}
	// data
	Status                 status  = Status::New ;
	::vmap_s<TargetDigest> targets = {}          ;
	::vmap_s<DepDigest   > deps    = {}          ;
	::string               stderr  = {}          ;
	JobStats               stats   = {}          ;
} ;

struct JobRpcReq {
	using P   = JobProc             ;
	using S   = Status              ;
	using SI  = SeqId               ;
	using JI  = JobIdx              ;
	using MDD = ::vmap_s<DepDigest> ;
	friend ::ostream& operator<<( ::ostream& , JobRpcReq const& ) ;
	// cxtors & casts
	JobRpcReq() = default ;
	JobRpcReq( P p , SI ui , JI j , ::string const& h , in_port_t pt           ) : proc{p} , seq_id{ui} , job{j} , host{h} , port  {pt                  } { SWEAR( p==P::Start                     ) ; }
	JobRpcReq( P p , SI ui , JI j ,                     S s                    ) : proc{p} , seq_id{ui} , job{j} ,           digest{.status=s           } { SWEAR( p==P::End && s<=S::Garbage      ) ; }
	JobRpcReq( P p ,         JI j ,                     S s , ::string&& e     ) : proc{p} ,              job{j} ,           digest{.status=s,.stderr{e}} { SWEAR( p==P::End && s==S::Err          ) ; }
	JobRpcReq( P p , SI ui , JI j , ::string const& h , JobDigest const& d     ) : proc{p} , seq_id{ui} , job{j} , host{h} , digest{d                   } { SWEAR( p==P::End                       ) ; }
	JobRpcReq( P p , SI ui , JI j , ::string const& h , ::string_view const& t ) : proc{p} , seq_id{ui} , job{j} , host{h} , txt   {t                   } { SWEAR( p==P::LiveOut                   ) ; }
	JobRpcReq( P p , SI ui , JI j , ::string const& h , MDD const& ds          ) : proc{p} , seq_id{ui} , job{j} , host{h} , digest{.deps=ds            } { SWEAR( p==P::ChkDeps || p==P::DepInfos ) ; }
	// services
	template<IsStream T> void serdes(T& s) {
		if (::is_base_of_v<::istream,T>) *this = JobRpcReq() ;
		::serdes(s,proc  ) ;
		::serdes(s,seq_id) ;
		::serdes(s,job   ) ;
		::serdes(s,host  ) ;
		switch (proc) {
			case P::Start    : ::serdes(s,port  ) ; break ;
			case P::LiveOut  : ::serdes(s,txt   ) ; break ;
			case P::ChkDeps  :
			case P::DepInfos :
			case P::End      : ::serdes(s,digest) ; break ;
			default          : ;
		}
	}
	// data
	P         proc   = P::None ;
	SI        seq_id = 0       ;
	JI        job    = 0       ;
	::string  host   ;                 // if proc==Start
	in_port_t port   = 0       ;       // if proc==Start
	JobDigest digest ;                 // if proc==ChkDeps || DepInfos || End
	::string  txt    ;                 // if proc==LiveOut
} ;

struct JobReason {
	friend ::ostream& operator<<( ::ostream& , JobReason const& ) ;
	using Tag = JobReasonTag ;
	// cxtors & casts
	JobReason(                   ) = default ;
	JobReason( Tag t             ) : tag{t}           { SWEAR( t< Tag::HasNode       ) ; }
	JobReason( Tag t , NodeIdx n ) : tag{t} , node{n} { SWEAR( t>=Tag::HasNode && +n ) ; }
	// accesses
	bool operator+() const { return +tag              ; }
	bool operator!() const { return !tag              ; }
	bool has_err  () const { return tag>=Tag::DepErr  ; }
	// services
	JobReason operator|(JobReason jr) const {
		if (   has_err()) return *this ;
		if (jr.has_err()) return jr    ;
		if (+*this      ) return *this ;
		/**/              return jr    ;
	}
	JobReason& operator|=(JobReason jr) { *this = *this | jr ; return *this ; }
	// data
	Tag     tag  = JobReasonTag::None ;
	NodeIdx node = 0                  ;
} ;

struct TargetSpec {
	friend ::ostream& operator<<( ::ostream& , TargetSpec const& ) ;
	// cxtors & casts
	TargetSpec( ::string const& p={} , bool ins=false , TFlags f={} , ::vector<VarIdx> c={} ) : pattern{p} , is_native_star{ins} , flags{f} , conflicts{c} {
		if (is_native_star) SWEAR(flags[TFlag::Star]) ;
	}
	template<IsStream S> void serdes(S& s) {
		::serdes(s,pattern       ) ;
		::serdes(s,is_native_star) ;
		::serdes(s,flags         ) ;
		::serdes(s,conflicts     ) ;
	}
	// services
	bool operator==(TargetSpec const&) const = default ;
	// data
	::string         pattern        ;
	bool             is_native_star = false ;
	TFlags           flags          ;
	::vector<VarIdx> conflicts      ;                      // the idx of the previous targets that may conflict with this one
} ;

ENUM_2( AutodepMethod
,	Ld   = LdAudit                                         // >=Ld means a lib is pre-loaded (through LD_AUDIT or LD_PRELOAD)
,	Dflt =                                                 // by default, use most reliable available method
		HAS_PTRACE   ? AutodepMethod::Ptrace
	:	HAS_LD_AUDIT ? AutodepMethod::LdAudit
	:	               AutodepMethod::LdPreload
,	None
,	Ptrace
,	LdAudit
,	LdPreload
)

struct JobRpcReply {
	friend ::ostream& operator<<( ::ostream& , JobRpcReply const& ) ;
	using Crc  = Hash::Crc ;
	using Proc = JobProc   ;
	// cxtors & casts
	JobRpcReply(                                                    ) = default ;
	JobRpcReply( Proc p                                             ) : proc{p}             {                               }
	JobRpcReply( Proc p , Bool3 o                                   ) : proc{p} , ok{o}     { SWEAR(proc==Proc::ChkDeps ) ; }
	JobRpcReply( Proc p , ::vector<pair<Bool3/*ok*/,Crc>> const& is ) : proc{p} , infos{is} { SWEAR(proc==Proc::DepInfos) ; }
	// services
	template<IsStream S> void serdes(S& s) {
		if (is_base_of_v<::istream,S>) *this = JobRpcReply() ;
		::serdes(s,proc) ;
		switch (proc) {
			case Proc::None     :                     break ;
			case Proc::DepInfos : ::serdes(s,infos) ; break ;
			case Proc::ChkDeps  : ::serdes(s,ok   ) ; break ;
			case Proc::Start :
				::serdes(s,addr            ) ;
				::serdes(s,ancillary_file  ) ;
				::serdes(s,autodep_method  ) ;
				::serdes(s,auto_mkdir      ) ;
				::serdes(s,chroot          ) ;
				::serdes(s,cwd             ) ;
				::serdes(s,env             ) ;
				::serdes(s,force_deps      ) ;
				::serdes(s,hash_algo       ) ;
				::serdes(s,ignore_stat     ) ;
				::serdes(s,interpreter     ) ;
				::serdes(s,is_python       ) ;
				::serdes(s,job_tmp_dir     ) ;
				::serdes(s,keep_tmp        ) ;
				::serdes(s,kill_sigs       ) ;
				::serdes(s,live_out        ) ;
				::serdes(s,lnk_support     ) ;
				::serdes(s,reason          ) ;
				::serdes(s,remote_admin_dir) ;
				::serdes(s,root_dir        ) ;
				::serdes(s,rsrcs           ) ;
				::serdes(s,script          ) ;
				::serdes(s,small_id        ) ;
				::serdes(s,stdin           ) ;
				::serdes(s,stdout          ) ;
				::serdes(s,targets         ) ;
				::serdes(s,timeout         ) ;
			break ;
			default : FAIL(proc) ;
		}
	}
	// data
	Proc                      proc             = Proc::None          ;
	in_addr_t                 addr             = 0                   ;         // proc == Start   , the address at which server can contact job, it is assumed that it can be used by subprocesses
	::string                  ancillary_file   ;                               // proc == Start
	AutodepMethod             autodep_method   = AutodepMethod::None ;         // proc == Start
	bool                      auto_mkdir       = false               ;         // proc == Start   , if true <=> auto mkdir in case of chdir
	::string                  chroot           ;                               // proc == Start
	::string                  cwd              ;                               // proc == Start
	::vmap_ss                 env              ;                               // proc == Start
	::vector_s                force_deps       ;                               // proc == Start   , deps that may clash with targets
	Hash::Algo                hash_algo        = Hash::Algo::Unknown ;         // proc == Start
	bool                      ignore_stat      = false               ;         // proc == Start   , if true <=> stat-like syscalls do not trigger dependencies
	::vector_s                interpreter      ;                               // proc == Start   , actual interpreter used to execute script
	bool                      is_python        = false               ;         // proc == Start   , if true <=> script is a Python script
	::string                  job_tmp_dir      ;                               // proc == Start
	bool                      keep_tmp         = false               ;         // proc == Start
	vector<int>               kill_sigs        ;                               // proc == Start
	bool                      live_out         = false               ;         // proc == Start
	LnkSupport                lnk_support      = LnkSupport  ::None  ;         // proc == Start
	JobReason                 reason           = JobReasonTag::None  ;         // proc == Start
	::string                  remote_admin_dir ;                               // proc == Start
	::string                  root_dir         ;                               // proc == Start
	::vmap_ss                 rsrcs            ;                               // proc == Start
	::string                  script           ;                               // proc == Start
	SmallId                   small_id         = 0                   ;         // proc == Start
	::string                  stdin            ;                               // proc == Start
	::string                  stdout           ;                               // proc == Start
	::vector<TargetSpec>      targets          ;                               // proc == Start
	Time::Delay               timeout          ;                               // proc == Start
	Bool3                     ok               = Maybe               ;         // proc == ChkDeps , if No <=> deps in error, if Maybe <=> deps not ready
	::vector<pair<Bool3,Crc>> infos            ;                               // proc == DepInfos
} ;

struct JobInfo {
	friend ::ostream& operator<<( ::ostream& , JobInfo const& ) ;
	using Date = Time::ProcessDate ;
	template<IsStream T> void serdes(T& s) {
		::serdes(s,end_date) ;
		::serdes(s,stdout  ) ;
		::serdes(s,wstatus ) ;
	}
	// data
	Date     end_date ;
	::string stdout   ;
	int      wstatus  = 0 ;
} ;

ENUM( JobExecRpcProc
,	None
,	ChkDeps
,	CriticalBarrier
,	DepInfos
,	Heartbeat
,	Kill
,	Tmp                                // write activity in tmp has been detected (hence clean up is required)
,	Trace                              // no algorithmic info, just for tracing purpose
,	Access
)

struct JobExecRpcReq {
	friend ::ostream& operator<<( ::ostream& , JobExecRpcReq const& ) ;
	// make short lines
	using P   = JobExecRpcProc    ;
	using PD  = Time::ProcessDate ;
	using DD  = Time::DiskDate    ;
	using MDD = ::vmap_s<DD>      ;
	using S   = ::string          ;
	using VS  = ::vector_s        ;
	using DFs = DFlags            ;
	using TFs = TFlags            ;
	//
	struct AccessInfo {                                                        // order is read, then write, then unlink
		friend ::ostream& operator<<( ::ostream& , AccessInfo const& ) ;
		// accesses
		bool idle() const { return !write && !unlink ; }
		// services
		bool operator==(AccessInfo const& other) const = default ;             // XXX : why is this necessary at all ?!?
		AccessInfo operator|(AccessInfo const& other) const {                  // *this, then other
			return  {
				dfs                           |  (idle()?other.dfs:DFlags())   // if we have already written or unlinked, new reads are not recorded any more
			,	write                         || other.write
			,	( neg_tfs &  ~other.pos_tfs ) |  other.neg_tfs
			,	( pos_tfs &  ~other.neg_tfs ) |  other.pos_tfs
			,	( unlink  && !other.write   ) || other.unlink                  // if we write, we are not unlink any more
			} ;
		}
		AccessInfo& operator|=(AccessInfo const& other) { *this = *this | other ; return *this ; }
		// data
		DFs  dfs     = {}    ;         // if +dfs <=> files are read
		bool write   = false ;         // if true <=> files are written
		TFs  neg_tfs = {}    ;         // if write, removed TFlags
		TFs  pos_tfs = {}    ;         // if write, added   TFlags
		bool unlink  = false ;         // if true <=> files are unlinked
	} ;
	// statics
private :
	static MDD _s_mk_mdd(::vector_s const& fs) { MDD res ; for( ::string const& f : fs ) res.emplace_back(       f ,DD()) ; return res ; }
	static MDD _s_mk_mdd(::vector_s     && fs) { MDD res ; for( ::string      & f : fs ) res.emplace_back(::move(f),DD()) ; return res ; }
	// cxtors & casts
public :
	JobExecRpcReq(                S const& c={} ) :                     comment{c} {                       }
	JobExecRpcReq( P p , bool s , S const& c={} ) : proc{p} , sync{s} , comment{c} { SWEAR(!has_files()) ; }
	JobExecRpcReq( P p ,          S const& c={} ) : proc{p}           , comment{c} { SWEAR(!has_files()) ; }
	//
	JobExecRpcReq( P p , MDD&& fs , DFs dfs , S const& c={} ) : proc{p} , sync{true} ,                   files{          ::forward<MDD>(fs) } , info{.dfs=dfs} , comment{c} { SWEAR(p==P::DepInfos) ; }
	JobExecRpcReq( P p , VS && fs , DFs dfs , S const& c={} ) : proc{p} , sync{true} , auto_date{true} , files{_s_mk_mdd(::forward<VS >(fs))} , info{.dfs=dfs} , comment{c} { SWEAR(p==P::DepInfos) ; }
	//
	JobExecRpcReq( MDD&& fs , bool ad , AccessInfo const& ai , bool s , S const& c={} ) :
		proc     {P::Access         }
	,	sync     {s                 }
	,	auto_date{ad                }
	,	files    {::forward<MDD>(fs)}
	,	info     {ai                }
	,	comment  {c                 }
	{}
	JobExecRpcReq( MDD&& fs , AccessInfo const& ai , bool s , S const& c={} ) : JobExecRpcReq{          ::forward<MDD>(fs) ,false/*audo_date*/,ai,s            ,c} {}
	JobExecRpcReq( MDD&& fs , AccessInfo const& ai ,          S const& c={} ) : JobExecRpcReq{          ::forward<MDD>(fs) ,false/*audo_date*/,ai,false/*sync*/,c} {}
	JobExecRpcReq( VS && fs , AccessInfo const& ai , bool s , S const& c={} ) : JobExecRpcReq{_s_mk_mdd(::forward<VS >(fs)),true /*audo_date*/,ai,s            ,c} {}
	JobExecRpcReq( VS && fs , AccessInfo const& ai ,          S const& c={} ) : JobExecRpcReq{_s_mk_mdd(::forward<VS >(fs)),true /*audo_date*/,ai,false/*sync*/,c} {}
	//
	bool has_files() const { return proc==P::DepInfos || proc==P::Access ; }
	// services
public :
	template<IsStream T> void serdes(T& s) {
		if (::is_base_of_v<::istream,T>) *this = JobExecRpcReq() ;
		/**/              ::serdes(s,proc     ) ;
		/**/              ::serdes(s,date     ) ;
		/**/              ::serdes(s,sync     ) ;
		if (!has_files()) return ;
		/**/              ::serdes(s,auto_date) ;
		/**/              ::serdes(s,files    ) ;
		/**/              ::serdes(s,info     ) ;
		/**/              ::serdes(s,comment  ) ;
	}
	// data
	P          proc      = P::None     ;
	PD         date      = PD::s_now() ;                   // access date to reorder accesses during analysis
	bool       sync      = false       ;
	bool       auto_date = false       ;                   // if has_files(), if true <=> files must be solved and dates added by probing disk (for autodep internal use, not to be sent to job_exec)
	MDD        files     ;                                 // file date when accessed with +dfs and !auto_date to identify content
	AccessInfo info      ;
	S          comment   ;
} ;

struct JobExecRpcReply {
	friend ::ostream& operator<<( ::ostream& , JobExecRpcReply const& ) ;
	using Proc = JobExecRpcProc ;
	using Crc  = Hash::Crc      ;
	// cxtors & casts
	JobExecRpcReply(                                                    ) = default ;
	JobExecRpcReply( Proc p                                             ) : proc{p}             { SWEAR( proc!=Proc::ChkDeps && proc!=Proc::DepInfos ) ; }
	JobExecRpcReply( Proc p , Bool3 o                                   ) : proc{p} , ok{o}     { SWEAR( proc==Proc::ChkDeps                         ) ; }
	JobExecRpcReply( Proc p , ::vector<pair<Bool3/*ok*/,Crc>> const& is ) : proc{p} , infos{is} { SWEAR( proc==Proc::DepInfos                        ) ; }
	JobExecRpcReply( JobRpcReply const& jrr ) {
		switch (jrr.proc) {
			case JobProc::None     :                        proc = Proc::None     ;                     break ;
			case JobProc::ChkDeps  : SWEAR(jrr.ok!=Maybe) ; proc = Proc::ChkDeps  ; ok    = jrr.ok    ; break ;
			case JobProc::DepInfos :                        proc = Proc::DepInfos ; infos = jrr.infos ; break ;
			default : FAIL(jrr.proc) ;
		}
	}
	// services
	template<IsStream S> void serdes(S& s) {
		if (::is_base_of_v<::istream,S>) *this = JobExecRpcReply() ;
		::serdes(s,proc) ;
		switch (proc) {
			case Proc::ChkDeps  : ::serdes(s,ok   ) ; break ;
			case Proc::DepInfos : ::serdes(s,infos) ; break ;
			default : ;
		}
	}
	// data
	Proc                            proc  = Proc::None ;
	Bool3                           ok    = Maybe      ;   // if proc==ChkDeps
	::vector<pair<Bool3/*ok*/,Crc>> infos ;                // if proc==DepInfos
} ;
