#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <tuiliboa.h>
#include <peekpoke.h>
#include <ctype.h>


void bgc(int color) {
int i;
long m = 48000l;
   for (i=1; i<29; ++i) {
	   POKE(m+(i*40),color);
   }	
}

void fgc(int color) {
	
int i;
long m = 48000l;
   for (i=1; i<29; ++i) {
	   POKE(m+(i*40)+1,color);
   }	
	
}


/*
int button_focus_color=COLOR_WHITE;
*/

void cleanscrn(tgui_Window *win){

int i, len;

    len = win->x2 - win->x1 +1;  //+1

    for (i=win->y1; i<=win->y2; ++i){
	   cclearxy(win->x1, i, len);
	};

}  // end of cleanscrn




void drawbox(x1,y1,x2,y2){
unsigned i;
char hch='-';
char vch='|';

  //revers(1);
  //revers(0);
  cputcxy(x1,y1,47);  //47 /
  cputcxy(x2,y1,92);  //92 \
  cputcxy(x1,y2,92);  // 92
  cputcxy(x2,y2,47);  // 47

  for (i=x1+1 ; i<x2; ++i) {
	   cputcxy(i,y1,hch);  
	   cputcxy(i,y2,hch);
  } 
  for(i=y1+1; i<y2; ++i){
	  cputcxy(x1,i,vch);  
	  // clear inbetween
	  cputcxy(x2,i,vch);  
  }
	revers(0);
} // end of drawbox



unsigned box(tgui_Window *tmp) { 
 
 
 cleanscrn(tmp);
  
  
  //if (tmp->x1<0 || tmp->x1>35)             { return(10);}    // x1 out of bounds
  //if (tmp->x2 < tmp->x1+4 || tmp->x2 > 39) { return(11);}    // x2 out of bounds
  //if (tmp->y1<0 || tmp->y1>20)             { return(20);}    // y1 out of bounds
  //if (tmp->y2>24 || tmp->y2<tmp->y1+3)     { return(21);}    // y2 out of bounnds
  //if (tmp->color < 0 || tmp->color > 15)   { return(30);}    // invalid color
   
  
  //if (tmp->parent == 0 ) revers(1);
  
  drawbox(tmp->x1, tmp->y1, tmp->x2, tmp->y2);
  
  
  cputsxy(tmp->x1+1,tmp->y1,tmp->title);
 
  
   
   return(0);
   
} /* end box */


 
 
void ss(tgui_Window *win) {	

long s = 48000l;
int c, y;
int size = win->x2-win->x1+1;

  	c=0;
	
	for (y=win->y1 ; y<=win->y2; ++y){
		memcpy(win->scrnchar+(c*size), s+(y*40)+win->x1, size);
		//memcpy(win->scrncolr+(c*size), COLOR_RAM + (y*40)+win->x1, size);
		++c;
	}
	
}  // endof_ss


void ds(tgui_Window *win) {
long s = 48000l;
int c=0;
int y;
int size = win->x2-win->x1+1; 

   
   	for (y=win->y1 ; y<=win->y2; ++y){
		memcpy(s+(y*40)+win->x1,          win->scrnchar+(c*size), size);
		//memcpy(COLOR_RAM + (y*40)+win->x1,win->scrncolr+(c*size), size);
		++c;
	}

	
}  // end of ds 
 
 
 
 
 
 


void closewindow(tgui_Window *win) {
	
tgui_Obj *ptr, *nptr;

tgui_Menu *mptr, *nmptr;

	if (win->parent ==0 ) ds(win);
	else { cleanscrn(win); }
	//cleanscrn(win);
	// menus
	mptr=win->firstmenu;
	if (mptr!=NULL) {
		do {
		    nmptr=mptr->next;
		    free(mptr);
		    if (nmptr==win->firstmenu) mptr=NULL;
		    else mptr=nmptr;
		 
		 } while (mptr!=NULL);
	
	}
	// free objects
	
    ptr=win->firstobj;
   	
    while (ptr!=NULL) { 
        nptr = ptr->next;
		free(ptr->data);  // free data buffer for each obj
		
		// add code to free a listbox list
		
	    free(ptr);        // free memory for object
	    ptr=nptr;
	};
		
	free(win->scrnchar);
	//free(win->scrncolr); 
	free(win);
		
} // closewindow()




