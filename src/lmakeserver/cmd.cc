// This file is part of the lmake distribution (git@github.com:cesar-douady/open-lmake.git)
// Copyright (c) 2023 Doliam
// This program is free software: you can redistribute/modify under the terms of the GPL-v3 (https://www.gnu.org/licenses/gpl-3.0.html).
// This program is distributed WITHOUT ANY WARRANTY, without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "core.hh"

using namespace Disk ;

namespace Engine {

	CmdFunc g_cmd_tab[+ReqProc::N] ;
	static bool _inited = (                                                    // PER_CMD : add an entry to point to the function actually executing your command (use show as a template)
		g_cmd_tab[+ReqProc::Forget] = forget
	,	g_cmd_tab[+ReqProc::Freeze] = freeze
	,	g_cmd_tab[+ReqProc::Show  ] = show
	,	true
	) ;

	bool/*ok*/ forget( Fd fd , ReqOptions const& ro , ::vector<Node> const& targets ) {
		bool ok = true ;
		switch (ro.key) {
			case ReqKey::None :
				for( Node t : targets ) ok &= t.forget() ;
			break ;
			case ReqKey::Error :
				g_store.invalidate_exec(true/*cmd_ok*/) ;
			break ;
			case ReqKey::Resources :
				for( Rule r : Rule::s_lst() ) {
					if (r->cmd_gen==r->rsrcs_gen) continue ;
					r.rule_data().cmd_gen = r->rsrcs_gen ;
					r.stamp() ;                                                          // we have modified rule, we must record it to make modif persistent
					audit( fd , ro , Color::Note , 0 , to_string("refresh ",r->name) ) ;
				}
			break ;
			default : FAIL(ro.key) ;
		}
		return ok ;
	}

