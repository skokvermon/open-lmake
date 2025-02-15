// This file is part of the lmake distribution (git@github.com:cesar-douady/open-lmake.git)
// Copyright (c) 2023 Doliam
// This program is free software: you can redistribute/modify under the terms of the GPL-v3 (https://www.gnu.org/licenses/gpl-3.0.html).
// This program is distributed WITHOUT ANY WARRANTY, without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "rpc_job.hh"

::ostream& operator<<( ::ostream& os , JobReason const& jr ) {
	os << "JobReason(" << jr.tag ;
	if (jr.tag>=JobReasonTag::HasNode) os << ',' << jr.node ;
	return os << ')' ;
}

::ostream& operator<<( ::ostream& os , DepDigest const& dd ) {
	os << "DepDigest(" ;
	if (dd.garbage) os << "garbage,"    ;
	else            os << dd.date <<',' ;
	os << dd.flags ;
	return os <<','<< dd.order <<')' ;
}

::ostream& operator<<( ::ostream& os , TargetDigest const& td ) {
	os << "TargetDigest(" ;
	const char* sep = "" ;
	if (+td.dfs  ) { os <<sep<< td.dfs  ; sep = "," ; }
	if ( td.write) { os <<sep<< "write" ; sep = "," ; }
	if (+td.tfs  ) { os <<sep<< td.tfs  ; sep = "," ; }
	if (+td.crc  ) { os <<sep<< td.crc  ; sep = "," ; }
	return os <<')' ;
}

::ostream& operator<<( ::ostream& os , JobDigest const& jd ) {
	return os << "JobDigest(" << jd.status <<','<< jd.targets <<','<< jd.deps << ')' ;
}

::ostream& operator<<( ::ostream& os , JobRpcReq const& jrr ) {
	os << "JobRpcReq(" << jrr.proc <<','<< jrr.seq_id <<','<< jrr.job ;
	switch (jrr.proc) {
		case JobProc::Start    : os <<','<< jrr.host                    ; break ;
		case JobProc::LiveOut  : os <<','<< jrr.txt                     ; break ;
		case JobProc::DepInfos : os <<','<< jrr.digest.deps             ; break ;
		case JobProc::End      : os <<','<< jrr.host <<','<< jrr.digest ; break ;
		default                :                                          break ;
	}
	return os << ')' ;
}

::ostream& operator<<( ::ostream& os , TargetSpec const& tf ) {
	return os << "TargetSpec(" << tf.pattern <<','<< tf.flags <<','<< tf.conflicts <<')' ;
}

::ostream& operator<<( ::ostream& os , JobRpcReply const& jrr ) {
	os << "JobRpcReply(" << jrr.proc ;
	switch (jrr.proc) {
		case JobProc::ChkDeps  : os <<','<< jrr.ok    ; break ;
		case JobProc::DepInfos : os <<','<< jrr.infos ; break ;
		case JobProc::Start :
			/**/                         os       << hex<<jrr.addr<<dec   ;
			/**/                         os <<',' << jrr.ancillary_file   ;
			if (jrr.auto_mkdir         ) os <<',' << "auto_mkdir"         ;
			if (!jrr.chroot.empty()    ) os <<',' << jrr.chroot           ;
			if (!jrr.cwd   .empty()    ) os <<',' << jrr.cwd              ;
			/**/                         os <<',' << jrr.env              ;
			if (!jrr.force_deps.empty()) os <<',' << jrr.force_deps       ;
			/**/                         os <<',' << jrr.interpreter      ;
			/**/                         os <<',' << jrr.job_tmp_dir      ;
			if (jrr.keep_tmp           ) os <<',' << "keep_tmp"           ;
			/**/                         os <<',' << jrr.kill_sigs        ;
			if (jrr.live_out           ) os <<',' << "live_out"           ;
			/**/                         os <<',' << jrr.reason           ;
			/**/                         os <<',' << jrr.remote_admin_dir ;
			/**/                         os <<',' << jrr.rsrcs            ;
			/**/                         os <<',' << jrr.small_id         ;
			if (!jrr.stdin .empty()    ) os <<'<' << jrr.stdin            ;
			if (!jrr.stdout.empty()    ) os <<'>' << jrr.stdout           ;
			/**/                         os <<"*>"<< jrr.targets          ;
			if (+jrr.timeout           ) os <<',' << jrr.timeout          ;
			/**/                         os <<',' << jrr.script           ; // last as it is most probably multi-line
			;
		break ;
		default : ;
	}
	return os << ')' ;
}

::ostream& operator<<( ::ostream& os , JobInfo const& ji ) {
	return os << "JobInfo(" << ji.end_date <<','<< ji.stdout.size() <<','<< ji.wstatus <<')' ;
}

::ostream& operator<<( ::ostream& os , JobExecRpcReq::AccessInfo const& ai ) {
	os << "AccessInfo(" ;
	const char* sep = "" ;
	if (+ai.dfs            ) { os <<sep     << ai.dfs     ; sep = "," ; }
	if ( ai.write          ) { os <<sep     << ai.write   ; sep = "," ; }
	if (+ai.neg_tfs        ) { os <<sep<<'-'<< ai.neg_tfs ; sep = "," ; }
	if (+ai.pos_tfs        ) { os <<sep<<'+'<< ai.pos_tfs ; sep = "," ; }
	if ( ai.unlink         ) { os <<sep     << ai.unlink  ; sep = "," ; }
	return os <<')' ;
}

::ostream& operator<<( ::ostream& os , JobExecRpcReq const& jerr ) {
	os << "JobExecRpcReq(" << jerr.proc <<','<< jerr.date ;
	if (jerr.sync            ) os << ",sync"            ;
	if (jerr.auto_date       ) os << ",auto_date"       ;
	/**/                       os <<',' << jerr.info    ;
	if (!jerr.comment.empty()) os <<',' << jerr.comment ;
	if (jerr.has_files()) {
		if ( +jerr.info.dfs && !jerr.auto_date ) {
			os <<','<< jerr.files ;
		} else {
			::vector_s fs ;
			for( auto [f,d] : jerr.files ) fs.push_back(f) ;
			os <<','<< fs ;
		}
	}
	return os <<')' ;
}

::ostream& operator<<( ::ostream& os , JobExecRpcReply const& jerr ) {
	os << "JobExecRpcReply(" << jerr.proc ;
	switch (jerr.proc) {
		case JobExecRpcProc::ChkDeps  : os <<','<< jerr.ok    ; break ;
		case JobExecRpcProc::DepInfos : os <<','<< jerr.infos ; break ;
		default : ;
	}
	return os << ')' ;
}