tgui_Window *newwindow(int x1, int y1, int x2, int y2, int color, unsigned parent, char* title) {

tgui_Window *tgw = malloc(sizeof (tgui_Window));

unsigned bufsize = (y2 -y1+1) * (x2 - x1 + 1);

  
       
	tgw->scrnchar = malloc(sizeof (char) * bufsize);
    //tgw->scrncolr = malloc(sizeof (char)*bufsize);
	
	tgw->x1=x1;
	tgw->y1=y1;
	tgw->x2=x2;
	tgw->y2=y2;
	tgw->color=color;
    tgw->title = title;
	tgw->parent = parent;
	tgw->quitflag = 0; 
	tgw->firstobj = NULL;
	tgw->lastobj=NULL;
	tgw->firstmenu=NULL;
	tgw->lastmenu=NULL;
	
	if (tgw->parent==0) ss(tgw);  
	
	box(tgw);
	
	return tgw;
} // end of newwindow()



tgui_Obj *get_obj(tgui_Window *w, unsigned id){
tgui_Obj *obj;

    obj = w->firstobj;
	while (id!=obj->id) { obj=obj->next;}
	return obj;
	
} //end of get_obj




int get_objstate(tgui_Window *w, unsigned id){
tgui_Obj *obj;

    obj = w->firstobj;
	while (id!=obj->id) { obj=obj->next;}
	return obj->state;
	
} //end of get_objstate




char *get_objdata(tgui_Window *w, unsigned id){
tgui_Obj *obj;

    obj = w->firstobj;
	while (id!=obj->id) { obj=obj->next;}
	return obj->data;
	
} //end of getdata




void showlistbox(tgui_Obj *obj, int x, int y, int color, int startid){
tgui_Listitem *item;
int i;
  
    //textcolor(color);
    cputsxy(x,y-1,obj->caption);
    drawbox(x, y, x+obj->width, y + obj->height);
    item = obj->firstitem;
    i=0;
	obj->topid=startid;
	
	
    while (item->li_id != startid){
		item=item-> next;
	}
	
	
    if (startid != 0 ) cputcxy(x+obj->width-1, y, '+');	

	
	    if (item!=NULL ) {
	        do {
	            if (obj->state == item->li_id ) revers(1);
				cputsxy(x+1,y+i+1,item->caption);
				cclearxy(x+strlen(item->caption)+1 ,y+i+1, obj->width - strlen(item->caption)-1);
				if (obj->state == item->li_id){
					//cclearxy(x+strlen(item->caption)+1 ,y+i+1, obj->width - strlen(item->caption)-1);
					revers(0);
                }
					++i;
	            item=item->next;
			} while (item != obj->firstitem && i+1 < obj->height);		
	        if (item != obj->firstitem ) {
				cputcxy(x+obj->width-1, y+obj->height, '+');
			}
			else cputcxy(x+obj->width-1, y+obj->height, ' ');
			
        }
	
	
	
}   // end of showlistbox