	static void _freeze( Fd fd , ReqOptions const& ro , Job j , bool add , size_t w ) {
		Trace trace("_freeze_job",STR(add),j) ;
		SWEAR(j.active()) ;
		j->status = add ? Status::Frozen : Status::New ;
		audit( fd , ro  , add?Color::Warning:Color::Note , 0 , to_string(add?"froze job ":"melted job ",::setw(w),j->rule->user_name(),' ') , j.name() ) ;
	}
	static void _freeze( Fd fd , ReqOptions const& ro , Node t , bool add ) {
		Trace trace("_freeze_node",STR(add),t) ;
		if (add) {
			::string     tn  = t.name() ;
			UNode        un  { t      } ;
			FileInfoDate fid { tn     } ;
			t .mk_src() ;
			un.refresh( fid.tag==FileTag::Lnk , Crc(tn,g_config.hash_algo) , fid.date ) ;
			un->actual_job_tgt->status = Status::Frozen ;
		} else {
			t.mk_no_src() ;
		}
		audit( fd , ro  , add?Color::Warning:Color::Note , 0 , add?"froze file":"melted file" , t.name() ) ;
	}
	bool/*ok*/ freeze( Fd fd , ReqOptions const& ro , ::vector<Node> const& targets ) {
		bool do_job  = ro.flags[ReqFlag::Job  ] ;
		bool do_node = ro.flags[ReqFlag::File ] ;
		bool force   = ro.flags[ReqFlag::Force] ;
		bool add     = ro.key==ReqKey::Add     ;
		bool lst     = ro.key==ReqKey::List    ;
		bool glb     = false                   ;
		Trace trace("freeze",ro,targets) ;
		switch (ro.key) {
			case ReqKey::DeleteAll :
			case ReqKey::List      : glb = true  ; break ;
			default : ;
		}
		if (glb) SWEAR(targets.empty()   ) ;
		/**/     SWEAR(!(do_job&&do_node)) ;

		if (glb) {
			if (lst) {
				if (!do_node) {
					size_t w = 0 ;
					::vector<Job> frozens = Job::s_frozens() ;
					trace("job_list",frozens) ;
					for( Job j : frozens ) w = ::max(w,j->rule->user_name().size()) ;
					audit(fd,ro,Color::Note,0,"jobs :") ;
					for( Job j : frozens ) audit(fd,ro,Color::Warning,1,to_string(::setw(w),j->rule->user_name()),j.user_name()) ;
				}
				if (!do_job) {
					::vector<Node> frozens = Node::s_frozens() ;
					trace("node_list",frozens) ;
					audit(fd,ro,Color::Note,0,"files :") ;
					for( Node t : frozens ) audit(fd,ro,Color::Warning,1,{},t.name()) ;
				}
			} else {
				if (!do_node) {
					size_t w = 0 ;
					for( Job j : Job::s_frozens() ) w = ::max(w,j->rule->user_name().size()) ;
					for( Job j : Job::s_frozens() ) _freeze(fd,ro,j,false/*add*/,w) ;
					Job::s_frozens({}) ;
				}
				if (!do_job) {
					for( Node t : Node::s_frozens() ) _freeze(fd,ro,t,false/*add*/) ;
					Node::s_frozens({}) ;
				}
			}
		} else {
			::string       job_err         ;
			::string       node_err        ;
			Node           job_err_target  ;
			Node           node_err_target ;
			::vector<Node> to_do           ; to_do.reserve(targets.size()) ;
			size_t         w               = 0 ;
			//check
			if (!do_node) {
				for( Node t : targets ) {
					Job j = t->actual_job_tgt ;
					if ( !j.active() || t->is_src() )                                                  { job_err = "job not found"                              ; job_err_target = t ; break    ; }
					if ( add                        ) for( Req r [[maybe_unused]] : j.running_reqs() ) { job_err = "job is running"                             ; job_err_target = t ; break    ; }
					if ( !force && !t->conform()    )                                                  { job_err = "target was not produced by its offical job" ; job_err_target = t ; break    ; }
					if ( (j->frozen())==add         )                                                  { trace("already_done",j->status,j) ;                                           continue ; }
					to_do.push_back(t) ;
					w = ::max( w , j->rule->user_name().size() ) ;
				}
			}
			if (!do_job) {
				if (add) {
					for( Node t : targets ) {
						Job j = t->actual_job_tgt ;
						if (!j.active()) { to_do.push_back(t) ;                                                                     continue ; }
						if (j->frozen()) { trace("already_frozen",t) ;                                                              continue ; }
						if (t->is_src()) { node_err =           "file is a source"                          ; node_err_target = t ; break    ; }
						else             { node_err = to_string("file is produced by",j->rule->user_name()) ; node_err_target = t ; break    ; }
					}
				} else {
					for( Node t : targets ) {
						Job j = t->actual_job_tgt ;
						if (!j.active() ) { trace("already_melted",j,t) ;                                                                   continue ; }
						if (!j->frozen()) { trace("already_melted",j,t) ;                                                                   continue ; }
						if (!t->is_src()) { node_err = to_string("file is produced by frozen",j->rule->user_name()) ; node_err_target = t ; break    ; }
						to_do.push_back(t) ;
					}
				}
			}
			if (to_do.empty()) { trace("empty") ; return true ;}               // nothing to do
			// report error if any
			SWEAR( +job_err_target || +node_err_target ) ;                     // there are never any choice
			//
			do_job  |= !do_node && !job_err_target  ;
			do_node |= !do_job  && !node_err_target ;
			SWEAR(!( do_job && do_node )) ;
			if ( do_node && Req::s_n_reqs() ) { audit(fd,ro,Color::Err,0,"cannot freeze/melt files while running") ; return false ; }
			bool err = false ;
			if ( +job_err_target  && !do_node ) { audit(fd,ro,Color::Err,0,to_string(job_err ," :"),job_err_target .name()) ; err = true ; }
			if ( +node_err_target && !do_job  ) { audit(fd,ro,Color::Err,0,to_string(node_err," :"),node_err_target.name()) ; err = true ; }
			if ( err                          ) { trace("both_err",job_err_target,node_err_target) ; return false ;                        }
			// do what is asked
			if (do_job) {
				::uset<Job> frozens = mk_uset(Job::s_frozens()) ;
				for( Node t : to_do ) {
					Job j = t->actual_job_tgt ;
					_freeze(fd,ro,j,add,w) ;
					if (add) frozens.insert(j) ;
					else     frozens.erase (j) ;
				}
				Job::s_frozens(mk_vector(frozens)) ;
			}
			if (do_node) {
				::uset<Node> frozens = mk_uset(Node::s_frozens()) ;
				for( Node t : to_do ) {
					_freeze(fd,ro,t,add) ;
					if (add) frozens.insert(t) ;
					else     frozens.erase (t) ;
				}
				Node::s_frozens(mk_vector(frozens)) ;
				EngineStore::s_invalidate_match() ;                            // seen from the engine, we have modified sources, we must rematch everything
			}
		}
		return true ;
	}

	static void _send_node( Fd fd , ReqOptions const& ro , bool always , Bool3 hide , ::string const& pfx , Node node , DepDepth lvl=0 ) {
		Color color = Color::None ;
		if      ( hide==Yes             ) color =                         Color::HiddenNote ;
		else if (!node->has_actual_job()) color = hide==No ? Color::Err : Color::HiddenNote ;
		else if ( node->err()           ) color =            Color::Err                     ;
		//
		if ( always || color!=Color::HiddenNote ) audit( fd , ro , color , lvl , pfx , node.name() ) ;
	}

