// This file is part of the lmake distribution (git@github.com:cesar-douady/open-lmake.git)
// Copyright (c) 2023 Doliam
// This program is free software: you can redistribute/modify under the terms of the GPL-v3 (https://www.gnu.org/licenses/gpl-3.0.html).
// This program is distributed WITHOUT ANY WARRANTY, without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "app.hh"
#include "rpc_job.hh"

void _print_map(::vmap_ss const& m) {
	size_t w = 0 ;
	for( auto const& [k,v] : m ) w = ::max(w,k.size()) ;
	for( auto const& [k,v] : m ) ::cout <<'\t'<< ::setw(w)<<k <<" : "<< v <<'\n' ;
}

template<class A> void _print_attrs(::vmap_s<A> const& m) {
	size_t w = 0 ;
	for( auto const& [k,v] : m ) w = ::max(w,to_string(v).size()) ;
	for( auto const& [k,v] : m ) ::cout <<'\t'<< ::setw(w)<<to_string(v) <<" : "<< k <<'\n' ;
}

void _print_vector(::vector_s const& v) {
	for( ::string const& x : v ) ::cout <<'\t'<< x <<'\n' ;
}

void print_req(::istream & s) {
	JobRpcReq jrr ; deserialize(s,jrr) ;
	SWEAR(jrr.proc==JobProc::Start) ;
	::cout << "--req--\n" ;
	::cout << "seq_id : " << jrr.seq_id <<'\n' ;
	::cout << "job    : " << jrr.job    <<'\n' ;
	::cout << "host   : " << jrr.host   <<'\n' ;
}

void print_start(::istream & s) {
	JobRpcReply jrr ; deserialize(s,jrr) ;
	SWEAR(jrr.proc==JobProc::Start) ;
	::cout << "--start--\n" ;
	::cout << "addr           : "  << hex<<  jrr.addr          <<dec <<'\n' ;
	::cout << "ancillary_file : "  <<        jrr.ancillary_file      <<'\n' ;
	::cout << "autodep_method : "  <<        jrr.autodep_method      <<'\n' ;
	::cout << "auto_mkdir     : "  <<        jrr.auto_mkdir          <<'\n' ;
	::cout << "ignore_stat    : "  <<        jrr.ignore_stat         <<'\n' ;
	::cout << "chroot         : "  <<        jrr.chroot              <<'\n' ;
	::cout << "hash_algo      : "  <<        jrr.hash_algo           <<'\n' ;
	::cout << "interpreter    : "  <<        jrr.interpreter         <<'\n' ;
	::cout << "is_python      : "  <<        jrr.is_python           <<'\n' ;
	::cout << "kill_sigs      : "  <<        jrr.kill_sigs           <<'\n' ;
	::cout << "live_out       : "  <<        jrr.live_out            <<'\n' ;
	::cout << "lnk_support    : "  <<        jrr.lnk_support         <<'\n' ;
	::cout << "reason         : "  <<        jrr.reason              <<'\n' ;
	::cout << "root_dir       : "  <<        jrr.root_dir            <<'\n' ;
	::cout << "small_id       : "  <<        jrr.small_id            <<'\n' ;
	::cout << "stdin          : "  <<        jrr.stdin               <<'\n' ;
	::cout << "stdout         : "  <<        jrr.stdout              <<'\n' ;
	::cout << "targets        : "  <<        jrr.targets             <<'\n' ;
	::cout << "timeout        : "  <<        jrr.timeout             <<'\n' ;
	//
	::cout << "rsrcs :\n"      ; _print_map   (jrr.rsrcs     )       ;
	::cout << "force_deps :\n" ; _print_vector(jrr.force_deps)       ;
	::cout << "env :\n"        ; _print_map   (jrr.env       )       ;
	::cout << "script :\n"     ; ::cout << indent(jrr.script) <<'\n' ;
}

void print_end(::istream & s) {
	JobRpcReq jrr ; deserialize(s,jrr) ;
	SWEAR(jrr.proc==JobProc::End) ;
	::cout << "--end--\n" ;
	::cout << "seq_id             : " << jrr.seq_id <<'\n' ;
	::cout << "job                : " << jrr.job    <<'\n' ;
	//
	JobDigest const & jd = jrr.digest ;
	::cout << "digest.status      : " << jd.status  <<'\n' ;
	//
	JobStats const& st = jd.stats ;
	::cout << "digest.stats.cpu   : " << st.cpu   <<'\n' ;
	::cout << "digest.stats.job   : " << st.job   <<'\n' ;
	::cout << "digest.stats.total : " << st.total <<'\n' ;
	::cout << "digest.stats.mem   : " << st.mem   <<'\n' ;
	//
	::cout << "digest.targets :\n" ; _print_attrs(jd.targets)            ;
	::cout << "digest.deps :\n"    ; _print_attrs(jd.deps   )            ;
	::cout << "digest.stderr :\n"  ; ::cout << indent(jd.stderr ) <<'\n' ;
}

void print_info(::istream & s) {
	JobInfo ji ; deserialize(s,ji) ;
	::cout << "--info--\n" ;
	::cout << "end_date : "  <<        ji.end_date  <<'\n' ;
	::cout << "wstatus  : "  <<        ji.wstatus   <<'\n' ;
	::cout << "stdout   :\n" << indent(ji.stdout  ) <<'\n' ;
}

int main( int argc , char* argv[] ) {
	//
	if (argc!=2) exit(2,"usage : ldump_job file") ;
	app_init(true/*search_root*/,true/*cd_root*/) ;
	::cout << ::left ;
	//
	IFStream job_stream{ argv[1] } ;
	print_req  (job_stream) ;
	print_start(job_stream) ;
	print_end  (job_stream) ;
	print_info (job_stream) ;
	//
	return 0 ;
}