void process_listbox(tgui_Window *win, tgui_Obj *obj){
	
tgui_Listitem *li;
int i, ox, oy, y, ostate, maxy;
unsigned key;

    //textcolor(obj->color);
    ox = win->x1 + 2 + obj->x;
    oy = win->y1 + 3 + obj->y;
	maxy=obj->y + obj->height + 1;
	   
    y=oy;
    li = obj->firstitem;
	ostate = obj->state;
	revers(1);
	cputsxy(ox-1,oy-2, obj->caption);
	revers(0);
		
	for (i=0; i<obj->topid; i++ ) {
		li = li->next;
	}
	
	for (i=obj->topid; i<obj->state; i++) {
		li = li->next;
		++y;
	}
	
   	//cursor(1);
	gotoxy(ox,y);
	
	do { 
	   key=cgetc();
	 
	   switch(key){
		   case CH_ENTER:
		       //closewindow(miwin);
                revers(0);
	           cputsxy(ox-1,oy-2, obj->caption);
	           revers(1);
		       return;
			   
		   case CH_F1:
		       //closewindow(miwin);
			   obj->state = ostate; 
			   // move bar back to orig place
		       return ;   // escape
		   case CH_CURS_LEFT:
		   case CH_CURS_DOWN:
		       if (li->next->li_id==0) { break; }  //y = oy;
		       revers(0);
  		       cputsxy(ox, y, li->caption);
               cclearxy(ox+strlen(li->caption) ,y, obj->width - strlen(li->caption)-1);
			   li=li->next;
			   obj->state = li->li_id;
			   if (y==maxy ) {
				   obj->topid = obj->topid+1;
				   showlistbox(obj, ox-1, oy-1, obj->color, obj->topid);
				   
			   }
			   
			   
			   if (y<maxy) ++y;
			   
			   if (y == maxy && li->next->li_id ==0){ 
			       cputcxy(obj->x+obj->width, obj->y+obj->height+2, '_');
			   }
			   else if (y==maxy && li->next->li_id !=0) {
	           				   
				   cputcxy(obj->x+obj->width, obj->y+obj->height+2, '+');
			   }
			   			   
			   revers(1);
			   cputsxy(ox, y, li->caption);
			   cclearxy(ox+strlen(li->caption) ,y, obj->width - strlen(li->caption)-1);
			   gotoxy(ox,y);
			   revers(0);
			   if(y==maxy && li->li_id >=(obj->height-1))  cputcxy(obj->x+obj->width, oy-1, '+');
			 
			   break;
			   
		   case CH_CURS_RIGHT:
		   case CH_CURS_UP:
		       if (li->li_id==0) {
				   
				   break; }   // y = my+2+mid;
		       revers(0);
		       cputsxy(ox, y, li->caption);
			   cclearxy(ox+strlen(li->caption) ,y, obj->width - strlen(li->caption)-1);
			   
			   li=li->prev;
			   obj->state = li->li_id;
			   if (y==oy) {
				   obj->topid = obj->topid-1;
				   showlistbox(obj, ox-1, oy-1, obj->color, obj->topid);
			    }
			   
			   if (y > oy) --y;
			   		  
			   if(y == oy && li->next->li_id != 0){
				   cputcxy(obj->x+obj->width, oy-1, '+');
			   }
			   	   
			   revers(1);
			   cputsxy(ox, y, li->caption);
			   cclearxy(ox+strlen(li->caption) ,y, obj->width - strlen(li->caption)-1);
			   gotoxy(ox,y);
			   revers(0);
			   
			   if (y==oy && li->li_id==0) cputcxy(obj->x+obj->width, oy-1, '_');
			   
			   if (y==oy && (li->li_id +((obj->height)-1) - 1 < obj->lastitem->li_id ) ) {
				  cputcxy(obj->x+obj->width, oy+obj->height-1, '+');
			   }
			  
			   break;
		    	   
	       } // switch   
	       gotoxy(ox,y);
	   } while(1);   
	
	
	
	
}  // end processs_listbox


void process_checkbox(tgui_Window *win, tgui_Obj *obj){
int x, y;
char key;
    
    x=win->x1 + 1 + obj->x;
    y=win->y1 + 2 + obj->y;
	
	gotoxy(x+1,y); 
	//cursor(1);
	revers(1);
	if (obj->state==0) cputc(' ');
	else cputc('X');
	
	do {
		gotoxy(x+1,y);
	    revers(1);
	    if (obj->state==0) cputc(' ');
	    else cputc('X');
	    gotoxy(x+1, y);
	    key=cgetc();
	
	    if (key==' ') { 
	        if(obj->state == 0 ) obj->state =1;
			else obj->state = 0;
		}     		
	    
	} while (key != CH_ENTER);
	
		gotoxy(x+1,y);
	    revers(0);
	    if (obj->state==0) cputc(' ');
	    else cputc('X');
	cursor(0);
    
}  // process_checkbox



