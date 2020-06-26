struct Msg
{
  int view;
  int vround;
  int opnumber;
  int nround;
  
  int replica;
  void* log;
};
typedef struct Msg msg;
typedef struct List
{
  msg *message;
  struct List *next;
  int size;
} list;

enum vround_typ {STARTVIEWCHANGE, DOVIEWCHANGE, STARTVIEW};

msg * recv();

void send(int addr, msg * m);

int count_messages(list * mbox, int view, enum vround_typ vround);

int main(int p, int n, int f);
int main(int p, int n, int f)
{
    int all;
    int view;
    enum vround_typ vround;

    msg* m;
    msg* recv_msg;

    list* mbox;
    
    vround = STARTVIEWCHANGE;
    view = 0;
    send(all, message(view, STARTVIEWCHANGE, NULL, NULL, p)); 

    while(1){

        mbox = havoc(view, vround);
        if(vround == STARTVIEWCHANGE && p==primary(view,n) && mbox->size > f){
            vround = DOVIEWCHANGE;
            continue;
        }      
        
        if(vround == STARTVIEWCHANGE && p!=primary(view,n) && mbox->size > f){
            vround = DOVIEWCHANGE;
            send(primary(view,n), message(view, DOVIEWCHANGE, NULL, NULL, p, local_log()));  
            vround = STARTVIEW;

            continue;
        }

        if(vround == DOVIEWCHANGE && p==primary(view,n) && mbox->size > f){
            computes_new_log();
            vround = STARTVIEW;
            send(all, message(view, STARTVIEW, NULL, NULL, p, local_log())); 

            view++;
            vround = STARTVIEWCHANGE;
            send(all, message(view, STARTVIEWCHANGE, NULL, NULL, p)); 
    
            continue;
        }
        
        if(vround == STARTVIEW && p!=primary(view,n) && mbox->size == 1){
            computes_new_log();
            
            view++;
            vround = STARTVIEWCHANGE;
            send(all, message(view, STARTVIEWCHANGE, NULL, NULL, p)); 

            continue;
        }
        
    }

}
