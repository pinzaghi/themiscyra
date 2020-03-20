#include "modsmartsync_types.h"
#include "modsmartsync_decl.h"
int func(int p, int n)
{
    int round;
    int leader;

    int all=1000;

    int creg = 0;
    int nreg = 0;
    
    msg* m;
    msg* recv_msg;

    list* stop_mbox=NULL;
    list* stopdata_mbox=NULL;
    list* sync_mbox=NULL;

    list* decided_log=NULL;
    list* valid_timedout_msgs=NULL;

    while(1){

        stop_mbox = havoc(creg);
        stopdata_mbox = havoc(creg);
        sync_mbox = havoc(creg);
 
        if(timeout() && nreg == creg){
            // @assert nreg == creg
            nreg = creg+1;
            round = STOP;

            cancelTimers();
            msg* m = (msg *) malloc(sizeof(msg));
            m->round = STOP;
            m->reg = nreg;
            m->timedout_msgs = timedout_messages();
            send(all,m);
            
            continue; 
        }

        if(count_change_regency(stop_mbox, creg+1)>n/3 && nreg == creg){
            // @assert nreg == creg
            nreg = creg+1;
            round = STOP;

            valid_timedout_msgs = valid_timedout_messages(stop_mbox);
            add_messages_to_order(valid_timedout_msgs);

            cancelTimers();
            msg* m = (msg *) malloc(sizeof(msg));
            m->round = STOP;
            m->reg = nreg;
            m->timedout_msgs = valid_timedout_msgs;
            send(all,m);

            continue;
        }

        if(round == STOP && count_change_regency(stop_mbox, creg+1)>2*n/3 && nreg > creg){
            creg = nreg;
            round = STOPDATA;

            activateTimers();
            leader = creg % n;

            msg* m = (msg *) malloc(sizeof(msg));
            m->round = STOPDATA;
            m->reg = nreg;
            m->decided_log = decided_log;
            send(leader,m);

            continue;
        }

        // leader
        if(round == STOPDATA && (p == creg % n) && size(stopdata_mbox) > 2*n/3){

            round = SYNC;

            msg* m = (msg *) malloc(sizeof(msg));
            m->round = SYNC;
            m->reg = creg;
            m->proofs = stopdata_mbox;
            send(all,m);

            continue;
        }


        if(round == STOPDATA && size(sync_mbox) == 1 && nreg == creg && from_leader(first_message(sync_mbox), creg) && check_proofs()){

            round = SYNC;
            
            if(size(valid_sync_data())> 2*n/3){
                rebuild_state(stopdata_mbox);
                vp_decide(current_log());
            }
            
            continue;
        }

    }

}