char process_numbox(tgui_Window *win, tgui_Obj *obj){
	
int ox, oy, x, maxx, bctr, num;
unsigned key;
unsigned min, max;

char *buf = malloc (sizeof(char)*(obj->width+1));


    strcpy(buf, obj->data); 
	num=atoi(buf);
	
    min=obj->state;
	max=obj->height;
	
	//textcolor(p4color(obj->color));
    ox = win->x1 + 1 + obj->x;
    oy = win->y1 + 2 + obj->y;
    x=ox;
	
	//revers(1);
	cputsxy(x,oy, buf);
	//revers(0);
	
	            
	   revers(1);
	   cputcxy(x+obj->width, oy,'^');
	   revers(0);
	
	
	maxx=x+obj->width;
    x=x+strlen(buf);
    bctr=strlen(buf);
	
	
	//cursor(1);
	
	do {
		gotoxy(ox+obj->width, oy);
		key = cgetc();	
		//gotoxy(3,22);  cprintf ("   "); gotoxy(3,22); cprintf("%d",key);
		
		if (key == CH_CURS_LEFT || key == CH_CURS_DOWN){
			if (num>min) {
			   // revers(1);
			    --num;
				itoa(num,buf,10);
				gotoxy(ox,oy);
				cclearxy(ox,oy, obj->width);
				gotoxy(ox,oy);
				cputs(buf);
			    
			}
			
		}
			if (key == CH_CURS_RIGHT || key == CH_CURS_UP){
			if (num<max) {
			   // revers(1);
			    ++num;
				itoa(num,buf,10);
				gotoxy(ox,oy);
				cclearxy(ox, oy, obj->width);
				gotoxy(ox,oy);
				cputs(buf);
			   
			}
			
		}
	} while (key!=CH_ENTER && key!=CH_F1 );
	
	cursor(0);
	
	if (key==CH_ENTER ) {
        revers(0);
        strcpy(obj->data, buf);
		
	}
	
	if (key==CH_ESC) {
	   revers(1);
	   gotoxy(ox,oy);
	   cclearxy(ox,oy,obj->width);
	   cputsxy(ox, oy, obj->data);	
	}
	revers(0);
	cputcxy(ox+obj->width, oy,'^');
	
	
	free(buf);
	return key;
				
} // end of process_num_text




char process_simpletext(tgui_Window *win, tgui_Obj *obj){
	
int ox, oy, x, maxx, bctr;
unsigned key;

char *buf = malloc (sizeof(char)*(obj->width+1));


    strcpy(buf, obj->data); 
	

    ox = win->x1 + 1 + obj->x;
    oy = win->y1 + 2 + obj->y;
    x=ox;
	
	revers(0);
	
	cputsxy(x,oy, buf);
	
	revers(1);
	cputc(' ');
	revers(0);
	
	maxx=x+obj->width;
    x=x+strlen(buf);
    bctr=strlen(buf);
	gotoxy(x, oy);
	//cursor(1);
	
	do {
		key = cgetc();	
		//gotoxy(3,22); cprintf("%d",key);
		if (isalnum(key) || ispunct(key) || key==32) {
			if (x<=maxx){
				revers(0);
		        cputcxy(x,oy,key);
				buf[bctr]=key;
				++bctr;
				buf[bctr]='\0';
		        ++x;
				revers(1);
				cputc(' ');
				revers(0);
	
			}
		}
		if (key == CH_CURS_LEFT|| key == CH_DEL){
			if (x>ox) {
			    
			    if (x<=maxx){ revers(1); cputcxy(x, oy, ' '); revers(0);}
			    --bctr;
			    buf[bctr]='\0';
			    --x;
				gotoxy(x,oy);
				revers(1);
				cputc(' ');
				revers(0);
			}
			
		}
		
	} while (key!=CH_ENTER && key!=CH_F1 );
	
	cursor(0);
	
	if (key==CH_ENTER ) {
	    revers(0);
     	if (x<maxx) cputcxy(x,oy, ' ');
        
		strcpy(obj->data, buf);
	}
	
	//ESCAPE
	if (key==CH_F1) {
	   revers(1);
	   cputsxy(ox, oy, obj->data);	
	}
	
	//textcolor(win->color);
	
	free(buf);
	return key;
			
} // end of process_simple_text



void showbutton(tgui_Obj *obj, int x, int y, int focus) {
	
	revers(1);
	cputcxy(x,y,'(');
	if (focus==1)revers(0);
	
	cputs(obj->caption);
	revers(1);
	cputc(')');
	revers(0);
	
}  // end of showbutton




void process_button(tgui_Window *win, tgui_Obj *obj){
int x, y;
char key;
    
    x=win->x1 + 1 + obj->x;
    y=win->y1 + 2 + obj->y;
	showbutton(obj, x, y, 1);
	gotoxy(x,y);
	key=cgetc();
	if (key==CH_ENTER && obj->fp!=NULL) { 
	    cursor(0);
	    obj->fp(win);
	    //cursor(1);
	}
	
	showbutton(obj, x, y, 0);
	
}  // process_button