	static void _send_job( Fd fd , ReqOptions const& ro , Bool3 show_deps , bool hide , Job job , DepDepth lvl=0 ) {
		Color color = Color::None ;
		Rule  rule  = job->rule   ;
		if      (hide                   ) color = Color::HiddenNote ;
		else if (job->status==Status::Ok) color = Color::Ok         ;
		else if (job->frozen()          ) color = Color::Warning    ;
		else                              color = Color::Err        ;
		audit( fd , ro , color , lvl , rule->user_name() , job.user_name() ) ;
		if (show_deps==No) return ;
		NodeIdx critical_lvl = 0 ;
		size_t  w            = 0 ;
		for( VarIdx d=0 ; d<rule->n_deps() ; d++ ) w = ::max( w , rule->deps.dct[d].first.size() ) ;
		for( NodeIdx d=0 ; d<job->deps.size() ; d++ ) {
			Dep const& dep = job->deps[d] ;
			DepOrder   cdo = d  >0                ? dep           .order : DepOrder::Seq ;
			DepOrder   ndo = d+1<job->deps.size() ? job->deps[d+1].order : DepOrder::Seq ;
			if (cdo==DepOrder::Critical) critical_lvl++ ;
			::string pfx = to_string( ::setw(w) , d<rule->n_deps()?rule->deps.dct[d].first:""s , ' ' ) ;
			if      ( cdo!=DepOrder::Parallel && ndo!=DepOrder::Parallel ) pfx.push_back(' ' ) ;
			else if ( cdo!=DepOrder::Parallel && ndo==DepOrder::Parallel ) pfx.push_back('/' ) ;
			else if ( cdo==DepOrder::Parallel && ndo==DepOrder::Parallel ) pfx.push_back('|' ) ;
			else                                                           pfx.push_back('\\') ;
			_send_node( fd , ro , show_deps==Yes , (Maybe&!dep.flags[DFlag::Required])|hide , pfx , dep , lvl+1+critical_lvl ) ;
		}
	}