void processobj(tgui_Window *win, tgui_Obj *obj){

   do {   
        switch (obj->objtype){

		    case label :
		        break;
		    case button:
		        process_button(win, obj);
		        break;
	        case simpletext :
	            process_simpletext(win, obj);
			    break;
		    case numbox:
			    process_numbox(win, obj);
				break;
			case checkbox:
			    process_checkbox(win, obj);
				break;
			case listbox:
			    process_listbox(win, obj);
				break;
				
	       default :
	       	   break;
		} // switch   
		 
		obj=obj->next; 
		if (obj==NULL) obj=win->firstobj;
		
   }while (win->quitflag == 0);   
	   

} // end processobj



void add_li(tgui_Obj *obj, char *caption){
	
tgui_Listitem *new;

new = malloc(sizeof(tgui_Listitem));
    
	
    new->caption=caption;

    if (obj->firstitem ==NULL) {
		obj->firstitem=new;
		obj->lastitem = new;
		new->next = new;
		new->prev = new;
		new->li_id=0;
	}
    else {
		new->prev=obj->lastitem;
		new->next = obj->firstitem;
		obj->lastitem->next = new;
		obj->lastitem = new;
		new->li_id = new->prev->li_id+1;
		
	}
	
	
}




void newobject(int x, int y, int width, int height, int color, unsigned state, 
                char *caption, char *data, tgui_Object_type type, 
				tgui_Window *win, void (*fp)(void *)){
	
	tgui_Obj *newobj= malloc(sizeof (tgui_Obj));
	char *databuf = malloc(sizeof (char)*(width+1));
	bzero(databuf, width+1);
	databuf[0]='\0';
	strcpy(databuf, data);
	
	newobj->x=x;
	newobj->y=y;
	newobj->width = width;
	newobj->height = height;
	newobj->color = color;
	newobj->caption = caption;
	newobj->data = databuf;
	newobj->objtype = type;
	newobj->state = state;
	newobj->topid = 0;
	newobj->next = NULL;
	newobj->fp = fp;
	
	newobj->firstitem=NULL;
	
	if (win->firstobj==NULL) { 
	    win->firstobj=newobj;
	    newobj->id=0;
		}
    if (win->lastobj!=NULL) { 
	    win->lastobj->next=newobj;
		newobj->id = win->lastobj->id +1;
		}
		
	win->lastobj = newobj;
	
}





void showobj(tgui_Obj *obj, tgui_Window *win) {
int x, y, i, h;

   x=win->x1 + 1 + obj->x;
   y=win->y1 + 2 + obj->y;
   //textcolor(p4color(obj->color));
   
   if (obj->objtype==label) {
	     if (win->parent==0) revers(1);
	     cputsxy(x,y,obj->caption);
         revers(0);		 
   }
      else if (obj->objtype==rectangle) {
	 drawbox(x,y,x+obj->width, y+obj->height) ; 
	
   }
   else if (obj->objtype==checkbox) {
	    cputcxy(x,y,'[');
		if (obj->state==0) cputc(' '); // 119 open 113 filled
	    else cputc('X');
		cputc(']'); cputc(' ');
	    cputs(obj->caption);
   }  
   else if (obj-> objtype == simpletext || obj->objtype == numbox) {
	 
	   revers(1);
	  // cclearxy(x, y, obj->width);
      gotoxy(x,y);
      for(i=0; i<obj->width; i++) cputc(' ');      
	  if (obj->objtype == numbox) cputc('^');
	   gotoxy(x,y);
	   cputs(obj->data);
	   revers(0);
   }   
   else if (obj->objtype == button) {
	   showbutton(obj, x, y, 0);
   }
   else if (obj->objtype == multitext){
	  gotoxy(x,y);
	  revers(1);
	  for(h=1; h <= obj->height ; ++h){
	     for (i=0; i<=obj->width; ++i) { cputc(' ');}
		 gotoxy(x,y+h);
	  }
	 
	   revers(0);  
	   
   }
   else if(obj->objtype == listbox) {
	   showlistbox(obj, x, y, obj->color, 0);
	   
    }   
   
   //cgetc();
	
   //cgetc();
	
	
} // end of showobj


void showallobjects (tgui_Window *win) {
tgui_Obj *ptr;

   ptr=win->firstobj;
   	
   while (ptr!=NULL) { 
      showobj(ptr, win);
      ptr = ptr->next;
	  }
	
	
} // end of show all objects

void ib_cancel(void *ptr){
tgui_Window *win;
    win=ptr;
	win->quitflag = CANCEL;
}



void ib_ok(void *ptr) {
tgui_Window *win;
    win=ptr;
	win->quitflag=OK;
}



// input box
void inputbox(char *caption, char *buf, unsigned size){
	
tgui_Window *mwin;
void (*okfp)(void *);
void (*canfp) (void *);


    okfp=ib_ok;
	canfp=ib_cancel;

    //textcolor(COLOR_WHITE);
	
	mwin=newwindow(5,7,35,14,COLOR_BLACK, 0," input \0");
	newobject(0,1,0,1,0,0,caption,"\0", label, mwin,NULL);
	newobject(0,2,size,1,0,0,"\0","\0", simpletext, mwin,NULL);
    newobject( 5,4,19,1,0,0,"ok\0", "\0",button, mwin, okfp);  //&foo
	newobject(15,4,31,1,0,0,"quit\0", "\0",button, mwin, canfp);  //&foo
	
	showallobjects(mwin);	
	processobj(mwin, mwin->firstobj);
	
	//cgetc();
	revers(0);
	if (mwin->quitflag==OK) {
		strcpy(buf, get_objdata(mwin,1));
	}
	else strcpy(buf,"\0");
	
    closewindow(mwin);	
    		
} // end inputbox



// message box
void msgbox(char *msg){
	
tgui_Window *mwin;

    //textcolor(COLOR_RED);
	revers(1);
	mwin=newwindow(5,9,35,14,COLOR_BLACK,0," message \0");
	//textcolor(COLOR_RED);
	cputsxy(6,12,msg);
	cgetc();
	revers(0);
    closewindow(mwin);	
    		
} // end msgbox


int processmenuitems(tgui_Window *win, tgui_Menu *menu, int mx, int my){
	
tgui_Menuitem *mi;
tgui_Window *miwin;
int mid, c, y;
char key;

    mi=menu->firstmi;                      // get ptr to first submenu item
	mid = menu->lastmi->menu_item_id;      // get the id of the last menu id (count-1 of items for size)
    
	miwin=newwindow(mx, my+1, mx+menu->mi_maxwidth+1, my+1+mid+2, COLOR_BLACK, 0, "\0");
	
	
	//textcolor(win->color);                // set menu text color to match window it is in
	revers(1);
	y=my+2;                               // top item y value	
	c=0;
	//cursor(0);
    // fill menu with items
    do {
	   
	   cputsxy(mx+1,y+c,mi->caption);
	   mi=mi->next;
	   c++;	
	   revers(0);
	} while(mi!=menu->firstmi);
	
	
	do { 
	   key=cgetc();
	 
	   switch(key){
		   case CH_ENTER:
		          closewindow(miwin);
		          if (mi->fp!=NULL) { 
				      mi->fp(win); 
                      //textcolor(win->color);
					  return -1;
					  }
		           
				  
				  return(mi->menu_item_id);
			     
		   case CH_F1:
		       closewindow(miwin);
			   
		       return -1;   // escape
		   case CH_CURS_LEFT:
		   case CH_CURS_DOWN:
		       cputsxy(mx+1, y, mi->caption);
			   mi=mi->next;
			   if (mi->menu_item_id==0) { y = my+2;}
			   else ++y;
			   revers(1);
			   cputsxy(mx+1, y, mi->caption);
			   revers(0);
			   break;
			   
		   case CH_CURS_RIGHT:
		   case CH_CURS_UP:
		       cputsxy(mx+1, y, mi->caption);
			   if (mi->menu_item_id==0) { y = my+2+mid;}
			   else --y;
			   mi=mi->prev;
			   revers(1);
			   cputsxy(mx+1, y, mi->caption);
			   revers(0);
			   break;
		    	   
	   } // switch   		   
	   } while(1);   
	
	return 0;
	
} // process_menu




/* 
value returns represents an int beween 10 and 99
where the tens value is the main menu item 10 is 1, 20 is 2 etc
where the ones value is the sub menu item number  11, is one, 12, is 2 etc

note: this is offest by the menu_item_id's by one as they start at 0
*/