	bool/*ok*/ show( Fd fd , ReqOptions const& ro , ::vector<Node> const& targets ) {
		Trace trace("show",ro.key) ;
		bool ok = true ;
		for( Node target : targets ) {
			trace("target",target) ;
			DepDepth lvl = 0 ;
			if (targets.size()>1) {
				_send_node( fd , ro , true/*always*/ , Maybe/*hide*/ , {} , target ) ;
				lvl++ ;
			}
			JobTgt jt = target->actual_job_tgt ;
			if (!jt.active()) {
				jt = target->makable() ? target->conform_job_tgt() : JobTgt() ;
				if (!jt.active()) {
					audit( fd , ro , Color::Err  , lvl   , "target not built"                ) ;
					audit( fd , ro , Color::Note , lvl+1 , "consider : lmake", target.name() ) ;
					ok = false ;
					continue ;
				}
			}
			Rule rule = jt->rule ;
			trace("target",target,jt) ;
			switch (ro.key) {
				case ReqKey::Env        :
				case ReqKey::ExecScript :
				case ReqKey::Info       :
				case ReqKey::Script     :
				case ReqKey::Stderr     :
				case ReqKey::Stdout     : {
					if (rule.is_special()) {
						switch (ro.key) {
							case ReqKey::Info :
							case ReqKey::Stderr : {
								_send_job( fd , ro , No/*show_deps*/ , false/*hide*/ , jt , lvl ) ;
								audit    ( fd , ro , Color::None , lvl+1 , jt.special_stderr()  ) ;
							} break ;
							case ReqKey::ExecScript :
							case ReqKey::Script     :
							case ReqKey::Stdout     :
								_send_job( fd , ro , No/*show_deps*/ , false/*hide*/ , jt , lvl               ) ;
								audit    ( fd , ro , Color::Err , lvl+1 , "no "+mk_snake(ro.key)+" available" ) ;
							break ;
							default : FAIL(ro.key) ;
						}
					} else {
						::ifstream job_stream{ jt.ancillary_file() } ;
						//
						JobRpcReq   report_req   ; bool has_req   = false ;
						JobRpcReply report_start ; bool has_start = false ;
						JobRpcReq   report_end   ; bool has_end   = false ;
						JobInfo     report_info  ; bool has_info  = false ;
						try { deserialize(job_stream,report_req  ) ; has_req   = true ; } catch (...) { goto Go ; }
						try { deserialize(job_stream,report_start) ; has_start = true ; } catch (...) { goto Go ; }
						try { deserialize(job_stream,report_end  ) ; has_end   = true ; } catch (...) { goto Go ; }
						try { deserialize(job_stream,report_info ) ; has_info  = true ; } catch (...) { goto Go ; }
					Go :
						switch (ro.key) {
							case ReqKey::Env : {
								size_t w = 0 ;
								if (!has_start) { audit( fd , ro , Color::Err , lvl , "no info available" ) ; break ; }
								for( auto const& [k,v] : report_start.env ) w = max(w,k.size()) ;
								for( auto const& [k,v] : report_start.env ) audit( fd , ro , Color::None , lvl , to_string(::setw(w),k," : ",v) ) ;
							} break ;
							case ReqKey::ExecScript : {
								if (!has_start) { audit( fd , ro , Color::Err , 0 , "no info available" ) ; break ; }
								::string script = "exec env -i\\\n" ;
								for( auto const& [k,v] : report_start.env ) append_to_string(script,'\t',k,'=',mk_shell_str(v),"\\\n") ;
								//
								append_to_string( script , "\tTMPDIR="      , mk_shell_str(report_start.job_tmp_dir) , "\\\n" ) ;
								append_to_string( script , "\tROOT_DIR="    , mk_shell_str(*g_root_dir             ) , "\\\n" ) ;
								append_to_string( script , "\tSEQUENCE_ID=" , to_string   (report_req  .seq_id     ) , "\\\n" ) ;
								append_to_string( script , "\tSMALL_ID="    , to_string   (report_start.small_id   ) , "\\\n" ) ;
								//
								for( ::string const& c : report_start.interpreter ) append_to_string(script,      mk_shell_str(c                  ),' ') ;
								/**/                                                append_to_string(script,"-c ",mk_shell_str(report_start.script)    ) ;
								audit( fd , ro , Color::None , 0 , script ) ;
							} break ;
							case ReqKey::Script : {
								if (!has_start) { audit( fd , ro , Color::Err , lvl , "no info available" ) ; break ; }
								::string hdr = "#!" ;
								::string sep ; sep.reserve(1) ;
								for( ::string const& c : report_start.interpreter ) { hdr+=sep ; hdr+=c ; sep=" " ; } hdr+='\n' ;
								audit( fd , ro , Color::None , lvl , hdr                 ) ;
								audit( fd , ro , Color::None , lvl , report_start.script ) ;
							} break ;
							case ReqKey::Stdout :
								if (!has_info) { audit( fd , ro , Color::Err , lvl , "no info available" ) ; break ; }
								_send_job( fd , ro , No/*show_deps*/ , false/*hide*/ , jt , lvl ) ;
								audit    ( fd , ro , Color::None , lvl+1 , report_info.stdout   ) ;
							break ;
							case ReqKey::Stderr :
								if (!has_end) { audit( fd , ro , Color::Err , lvl , "no info available" ) ; break ; }
								_send_job( fd , ro , No/*show_deps*/ , false/*hide*/ , jt , lvl     ) ;
								audit    ( fd , ro , Color::None , lvl+1 , report_end.digest.stderr ) ;
							break ;
							case ReqKey::Info : {
								int      ws  = report_info.wstatus                                                                                                               ;
								::string rc  = WIFEXITED(ws) ? to_string("exited ",WEXITSTATUS(ws)) : WIFSIGNALED(ws) ? to_string("signaled ",::strsignal(WTERMSIG(ws))) : "??"s ;
								::string ids = to_string( "job=",report_req.job , " , seq=",report_req.seq_id )                                                               ;
								if (has_start) append_to_string(ids," , small=",report_start.small_id);
								_send_job( fd , ro , No/*show_deps*/ , false/*hide*/ , jt , lvl ) ;
								bool wn =  has_start && +report_start.reason.node ;
								if (wn) {
									::pair_s<Node> reason = Job::s_reason_str(report_start.reason) ;
									bool           err    = report_start.reason.has_err()          ;
									_send_node( fd , ro , true/*always*/ , Maybe&!err/*hide*/ , to_string("reason         : ",reason.first," :") , reason.second.name() , lvl+1 ) ;
								}
								if (!wn) audit( fd , ro , Color::None , lvl+1 , "reason         : "+( has_start ? Job::s_reason_str(report_start.reason).first                        : "N/A" ) ) ;
								/**/     audit( fd , ro , Color::None , lvl+1 , "host           : "+( has_req   ?                   report_req  .host                                 : "N/A" ) ) ;
								/**/     audit( fd , ro , Color::None , lvl+1 , "id's           : "+( has_req   ? ids                                                                 : "N/A" ) ) ;
								/**/     audit( fd , ro , Color::None , lvl+1 , "tmp dir        : "+( has_start ? report_start.job_tmp_dir                                            : "N/A" ) ) ;
								/**/     audit( fd , ro , Color::None , lvl+1 , "end date       : "+( has_info  ? report_info.end_date.str()                                          : "N/A" ) ) ;
								/**/     audit( fd , ro , Color::None , lvl+1 , "rc             : "+( has_info  ? rc                                                                  : "N/A" ) ) ;
								/**/     audit( fd , ro , Color::None , lvl+1 , "cpu time       : "+( has_end   ? to_string(float(report_end.digest.stats.cpu  )         ,'s' )       : "N/A" ) ) ;
								/**/     audit( fd , ro , Color::None , lvl+1 , "elapsed in job : "+( has_end   ? to_string(float(report_end.digest.stats.job  )         ,'s' )       : "N/A" ) ) ;
								/**/     audit( fd , ro , Color::None , lvl+1 , "elapsed total  : "+( has_end   ? to_string(float(report_end.digest.stats.total)         ,'s' )       : "N/A" ) ) ;
								/**/     audit( fd , ro , Color::None , lvl+1 , "memory         : "+( has_end   ? to_string(      report_end.digest.stats.mem  /1'000'000,"MB")       : "N/A" ) ) ;
								/**/     audit( fd , ro , Color::None , lvl+1 , "resources      : "+( has_start ? ""s                                                                 : "N/A" ) ) ;
								if (has_start) {
									if (!report_start.rsrcs.empty()) {
										size_t kw = 0 ;
										for( auto const& [k,_] : report_start.rsrcs ) kw = ::max(kw,k.size()) ;
										for( auto const& [k,v] : report_start.rsrcs ) audit( fd , ro , Color::None , lvl+2 , to_string(::setw(kw),k," : ",v) ) ;
									} else {
										audit( fd , ro , Color::None , lvl+2 , "<none>" ) ;
									}
								}
							} break ;
							default : FAIL(ro.key) ;
						}
					}
				} break ;
				case ReqKey::AllDeps :
				case ReqKey::Deps    : {
					bool     always      = ro.key==ReqKey::AllDeps ;
					::string uphill_name = dir_name(target.name()) ;
					double   prio        = -Infinity               ;
					if (!uphill_name.empty()) _send_node( fd , ro , always , Maybe/*hide*/ , "U" , Node(uphill_name) , lvl ) ;
					for( JobTgt job_tgt : target->job_tgts ) {
						if (job_tgt->rule->prio<prio) break ;
						if (job_tgt==jt ) { prio = rule->prio ; continue ; }                                                          // actual job is output last as this is what user views first
						bool hide = !job_tgt.produces(target) ;
						if      (always) _send_job( fd , ro , Yes   , hide          , job_tgt , lvl ) ;
						else if (!hide ) _send_job( fd , ro , Maybe , false/*hide*/ , job_tgt , lvl ) ;
					}
					if (prio!=-Infinity) _send_job( fd , ro , always?Yes:Maybe , false/*hide*/ , jt ) ; // actual job is output last as this is what user views first
				} break ;
				case ReqKey::Targets :
					if (rule.is_special()) {
						_send_node( fd , ro , true/*always*/ , Maybe/*hide*/ , {} , target , lvl ) ;
					} else {
						Rule::Match       match = jt.match()             ;
						::vector_view_c_s sts   = match.static_targets() ;
						size_t            wk    = 0                      ;
						for( auto const& [k,_] : rule->targets ) wk = ::max(wk,k.size()) ;
						auto send_target = [&]( VarIdx ti , char star , Node t )->void {
							bool m = rule->flags(ti)[TFlag::Match] ;
							_send_node( fd , ro , true/*always*/ , Maybe|!m/*hide*/ , to_string( m?' ':'!' , star , ::setw(wk) , rule->targets[ti].first ) , t , lvl ) ;
						} ;
						for( VarIdx t=0 ; t<sts.size() ; t++ ) send_target( t                   , ' ' , Node(sts[t]) ) ;
						for( Target t : jt->star_targets     ) send_target( match.idx(t.name()) , '*' , t            ) ;
					}
				break ;
				case ReqKey::InvDeps :
					for( Job job : g_store.job_lst() )
						for( Dep const& dep : job->deps ) {
							if (dep!=target) continue ;
							_send_job( fd , ro , No , false/*hide*/ , job , lvl ) ;
							break ;
						}
				break ;
				default : FAIL(ro.key) ;
			}
		}
		trace(STR(ok)) ;
		return ok ;
	}

}