int processmenu(tgui_Window *win){

tgui_Menu *menu;
int x, y, lastx, retval;
unsigned key;

	if (win->firstmenu==NULL) return -1;
	retval=0;
	x = win->x1 + 2;
	y = win->y1 + 1;
	
	lastx=x;
	
	menu=win->firstmenu;
	//textcolor(win->color);
	do { 
	   lastx = lastx + strlen(menu->caption) + 2;
	   menu = menu->next;
	   } while (menu!=win->lastmenu);
	

	
	menu=win->firstmenu;
	cputsxy(x,y, menu->caption);
	
	
	
	do { 
	   gotoxy(x+ strlen(menu->caption),y);
	   //cursor(1);
	   key=cgetc();
	   switch (key) {
			case CH_ENTER:
			    
				if(menu->firstmi != NULL) {
					retval=processmenuitems(win, menu, x, y);
					if (retval>=0){
                           return ((menu->menu_id)+1)*10 + retval+1;
					}
                    else {
						key=0;
					}
				}	
				else if (menu->fp != NULL ) {
					
					menu->fp(win);
					//textcolor(win->color);
					key=' ';
					if (win->quitflag==1) key=CH_ENTER;
				}
				else if (menu->firstmi==NULL) return ((menu->menu_id)+1)*10;
				break;
		    case CH_CURS_RIGHT: 
			case CH_CURS_UP:
				revers(1);
				cputsxy(x,y, menu->caption);
				x = x + strlen(menu->caption) + 2;
				menu=menu->next;
				if (menu==win->firstmenu) { x=win->x1+2;}
				revers(0);
				cputsxy(x,y, menu->caption);
				break;
			case CH_CURS_LEFT:
			case CH_CURS_DOWN:
				revers(1);
				cputsxy(x,y, menu->caption);
				
				menu=menu->prev;
				if (menu==win->lastmenu)  {
					x=lastx;
				}
				else {
					x = x - strlen(menu->caption) - 2;
				}
				
				revers(0);
				cputsxy(x,y, menu->caption);
				break;
	   
	   }
	   
	} while (key!=CH_ENTER);	
	return menu->menu_id;
	
	
} // end of processmenu



void showmenu(tgui_Window *win){
	
tgui_Menu *menu;
unsigned x, y, len;

    //textcolor(win->color);
	if (win->firstmenu == NULL) { 
	return;}
	
	menu=win->firstmenu;
	
	x = win->x1 + 1;
	y = win->y1 + 1;
	len = win->x2 - win->x1 - 1;
	
	revers(1);
	cclearxy(x,y,len);
	x=x+1;
	do {
		cputsxy(x,y,menu->caption);
		x=x+2+strlen(menu->caption);
		menu = menu->next;
	} while (menu != win->firstmenu);
	revers(0);
				
} // end of showwindow()



//void addmenu(tgui_Window *win, char *caption){
	
void addmenu(tgui_Window *win, char *caption, void (*fp)(void *) ){	
tgui_Menu *newmenu= malloc(sizeof (tgui_Menu));	
	
	newmenu->caption = caption;
	newmenu->firstmi = NULL;
	newmenu->lastmi = NULL;
	newmenu->fp = fp;
	
	
	if (win->firstmenu!=NULL) {
	 	newmenu->next = win->firstmenu;
		newmenu->prev = win->lastmenu;
		
		win->lastmenu->next = newmenu;
		win->lastmenu = newmenu;
		win->firstmenu->prev = newmenu;
		
		newmenu->menu_id = newmenu->prev->menu_id + 1;
	}
	else {
		newmenu->next=newmenu;
		newmenu->prev=newmenu;
		win->firstmenu=newmenu;
		win->lastmenu=newmenu;
		newmenu->menu_id=0;
	}
	
} // end addmenu





void addmenuitem(tgui_Window *win, int menuid, char *caption, void (*fp)(void *))  {

tgui_Menu *menu;
tgui_Menuitem *newitem= malloc(sizeof (tgui_Menuitem));	


    menu=win->firstmenu;
	while (menu->menu_id != menuid) {
		menu=menu->next;
	}

	
	newitem->caption = caption;
	newitem->fp = fp;
	
	
	if (menu->firstmi!=NULL) {
	 	newitem->next = menu->firstmi;
		newitem->prev = menu->lastmi;
		
		if (strlen(caption) > menu->mi_maxwidth) menu->mi_maxwidth = strlen(caption);
			
		menu->lastmi->next = newitem;
		menu->lastmi = newitem;
		menu->firstmi->prev = newitem;
		
		newitem->menu_item_id = newitem->prev->menu_item_id + 1;
	}
	else {
		newitem->next=newitem;
		newitem->prev=newitem;
		menu->mi_maxwidth = strlen(caption);
		
		menu->firstmi=newitem;
		menu->lastmi=newitem;
		newitem->menu_item_id=0;
	}
	
} // end addmenuitem

